#include <avr/interrupt.h>
#include <math.h>
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stddef.h>
#include <stdint.h>

//Setup Enviorment
#define F_CPU 16000000UL
#define BAUD 9600
#define UBRRVALUE (F_CPU/16/BAUD - 1) //UBR value for baud rate

//CORECT PINS!!!
//Stepper Motor Pins
#define IN1PIN PB3
#define IN2PIN PB2
#define IN3PIN PB1
#define IN4PIN PB0

//Stepper Motor Ports and Variables
#define STEPPORT PORTB
#define STEPDDR  DDRB
#define STEPDELAY_US 2000 

//Ultrasonic Distance Pins
#define TRIGPIN PD5
#define ECHOPIN PD6

//Setup Timer Varuables
volatile uint16_t timerStartValue = 0;
volatile uint16_t timerEndValue = 0;
volatile uint8_t captureState = 0;

//Setup Timeout Variables
volatile uint8_t timeoutFlag = 0;
volatile uint8_t timer0OverflowCount = 0;

//Datatype to transmit throug Serial
typedef struct {
    uint16_t distance;//2 bytes: The distance
    uint16_t angle;//2 bytes: Angle
} DataPacket_t; 

const uint8_t waveSteps[] = {
    (1 << IN1PIN),             //1000
    (1 << IN2PIN),             //0100
    (1 << IN3PIN),             //0010
    (1 << IN4PIN)              //0001
};

void stepperInit() {
    //The stepper motor has a gear of 64:1, one full revolution is poisblie (360 degrees with 4096 steps)
    //Set the four pins as outputs
    STEPDDR |= (1 << IN1PIN) | (1 << IN2PIN) | (1 << IN3PIN) | (1 << IN4PIN);
    
    //Ensure all pins start low
    STEPPORT &= ~((1 << IN1PIN) | (1 << IN2PIN) | (1 << IN3PIN) | (1 << IN4PIN));
}

// Delay between steps (determines motor speed)
// A common value for this motor is 2000 us (2ms)
#define STEPDELAYUS 2000 

void moveStepper(int16_t steps) {
    //Number of steps to move
    int16_t numSteps = (steps > 0) ? steps : -steps;
    
    //1 forward -1 for backward
    int8_t direction = (steps > 0) ? 1 : -1;
    
    //Keep track of the current position in the 4-step sequence
    static int8_t sequenceIndex = 0; 

    for (int16_t i = 0; i < numSteps; i++) {
        
        sequenceIndex += direction;
        
        //Handle wrap-around for the 4-step sequence (0, 1, 2, 3)
        if (sequenceIndex > 3) {
            sequenceIndex = 0;
        } else if (sequenceIndex < 0) {
            sequenceIndex = 3;
        }

        STEPPORT &= ~((1 << IN1PIN) | (1 << IN2PIN) | (1 << IN3PIN) | (1 << IN4PIN));
        
        STEPPORT |= waveSteps[sequenceIndex];
        
        _delay_us(STEPDELAYUS);
    }
    
    //turn of to reduce Heat
    STEPPORT &= ~((1 << IN1PIN) | (1 << IN2PIN) | (1 << IN3PIN) | (1 << IN4PIN));
}

void usartInit() {
    //Set baud rate
    UBRR0H = (uint8_t)(UBRRVALUE >> 8);
    UBRR0L = (uint8_t)UBRRVALUE;

    //Enable transmitter
    UCSR0B = (1 << TXEN0);

    //Set frame format: 8 data bits no parity 1 stop bit
    UCSR0C = (3 << UCSZ00); 
}

void usartTransmit(char data) {
    //Wait for empty transmit buffer (UDRE0)
    while (!(UCSR0A & (1 << UDRE0)));
    UDR0 = data;
}

//Function to send the entire struct
void usartTransmitBinary(void *data, size_t size) {
    char *ptr = (char *)data;
    for (size_t i = 0; i < size; i++) {
        usartTransmit(*ptr++);
    }
}

void triggerUltrasonic(){
    captureState = 0; //Define capture
    timeoutFlag = 0;
    timer0OverflowCount = 0;
    TCNT1 = 0; //Reset timer 1
    TCNT0 = 0; //Reset timer 0

    //Start Timeout Timer
    TCCR0B = (1 << CS02) | (1 << CS00); //Set Prescaler to 1024
    TIMSK0 |= (1 << TOIE0); //Enable Timer 0 overflow interupt

    PORTD |= (1 << TRIGPIN); //Enable Trigger
    _delay_us(10); //Wait 10mili seconds
    PORTD &= ~(1 << TRIGPIN); //Disable Trigger
}

__attribute__((weak)) ISR(TIMER0_OVF_vect){
    timer0OverflowCount++;

    //2 overfows ~33ms
    //256 ticks/ovf * 1024 prescaler / 16MHz = 16.384ms per overflow 
    if(timer0OverflowCount>=2){
        timeoutFlag=1;

        TCCR0B = 0;

        TIMSK0 &= ~(1 << TOIE0);

    }
}

ISR(TIMER1_CAPT_vect) {
    if (captureState == 0) {
        timerStartValue = ICR1; //Save timer value
        TCCR1B &= ~(1 << ICES1);  //capture on FALLING edge
        TIFR1 = (1 << ICF1);      //Clear flag 
        captureState = 1;        //Update state
        
    } else { //End of Pulse
        timerEndValue = ICR1;   //Store the timer value
        TCCR1B |= (1 << ICES1);   //Switch back to capture on RISING edge for next measurement
        TIFR1 = (1 << ICF1);      //Clear flag manually
        captureState = 2;        //Measurement complete
    }
}
void ultrasonicInit(){
    DDRD |= (1 << TRIGPIN); //Set TRIG as Output
    DDRD &= ~(1 << ECHOPIN); //Set ECHO as Input
    PORTD &= ~(1 << TRIGPIN);

    TCCR1A = 0;//not used for PWM
    TCCR1B = (1 << ICNC1) | //Enable Noise Canceler
             (1 << ICES1) | //Capture on Rising Edge first
             (1 << CS11); //rescaler = 8 (0.5us)
    
    TCNT1 = 0; //Start the counter at zero
    TIFR1 = (1 << ICF1); //Clear pending Input Capture Flag
    TIMSK1 = (1 << ICIE1); //Enable Input Capture Interrupt
    
    TCCR0A = 0;//Normal Mode
    TIMSK0 = 0;

    sei(); //Enable global interrupts
}

int main (void){
    //initialize Code
    ultrasonicInit();
    usartInit();
    stepperInit();

    DataPacket_t dataOut;
    int8_t direction = 1;
    uint16_t currentAngle = 0; //Variable that saves degrees 

while(1){
    if (currentAngle >= 360) {
            currentAngle = 360; 
            direction = -1; 
    } else if (currentAngle <= 0) {
            currentAngle = 0; 
            direction = 1; 
    }

    int16_t stepsToMove = (int16_t)round((float)5 * 11.3777);

    moveStepper(stepsToMove * direction);
    currentAngle += (5 * direction);
        
    _delay_ms(50); 
        
    triggerUltrasonic();

    while(captureState != 2 && timeoutFlag == 0){}//add timeout

        if (!timeoutFlag) {
        uint16_t pulseDurationTicks = timerEndValue - timerStartValue;
        
        //Convert ticks to microseconds (1  = 0.5us)
        float pulseDurationUs = (float)pulseDurationTicks * 0.5;

        //Divide by 58
        float distanceCm = pulseDurationUs / 58.0;

        dataOut.distance = (uint16_t)distanceCm;
        dataOut.angle = (uint16_t)currentAngle;

        }else{
            dataOut.distance = 0; //send 0 to Signal error
            dataOut.angle = 0xFFFF; //Maximum value to signal error
        }

        usartTransmitBinary(&dataOut, sizeof(dataOut));

        _delay_ms(100);
    }
    return 0;
}
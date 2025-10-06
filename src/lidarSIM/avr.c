#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>

#define TRIG_PIN PD5
#define ECHO_PIN PD6

volatile uint16_t timerStartValue = 0;
volatile uint16_t timerEndValue = 0;
volatile uint8_t captureState = 0;

void triggerUltrasonic(){
    captureState = 0; //Define capture
    TCNT1 = 0; //Reset Timer

    PORTD |= (1 << TRIG_PIN); //Enable Trigger
    delay(10); //Wait 10mili seconds
    PORTD &= (1 << TRIG_PIN); //Disable Trigger
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
void initUltrasonic(){
    DDRD |= (1 << TRIG_PIN); //Set TRIG as Output
    DDRB &= ~(1 << ECHO_PIN); //Set ECHO as Input
    PORTD &= ~(1 << TRIG_PIN);

    TCCR1A = 0;//not used for PWM
    TCCR1B = (1 << ICNC1) | //Enable Noise Canceler
             (1 << ICES1) | //Capture on Rising Edge first
             (1 << CS11); //rescaler = 8 (0.5us)
    
    TCNT1 = 0; //Start the counter at zero
    TIFR1 = (1 << ICF1); //Clear pending Input Capture Flag
    TIMSK1 = (1 << ICIE1); //Enable Input Capture Interrupt
    
    sei(); //Enable global interrupts
}

int main (void){
  initUltrasonic();

  while(1){
    triggerUltrasonic();

    while(captureState != 2){}//add timeout

        uint16_t pulse_duration_ticks = timer_end_value - timer_start_value;
        
        //Convert ticks to microseconds (1  = 0.5us)
        float pulse_duration_us = (float)pulse_duration_ticks * 0.5;

        //Divide by 58
        float distance_cm = pulse_duration_us / 58.0;

        delay(50);
  }
  
}
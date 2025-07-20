# Arduino Ultrasonic-based LiDAR Simulation

This project simulates a 2D LiDAR system using SR04 ultrasonic sensors mounted on a stepper motor. The system is controlled by an Arduino UNO (Elegoo R3) and scans the surroundings, sending distance data to a connected computer for visualization or processing.

## Features

- 2D scanning of the environment using ultrasonic sensors
- Stepper-controlled rotation for area coverage
- Data transmission to a PC for visualization

## Inspiration

The inspiration for this project came from observing real-world LiDAR systems, which operate in a similar (but more advanced) way.

## Requirements

- Arduino UNO (Elegoo R3 recommended)
- SR04 Ultrasonic sensor
- Stepper motor
- Arduino core libraries

## Installation

1. Clone this repository:
    ```
    git clone https://github.com/ABCLukas/Arduino-Ultrasonic-LiDAR-Siumulation.git
    ```
2. Open the Arduino sketch in the Arduino IDE.
3. Install any required libraries using the Arduino Library Manager.
4. Compile the c++ code
```
g++ inputHandler.cpp Renderer.cpp main.cpp -o main $(sdl2-config --cflags --libs)
```
5. Run and see The collected data

## Wiring Diagram

![schematic](https://github.com/ABCLukas/Arduino-Ultrasonic-LiDAR-Siumulation/blob/main/LiDAR.svg)

## Usage

1. Upload the code to your Arduino UNO.
2. Connect the ultrasonic sensor and stepper motor as shown in the wiring diagram.
3. Mount the sensor on the stepper motor.
4. Open the Serial Monitor to see distance data output.
5. Use the C++ code to visualize the data.

## Contributing

Contributions are welcome! Please open issues or submit pull requests for improvements. 

## License

This Project lays under the MIT licence for more information see [LICENCE](https://github.com/ABCLukas/Arduino-Ultrasonic-LiDAR-Siumulation/blob/main/LICENSE)

## Acknowledgments

- Thanks to the creators of Arduino, the SR04 sensor and KiCad.

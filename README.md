# Arduino Ultrasonic-based LiDAR Simulation

This project simulates a 2D LiDAR system using SR04 ultrasonic sensors mounted on a stepper motor. The system interacts with the Sensors through a Arduino UNO R3 and scans the surroundings, sending distance data to a connected linux device for visualization and processing.

## Project Overview

- 2D Scanning of the environment using ultrasonic sensors
- Stepper-controlled rotation for area coverage
- Data transmission to a PC for visualization

The inspiration for this project came from observing real-world LiDAR systems, which operate in a similar (but more advanced) way.

## Getting Started

### Requirements

- Arduino UNO R3
- SR04 Ultrasonic sensor
- Stepper motor
- Arduino core Stepper Library
- Erick Simões Ultrasonic Distance Library

### instalation

1. Clone this repository:
    ```
    git clone https://github.com/ABCLukas/Arduino-Ultrasonic-LiDAR-Siumulation.git
    ```
1. Open the Arduino sketch in the Arduino IDE.
1. Compile the c++ code with cmake:
    ```
    mkdir build && cd build
    cmake ../src
    make
    ```
1. Run and see The collected data

### How To Setup

1. Upload the code to your Arduino UNO.
1. Connect the ultrasonic sensor and stepper motor as shown in the wiring diagram.
1. Mount the sensor on the stepper motor.
1. Use the C++ code to visualize the data.



## Wiring Diagram

![schematic](https://github.com/ABCLukas/Arduino-Ultrasonic-LiDAR-Siumulation/blob/main/images/LiDAR.png)

## Contributing

Contributions are welcome! Please open issues or submit pull requests for improvements. 

## License

This Project lays under the MIT licence for more information see [LICENCE](https://github.com/ABCLukas/Arduino-Ultrasonic-LiDAR-Siumulation/blob/main/LICENSE)

## Acknowledgments

- Thanks to the creators of Arduino, the SR04 sensor and KiCad.

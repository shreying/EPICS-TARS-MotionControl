# EPICS-TARS-MotionControl

This repository contains the code, Keil project files, and HEX files for the motion control module of the Tri-Modal Autonomous Rescue System (TARS). This module is responsible for controlling the movement of the rescue robot in land, water, and air search and rescue operations.

## Repository Contents
- **Keil Project Files**: Includes all necessary project files to compile and simulate the motion control code.
- **HEX Files**: Contains the compiled HEX files to be loaded onto the microcontroller for real-world implementation.
- **Source Code**: The motion control algorithms and logic to control the robot's movement.

## Setup and Usage

### Prerequisites
1. **Keil uVision**: Make sure you have Keil uVision installed to compile and simulate the code.
2. **Microcontroller Hardware**: The HEX files are intended for use with a compatible microcontroller. Ensure you have the appropriate hardware setup.
3. **Programming Interface**: You'll need a suitable programmer to upload the HEX files onto the microcontroller (e.g., ST-Link, J-Link, etc.).

### Steps to Compile and Flash the Code
1. Open the Keil project file (`*.uvprojx`) in Keil uVision.
2. Compile the project by clicking on the "Build" button.
3. The resulting HEX file will be available in the **Output** folder or the directory specified in the project settings.
4. Flash the HEX file onto the microcontroller using your preferred programming interface.

### Running the Motion Control
Once the HEX file is flashed onto the microcontroller, the robot's motion control will be executed based on the integrated code, enabling the robot to perform land, water, and air-based operations.

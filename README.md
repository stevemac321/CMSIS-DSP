# STM32CubeIDE Project: Real-Time Voltage Analysis with FFT and Graph Processing

## Overview

This project demonstrates the use of ADC (Analog-to-Digital Converter) for real-time voltage readings on an STM32 microcontroller, the CMSIS/DSP math libraries for FFT (Fast Fourier Transform), and further analysis using a custom priority queue and graph structures. The project was developed and built using STM32CubeIDE, and it utilizes both CMSIS and HAL (Hardware Abstraction Layer) libraries.

## Project Structure

- **ADC Polling**: Captures real-time voltage data from the system using the STM32's ADC.
- **CMSIS/DSP FFT**: Processes the voltage readings using the CMSIS DSP library, performing FFT on the sampled data.
- **Priority Queue**: Further analysis of the FFT results using a priority queue for peak detection.
- **Graph Processing**: A graph is constructed from the FFT peaks and valleys, using a bitvector to represent the adjacency list for each vertex. 

## Setup Instructions

1. **Install STM32CubeIDE**: This project is built in STM32CubeIDE. Download and install the IDE from the official STMicroelectronics website.

2. **Configure DSP Libraries**: Follow the instructions from the ST community article [Configuring DSP Libraries on STM32CubeIDE](https://community.st.com/t5/stm32-mcus/configuring-dsp-libraries-on-stm32cubeide/ta-p/49637).

3. **Include Required Files**: Ensure you have the following file from the CMSIS/DSP source:
   - `Drivers\CMSIS\DSP\Source\TransformFunctions\arm_cfft_init_f32.c`
   
   This file is included in the project for successful FFT initialization.

4. **Build and Run**: Open the project in STM32CubeIDE, build, and flash it to your STM32 microcontroller. The project captures ADC readings and performs FFT analysis in real-time.

## Features

- **FFT Analysis**: The project processes 16 real-time voltage samples using a 16-point FFT.
- **Graph Representation**: Peaks and valleys in the FFT output are represented in a graph structure using bitvectors for adjacency lists.
- **Custom Priority Queue**: A custom priority queue is used to sort and manage peaks detected in the FFT results.

## Key Files

- `main.c`: Contains the main logic for ADC sampling, FFT computation, and graph processing.
- `adc_init.c`: Initialization file for ADC.
- `arm_cfft_init_f32.c`: Required for FFT initialization (added from CMSIS/DSP source).
- `README.md`: This file.

## Usage

- The project captures real-time voltage readings via ADC, processes them using FFT, and stores the results in an adjacency graph for further analysis.
- Print statements are used to output both the FFT and magnitude results to a UART for real-time monitoring.

## License

This project is licensed under the terms of the GPL License 2.0.

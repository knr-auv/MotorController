# STM AUV Controller
This app is used to control "Okoń" - an AUV built by KNR AUV Team from Warsaw University of Technology. 

## Table of contents
* [General Info](#general-info)
* [Used Libraries](#used-libraries)
* [Target Device](#target_device)
* [Functional Overview ](#functional-overview)
    - [Boat](#boat-overwiev)
    - [Comunication](#comunication)
    - [Pin Descriptions](#pin_descriptions)
* [PID Controller](#pid-controller)
* [Developer Manual](#developer-manual)
    - [Compiling](#compiling)
    - [Debuging](#debuging)
    - [Flashing](#flashing)
* [Setup](#setup)

## General Info
This app is used to control "Okoń" - an AUV built by KNR AUV Team from Warsaw University of Technology. App provides PID controler, and act as an interface between sensors and NVIDIA Jetson Nano - AUV core.

## Used Libraries
In order to keep app optimized we only used CMSIS and std peripheral libraries provided by STM. Drivers for each external peripheral were created on our own.

## Target Device
Target device is STM32 F411RE, integrated in custom made PCB.

## Functional Overview
Main purpose of our app is to provide interface between Jetson and peripheral devices: motors, grabber, sensors etc.

### Boat Overwiev
TODO: Put some nice pic and write a few words about it.
### Comunication
For comunication with Jetson we use serial protocol at 115200 baudrate.

TODO: Describe protocol

### Pin Descriptions
TODO: put here nice img with pin description :D

## PID Controller
TODO: Describe PID controller

## Developer Manual
This project was developed in VSCode using CMake for Makefile generation. Project compiles with GNU Arm Embedded Toolchain, not tested with GCC. Repository conatains all necessary setup files to debug and compile it in VSCode under windows 10.

### Compiling
To compile this project you need to download and install  GNU Arm Embedded Toolchain. Repo contains arm.cmake toolchain file. In order to compile project make sure that line 5 of this file is pointing to valid compiler directory. After adjusting compiler path, VSCode should be able to compile your project. If you are not using VSCode please refer to your IDE manual about using Cmake.

### Debuging
If you want to debug under VSCode follow these steps:
    
    - Download and install OpenOCD
    - Add OpenOCD to path variable

Now press F5 to build, flash and debug. If you are using diferent enviroment please refer to its manual.

### Flashing
Cmake is generating custom target "program". If you have OpenOCD installed just go to build directory and type "make program".

## Setup
- Get binary file
    - Download precompiled binary
    - Download GNU Arm Embedded Toolchain, CMake and compile project
- Get STM programmer STM32 Cube Programmer works fine
- Connect board and flash it.
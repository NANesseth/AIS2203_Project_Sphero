# AIS2203_Project_Sphero
Robot control - exam project for NTNU AIS2203

## Introduction
This project focuses on remotely controlling the Sphero RVR in real-time utilizing UDP communication protocols. 
It leverages multithreading and OpenCV, enabling simultaneous transmission and reception of data, as well as real-time display of images within the graphical user interface.
The server, located on the Rasperry pi, mounted on the Sphero RVR, is responsible for receiving and transmitting data to the client(this repo). The client sends and receives data from the server, and is responsible for displaying the GUI and sending commands to the server.
The client is also responsible for displaying the videofeed from the server.
The program includes three different controlling methods to control the robot. Keyboard, Xbox controller and autonomous driving. Autonomous driving is currently set to follow a red color.
### What works.
- Duplex UDP communication between Server and Client utilizing a JSON format.
- Keyboard Controls
- Xbox Controls
- Autonomous driving(following a ball)
- Multithreading
- "live" videofeed.
- Simple GUI implemented with openCV
- Collision detection
- Distance indicator
- Battery level indicator
- PID controller for camera in autonomous driving

### What does not work/ works poorly.
- GUI and camerafeed windows lock if clicked on with the mouse cursor.
- The robot could be better at tracking the ball but works.
- Latency on the videofeed could also be improved but is >0.5s at 160x120p.
- PID controller for camera in autonomous driving is not optimal and could be improved.
The following files, tests and demos listed below are not implemented in relation with the Sphero RVR. But is a work in progress of a more advanced autonomous tracking system.
    
###### Demos
    - Color_calibration_demo
    - Task_manager_demo
    - Object_detection_demo
  ###### Files
    - PCCamera.hpp
    - RaspberryCamera.hpp
    - JsonConverter.hpp
    - JsonUtils.hpp
    - Observer.hpp
    - Task.Manager.hpp
    - Core folder
    - vision folder


### How to get it running
#### Server instructions
- Follow Sphero instructions to setup raspberry pi and Sphero RVR. (https://sdk.sphero.com/raspberry-pi-setup)
- Once the setup instructions are followed and a wifi and SSH connection is established, connect to the raspberry and run the following commands:
  - sudo apt-get update
  - sudo apt-get install git
  - pip3 install opencv-python (or sudo apt-get install python3-opencv)
  - pip3 install qwiic
  - pip3 install numpy
  - pip3 install pi_servo_hat

- Download the udp server on the raspberry pi. (https://github.com/tostoner/udp_server)
- Run Main.py using sudo and python3. (sudo python3 Main.py)

After the server is running and finished initializing, the green lights should appear on the Sphero RVR which indicates that the RVR is ready to connect to the client.
Have fun and drive safe!
#### Client instructions
- This project is easiest built using vcpkg.
- Call CMake with `-DCMAKE_TOOLCHAIN_FILE=[path to vcpkg]/scripts/buildsystems/vcpkg.cmake` (using manifest mode).
- See [vcpkg.json](vcpkg.json) for available features.
- Set correct serverIP and serverPort in main.cpp (note: serverIP is the IP of the raspberry pi)


### Github links
- [Sphero RVR project client](https://github.com/NANesseth/AIS2203_Project_Sphero.git)
- [UDP server link](https://github.com/tostoner/udp_server)
- [Sphero RVR python SDK](https://github.com/sphero-inc/sphero-sdk-raspberrypi-python)

### Screenshots
#### Main menu
![MainMenu](Screenshots/MainMenu.png)
#### Keyboard menu
![KeyboardInput](Screenshots/KeyboardInput.png)
#### Xbox menu
![XboxMenu](Screenshots/XboxKeybindings.png)
#### Program running
![RunningClient](Screenshots/runningClient.png)



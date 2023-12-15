# AIS2203_Project_Sphero
Robot control - exam project for NTNU AIS2203

## Current state of the project
The project is currently in a state where the robot can be controlled by a remote computer over UDP. 

### What works.
- Duplex communication between Robot and client
- Keyboard Controls
- Xbox Controls
- Autonomous ball tracking (can be better)
- "live" videofeed with approx. 1 second delay
- Simple GUI implemented with openCV
- Collision detection
- Battery level indicator

### What does not work/ works poorly.
- "Managable inheritance system" code part for the Client is not implemented.
- GUI and camerafeed windows lock if clicked on with the mouse cursor.
- The robot is not very good at tracking the ball.
- Latency on the videofeed is too high.

### How to get it running

#### Server instructions
- Follow Sphero instructions to setup raspberry pi and Sphero RVR. (https://sdk.sphero.com/raspberry-pi-setup)
- Once the setup instructions are followed and a wifi and SSH connection is established, connect to the raspberry and run the following commands:
  - sudo apt-get update
  - sudo apt-get install git
  - sudo apt-get install python3-opencv (or pip install opencv-python)
  - pip install qwiic
  - pip install numpy
  - pip install pi_servo_hat

- Download the udp server on the raspberry pi. (https://github.com/tostoner/udp_server)
- Run Main.py using sudo and python3. (sudo python3 Main.py)

#### Client instructions
- This project is easiest built using vcpkg.
- Call CMake with `-DCMAKE_TOOLCHAIN_FILE=[path to vcpkg]/scripts/buildsystems/vcpkg.cmake` (using manifest mode).
- See [vcpkg.json](vcpkg.json) for available features.


### UDP Server Github link
The UDP server was created on a seperate git repo.
easily git pull 

https://github.com/tostoner/udp_server

### Screenshots
#### Main menu
![MainMenu.png](Screenshots/MainMenu.png)
#### KeyboardInput
![KeyboardInput.png](Screenshots/KeyboardInput.png)
#### Xbox menu
![XboxMenu.png](Screenshots/XboxMenu.png)



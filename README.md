# **MODULAR ROBOT **
# **PROJECT "PCB"**
Here are all the PCB's for manufacturing the EMERGE modular robot.
# **PROJECT "Optimization_Algorithms.rar" CONTENT**
The content of this project is divided into three sections:
* The first "Module" project must be programmed in all modules of the robotic platform.
* The Second Project "Xbee_Snifer_Teraterm" must be programmed in the accessory card that has an Xbee module only (previously configured) and to perform the data registration you can use the project "Capture_Interface.rar" which is designed in MATLAB and simply begins its registration by pressing the Start button and turn on the robot.
To configure the Xbee module you can follow the following link;
http://xbee.cl/xbee-serie-2-configuracion/
* Finally, there are optimization algorithms, of which you must choose one and program it in the accessory that has the ultrasound sensor only.
# **PROJECT "Robot_Test.rar" CONTENT**
This project is used to test the different peripherals of the robot.
## Test_LED	
This code activates (Frecuency=10Hz) LED lights on each side.
## Test_All_Sensor	
The initialization routine is in charge of turning on the LED located next to each sensor, it will do it 5 times if there is communication with it, otherwise it will only do it once.
Once initialize each sensor, if one of them is obstructed turn on the LED that is next to it.
## Test_Motor	
This program turns on the motor led once it communicates with it, moves it from right to left and vice versa at intervals of 5 seconds.
## Test_Sensor_F1	
All LEDs turn on and off, if there is communication with the face sensor 1.
## Test_Sensor_F2	
All LEDs turn on and off, if there is communication with the face sensor 2.
## Test_Sensor_F3	
All LEDs turn on and off, if there is communication with the face sensor 3.
## Test_Sensor_F4	
All LEDs turn on and off, if there is communication with the face sensor 4.
## Test_CAN_Module_1
Each module sends and receives a data, if the data is accepted, turn on the led of face 2 in each module, otherwise it will not turn on.
## Test_CAN_Module_2	
CAN_Psoc_kit	
Show the information on the CAN bus on an LCD display.


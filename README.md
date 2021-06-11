# odrive_can_functions
C++ library implementing functions for communication over CANBUS for ODrive motor controller 

This library contains CAN protocol functions for the ODrive 3.2.

**odrive_can_functions/pc104_controller/src/odrive_can_functions.cpp** : Contains the CAN protocol functions for the ODrive as described [here](https://docs.odriverobotics.com/can-protocol). Uses SockatCAN for CAN communication 

**odrive_can_functions/pc104_controller/src/task.cpp** : Real - time periodic task using pthreads 

**odrive_can_functions/teensy_quadruped/** : Contains the CAN communication functions for the Teensy 3.2. Uses a FlexCAN library for CAN communication


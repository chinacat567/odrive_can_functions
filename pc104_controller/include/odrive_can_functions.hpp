#include<linux/can.h>
#include<sys/socket.h>
 #include <unistd.h>
#include<sys/types.h>
#include<sys/ioctl.h>
#include<pthread.h>
#include<stdio.h>
#include<stdint.h>
#include<iostream>

#ifndef _CF_H_
#define _CF_H_

#define RTR 1

#define BIT_MASK_0 0xFF
#define BIT_MASK_1 0xFF00
#define BIT_MASK_2 0xFF0000
#define BIT_MASK_3 0xFF000000
#define BIT_MASK_4 0xFF00000000
#define BIT_MASK_5 0xFF0000000000
#define BIT_MASK_6 0xFF000000000000
#define BIT_MASK_7 0xFF00000000000000

using namespace std;

// define motor array index
#define NO_OF_MOTOR       12

#define FRONT_LEFT_KNEE       1
#define FRONT_LEFT_HIP        2
#define FRONT_LEFT_ABDUCTION  3
#define FRONT_RIGHT_KNEE      4
#define FRONT_RIGHT_HIP       5
#define FRONT_RIGHT_ABDUCTION 6
#define BACK_LEFT_KNEE        7
#define BACK_LEFT_HIP         8
#define BACK_LEFT_ABDUCTION   9
#define BACK_RIGHT_KNEE       10
#define BACK_RIGHT_HIP        11
#define BACK_RIGHT_ABDUCTION  12

#define CLAW_LEFT_1        13
#define CLAW_LEFT_2        14
#define CLAW_RIGHT_1       15
#define CLAW_RIGHT_2       16

//define command ids
#define ODRIVE_HEARTBEAT        1
#define ESTOP                   2
#define GET_MOTOR_ERROR         3/*IMPLEMENTED IN REQUEST_MSG*/
#define GET_ENCODER_ERROR       4/*IMPLEMENTED IN REQUEST_MSG*/
#define SET_AXIS_NODE_ID        6
#define SET_AXIS_REQUESTED_STATE  7
#define GET_ENCODER_ESTIMATE    9 /*IMPLEMENTED IN REQUEST_MSG*/
#define GET_ENCODER_COUNTS      10 /*IMPLEMENTED IN REQUEST_MSG*/
#define MOVE_TO_POS             11
#define SET_POS_SETPOINT        12
#define SET_VEL_SETPOINT        13
#define SET_CUR_SETPOINT        14
#define SET_VEL_LIMIT           15
#define START_ANTI_COGGING      16
#define SET_TRAJ_VEL_LIMIT      17
#define SET_TRAJ_ACCEL_LIMIT    18
#define SET_TRAJ_A_PER_CSS      19
#define GET_IQ_VALUES           20 /*IMPLEMENTED IN REQUEST_MSG*/
#define REBOOT_ODRIVE           22
#define GET_VBUS_VOLTAGE        23 /*IMPLEMENTED IN REQUEST_MSG*/
#define SET_VEL_PI_GAIN         24


/* struct for storing the motor data points */

struct odrive_motor
{
    uint32_t axis_current_state;
    uint32_t motor_error;
    uint32_t encoder_error;
    uint32_t sensorless_error;
    uint32_t axis_error;
    uint16_t axis_can_node_id;
    uint32_t axis_requested_state;
    float encoder_pos_estimate;
    float encoder_vel_estimate;
    int32_t encoder_shadow_count;
    int32_t encoder_count_in_cpr;
    int32_t goal_position;
    int32_t pos_setpoint;
    int16_t vel_ff; //0.1 factor
    int16_t current_ff; // 0.01 factor
    int32_t vel_setpoint; //0.01 factor
    int32_t current_setopint; // 0.01 factor
    float vel_limit;
    float traj_vel_limit;
    float traj_accel_limit;
    float decel_limit;
    float traj_a_per_css;
    float iq_setpoint;
    float iq_measured;
    float sensorless_pos_estimate;
    float sensorless_vel_estimate;
    float vbus_voltage = 0.0; 
    float velocity_p_gain; 
    float velocity_i_gain; 
};

struct can_frame_odrive
{
        can_frame cframe;
        uint32_t node_id;
        uint32_t cmd_id;
        int index;
};


class controller{

public:
    /*constructor*/
    controller(int writesocket_fd,int readsocket_f);

    virtual ~controller() {};
    
    /* method to write tx_msg class member on the CAN BUS*/
    bool can_write();
    /*  method to read data from the CAN BUS and populate rx_data class member*/
    bool can_read();
    /* msg handling method to manage incoming data and poulate member variables accordingly*/
    void msg_handler();
    
    /* methods that set ODrive parameters*/
    void estop(uint32_t node_id);
    void set_axis_node_id(uint32_t node_id, uint16_t axis_can_node_id);
    void set_axis_requested_state(uint32_t node_id, uint32_t axis_requested_state);
    void move_to_pos(uint32_t node_id, int32_t pos);
    void set_pos_setpoint(uint32_t node_id, int32_t pos_setpoint, int16_t vel_ff,int16_t current_ff);
    void set_vel_setpoint(uint32_t node_id, int32_t vel_setpoint , int16_t current_ff);
    void set_cur_setpoint(uint32_t node_id, int32_t cur_setpoint);
    void set_vel_limit(uint32_t node_id, float vel_limit);
    void start_anticogging(uint32_t node_id);
    void set_traj_vel_limit(uint32_t node_id, float traj_vel_limit);
    void set_traj_accel_limit(uint32_t node_id ,float accel_limit, float decel_limit);
    void set_traj_a_per_css(uint32_t node_id, float traj_a_per_css);
    void reboot_odrive(uint32_t node_id);
    void get_vbus_voltage(uint32_t node_id);
    void set_vel_pi_gain(uint32_t node_id ,float vel_p_gain, float vel_i_gain);


    /* accessor methods : used for accessing private class members from outside the class*/
    odrive_motor get_motor_data(int x);

    /*MULTITHREADING FUNCTIONS*/
    void set_mutex_lock(pthread_mutex_t &lock);
    void set_internal_thread(pthread_t &thread);
    /*returns true is the thread was started successfully, false in case of errors*/
    bool start_internal_thread();

private:
    can_frame_odrive rx_msg;
    can_frame_odrive tx_msg;
    int read_socket;
    int write_socket;
    pthread_t thread;
   /*'legs' member variable, contains motor data for all the 12 motors*/
    odrive_motor motors[NO_OF_MOTOR];
    bool signit_handler;
    pthread_mutex_t mutex_lock;
    /* thread function that runs an infinite loop*/
    void* internal_thread_function();
    static void * InternalThreadEntryFunc(void * This);
    
};


#endif /* _CF_H_ */











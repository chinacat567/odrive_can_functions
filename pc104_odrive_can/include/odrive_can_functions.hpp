#include<linux/can.h>
#include<sys/socket.h>
 #include <unistd.h>
#include<sys/types.h>
#include<sys/ioctl.h>
#include<pthread.h>
#include<stdio.h>
#include<stdint.h>
#include<iostream>


#define rtr 1

#define BIT_MASK_0 0xFF
#define BIT_MASK_1 0xFF00
#define BIT_MASK_2 0xFF0000
#define BIT_MASK_3 0xFF000000

using namespace std;
//define command ids

const uint32_t odrive_heartbeat_cmd = 1;
const uint32_t estop_cmd = 2;
const uint32_t get_motor_error_cmd = 3;/*implemented in request_msg*/
const uint32_t get_encoder_error_cmd = 4;/*implemented in request_msg*/
const uint32_t set_axis_node_id_cmd = 6;
const uint32_t set_axis_requested_state_cmd = 7;
const uint32_t get_encoder_estimate_cmd = 9; /*implemented in request_msg*/
const uint32_t get_encoder_counts_cmd = 10; /*implemented in request_msg*/
const uint32_t move_to_pos_cmd = 11;
const uint32_t set_pos_setpoint_cmd = 12;
const uint32_t set_vel_setpoint_cmd = 13;
const uint32_t set_cur_setpoint_cmd = 14;
const uint32_t set_vel_limit_cmd = 15;
const uint32_t start_anti_cogging_cmd = 16;
const uint32_t set_traj_vel_limit_cmd = 17;
const uint32_t set_traj_accel_limit_cmd = 18;
const uint32_t set_traj_a_per_css_cmd = 19;
const uint32_t get_iq_values_cmd = 20; /*implemented in request_msg*/
const uint32_t reboot_odrive_cmd = 22;
const uint32_t get_vbus_voltage_cmd = 23; /*implemented in request_msg*/
const uint32_t set_vel_pi_gain_cmd = 24;



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
    float vbus_voltage; 
    float velocity_p_gain; 
    float velocity_i_gain; 
};

struct identifier
{
    int leg_no; /* leg_no identifier*/
    int type_no; /* type_no identifier 0-knee, 1-abduction, 2-hip*/
};


struct can_frame_odrive
{
        can_frame socket_can;
        uint32_t node_id;
        uint32_t cmd_id;
        identifier idn;
};

class controller{

public:
    /*constructor*/
    controller();
    virtual ~controller() {};
    
    /* method to write tx_msg class member on the CAN BUS*/
    bool can_write();
    /*  method to read data from the CAN BUS and populate rx_data class member*/
    bool can_read();
    /* msg handling method to manage incoming data and poulate member variables accordingly*/
    void msg_handler();
    /* method to set the socketcan filehandler*/
    void set_socket(int socket_file_handler);
 
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
    odrive_motor get_motor_data(int x, int y);

    /*MULTITHREADING FUNCTIONS*/
    void set_mutex_lock(pthread_mutex_t &lock);
    void set_internal_thread(pthread_t &thread);
    /*returns true is the thread was started successfully, false in case of errors*/
    bool start_internal_thread();
    

private:
    can_frame_odrive rx_msg;
    can_frame_odrive tx_msg;
    int socket_file_handler;
    pthread_t thread;
   /*'legs' member variable, contains motor data for all the 12 motors*/
    odrive_motor legs[4][3];
    bool signit_handler;
    pthread_mutex_t mutex_lock;
    /* thread function that runs an infinite loop*/
    void* internal_thread_function();
    static void * InternalThreadEntryFunc(void * This);
    
};













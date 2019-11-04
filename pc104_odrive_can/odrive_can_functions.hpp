#include<linux/can.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<sys/ioctl.h>
#include<pthread.h>
#include<request_msg.hpp>
#include<stdio.h>

#define rtr 1;

#define BIT_MASK_0 0xFF;
#define BIT_MASK_1 0xFF00;
#define BIT_MASK_2 0xFF0000;
#define BIT_MASK_3 0xFF000000;
#define BIT_MASK_4 0xFF00000000;
#define BIT_MASK_5 0xFF00000000;
#define BIT_MASK_6 0xFF0000000000;
#define BIT_MASK_7 0xFF000000000000;

using namespace std;
//define command ids

const uint32_t odrive_heartbeat = 1;
const uint32_t estop = 2;
const uint32_t get_motor_error = 3;/*implemented in request_msg*/
const uint32_t get_encoder_error = 4;/*implemented in request_msg*/
const uint32_t set_axis_node_id = 6;
const uint32_t set_axis_requested_state= 7;
const uint32_t get_encoder_estimate = 9; /*implemented in request_msg*/
const uint32_t get_encoder_counts = 10; /*implemented in request_msg*/
const uint32_t move_to_pos = 11;
const uint32_t set_pos_setpoint = 12;
const uint32_t set_vel_setpoint = 13;
const uint32_t set_cur_setpoint = 14;
const uint32_t set_vel_limit = 15;
const uint32_t start_anti_cogging = 16;
const uint32_t set_traj_vel_limit = 17;
const uint32_t set_traj_accel_limit = 18;
const uint32_t set_traj_a_per_css= 19;
const uint32_t get_iq_values = 20; /*implemented in request_msg*/
const uint32_t reboot_odrive = 22;
const uint32_t get_vbus_voltage = 23; /*implemented in request_msg*/
const uint32_t set_vel_pi_gain = 24;


/*controller that runs in real time*/


struct odrive_motor
{
    uint32_t axis_error;
    uint32_t axis_current_state;
    uint32_t motor_error;
    uint32_t encoder_error;
    uint32_t sensorless_error;
    uint32_t axis_error;
    uint32_t axis_error;
    uint16_t axis_can_nod_id;
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
    float decel_limit;
    float traj_a_per_css;
    float iq_setpoint;
    float iq_measured;
    float sensorless_pos_estimate;
    float sensorless_vel_estimate;
    float vbus_voltage; 
    float velocity_p_gain; 
    float velocity_i_gain; 
}

struct robot_leg{
    odrive_motor knee;
    odrive_motor abduction;
    odrive_motor hip;

};
struct can_frame_odrive
{
        can_frame socket_can;
        uint32_t node_id;
        uint32_t cmd_id;
        int leg_no;

}


void bit_masking;


class controller{

public:
    // I/O functions
    void controller();
    bool can_available();
    bool can_write();
    bool can_read();
    void msg_handler();
    void estop(uint32_t node_id);
    void set_axis_node_id(uint32_t node_id, uint16_t axis_can_node_id);
    void set_axis_requested_state(uint32_t node_id, uint32_t axis_requested_state);
    void move_to_pos(uint32_t node_id, int32_t pos);
    void set_pos_setpoint(uint32_t node_id, int32_t pos_setpoint, int16_t vel_ff,int16_t current_ff);
    void set_vel_setpoint(uint32_t node_id, int32_t vel_setpoint , int16_t current_ff);
    void set_cur_setpoint(uint32_t node_id, int32_t cur_setpoint);
    void set_vel_limit(uint32_t node_id, float vel_limit);
    void start_anticogging(uint32_t node_id);
    void set_traj_vel_limit(uint32_t node_id float traj_vel_limit);
    void set_traj_accel_limit(uint32_t node_id float accel_limit, float decel_limit);
    void set_traj_a_per_css(uint32_t node_id float accel_limit, float traj_a_per_css);
    void reboot_odrive(uint32_t node_id);
    void get_vbus_voltage(uint32_t node_id);
    void set_vel_pi_gain(uint32_t node_id float vel_p_gain, float vel_i_gain);
    void set_mutex_lock(pthread_mutex_t &lock);
    void set_internal_thread(pthread_t &thread);
    bool start_internal_thread();

private:
    can_frame_odrive rx_msg;
    can_frame_odrive tx_msg; //initialize payload size
    int socket_file_handler;
    pthread_t thread;
    std::vector<robot_leg> legs(4);
    bool signit_handler;
    pthread_mutex_t mutex_lock;













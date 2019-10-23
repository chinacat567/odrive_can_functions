#include<linux/can.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<sys/ioctl.h>



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
        int node_id;
        int cmd_id;
        int leg_no;

}


void bit_masking;


class controller{

public:
    // I/O functions
    bool can_available();
    bool can_write();
    bool can_read();
    void msg_handler();
    void real_time_thread();
    

   


    
private:
    can_frame_odrive rx_msg;
    can_frame_odrive tx_msg;
    int socket_file_handler;
    std::vector<robot_leg> legs(4);





    
   







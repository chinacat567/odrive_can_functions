/*
class for sending out request msgs to odrive on a seperate thread
*/
#include<can_function.cpp>
class request_msg{

public:
    void request_msg();
    void get_motor_error(uint32_t node_id);
    void get_encoder_estimate(uint32_t node_id);
    void get_encoder_counts(uint32_t node_id);
    void get_iq_values(uint32_t node_id);
    void get_encoder_error(uint32_t node_id)
    void write_msg();

private:
    can_frame_odrive write_msg;
    int socket_handler;
    pthread_t request_routine_thread;


}
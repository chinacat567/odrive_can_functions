/*
class for sending out request msgs to odrive on a seperate thread
*/
#include "odrive_can_functions.hpp"
class request_msg{

public:
    request_msg();
    /* ODrive request msgs */
    void get_motor_error(uint32_t node_id);
    void get_encoder_estimate(uint32_t node_id);
    void get_encoder_counts(uint32_t node_id);
    void get_iq_values(uint32_t node_id);
    void get_encoder_error(uint32_t node_id);
    void get_vbus_voltage(uint32_t node_id);

    int send_msg();
    /* set socket method */
    void set_socket(int socket_fd);


private:
    can_frame_odrive write_msg;
    int socket_handler;
};




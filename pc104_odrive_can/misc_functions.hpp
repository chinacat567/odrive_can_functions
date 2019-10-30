#include<can_function.hpp>
#include<request_msg.hpp>

int binary_to_decimal(std::string binaryString);
void bit_masking(&can_frame_odrive can_frame);
void sort_can_node_id(std::vector<robot_leg> &legs,can_frame_odrive &msg);

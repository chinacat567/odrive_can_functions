#include<string>

using namespace std;

int binary_to_decimal(std::string binaryString);
void bit_masking(can_frame_odrive &can_frame);
void sort_can_node_id(odrive_motor (&legs)[4][3],can_frame_odrive &msg);

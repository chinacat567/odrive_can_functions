#include<string>

using namespace std;

int binary_to_decimal(std::string binaryString);
void bit_masking(can_frame_odrive &can_frame);
void sort_can_node_id(odrive_motor (&legs)[4][3],can_frame_odrive &msg);
void float2Bytes(float float_variable, uint8_t *bytes_temp);
void bytes2Float(uint8_t *bytes_temp, float *float_variable);
void rvereseArray(auto arr[], int start, int end);

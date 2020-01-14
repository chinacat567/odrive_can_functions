#ifndef _MF_H_
#define _MF_H_

#define CMD_ID_MASK 0x1F
#define NODE_ID_MASK 0x7E0

#include<cstring>
#include"odrive_can_functions.hpp"

using namespace std;

void bit_masking(can_frame_odrive &can_frame);
void sort_can_node_id(odrive_motor (&legs)[4][3],can_frame_odrive &msg);
void float2Bytes(float float_variable, uint8_t *bytes_temp);
void bytes2Float(uint8_t *bytes_temp, float *float_variable);
//void rvereseArray(auto arr[], int start, int end);

#endif /* _MF_H_ */

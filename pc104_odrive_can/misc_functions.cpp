#include<misc_functions.hpp>

int binary_to_decimal(std::string binaryString)
{
	int int_value = 0;
	int indexCounter = 0;
	for(int i=binaryString.length()-1;i>=0;i--){

    	if(binaryString[i]=='1'){
        	int_value += pow(2, indexCounter);
    	}
    	indexCounter++;
	}
	return int_value;
}

//bit masking function to seperate upper 6 (node_id) and lower 5 (cmd_id) bits of the can_id in the can frame
void bit_masking(&can_frame_odrive can_frame)
{
	
	__u32 cmd_id_mask = 0x1F;
	__u32 node_id_mask = 0x7E0;
	can_frame.cmd_id = (cmd_id_mask & can_frame.socket_can.can_id) ;
	can_frame.node_id = (node_id_mask & can_frame.socket_can.can_id) >> 5;	
    
}

//function for sorting CAN node ids into respective robot legs
void sort_can_node_id(std::vector<robot_leg> &legs,can_frame_odrive &msg)
{
	
	/*	routine for allotting can_node_id of ODrive. Motors are numbered from 1-13 in the following format
	Leg 0 (Knee -1 Abduction -2 Hip -3)
	Leg 1 (Knee -4 Abduction -5 Hip -6)
	Leg 2 (Knee -7 Abduction -8 Hip -9)
	Leg 3 (Knee -10 Abduction -11 Hip -12)*/

	if(msg.node_id%3 == 1) // knee motor
		{
			for (int i = 1; i < 5; ++i)
			{
				if((msg.node_id+2)/3 ==i) 
			{
				legs[i-1].knee.axis_can_node_id = 3*i-2; 
				msg.leg_no = i-1;
			}

			}		
		}

		else if(msg.node_id%3 == 2) // abduction motor
		{
			
			for (int i = 1; i < 5; ++i)
			{
				if((msg.node_id+1)/3 ==i) 
			{
				legs[i-1].abduction.axis_can_node_id = 3*i-1; 
				msg.leg_no = i-1;
			}

			}	

		} 


		else if(msg.node_id%3 == 0) // hip motor
		{
			for (int i = 1; i < 5; ++i)
			{
				if((msg.node_id)/3 ==i) 
			{
				legs[i-1].hip.axis_can_node_id = 3*i; 
				msg.leg_no = i-1;
			}

			}

		} 
		else
		{
			cout<<"CAN Node ID sorting failed" <<endl;
		}

}

#include<can_functions.hpp>
#define FREQ 


int controller::can_read()
{	
	int nbytes;
	nbytes = read(this->socket_file_handler, this->rx_msg, sizeof(struct can_frame));
	printf("Read %d bytes\n", nbytes);
	
}


controller::send_msg(int motor_no, can_frame &send_message)
{


}

odrive_heartbeat()
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
void bit_masking(can_frame_odrive &msg)
{
	std::string binary = std::bitset<11>(msg.socket_can.can_id).to_string(); //convert to 11 bit binary
    msg.cmd_id = binary.substr(6); // slice last 6 bits
    msg.node_id= binary.substr(0,5); //slice first 5 bits
    
    
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


void controller::msg_handler()
{

	bit_masking(this->rx_msg); // get node id, cmd id
	sort_can_node_id(this->legs,this->rx_msg); // assign node id to respective leg

	//read the received can frame
	switch(this->rx_msg.cmd_id)
	{
		case '1':
			this->legs[this->rx_msg.leg_no].axis_error = this->rx_msg.socket_can.data[0];
			this->legs[this->rx_msg.leg_no].axis_current_state = this->rx_msg.socket_can.data[4];
		case '3':
			this->legs[this->rx_msg.leg_no].motor_error = this->rx_msg.socket_can.data[0];
		case '4':
			this->legs[this->rx_msg.leg_no].encoder_error = this->rx_msg.socket_can.data[0];
		case '5':
			this->legs[this->rx_msg.leg_no].sensorless_error = this->rx_msg.socket_can.data[0];	
		case '20':
			this->legs[this->rx_msg.leg_no].iq_setpoint = this->rx_msg.socket_can.data[0];
			this->legs[this->rx_msg.leg_no].iq_measured = this->rx_msg.socket_can.data[4];
		default:
			cout << "Failed to read incoming CAN frame" << endl;

	}


void controller::send_msg()
{
	

}	

	


		






int controller::can_write()
{	
	int nbytes;
	nbytes = write(this->socket_file_handler, this->tx_msg, sizeof(struct can_frame));
	printf("Wrote %d bytes\n", nbytes);
	return nbytes;

}



int main()
{
	//create controller object;
	controller ctrl();
	struct sockaddr_can addr; //socket can adress
	struct ifreq ifr; //interface structure

	const char *ifname = "vcan0";
	ctrl.socket_file_handler = socket(PF_CAN, SOCK_RAW, CAN_RAW)

	if((ctrl.socket_file_handler) < 0) {
		perror("Error while opening socket");
		return -1;
	}

	//set the name of the CAN device
	strcpy(ifr.ifr_name, ifname);
	ioctl(s, SIOCGIFINDEX, &ifr);
	
	//set properties
	addr.can_family  = AF_CAN;
	addr.can_ifindex = ifr.ifr_ifindex;

	printf("%s at index %d\n", ifname, ifr.ifr_ifindex);

	//bind the socket to the can file handler
	if(bind(s, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
		perror("Error in socket bind");
		return -2;
	}


	ctrl.can_read();
	ctrl.can_write();



	

	

	
	
	=
	
}


	
#include<can_functions.hpp>
#include<mis_functions.hpp>

void controller::controller()
{
	for (int i = 0; i < 7; ++i)
	{
		this->tx_msg.socket_can.data[i] = 0;
	}
	this->tx_msg.socket_can.can_dlc = 8; // 8 byte payload size
	this->msg_req   //initialize this
}

int controller::can_read()
{	
	int nbytes;
	nbytes = read(this->socket_file_handler, this->rx_msg, sizeof(struct can_frame));
	printf("Read %d bytes\n", nbytes);
	
}

void controller::msg_handler()
{

	bit_masking(this->rx_msg); // get node id, cmd id
	sort_can_node_id(this->legs,this->rx_msg); // assign node id to respective leg

	//read the received can frame
	switch(this->rx_msg.cmd_id)
	{
		case '1': //hearbeat message
			this->legs[this->rx_msg.leg_no].axis_error = (uint32_t(this->rx_msg.socket_can.data[0]) & uint32_t(this->rx_msg.socket_can.data[1])<< 8 & uint32_t(this->rx_msg.socket_can.data[2]) << 16 & uint32_t(this->rx_msg.socket_can.data[3]) << 24);
			this->legs[this->rx_msg.leg_no].axis_current_state = (uint32_t(this->rx_msg.socket_can.data[4]) & uint32_t(this->rx_msg.socket_can.data[5])<< 8 & uint32_t(this->rx_msg.socket_can.data[6]) << 16 & uint32_t(this->rx_msg.socket_can.data[7]) << 24);
		case '3': //get motor error
			this->legs[this->rx_msg.leg_no].motor_error = (uint32_t(this->rx_msg.socket_can.data[0]) & uint32_t(this->rx_msg.socket_can.data[1])<< 8 & uint32_t(this->rx_msg.socket_can.data[2]) << 16 & uint32_t(this->rx_msg.socket_can.data[3]) << 24);
		case '4': //get encoder error
			this->legs[this->rx_msg.leg_no].encoder_error = (uint32_t(this->rx_msg.socket_can.data[4]) & uint32_t(this->rx_msg.socket_can.data[5])<< 8 & uint32_t(this->rx_msg.socket_can.data[6]) << 16 & uint32_t(this->rx_msg.socket_can.data[7]) << 24);;
		case '9': //get encoder estimates
			this->legs[this->rx_msg.leg_no].encoder_pos_estimate = (float(this->rx_msg.socket_can.data[0]) & float(this->rx_msg.socket_can.data[1])<< 8 & float(this->rx_msg.socket_can.data[2]) << 16 & float(this->rx_msg.socket_can.data[3]) << 24);
			this->legs[this->rx_msg.leg_no].encoder_vel_estimate =(float(this->rx_msg.socket_can.data[4]) & float(this->rx_msg.socket_can.data[5])<< 8 & float(this->rx_msg.socket_can.data[6]) << 16 & float(this->rx_msg.socket_can.data[7]) << 24);
		case '10': //get encoder counts
			this->legs[this->rx_msg.leg_no].encoder_shadow_count = (int32_t(this->rx_msg.socket_can.data[0]) & int32_t(this->rx_msg.socket_can.data[1])<< 8 & int32_t(this->rx_msg.socket_can.data[2]) << 16 & int32_t(this->rx_msg.socket_can.data[3]) << 24);
			this->legs[this->rx_msg.leg_no].encoder_count_in_cpr =(int32_t(this->rx_msg.socket_can.data[4]) & int32_t(this->rx_msg.socket_can.data[5])<< 8 & int32_t(this->rx_msg.socket_can.data[6]) << 16 & int32_t(this->rx_msg.socket_can.data[7]) << 24);
		case '20': //get IQ
			this->legs[this->rx_msg.leg_no].iq_setpoint = (float(this->rx_msg.socket_can.data[0]) & float(this->rx_msg.socket_can.data[1])<< 8 & float(this->rx_msg.socket_can.data[2]) << 16 & float(this->rx_msg.socket_can.data[3]) << 24);
			this->legs[this->rx_msg.leg_no].iq_measured = (float(this->rx_msg.socket_can.data[4]) & float(this->rx_msg.socket_can.data[5])<< 8 & float(this->rx_msg.socket_can.data[6]) << 16 & float(this->rx_msg.socket_can.data[7]) << 24);
		case '23':
			this->legs[this->rx_msg.leg_no].vbus_voltage = (float(this->rx_msg.socket_can.data[0]) & float(this->rx_msg.socket_can.data[1])<< 8 & float(this->rx_msg.socket_can.data[2]) << 16 & float(this->rx_msg.socket_can.data[3]) << 24);

		default:
			cout << "Failed to read incoming CAN frame" << endl;

	}

	// error detection

}	// create a data stream for the sensor data 

void controller::estop(uint32_t node_id)
{
	this->tx_msg.node_id = node_id;
	this->tx_msg.cmd_id = estop;
	this->tx_msg.socket_can.can_id = (this->tx_msg.cmd_id & this->tx_msg.node_id << 5);
}
void controller::set_axis_node_id(uint32_t node_id, uint16_t axis_can_node_id)
{
	this->tx_msg.node_id = node_id;
	this->tx_msg.cmd_id = set_axis_node_id;;
	this->tx_msg.socket_can.can_id = (this->tx_msg.cmd_id & this->tx_msg.node_id << 5);
	this->tx_msg.socket_can.data[0] = axis_can_node_id & 0xFF;
	this->tx_msg.socket_can.data[1] = axis_can_node_id & 0xFF00;
	
}
void controller::set_axis_requested_state(uint32_t node_id, uint32_t axis_requested_state)
{
	this->tx_msg.node_id = node_id;
	this->tx_msg.cmd_id = set_axis_requested_state;
	this->tx_msg.socket_can.can_id = (this->tx_msg.cmd_id & this->tx_msg.node_id << 5);
	this->tx_msg.socket_can.data[0] = axis_requested_state & BIT_MASK_0;
	this->tx_msg.socket_can.data[1] = axis_requested_state & BIT_MASK_1;
	this->tx_msg.socket_can.data[2] = axis_requested_state & BIT_MASK_2;
	this->tx_msg.socket_can.data[3] = axis_requested_state & BIT_MASK_3;	
	
}

void controller::move_to_pos(uint32_t node_id, int32_t pos)
{
	this->tx_msg.node_id = node_id;
	this->tx_msg.cmd_id = move_to_pos;
	this->tx_msg.socket_can.can_id = (this->tx_msg.cmd_id & this->tx_msg.node_id << 5);
	this->tx_msg.socket_can.data[0] = pos & BIT_MASK_0;
	this->tx_msg.socket_can.data[1] = pos & BIT_MASK_1;
	this->tx_msg.socket_can.data[2] = pos & BIT_MASK_2;
	this->tx_msg.socket_can.data[3] = pos & BIT_MASK_3;	
	
}

void controller::set_pos_setpoint(uint32_t node_id, int32_t pos_setpoint, int16_t vel_ff,int16_t current_ff)
{
	this->tx_msg.node_id = node_id;
	this->tx_msg.cmd_id = set_pos_setpoint;
	this->tx_msg.socket_can.can_id = (this->tx_msg.cmd_id & this->tx_msg.node_id << 5);
	this->tx_msg.socket_can.data[0] = pos_setpoint & BIT_MASK_0;
	this->tx_msg.socket_can.data[1] = pos_setpoint & BIT_MASK_1;
	this->tx_msg.socket_can.data[2] = pos_setpoint & BIT_MASK_2;
	this->tx_msg.socket_can.data[3] = pos_setpoint & BIT_MASK_3;	

	this->tx_msg.socket_can.data[4] = vel_ff/0.1 & BIT_MASK_0;	
	this->tx_msg.socket_can.data[5] = vel_ff/0.1 & BIT_MASK_1;	
	
	this->tx_msg.socket_can.data[6] = current_ff/0.01 & BIT_MASK_0;	
	this->tx_msg.socket_can.data[7] = current_ff/0.01 & BIT_MASK_1;	
	
}

void controller::set_vel_setpoint(uint32_t node_id, int32_t vel_setpoint , int16_t current_ff)
{
	this->tx_msg.node_id = node_id;
	this->tx_msg.cmd_id = set_vel_setpoint;
	this->tx_msg.socket_can.can_id = (this->tx_msg.cmd_id & this->tx_msg.node_id << 5);
	this->tx_msg.socket_can.data[0] = vel_setpoint/0.01 & BIT_MASK_0;
	this->tx_msg.socket_can.data[1] = vel_setpoint/0.01 & BIT_MASK_1;
	this->tx_msg.socket_can.data[2] = vel_setpoint/0.01 & BIT_MASK_2;
	this->tx_msg.socket_can.data[3] = vel_setpoint/0.01 & BIT_MASK_3;

	this->tx_msg.socket_can.data[4] = current_ff/0.01 & BIT_MASK_0;
	this->tx_msg.socket_can.data[5] = current_ff/0.01 & BIT_MASK_1;

	
	
}

void controller::set_cur_setpoint(uint32_t node_id, int32_t cur_setpoint)
{
	this->tx_msg.node_id = node_id;
	this->tx_msg.cmd_id = set_cur_setpoint;
	this->tx_msg.socket_can.can_id = (this->tx_msg.cmd_id & this->tx_msg.node_id << 5);
	this->tx_msg.socket_can.data[0] = cur_setpoint/0.01 & BIT_MASK_0;
	this->tx_msg.socket_can.data[1] = cur_setpoint/0.01 & BIT_MASK_1;
	this->tx_msg.socket_can.data[2] = cur_setpoint/0.01 & BIT_MASK_2;
	this->tx_msg.socket_can.data[3] = cur_setpoint/0.01 & BIT_MASK_3;	
	
}

void controller::set_vel_limit(uint32_t node_id, float vel_limit)
{
	this->tx_msg.node_id = node_id;
	this->tx_msg.cmd_id = set_vel_limit;
	this->tx_msg.socket_can.can_id = (this->tx_msg.cmd_id & this->tx_msg.node_id << 5);
	this->tx_msg.socket_can.data[0] = vel_limit & BIT_MASK_0;
	this->tx_msg.socket_can.data[1] = vel_limit & BIT_MASK_1;
	this->tx_msg.socket_can.data[2] = vel_limit & BIT_MASK_2;
	this->tx_msg.socket_can.data[3] = vel_limit & BIT_MASK_3;	
	
}
void controller::start_anticogging(uint32_t node_id)
{
	this->tx_msg.node_id = node_id;
	this->tx_msg.cmd_id = start_anticogging;
	this->tx_msg.socket_can.can_id = (this->tx_msg.cmd_id & this->tx_msg.node_id << 5);
	
}
void controller::set_traj_vel_limit(uint32_t node_id float traj_vel_limit)
{
	this->tx_msg.node_id = node_id;
	this->tx_msg.cmd_id = set_traj_vel_limit;
	this->tx_msg.socket_can.can_id = (this->tx_msg.cmd_id & this->tx_msg.node_id << 5);

	this->tx_msg.socket_can.data[0] = traj_vel_limit & BIT_MASK_0;
	this->tx_msg.socket_can.data[1] = traj_vel_limit & BIT_MASK_1;
	this->tx_msg.socket_can.data[2] = traj_vel_limit & BIT_MASK_2;
	this->tx_msg.socket_can.data[3] = traj_vel_limit & BIT_MASK_3;	
	
}

void controller::set_traj_accel_limit(uint32_t node_id float accel_limit, float decel_limit)
{
	this->tx_msg.node_id = node_id;
	this->tx_msg.cmd_id = set_traj_accel_limit;
	this->tx_msg.socket_can.can_id = (this->tx_msg.cmd_id & this->tx_msg.node_id << 5);

	this->tx_msg.socket_can.data[0] = accel_limit & BIT_MASK_0;
	this->tx_msg.socket_can.data[1] = accel_limit & BIT_MASK_1;
	this->tx_msg.socket_can.data[2] = accel_limit & BIT_MASK_2;
	this->tx_msg.socket_can.data[3] = accel_limit & BIT_MASK_3;	

	this->tx_msg.socket_can.data[4] = decel_limit & BIT_MASK_0;
	this->tx_msg.socket_can.data[5] = decel_limit & BIT_MASK_1;
	this->tx_msg.socket_can.data[6] = decel_limit & BIT_MASK_2;
	this->tx_msg.socket_can.data[7] = decel_limit & BIT_MASK_3;	
	
}

void controller::set_traj_a_per_css(uint32_t node_id float accel_limit, float traj_a_per_css)
{
	this->tx_msg.node_id = node_id;
	this->tx_msg.cmd_id = set_traj_a_per_css;
	this->tx_msg.socket_can.can_id = (this->tx_msg.cmd_id & this->tx_msg.node_id << 5);

	this->tx_msg.socket_can.data[0] = traj_a_per_css & BIT_MASK_0;
	this->tx_msg.socket_can.data[1] = traj_a_per_css & BIT_MASK_1;
	this->tx_msg.socket_can.data[2] = traj_a_per_css & BIT_MASK_2;
	this->tx_msg.socket_can.data[3] = traj_a_per_css & BIT_MASK_3;		
	
}

void controller::reboot_odrive(uint32_t node_id)
{
	this->tx_msg.node_id = node_id;
	this->tx_msg.cmd_id = reboot_odrive;
	this->tx_msg.socket_can.can_id = (this->tx_msg.cmd_id & this->tx_msg.node_id << 5);		
}

void controller::get_vbus_voltage(uint32_t node_id)
{
	this->tx_msg.node_id = node_id;
	this->tx_msg.cmd_id = get_vbus_voltage;
	this->tx_msg.socket_can.can_id = (this->tx_msg.cmd_id & this->tx_msg.node_id << 5);
	
}
void controller::set_vel_pi_gain(uint32_t node_id float vel_p_gain, float vel_i_gain)
{
	this->tx_msg.node_id = node_id;
	this->tx_msg.cmd_id = set_vel_pi_gain;
	this->tx_msg.socket_can.can_id = (this->tx_msg.cmd_id & this->tx_msg.node_id << 5);

	this->tx_msg.socket_can.data[0] = vel_p_gain & BIT_MASK_0;
	this->tx_msg.socket_can.data[1] = vel_p_gain & BIT_MASK_1;
	this->tx_msg.socket_can.data[2] = vel_p_gain & BIT_MASK_2;
	this->tx_msg.socket_can.data[3] = vel_p_gain & BIT_MASK_3;

	this->tx_msg.socket_can.data[4] = vel_i_gain & BIT_MASK_0;
	this->tx_msg.socket_can.data[5] = vel_i_gain & BIT_MASK_1;
	this->tx_msg.socket_can.data[6] = vel_i_gain & BIT_MASK_2;
	this->tx_msg.socket_can.data[7] = vel_i_gain & BIT_MASK_3;			
	
}
	

	


		






int controller::can_write()
{	
	int nbytes;
	nbytes = write(this->socket_file_handler, this->tx_msg, sizeof(struct can_frame));
	printf("Wrote %d bytes\n", nbytes);
	return nbytes;

}







	

	

	
	
	



	
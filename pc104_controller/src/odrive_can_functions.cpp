/* Created by Sumantra on 9th December 2019 */


#include "misc_functions.hpp"

/*CAN READ AND WRITE FUNCTIONS*/

controller::controller()
{
	
	/*initialize rx and tx can messages structs*/
	for (int i = 0; i < 7; ++i)
	{
		this->tx_msg.socket_can.data[i] = 0;
		this->rx_msg.socket_can.data[i] = 0;
	}
	this->tx_msg.socket_can.can_dlc = 8; 
	this->rx_msg.socket_can.can_dlc = 8; 
	this->signit_handler = true;
}

bool controller::can_read()
{	
	int nbytes;
	nbytes = read(this->socket_file_handler, &this->rx_msg, sizeof(struct can_frame));

    /* paranoid check ... */
    if (nbytes < sizeof(struct can_frame)) {
            fprintf(stderr, "read: incomplete CAN frame\n");
            return 1;
    }
	
}
void controller::set_socket(int socket_file_handler)
{
	this->socket_file_handler = socket_file_handler;
}

bool controller::can_write()
{	
	int nbytes;
	nbytes = write(this->socket_file_handler, &this->tx_msg, sizeof(struct can_frame));
	if (nbytes < 0) {
            perror("can raw socket write");
            return 1;
    }

    /* paranoid check ... */
    if (nbytes < sizeof(struct can_frame)) {
            fprintf(stderr, "write: incomplete CAN frame\n");
            return 1;
    }

}

/*MSG HANDLER FUNCTIONS FOR PROCESSING AND ORGANIZING INCOMING MSGS*/

void controller::msg_handler()
{

	bit_masking(this->rx_msg); /*get node id, cmd id*/
	sort_can_node_id(this->legs,this->rx_msg); /*assign node id to respective leg*/
	uint8_t a,b,c,d,e,f,g,h; /*temp storage variale for the CAN data packets*/
	int leg_no, type_no; /*temp storage variables for leg_no and type_no*/

	leg_no = this->rx_msg.idn.leg_no;
	type_no = this->rx_msg.idn.type_no;

	a = this->rx_msg.socket_can.data[0];
	b = this->rx_msg.socket_can.data[1];
	c = this->rx_msg.socket_can.data[2];
	d = this->rx_msg.socket_can.data[3];

	e = this->rx_msg.socket_can.data[4];
	f = this->rx_msg.socket_can.data[5];
	g = this->rx_msg.socket_can.data[6];
	h = this->rx_msg.socket_can.data[7];

	/*read the received can frame*/
	switch(this->rx_msg.cmd_id)
	{
		case '1': /*hearbeat message*/
			this->legs[leg_no][type_no].axis_error = (a | b << 8 | c << 16 | d << 24);
			this->legs[leg_no][type_no].axis_current_state = (e | f << 8 | g << 16 | h << 24);
			break;
		case '3': /*get motor error*/
			this->legs[leg_no][type_no].motor_error = (a | b << 8 | c << 16 | d << 24);
			break;
		case '4': /*get encoder error*/
			this->legs[leg_no][type_no].encoder_error = (e | f << 8 | g << 16 | h << 24);;
			break;
		case '9': /*get encoder estimates*/
			bytes2Float(&a, &this->legs[leg_no][type_no].encoder_pos_estimate);/*FLOAT*/
			bytes2Float(&e, &this->legs[leg_no][type_no].encoder_vel_estimate ); /*FLOAT*/
			break;
		case '10': /*get encoder counts*/
			this->legs[leg_no][type_no].encoder_shadow_count = (a | b << 8 | c << 16 | d << 24); /*SIGNED INT*/
			this->legs[leg_no][type_no].encoder_count_in_cpr =(e | f << 8 | g << 16 | h << 24); /*SIGNED INT*/
			break;
		case '20': /*get IQ*/
			 bytes2Float(&a, &this->legs[leg_no][type_no].iq_setpoint ); /*FLOAT*/
			 bytes2Float(&e, &this->legs[leg_no][type_no].iq_measured ); /*FLOAT*/
			break;
		case '23':/*vbus voltage*/
			 bytes2Float(&a, &this->legs[leg_no][type_no].vbus_voltage); /*FLOAT*/
			break;
		default:
			cout << "Failed to read incoming CAN frame" << endl;

	}

	/*error detection and handling*/

}	


/*PTHREAD RELATED FUNCTIONS*/

void* controller::internal_thread_function()
{
	while(signit_handler)
	{
		pthread_mutex_lock(&this->mutex_lock);
		this->can_read();
		this->msg_handler();
		pthread_mutex_unlock(&this->mutex_lock);

	}

}
	
void controller::set_internal_thread(pthread_t &thread)
{
	this->thread = thread;
}

bool controller::start_internal_thread()
{
	return (pthread_create(&this->thread, NULL, InternalThreadEntryFunc, NULL) == 0); /*complete this later on*/
}
void controller::set_mutex_lock(pthread_mutex_t &lock)
{
	this->mutex_lock = lock;
}

void * controller::InternalThreadEntryFunc(void * This)
{
	((controller *)This)->internal_thread_function();
	return NULL;

}

odrive_motor controller::get_motor_data(int x, int y)
 {
 	if( x<0 || x>3 || y<0 || y>2)
 	{
 		cout << " Invalid access" << endl ;
 	}
 	return this->legs[x][y];
 }


/*IMPLEMENTATION OF ALL ODRIVE FUNCTIONS EXCEPT FOR REQUEST MSGS*/

void controller::estop(uint32_t node_id)
{
	this->tx_msg.node_id = node_id;
	this->tx_msg.cmd_id = estop_cmd;
	this->tx_msg.socket_can.can_id = (this->tx_msg.cmd_id | this->tx_msg.node_id << 5);
}
void controller::set_axis_node_id(uint32_t node_id, uint16_t axis_can_node_id)
{
	this->tx_msg.node_id = node_id;
	this->tx_msg.cmd_id = set_axis_node_id_cmd;
	this->tx_msg.socket_can.can_id = (this->tx_msg.cmd_id | this->tx_msg.node_id << 5);
	this->tx_msg.socket_can.data[0] = axis_can_node_id & BIT_MASK_0;
	this->tx_msg.socket_can.data[1] = (axis_can_node_id & BIT_MASK_1) >>8;
	
}
void controller::set_axis_requested_state(uint32_t node_id, uint32_t axis_requested_state)
{
	this->tx_msg.node_id = node_id;
	this->tx_msg.cmd_id = set_axis_requested_state_cmd;
	this->tx_msg.socket_can.can_id = (this->tx_msg.cmd_id | this->tx_msg.node_id << 5);
	this->tx_msg.socket_can.data[0] = axis_requested_state & BIT_MASK_0;
	this->tx_msg.socket_can.data[1] = (axis_requested_state & BIT_MASK_1) << 8;
	this->tx_msg.socket_can.data[2] = (axis_requested_state & BIT_MASK_2) << 16;
	this->tx_msg.socket_can.data[3] = (axis_requested_state & BIT_MASK_3) << 24;	
	
}

void controller::move_to_pos(uint32_t node_id, int32_t pos)
{
	this->tx_msg.node_id = node_id;
	this->tx_msg.cmd_id = move_to_pos_cmd;
	this->tx_msg.socket_can.can_id = (this->tx_msg.cmd_id | this->tx_msg.node_id << 5);
	this->tx_msg.socket_can.data[0] = pos & BIT_MASK_0;
	this->tx_msg.socket_can.data[1] = (pos & BIT_MASK_1) << 8;
	this->tx_msg.socket_can.data[2] = (pos & BIT_MASK_2) << 16;
	this->tx_msg.socket_can.data[3] = (pos & BIT_MASK_3) << 24;	
}

void controller::set_pos_setpoint(uint32_t node_id, int32_t pos_setpoint, int16_t vel_ff,int16_t current_ff)
{
	this->tx_msg.node_id = node_id;
	this->tx_msg.cmd_id = set_pos_setpoint_cmd;
	this->tx_msg.socket_can.can_id = (this->tx_msg.cmd_id | this->tx_msg.node_id << 5);
	this->tx_msg.socket_can.data[0] = (pos_setpoint & BIT_MASK_0);
	this->tx_msg.socket_can.data[1] = (pos_setpoint & BIT_MASK_1) << 8;
	this->tx_msg.socket_can.data[2] = (pos_setpoint & BIT_MASK_2) << 16;
	this->tx_msg.socket_can.data[3] = (pos_setpoint & BIT_MASK_3) << 24;	

	this->tx_msg.socket_can.data[4] = ((vel_ff*10) & BIT_MASK_0);	
	this->tx_msg.socket_can.data[5] = ((vel_ff*10) & BIT_MASK_1) << 8;	
	
	this->tx_msg.socket_can.data[6] = ((current_ff*100) & BIT_MASK_0);	
	this->tx_msg.socket_can.data[7] = ((current_ff*100) & BIT_MASK_1) << 8;	
	
}

void controller::set_vel_setpoint(uint32_t node_id, int32_t vel_setpoint , int16_t current_ff)
{
	this->tx_msg.node_id = node_id;
	this->tx_msg.cmd_id = set_vel_setpoint_cmd;
	this->tx_msg.socket_can.can_id = (this->tx_msg.cmd_id | this->tx_msg.node_id << 5);
	this->tx_msg.socket_can.data[0] = ((vel_setpoint*100) & BIT_MASK_0);
	this->tx_msg.socket_can.data[1] = ((vel_setpoint*100) & BIT_MASK_1) << 8;
	this->tx_msg.socket_can.data[2] = ((vel_setpoint*100) & BIT_MASK_2) << 16;
	this->tx_msg.socket_can.data[3] = ((vel_setpoint*100) & BIT_MASK_3) << 24;

	this->tx_msg.socket_can.data[4] = ((current_ff*100) & BIT_MASK_0);
	this->tx_msg.socket_can.data[5] = ((current_ff*100) & BIT_MASK_1) << 8;

	
	
}

void controller::set_cur_setpoint(uint32_t node_id, int32_t cur_setpoint)
{
	this->tx_msg.node_id = node_id;
	this->tx_msg.cmd_id = set_cur_setpoint_cmd;
	this->tx_msg.socket_can.can_id = (this->tx_msg.cmd_id | this->tx_msg.node_id << 5);
	this->tx_msg.socket_can.data[0] = ((cur_setpoint*100) & BIT_MASK_0);
	this->tx_msg.socket_can.data[1] = ((cur_setpoint*100) & BIT_MASK_1) << 8;
	this->tx_msg.socket_can.data[2] = ((cur_setpoint*100) & BIT_MASK_2) << 16;
	this->tx_msg.socket_can.data[3] = ((cur_setpoint*100) & BIT_MASK_3) << 24;	
	
}

void controller::set_vel_limit(uint32_t node_id, float vel_limit)  /*FLOAT*/
{
	this->tx_msg.node_id = node_id;
	this->tx_msg.cmd_id = set_vel_limit_cmd;
	this->tx_msg.socket_can.can_id = (this->tx_msg.cmd_id | this->tx_msg.node_id << 5);

	float2Bytes(vel_limit, this->tx_msg.socket_can.data);
	rvereseArray(this->tx_msg.socket_can.data, 0, 3);
	// this->tx_msg.socket_can.data[0] = (vel_limit & BIT_MASK_0);
	// this->tx_msg.socket_can.data[1] = (vel_limit & BIT_MASK_1) << 8;
	// this->tx_msg.socket_can.data[2] = (vel_limit & BIT_MASK_2) << 16;
	// this->tx_msg.socket_can.data[3] = (vel_limit & BIT_MASK_3) << 24;	
	
}
void controller::start_anticogging(uint32_t node_id)
{
	this->tx_msg.node_id = node_id;
	this->tx_msg.cmd_id = start_anti_cogging_cmd;
	this->tx_msg.socket_can.can_id = (this->tx_msg.cmd_id | this->tx_msg.node_id << 5);
	
}
void controller::set_traj_vel_limit(uint32_t node_id , float traj_vel_limit) /*FLOAT*/
{
	this->tx_msg.node_id = node_id;
	this->tx_msg.cmd_id = set_traj_vel_limit_cmd;
	this->tx_msg.socket_can.can_id = (this->tx_msg.cmd_id | this->tx_msg.node_id << 5);

	float2Bytes(traj_vel_limit, this->tx_msg.socket_can.data);
	rvereseArray(this->tx_msg.socket_can.data, 0, 3);

	// this->tx_msg.socket_can.data[0] = (traj_vel_limit & BIT_MASK_0);
	// this->tx_msg.socket_can.data[1] = (traj_vel_limit & BIT_MASK_1) << 8;
	// this->tx_msg.socket_can.data[2] = (traj_vel_limit & BIT_MASK_2) << 16;
	// this->tx_msg.socket_can.data[3] = (traj_vel_limit & BIT_MASK_3) << 24;	
	
}

void controller::set_traj_accel_limit(uint32_t node_id , float accel_limit, float decel_limit)  /*FLOAT*/
{
	
	this->tx_msg.node_id = node_id;
	this->tx_msg.cmd_id = set_traj_accel_limit_cmd;
	this->tx_msg.socket_can.can_id = (this->tx_msg.cmd_id | this->tx_msg.node_id << 5);

	float2Bytes(accel_limit, this->tx_msg.socket_can.data);
	rvereseArray(this->tx_msg.socket_can.data, 0, 3);
	float2Bytes(decel_limit, &this->tx_msg.socket_can.data[4]);
	rvereseArray(this->tx_msg.socket_can.data, 4, 7);


	// this->tx_msg.socket_can.data[0] = (accel_limit & BIT_MASK_0);
	// this->tx_msg.socket_can.data[1] = (accel_limit & BIT_MASK_1) << 8;
	// this->tx_msg.socket_can.data[2] = (accel_limit & BIT_MASK_2) << 16;
	// this->tx_msg.socket_can.data[3] = (accel_limit & BIT_MASK_3) << 24;	

	// this->tx_msg.socket_can.data[4] = (decel_limit & BIT_MASK_0);
	// this->tx_msg.socket_can.data[5] = (decel_limit & BIT_MASK_1) << 8;
	// this->tx_msg.socket_can.data[6] = (decel_limit & BIT_MASK_2) << 16;
	// this->tx_msg.socket_can.data[7] = (decel_limit & BIT_MASK_3) << 24;	
	
	
}

void controller::set_traj_a_per_css(uint32_t node_id , float traj_a_per_css) /*FLOAT*/
{
	
	this->tx_msg.node_id = node_id;
	this->tx_msg.cmd_id = set_traj_a_per_css_cmd;
	this->tx_msg.socket_can.can_id = (this->tx_msg.cmd_id | this->tx_msg.node_id << 5);

	float2Bytes(traj_a_per_css, this->tx_msg.socket_can.data);
	rvereseArray(this->tx_msg.socket_can.data, 0, 3);

	// this->tx_msg.socket_can.data[0] = (traj_a_per_css & BIT_MASK_0);
	// this->tx_msg.socket_can.data[1] = (traj_a_per_css & BIT_MASK_1) << 8;
	// this->tx_msg.socket_can.data[2] = (traj_a_per_css & BIT_MASK_2) << 16;
	// this->tx_msg.socket_can.data[3] = (traj_a_per_css & BIT_MASK_3) << 24;	
			
	
}

void controller::reboot_odrive(uint32_t node_id)
{
	
	this->tx_msg.node_id = node_id;
	this->tx_msg.cmd_id = reboot_odrive_cmd;
	this->tx_msg.socket_can.can_id = (this->tx_msg.cmd_id | this->tx_msg.node_id << 5);	
		
}

void controller::get_vbus_voltage(uint32_t node_id)
{
	
	this->tx_msg.node_id = node_id;
	this->tx_msg.cmd_id = get_vbus_voltage_cmd;
	this->tx_msg.socket_can.can_id = (this->tx_msg.cmd_id | this->tx_msg.node_id << 5);
		
	
}
void controller::set_vel_pi_gain(uint32_t node_id, float vel_p_gain, float vel_i_gain)  /*FLOAT*/
{
	
	this->tx_msg.node_id = node_id;
	this->tx_msg.cmd_id = set_vel_pi_gain_cmd;
	this->tx_msg.socket_can.can_id = (this->tx_msg.cmd_id | this->tx_msg.node_id << 5);

	float2Bytes(vel_p_gain, this->tx_msg.socket_can.data);
	rvereseArray(this->tx_msg.socket_can.data, 0, 3);
	float2Bytes(vel_i_gain, &this->tx_msg.socket_can.data[4]);
	rvereseArray(this->tx_msg.socket_can.data, 4, 7);


	// this->tx_msg.socket_can.data[0] = (vel_p_gain & BIT_MASK_0);
	// this->tx_msg.socket_can.data[1] = (vel_p_gain & BIT_MASK_1) << 8;
	// this->tx_msg.socket_can.data[2] = (vel_p_gain & BIT_MASK_2) << 16;
	// this->tx_msg.socket_can.data[3] = (vel_p_gain & BIT_MASK_3) << 24;

	// this->tx_msg.socket_can.data[4] = (vel_i_gain & BIT_MASK_0);
	// this->tx_msg.socket_can.data[5] = (vel_i_gain & BIT_MASK_1) << 8;
	// this->tx_msg.socket_can.data[6] = (vel_i_gain & BIT_MASK_2) << 16;
	// this->tx_msg.socket_can.data[7] = (vel_i_gain & BIT_MASK_3) << 24;			
	
}











	

	

	
	
	



	
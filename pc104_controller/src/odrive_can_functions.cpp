/* Created by Sumantra on 9th December 2019 */
/* Ver 1.1 Major bug fix by Chang Hong 10 Dec 2019 */


#include "misc_functions.hpp"

/*CAN READ AND WRITE FUNCTIONS*/

controller::controller(int socket_fd)
{
	
	/*initialize rx and tx can messages structs*/
	for (int i = 0; i < 7; ++i)
	{
		this->tx_msg.cframe.data[i] = 0;
		this->rx_msg.cframe.data[i] = 0;
	}
	this->tx_msg.cframe.can_dlc = 8; 
	this->rx_msg.cframe.can_dlc = 8; 
	socket_file_handler = socket_fd;
	
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

bool controller::can_write()
{	
	int nbytes;
	printf("can_write %d:",socket_file_handler);
	
	nbytes = write(this->socket_file_handler, &(tx_msg.cframe), sizeof(struct can_frame));
	if (nbytes < 0) {
            perror("can raw socket write");
            return 1;
    }

    /* paranoid check ... */
    if (nbytes < sizeof(struct can_frame)) {
            perror("write: incomplete CAN frame\n");
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

	a = this->rx_msg.cframe.data[0];
	b = this->rx_msg.cframe.data[1];
	c = this->rx_msg.cframe.data[2];
	d = this->rx_msg.cframe.data[3];

	e = this->rx_msg.cframe.data[4];
	f = this->rx_msg.cframe.data[5];
	g = this->rx_msg.cframe.data[6];
	h = this->rx_msg.cframe.data[7];

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
	this->tx_msg.cmd_id = ESTOP;
	this->tx_msg.cframe.can_id = (this->tx_msg.cmd_id | this->tx_msg.node_id << 5);
	can_write();
}
void controller::set_axis_node_id(uint32_t node_id, uint16_t axis_can_node_id)
{
	this->tx_msg.node_id = node_id;
	this->tx_msg.cmd_id = SET_AXIS_NODE_ID;
	this->tx_msg.cframe.can_id = (this->tx_msg.cmd_id | this->tx_msg.node_id << 5);
	this->tx_msg.cframe.data[0] = axis_can_node_id & BIT_MASK_0;
	this->tx_msg.cframe.data[1] = (axis_can_node_id & BIT_MASK_1) >>8;
	can_write();
}
void controller::set_axis_requested_state(uint32_t node_id, uint32_t axis_requested_state)
{
	/*
		AXIS_STATE_UNDEFINED = 0
		AXIS_STATE_IDLE = 1
		AXIS_STATE_STARTUP_SEQUENCE = 2
		AXIS_STATE_FULL_CALIBRATION_SEQUENCE = 3
		AXIS_STATE_MOTOR_CALIBRATION = 4
		AXIS_STATE_SENSORLESS_CONTROL = 5
		AXIS_STATE_ENCODER_INDEX_SEARCH = 6
		AXIS_STATE_ENCODER_OFFSET_CALIBRATION = 7
		AXIS_STATE_CLOSED_LOOP_CONTROL = 8
		AXIS_STATE_LOCKIN_SPIN = 9
		AXIS_STATE_ENCODER_DIR_FIND = 10
	*/

	tx_msg.node_id = node_id;
	tx_msg.cmd_id = SET_AXIS_REQUESTED_STATE;
	tx_msg.cframe.can_id = (tx_msg.cmd_id | tx_msg.node_id << 5);
	tx_msg.cframe.data[0] = axis_requested_state & BIT_MASK_0;
	tx_msg.cframe.data[1] = ((axis_requested_state  & BIT_MASK_1 )>> 8 );
	tx_msg.cframe.data[2] = ((axis_requested_state  & BIT_MASK_2 )>> 16);
	tx_msg.cframe.data[3] = ((axis_requested_state  & BIT_MASK_3 )>> 24);	
	can_write();
}

void controller::move_to_pos(uint32_t node_id, int32_t pos)
{
	this->tx_msg.node_id = node_id;
	this->tx_msg.cmd_id = MOVE_TO_POS;
	this->tx_msg.cframe.can_id = (this->tx_msg.cmd_id | this->tx_msg.node_id << 5);
	this->tx_msg.cframe.data[0] = pos & BIT_MASK_0;
	this->tx_msg.cframe.data[1] = (pos & BIT_MASK_1) >> 8;
	this->tx_msg.cframe.data[2] = (pos & BIT_MASK_2) >> 16;
	this->tx_msg.cframe.data[3] = (pos & BIT_MASK_3) >> 24;	
	can_write();
}

void controller::set_pos_setpoint(uint32_t node_id, int32_t pos_setpoint, int16_t vel_ff,int16_t current_ff)
{
	this->tx_msg.node_id = node_id;
	this->tx_msg.cmd_id = SET_POS_SETPOINT;
	this->tx_msg.cframe.can_id = (this->tx_msg.cmd_id | this->tx_msg.node_id << 5);
	this->tx_msg.cframe.data[0] = (pos_setpoint & BIT_MASK_0);
	this->tx_msg.cframe.data[1] = (pos_setpoint & BIT_MASK_1) >> 8;
	this->tx_msg.cframe.data[2] = (pos_setpoint & BIT_MASK_2) >> 16;
	this->tx_msg.cframe.data[3] = (pos_setpoint & BIT_MASK_3) >> 24;	

	this->tx_msg.cframe.data[4] = ((vel_ff*10) & BIT_MASK_0);	
	this->tx_msg.cframe.data[5] = ((vel_ff*10) & BIT_MASK_1) >> 8;	
	
	this->tx_msg.cframe.data[6] = ((current_ff*100) & BIT_MASK_0);	
	this->tx_msg.cframe.data[7] = ((current_ff*100) & BIT_MASK_1) >> 8;	
	can_write();
	
}

void controller::set_vel_setpoint(uint32_t node_id, int32_t vel_setpoint , int16_t current_ff)
{
	this->tx_msg.node_id = node_id;
	this->tx_msg.cmd_id = SET_VEL_SETPOINT;
	this->tx_msg.cframe.can_id = (this->tx_msg.cmd_id | this->tx_msg.node_id << 5);
	this->tx_msg.cframe.data[0] = ((vel_setpoint*100) & BIT_MASK_0);
	this->tx_msg.cframe.data[1] = ((vel_setpoint*100) & BIT_MASK_1) >> 8;
	this->tx_msg.cframe.data[2] = ((vel_setpoint*100) & BIT_MASK_2) >> 16;
	this->tx_msg.cframe.data[3] = ((vel_setpoint*100) & BIT_MASK_3) >> 24;

	this->tx_msg.cframe.data[4] = ((current_ff*100) & BIT_MASK_0);
	this->tx_msg.cframe.data[5] = ((current_ff*100) & BIT_MASK_1) >> 8;
	can_write();
}

void controller::set_cur_setpoint(uint32_t node_id, int32_t cur_setpoint)
{
	this->tx_msg.node_id = node_id;
	this->tx_msg.cmd_id = SET_CUR_SETPOINT;
	this->tx_msg.cframe.can_id = (this->tx_msg.cmd_id | this->tx_msg.node_id << 5);
	this->tx_msg.cframe.data[0] = ((cur_setpoint*100) & BIT_MASK_0);
	this->tx_msg.cframe.data[1] = ((cur_setpoint*100) & BIT_MASK_1) >> 8;
	this->tx_msg.cframe.data[2] = ((cur_setpoint*100) & BIT_MASK_2) >> 16;
	this->tx_msg.cframe.data[3] = ((cur_setpoint*100) & BIT_MASK_3) >> 24;
	can_write();	
	
}

void controller::set_vel_limit(uint32_t node_id, float vel_limit)  /*FLOAT*/
{
	this->tx_msg.node_id = node_id;
	this->tx_msg.cmd_id = SET_VEL_LIMIT;
	this->tx_msg.cframe.can_id = (this->tx_msg.cmd_id | this->tx_msg.node_id << 5);
		
	float2Bytes(vel_limit, tx_msg.cframe.data);
	printf("inside class to %x of size %d of socket %d:cframe.data is %x %x %x %x\n", tx_msg.cframe.can_id, tx_msg.cframe.can_dlc, socket_file_handler, tx_msg.cframe.data[0], tx_msg.cframe.data[1], tx_msg.cframe.data[2],tx_msg.cframe.data[3]);
  	can_write();
}
void controller::start_anticogging(uint32_t node_id)
{
	this->tx_msg.node_id = node_id;
	this->tx_msg.cmd_id = START_ANTI_COGGING;
	this->tx_msg.cframe.can_id = (this->tx_msg.cmd_id | this->tx_msg.node_id << 5);
	
	can_write();	
}
void controller::set_traj_vel_limit(uint32_t node_id , float traj_vel_limit) /*FLOAT*/
{
	this->tx_msg.node_id = node_id;
	this->tx_msg.cmd_id = SET_TRAJ_VEL_LIMIT;
	this->tx_msg.cframe.can_id = (this->tx_msg.cmd_id | this->tx_msg.node_id << 5);

	float2Bytes(traj_vel_limit, this->tx_msg.cframe.data);
	
	can_write();	
}

void controller::set_traj_accel_limit(uint32_t node_id , float accel_limit, float decel_limit)  /*FLOAT*/
{
	u_int8_t bytes_temp[4];

	this->tx_msg.node_id = node_id;
	this->tx_msg.cmd_id = SET_TRAJ_ACCEL_LIMIT;
	this->tx_msg.cframe.can_id = (this->tx_msg.cmd_id | this->tx_msg.node_id << 5);

	float2Bytes(accel_limit, this->tx_msg.cframe.data);
	float2Bytes(decel_limit, &(this->tx_msg.cframe.data[4]));

	can_write();	

}

void controller::set_traj_a_per_css(uint32_t node_id , float traj_a_per_css) /*FLOAT*/
{
	
	this->tx_msg.node_id = node_id;
	this->tx_msg.cmd_id = SET_TRAJ_A_PER_CSS;
	this->tx_msg.cframe.can_id = (this->tx_msg.cmd_id | this->tx_msg.node_id << 5);

	float2Bytes(traj_a_per_css, this->tx_msg.cframe.data);
			
	can_write();	
	
}

void controller::reboot_odrive(uint32_t node_id)
{
	
	this->tx_msg.node_id = node_id;
	this->tx_msg.cmd_id = REBOOT_ODRIVE;
	this->tx_msg.cframe.can_id = (this->tx_msg.cmd_id | this->tx_msg.node_id << 5);	
		
	can_write();	
}

void controller::get_vbus_voltage(uint32_t node_id)
{
	
	this->tx_msg.node_id = node_id;
	this->tx_msg.cmd_id = GET_VBUS_VOLTAGE;
	this->tx_msg.cframe.can_id = (this->tx_msg.cmd_id | this->tx_msg.node_id << 5);
		
	can_write();	
}
void controller::set_vel_pi_gain(uint32_t node_id, float vel_p_gain, float vel_i_gain)  /*FLOAT*/
{
	
	this->tx_msg.node_id = node_id;
	this->tx_msg.cmd_id = SET_VEL_PI_GAIN;
	this->tx_msg.cframe.can_id = (this->tx_msg.cmd_id | this->tx_msg.node_id << 5);

	float2Bytes(vel_p_gain, this->tx_msg.cframe.data);
	float2Bytes(vel_i_gain, &(this->tx_msg.cframe.data[4]));

	can_write();	
}







	

	

	
	
	



	
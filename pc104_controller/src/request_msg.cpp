#include<request_msg.hpp>



void request_msg::start_internal_thread()
{
	return (pthread_create(&this->thread NULL, &internal_thread_function, NULL) == 0);
}


void request_msg::request_msg()
{
	this->write_msg.socket_can.can_dlc = 8;

}

void* request_msg::internal_thread_function()
{
	while(signit_handler)
	{
		for (int i = 1; i < 13; ++i)
		{
			this->get_motor_error(i);
			this->get_encoder_error(i);
			this->get_encoder_estimate(i);
			this->get_encoder_counts(i);
			this->get_iq_values(i);
			this->get_vbus_voltage(i);
		}
	}
}

void request_msg::get_motor_error(uint32_t node_id)
{
	this->write_msg.node_id = node_id;
	this->write_msg.cmd_id = get_motor_error;
	this->write_msg.socket_can.can_id = (this->write_msg.cmd_id & this->write_msg.node_id << 5 & rtr<<30);
	this->write_msg();
}

void request_msg::get_encoder_error(uint32_t node_id)
{
	this->write_msg.node_id = node_id;
	this->write_msg.cmd_id = get_encoder_error;
	this->write_msg.socket_can.can_id = (this->write_msg.cmd_id & this->write_msg.node_id << 5 & rtr<<30);
	this->write_msg();
}

void request_msg::get_encoder_estimate(uint32_t node_id)
{
	this->write_msg.node_id = node_id;
	this->write_msg.cmd_id = get_encoder_estimate;
	this->write_msg.socket_can.can_id = (this->write_msg.cmd_id & this->write_msg.node_id << 5 & rtr<<30);
	this->write_msg();
}

void request_msg::get_encoder_counts(uint32_t node_id)
{
	this->write_msg.node_id = node_id;
	this->write_msg.cmd_id = get_encoder_counts;
	this->write_msg.socket_can.can_id = (this->write_msg.cmd_id & this->write_msg.node_id << 5 & rtr<<30);
	this->write_msg();
}

void request_msg::get_iq_values(uint32_t node_id)
{
	this->write_msg.node_id = node_id;
	this->write_msg.cmd_id = get_iq_values;
	this->write_msg.socket_can.can_id = (this->write_msg.cmd_id & this->write_msg.node_id << 5 & rtr<<30);
	this->write_msg();
}
void request_msg::get_vbus_voltage(uint32_t node_id)
{
	this->write_msg.node_id = node_id;
	this->write_msg.cmd_id = get_vbus_voltage;
	this->write_msg.socket_can.can_id = (this->write_msg.cmd_id & this->write_msg.node_id << 5 & rtr<<30);
	this->write_msg();
}


void request_msg::write_msg()
{
	int nbytes;
	nbytes = write(this->socket_handler, this->write_msg, sizeof(struct can_frame));
	printf("Wrote %d bytes requesting data from ODrive\n", nbytes);
	return nbytes;
	

}
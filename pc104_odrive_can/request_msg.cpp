#include<request_msg.hpp>

void request_msg::request_msg()
{
	this->write_msg.socket_can.can_dlc = 8;


}

void request_msg::get_motor_error(uint32_t node_id)
{
	this->write_msg.node_id = node_id;
	this->write_msg.cmd_id = get_motor_error;
	this->write_msg.socket_can.can_id = (this->write_msg.cmd_id & this->write_msg.node_id << 5 & rtr<<30);
}

void request_msg::get_encoder_error(uint32_t node_id)
{
	this->write_msg.node_id = node_id;
	this->write_msg.cmd_id = get_encoder_error;
	this->write_msg.socket_can.can_id = (this->write_msg.cmd_id & this->write_msg.node_id << 5 & rtr<<30);
}

void request_msg::get_encoder_estimate(uint32_t node_id)
{
	this->write_msg.node_id = node_id;
	this->write_msg.cmd_id = get_encoder_estimate;
	this->write_msg.socket_can.can_id = (this->write_msg.cmd_id & this->write_msg.node_id << 5 & rtr<<30);
}

void request_msg::get_encoder_counts(uint32_t node_id)
{
	this->write_msg.node_id = node_id;
	this->write_msg.cmd_id = get_encoder_counts;
	this->write_msg.socket_can.can_id = (this->write_msg.cmd_id & this->write_msg.node_id << 5 & rtr<<30);
}

void request_msg::get_iq_values(uint32_t node_id)
{
	this->write_msg.node_id = node_id;
	this->write_msg.cmd_id = get_iq_values;
	this->write_msg.socket_can.can_id = (this->write_msg.cmd_id & this->write_msg.node_id << 5 & rtr<<30);
}


void request_msg::write_msg()
{
	int nbytes;
	nbytes = write(this->socket_handler, this->write_msg, sizeof(struct can_frame));
	printf("Wrote %d bytes requesting data from ODrive\n", nbytes);
	return nbytes;
	

}
#include"request_msg.hpp"

request_msg::request_msg()
{
	this->write_msg.cframe.can_dlc = 8;

}

void request_msg::get_motor_error(uint32_t node_id)
{
	this->write_msg.node_id = node_id;
	this->write_msg.cmd_id = GET_MOTOR_ERROR;
	this->write_msg.cframe.can_id = (this->write_msg.cmd_id | this->write_msg.node_id << 5 | RTR<<30);
	this->send_msg();
}

void request_msg::get_encoder_error(uint32_t node_id)
{
	this->write_msg.node_id = node_id;
	this->write_msg.cmd_id = GET_ENCODER_ERROR;
	this->write_msg.cframe.can_id = (this->write_msg.cmd_id | this->write_msg.node_id << 5 | RTR<<30);
	this->send_msg();
}

void request_msg::get_encoder_estimate(uint32_t node_id)
{
	this->write_msg.node_id = node_id;
	this->write_msg.cmd_id = GET_ENCODER_ESTIMATE;
	this->write_msg.cframe.can_id = (this->write_msg.cmd_id | this->write_msg.node_id << 5 | RTR<<30);
	this->send_msg();
}

void request_msg::get_encoder_counts(uint32_t node_id)
{
	this->write_msg.node_id = node_id;
	this->write_msg.cmd_id = GET_ENCODER_COUNTS;
	this->write_msg.cframe.can_id = (this->write_msg.cmd_id | this->write_msg.node_id << 5 | RTR<<30);
	this->send_msg();
}

void request_msg::get_iq_values(uint32_t node_id)
{
	this->write_msg.node_id = node_id;
	this->write_msg.cmd_id = GET_IQ_VALUES;
	this->write_msg.cframe.can_id = (this->write_msg.cmd_id | this->write_msg.node_id << 5 | RTR<<30);
	this->send_msg();
}
void request_msg::get_vbus_voltage(uint32_t node_id)
{
	this->write_msg.node_id = node_id;
	this->write_msg.cmd_id = GET_VBUS_VOLTAGE;
	this->write_msg.cframe.can_id = (this->write_msg.cmd_id | this->write_msg.node_id << 5 | RTR<<30);
	this->send_msg();
}


int request_msg::send_msg()
{
	int nbytes;
	nbytes = write(this->socket_handler, &this->write_msg.cframe, sizeof(struct can_frame));
	printf("Wrote %d bytes requesting data from ODrive\n", nbytes);

	if (nbytes < 0) {
            perror("can raw socket write");
            return 0;
    }

    /* paranoid check ... */
    if (nbytes < sizeof(struct can_frame)) {
            perror("write: incomplete CAN frame\n");
            return 0;
    }
    else if (nbytes > sizeof(struct can_frame)) {
            perror("write failed\n");
            return 0;
    }
    return 1;

}

void request_msg::set_socket(int socket_fd)
{
	this->socket_handler = socket_fd;
}
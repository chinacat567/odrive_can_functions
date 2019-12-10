#include<controller.hpp>
int main()
{
	//create controller object;
	controller ctrl();
	struct sockaddr_can addr; //socket can adress
	struct ifreq ifr; //interface structure

	const char *ifname = "can0";
	ctrl.socket_file_handler = socket(PF_CAN, SOCK_RAW, CAN_RAW)

	if((ctrl.socket_file_handler) < 0) {
		perror("Error while opening socket");
		return -1;
	}

	//set the name of the CAN device
	strcpy(ifr.ifr_name, ifname);
	ioctl(ctrl.socket_file_handler, SIOCGIFINDEX, &ifr);
	
	//set properties
	addr.can_family  = AF_CAN;
	addr.can_ifindex = ifr.ifr_ifindex;

	printf("%s at index %d\n", ifname, ifr.ifr_ifindex);

	//bind the socket to the can file handler
	if(bind(ctrl.socket_file_handler, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
		perror("Error in socket bind");
		return -2;
	}


	ctrl.can_read();
	ctrl.can_write();
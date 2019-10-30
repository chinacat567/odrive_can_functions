#include <linux/can/raw.h>
#include<linux/can.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<sys/ioctl.h>
#include<linux/if.h>
#include <stdio.h>
#include<string.h>
#include <unistd.h>
int main()
{
	int sockfd;
	int nbytes;
	struct sockaddr_can addr; //socket can adress
	struct ifreq ifr; //interface structure

	const char *ifname = "vcan0";
	sockfd= socket(PF_CAN, SOCK_RAW, CAN_RAW);

	if(sockfd < 0) {
		perror("Error while opening socket");
		return -1;
	}

	//set the name of the CAN device
	strcpy(ifr.ifr_name, ifname);
	ioctl(sockfd, SIOCGIFINDEX, &ifr);
	
	//set properties
	addr.can_family  = AF_CAN;
	addr.can_ifindex = ifr.ifr_ifindex;

	printf("%s at index %d\n", ifname, ifr.ifr_ifindex);

	//bind the socket to the can file handler
	if(bind(sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
		perror("Error in socket bind");
		return -2;
	}

	can_frame rx_msg;



	nbytes = read(sockfd, &rx_msg, sizeof(struct can_frame));
	printf("Read %d bytes\n", nbytes);

	




}

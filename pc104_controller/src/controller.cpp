#include "controller.hpp"

int main()
{
  //pthread_attr_t attr;
  pthread_t thread,thread2; 
  int ret,i;
  struct threadArg canArg1,canArg2;

  can_init(canArg1,canArg2);
  
  controller ctr1(canArg1.socket,canArg2.socket);
  canArg1.ctrArg = &ctr1;
  canArg2.ctrArg = &ctr1;

  /* Lock memory */
  if(mlockall(MCL_CURRENT|MCL_FUTURE) == -1) {
    printf("mlockall failed: %m\n");
    exit(-2);
  }
											
  set_periodic_task(&ctr1, &thread2, &canArg2, 2, 10000000,80);//read
  set_periodic_task(&ctr1, &thread, &canArg1, 1, 20000000,81);//write


 //ctr1.set_axis_requested_state(1,5);

// Join the thread (ie. freeze THIS parent thread until child is done) 
  ret = pthread_join(thread2, NULL);
  printf("join pthread success: %m\n");
	if (ret)
    printf("join pthread failed: %m\n");

if (ret)
	return 0;

}



int can_init(struct threadArg &canArg1,struct threadArg &canArg2)
//this function initializes the the CAN sockets and frames 
//Some dummy values were included for easy testing

{
	//initialse the can parameters
  	int s,s2,s3;
	int nbytes,nbytes2,nbytes3,nbytes4;
	struct sockaddr_can addrSource,addr1,addr2,addr3;
	struct can_frame frame,frame2,frame3,frame4;
	struct ifreq ifr;
	struct ifreq ifr2;
	struct ifreq ifr3;

	const char *ifname = "can0";
	const char *ifname2 = "can1";
	const char *ifname3 = "can1";
		

	long long InputData,InputData2,InputData3,InputData4;
	uint size, i=0;

	if((s = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0) {
		perror("Error while opening socket");
		return -1;
	}
	//experimenting with 2 separate socket
	if((s2 = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0) {
		perror("Error while opening socket");
		return -1;
	}
	//experimenting with 2 separate socket
	if((s3 = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0) {
		perror("Error while opening socket");
		return -1;
	}
	strcpy(ifr.ifr_name, ifname);
	ioctl(s, SIOCGIFINDEX, &ifr);
	strcpy(ifr2.ifr_name, ifname2);
	ioctl(s2, SIOCGIFINDEX, &ifr2);
	strcpy(ifr3.ifr_name, ifname3);
	ioctl(s3, SIOCGIFINDEX, &ifr3);
	
	addr1.can_family  = AF_CAN;
	addr1.can_ifindex = ifr.ifr_ifindex;
	//addr1.can_ifindex = 0;
	addr2.can_family  = AF_CAN;
	addr2.can_ifindex = ifr2.ifr_ifindex;
	//addr2.can_ifindex = 0;
	addr3.can_family  = AF_CAN;
	addr3.can_ifindex = ifr3.ifr_ifindex;
	//addr2.can_ifindex = 0;

	
	printf("%s at index %d\n", ifname, ifr.ifr_ifindex);
	printf("%s at index %d\n", ifname2, ifr2.ifr_ifindex);
	printf("%s at index %d\n", ifname3, ifr3.ifr_ifindex);

	if(bind(s, (struct sockaddr *)&addr1, sizeof(addr1)) < 0) {
		perror("Error in socket bind");
		return -2;
	}
	if(bind(s2, (struct sockaddr *)&addr2, sizeof(addr2)) < 0) {
		perror("Error in socket bind2");
		return -2;
	}
	if(bind(s3, (struct sockaddr *)&addr3, sizeof(addr3)) < 0) {
		perror("Error in socket bind3");
		return -2;
	}

	frame.can_id  = (0x01<<5) | 0x0f;	
//	InputData = 0xAABBCCDDEEFF0011;
	InputData = 0x4640e400; //this is 12345.0 when converted to float
//	frame2.can_id  = 0x2;	
//	InputData2 = 0x2233445566778899;
//	frame3.can_id  = 0x3;
//	InputData3 = 0xCC;
	frame2.can_id  = (0x01<<5) | 0x0f ; //set vel limit of node 1
	InputData2 = 0x123456789012345;

	//InputData = 0x1234;
	size = sizeof(InputData2);
	if(size>8 || size<0)
	{
			perror("Input data size too big");
			return -2;
	}
	else {
		do {
			frame.data[i] = (InputData & 0xFF) ;
			InputData = InputData >> 8 ;
			frame2.data[i] = (InputData2 & 0xFF) ;
			InputData2 = InputData2 >> 8 ;
			frame3.data[i] = (InputData3 & 0xFF) ;
			InputData3 = InputData3 >> 8 ;
			i++;
		}while (i != size);
	}
	frame.can_dlc = size;
	printf("bytes_arr is %d\n", size);	
	frame2.can_dlc = size;
	//frame3.can_dlc = size;
	frame4.can_dlc = size;


	canArg1.socket= s;
	canArg1.frame= frame;
	//canArg1.socket2= s2;
	//canArg1.frame2= frame2;
	canArg2.socket= s2;
	canArg2.frame= frame2;
	//canArg2->frame2= frame2;
	return 0;
}
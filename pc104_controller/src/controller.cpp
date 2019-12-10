#include<controller.hpp>

int main()
{
 struct sched_param param;
  pthread_attr_t attr;
  pthread_t thread,thread2;
  int ret,i;
  u_int8_t bytes_arr[4];
  struct canArg canArg1,canArg4;

  can_init(&canArg1,&canArg4);

  controller ctr1(canArg1.socket);
  	
  /* Lock memory */
  if(mlockall(MCL_CURRENT|MCL_FUTURE) == -1) {
    printf("mlockall failed: %m\n");
    exit(-2);
  }

  /* Initialize pthread attributes (default values) */
  ret = pthread_attr_init(&attr);
  if (ret) {
    printf("init pthread attributes failed\n");
    
  }

  /* Set a specific stack size  */
  ret = pthread_attr_setstacksize(&attr, PTHREAD_STACK_MIN);
  if (ret) {
    printf("pthread setstacksize failed\n");
    
  }

  /* Set scheduler policy and priority of pthread */
  ret = pthread_attr_setschedpolicy(&attr, SCHED_FIFO);
  if (ret) {
    printf("pthread setschedpolicy failed\n");
    
  }
  param.sched_priority = 80;
  ret = pthread_attr_setschedparam(&attr, &param);
  if (ret) {
    printf("pthread setschedparam failed\n");
    
  }
  /* Use scheduling parameters of attr */
  ret = pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);
  if (ret) {
    printf("pthread setinheritsched failed\n");
    
  }

  /* Create a pthread with specified attributes */
// thread(1) to write data
 /*ret = pthread_create(&thread, &attr, thread_func, &canArg1);
  if (ret) {
    printf("create pthread failed\n");
    
	}*/
//thread2 to continuously read
ret = pthread_create(&thread2, &attr, thread_func2, &canArg4);
  if (ret) {
    printf("create pthread failed\n");
    
  }

  printf("create pthread success\n");


  
  for (i=0; i<10;i++){
 		ctr1.set_vel_limit(1,12345.0);
 		sleep(1);	
  
  }
 //ctr1.set_axis_requested_state(1,5);

// Join the thread (ie. freeze THIS parent thread until child is done) 
  ret = pthread_join(thread2, NULL);
  printf("join pthread success: %m\n");
	if (ret)
    printf("join pthread failed: %m\n");

if (ret)
	return 0;

}

static void inc_period(struct period_info *pinfo)	
{
  pinfo->next_period.tv_nsec += pinfo->period_ns;

  while (pinfo->next_period.tv_nsec >= 1000000000) {
    /* timespec nsec overflow */
    pinfo->next_period.tv_sec++;
    pinfo->next_period.tv_nsec -= 1000000000;
  }
}

static void periodic_task_init(struct period_info *pinfo)
{
  /* for simplicity, hardcoding a 1ms period */
	// note to self 200000 periods is good for 
  pinfo->period_ns = 100000000; 
  clock_gettime(CLOCK_MONOTONIC, &(pinfo->next_period));
}



static void wait_rest_of_period(struct period_info *pinfo)
{
  inc_period(pinfo);
  /* for simplicity, ignoring possibilities of signal wakes */
  clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &pinfo->next_period, NULL);
}

static void do_rt_task(struct canArg *data)
{
	int nbytes,nbytes2,nbytes3,nbytes4;
	static int i;

	//do RT stuff here

	//ctr1.set_vel_limit(1,12345.0);

//use this if printing to 1 destination
/*	printf("tread writing %d to %x\n",data->socket, data->frame.can_id);
	
	nbytes = write(data->socket, &(data->frame), sizeof(struct can_frame));	
    	printf("tread 1: %d\n",nbytes);

	if (nbytes <0)
		perror("rttask1: Error while writing to socket");
	*/

}


void *thread_func(void *data)
{
  struct period_info pinfo;
  struct canArg * test = (struct canArg *)data;
  printf("passed in %d into thread1",test->socket);
  periodic_task_init(&pinfo);

  while (1) {
    do_rt_task(test);
    wait_rest_of_period(&pinfo);
  }

}

static void do_rt_task2(struct canArg *data){
	struct can_frame frame;	
    static int i;
    int nbytes,nbytes2,nbytes3,nbytes4;	

 	printf("tread 2 reading %d from %x\n",data->socket,data->frame.can_id);
	nbytes=read(data->socket, &(data->frame), sizeof(struct can_frame));
    	frame = data->frame;
    	printf("tread 2 can_id: %d\n",data->frame.can_id);
	
	printf("data read %d %d %x %x %x %x %x %x %x %x. Yay!\n",i,nbytes,frame.data[0], frame.data[1], frame.data[2], frame.data[3],
		 frame.data[4], frame.data[5], frame.data[6], frame.data[7]);	
	
}

void *thread_func2(void *data)
{
  struct period_info pinfo;
  struct canArg * test = (struct canArg *)data;
  printf("passed in %d into thread2 ",test->socket);
  periodic_task_init(&pinfo);
  
	while(1){
		do_rt_task2(test);
	    wait_rest_of_period(&pinfo);
	}

}


int can_init(struct canArg *canArg1,struct canArg *canArg4)
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
	/*if(bind(s2, (struct sockaddr *)&addr2, sizeof(addr2)) < 0) {
		perror("Error in socket bind2");
		return -2;
	}*/
	if(bind(s3, (struct sockaddr *)&addr3, sizeof(addr3)) < 0) {
		perror("Error in socket bind3");
		return -2;
	}


	frame.can_id  = (0x01<<5) | 0x0f;	
//	InputData = 0xAABBCCDDEEFF0011;
	InputData = 0x4640e400; //this is 12345.0 when converted to float
	frame2.can_id  = 0x2;	
	InputData2 = 0x2233445566778899;
	frame3.can_id  = 0x3;
	InputData3 = 0xCC;
	frame4.can_id  = (0x01<<5) | 0x0f ; //set vel limit of node 1
	InputData4 = 0x123456789012345;

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

	canArg1->socket= s;
	canArg1->frame= frame;
	canArg1->socket2= s2;
	canArg1->frame2= frame2;
	canArg4->socket= s3;
	canArg4->frame= frame4;
	//canArg4->frame2= frame2;
	return 0;
}
/* Ver 1.0 Created by CH LOW on 11th December 2019 */

#include"periodic_task.hpp"

static void do_rt_task(struct threadArg *data)
{
	int nbytes;
	static int i;
  
	controller *ctrT1=	data->ctrArg;
	//do RT stuff here

//use this if printing to 1 destination

		//printf("tread writing from socket %d to %x\n",data->socket, data->frame.can_id ); 
		if (i ==0){
			ctrT1->set_vel_limit(1,12345.0);
			i=1;
		}else{
			ctrT1->set_vel_limit(1,54321.0);
      ctrT1->get_vbus_voltage(1);
			//ctrT1->set_axis_requested_state(1,5);
			i=0;
		}

 		//ctrT1.move_to_pos(1,12345);

	/*
	nbytes = write(data->socket, &(data->frame), sizeof(struct can_frame));	
    	printf("tread 1: %d\n",nbytes);

	if (nbytes <0)
		perror("rttask1: Error while writing to socket");
	}
*/
}


void *thread_func(void *data)
{
  struct period_info pinfo;
  struct threadArg * test = (struct threadArg *)data;
  printf("passed in %d into thread1 at period %ld ",test->socket,test->taskperiod);
  periodic_task_init(&pinfo,test->taskperiod);

  while (1) {
    do_rt_task(test);
    wait_rest_of_period(&pinfo);
  
  }

}

static void do_rt_task2(struct threadArg *data){
	struct can_frame sframe;	
    static int i;
    int nbytes;	

    (data->ctrArg)->msg_handler();
    
 
	//do RT stuff here
 /*	printf("tread 2 reading %d\n",data->socket);
	nbytes=read(data->socket, &(data->frame), sizeof(struct can_frame));
    	sframe = data->frame;
    	printf("tread 2 can_id: %x\n",sframe.can_id);
	if (sframe.can_id != 41 | sframe.can_id != 21)
	//printf("data read nbytes:%d %x %x %x %x %x %x %x %x. Yay!\n",nbytes,sframe.data[0], sframe.data[1], sframe.data[2], sframe.data[3],
	//	 sframe.data[4], sframe.data[5], sframe.data[6], sframe.data[7]);	
	printf("data read nbytes:%d %d %d %d %d %d %d %d %d. Yay!\n",nbytes,sframe.data[0], sframe.data[1], sframe.data[2], sframe.data[3],
     sframe.data[4], sframe.data[5], sframe.data[6], sframe.data[7]); 
 */ 
}

void *thread_func2(void *data)
{
  struct period_info pinfo;
  struct threadArg * test = (struct threadArg *)data;
  printf("passed in %d into thread2 at period %ld ",test->socket,test->taskperiod);
  periodic_task_init(&pinfo,test->taskperiod);
  
	while(1){
		do_rt_task2(test);
	    wait_rest_of_period(&pinfo);
  
	}

}


void set_periodic_task(struct controller* ctrl_handler, pthread_t* thread_handler, threadArg* thread_arg, int function_number, long int period = 100000000, int priority=80){

//note: threads are declared in main and passed into function
//		intentionally to ensure developer are aware fo the number of thread
//		being created	

//arg: priority (set to between 20-80) (critical system priority is 99 [DO NOT USE], normal task have priority of -1) 
//arg: note, don't attempt to give 2 different period to 1 single threadArg. doesn't work, will always take from higher prioritiy( suspect: due to some interupt sequenceing?)
  struct sched_param param;
  pthread_attr_t attr;

  int ret =0;
  thread_arg->taskperiod = period;  

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
  param.sched_priority = priority;
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

if (function_number == 1 ){
ret = pthread_create(thread_handler, &attr, thread_func, thread_arg);
  if (ret) {
    printf("create pthread 1 failed\n");
    
  }

  printf("create pthread 1 success\n");
}

//thread2 to continuously read
if (function_number == 2 ){
ret = pthread_create(thread_handler, &attr, thread_func2, thread_arg);
  if (ret) {
    printf("create pthread 2 failed\n");
    
  }

  printf("create pthread 2 success\n");
}

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

static void periodic_task_init(struct period_info *pinfo, long int setperiod= 100000000)
{
  /* for simplicity, default is a 100ms period */
	printf("period %ld set\n", setperiod);
  pinfo->period_ns = setperiod; 
  clock_gettime(CLOCK_MONOTONIC, &(pinfo->next_period));
}



static void wait_rest_of_period(struct period_info *pinfo)
{
  inc_period(pinfo);
  /* for simplicity, ignoring possibilities of signal wakes */
  clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &pinfo->next_period, NULL);
}

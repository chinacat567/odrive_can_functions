
#ifndef _PT_H_
#define _PT_H_

#include <limits.h>
#include <pthread.h>
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/socket.h>
#include <sys/mman.h>


#include <unistd.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <sys/types.h>

#include"odrive_can_functions.hpp"


struct threadArg { 

	//for grabbing up to 2 socket info.
	long int taskperiod;
	struct can_frame frame,frame2;
	int socket,socket2;
	controller* ctrArg;
};

struct period_info {
  struct timespec next_period;
  long period_ns;
};


void set_periodic_task(struct controller* ctrl_handler, pthread_t* thread_handler, threadArg* thread_arg, int function_number, long int period, int priority);

static void inc_period(struct period_info *pinfo);	

static void periodic_task_init(struct period_info *pinfo, long int setperiod);

static void wait_rest_of_period(struct period_info *pinfo);

static void do_rt_task(struct threadArg *data);

void *thread_func(void *data);

static void do_rt_task2(struct threadArg *data);

void *thread_func2(void *data);

#endif /* _PT_H_*/
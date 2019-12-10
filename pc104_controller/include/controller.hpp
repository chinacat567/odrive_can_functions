#include <limits.h>
#include <pthread.h>
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/socket.h>

#include <unistd.h>
#include <string.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <sys/types.h>
#include <net/if.h>	
#include <sys/ioctl.h>
#include <errno.h>
#include<stdint.h>


#include"odrive_can_functions.hpp"
#include"misc_functions.hpp"

struct canArg { 

	//for grabbing up to 2 socket info.

	struct can_frame frame,frame2;
	int socket,socket2;
};

struct period_info {
  struct timespec next_period;
  long period_ns;
};

struct timespec curtime;


int can_init(struct canArg *canArg1,struct canArg *canArg4);

static void inc_period(struct period_info *pinfo);	

static void periodic_task_init(struct period_info *pinfo);

static void wait_rest_of_period(struct period_info *pinfo);

static void do_rt_task(struct canArg *data);

void *thread_func(void *data);

static void do_rt_task2(struct canArg *data);

void *thread_func2(void *data);

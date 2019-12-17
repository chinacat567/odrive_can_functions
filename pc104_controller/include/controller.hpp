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
#include <stdint.h>
#include <fcntl.h>


#include"odrive_can_functions.hpp"
#include"misc_functions.hpp"
#include"periodic_task.hpp"



int can_init(struct threadArg &canArg1,struct threadArg &canArg4);


#include "task.hpp"
#include "signal_generator.hpp"
#include "odrive_can_functions.hpp"
#include<stdio.h>
#include <iostream>
#include <csignal>
#include <net/if.h>	
#include <sys/ioctl.h>
#include<string.h>

using namespace std;
rt_task * tasks[2];
comedi_t *device;
const char filename[] = "/dev/comedi0";



void signalHandler( int signum ) 
{
   cout << "Interrupt signal (" << signum << ") received.\n";
   tasks[0]->set_signal_handler(false);
   tasks[1]->set_signal_handler(false);
   exit(signum);  
}	

std::pair <int,int> can_init(const char* read_port, const char*  write_port)

{
	int socket_read, socket_write;
    struct sockaddr_can addr_read, addr_write;
    struct ifreq ifr_read, ifr_write;

    struct can_filter rtr_filter;
    rtr_filter.can_id = (0xa);
    rtr_filter.can_mask = (CAN_RTR_FLAG | CAN_SFF_MASK);

    socket_write = socket(PF_CAN, SOCK_RAW, CAN_RAW);
    socket_read = socket(PF_CAN, SOCK_RAW, CAN_RAW);

    setsockopt(socket_read, SOL_CAN_RAW, CAN_RAW_FILTER, &rtr_filter, sizeof(rtr_filter));


    strcpy(ifr_read.ifr_name, read_port );
    ioctl(socket_read, SIOCGIFINDEX, &ifr_read);

    strcpy(ifr_write.ifr_name, write_port);
    ioctl(socket_write, SIOCGIFINDEX, &ifr_write);

    addr_read.can_family = AF_CAN;
    addr_read.can_ifindex = ifr_read.ifr_ifindex;

    addr_write.can_family = AF_CAN;
    addr_write.can_ifindex = ifr_write.ifr_ifindex;

    bind(socket_read, (struct sockaddr *)&addr_read, sizeof(addr_read));
    bind(socket_write, (struct sockaddr *)&addr_write, sizeof(addr_write));
    
    std::pair<int,int> pair(socket_read,socket_write);
    return pair;
}

/* calculate the gain online by calling this function*/

float calculate_gain( controller * ctrl)
{
	return K_P;
}

/* thread 1 : write data on CAN BUS*/
void* rt_task_func_1(void * arg)
{
	controller * ctrl = (controller *) arg;

	for (int i = 0; i < MOTOR_COUNT; ++i)
	{
		int32_t THETA_M = ctrl->get_motor_data(i)->encoder_count_in_cpr;
		int32_t THETA_R = adc_reading(device) * ANALOG_SCALE;
		cout << "ADC READING " << adc_reading(device) << endl;
		cout << "THETA_M " << THETA_M << endl;
		cout << "THETA_R " << THETA_R << endl;
		float curr_setpoint = K_P * (THETA_R - THETA_M) + CURR_FF;
		cout << "Current setpoint =" <<   K_P * (THETA_R - THETA_M) << endl;
		ctrl->set_cur_setpoint(i, curr_setpoint) ; 
	}
}
/* thread 2 : read data from CAN BUS*/


void* rt_task_func_2(void * arg)
{
	controller * ctrl = (controller *) arg;
	ctrl->msg_handler();
}

int main()
{
	/* intialize socket can */
	std::pair<int,int> socket_array	;
	socket_array = can_init("can0" , "can0");	
	/* declare controller class object */
	controller ctrl(socket_array.second, socket_array.first);
	
	/* create pthread on which the RT tasks run*/
	pthread_t thread_1, thread_2;

	/* online gain calculator*/


	/* comedi stuff*/
	device = comedi_open(filename);
	if (device == NULL) {
	  comedi_perror(filename);
	  return 1;
	}

	/* print pre processor values*/
	cout << " MOTOR_COUNT = " << MOTOR_COUNT << endl;
	cout << " N_READ = " << N_READ << endl;
	cout << " N_WRITE = " << N_WRITE << endl;
	cout << " ANALOG_SCALE = " << ANALOG_SCALE << endl;
	cout << " RT_PERIOD_1 = " << RT_PERIOD_1 << endl;
	cout << " RT_PERIOD_2 = " << RT_PERIOD_2 << endl;
	cout << " SCHED_P_1 = " << SCHED_P_1 << endl;
	cout << " SCHED_P_2 = " << SCHED_P_2 << endl;
	cout << " K_P = " << K_P << endl;

	
	/* create the RT tasks */
	/* 1 : write 2: read*/
	rt_task task_1(&thread_1, &rt_task_func_1, (void *) &ctrl, RT_PERIOD_1, SCHED_P_1);
	task_1.create_rt_task();

	rt_task task_2(&thread_2, &rt_task_func_2, (void *) &ctrl, RT_PERIOD_2, SCHED_P_2);
	task_2.create_rt_task();

	/* signal handling stuff */
	/* for passing to the signit handler*/
	tasks[0] = &task_1;
	tasks[1] = &task_2;
   	signal(SIGINT, signalHandler);


	/* clean up the RT tasks*/
	task_1.end_periodic_task();
	task_2.end_periodic_task();




}



#ifndef _RT_H_
#define _RT_H_

#include <pthread.h>
#include <stdio.h>
#include <limits.h>
#include <iostream>

#define handle_error_en(en, msg) \
	do { errno = en; perror(msg); exit(EXIT_FAILURE); } while (0)

#define handle_error(msg) \
	do { perror(msg); exit(EXIT_FAILURE); } while (0)

struct period_info {
  struct timespec next_period;
  long period_ns;
};


class rt_task {

public:
	rt_task(pthread_t *thread, void *(*func) (void *), void* arg, long period, int priority);
	void create_rt_task();
	/* blocking call, waits for the RT thread to join */
	void end_periodic_task();
	/* helper function*/
	/* accesor methods for private members*/
	void set_signal_handler(bool flag);




private:

	/* function and arguments to run inside the rt task*/
	void *(*_rt_function) (void *);
	void * _arg;	
	/* the thread object in which the rt task is to be run*/
	pthread_t * _thread;
	/* the loop time of the rt task in nano seconds*/
	long _period_ns;
	/* the scheduler priority*/
	int _priority;
	/* hanlder for exiting infinite loop */
	bool _signal_handler;

	/* internal thread related functions, not available to public */
	static void *helper(void * arg);
	void *periodic_task(void);
	static void periodic_task_init(struct period_info *pinfo, long int setperiod);
	static void wait_rest_of_period(struct period_info *pinfo);
	static void inc_period(struct period_info *pinfo);



};

#endif







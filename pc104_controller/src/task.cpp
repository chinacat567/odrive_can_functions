#include "task.hpp"

rt_task::rt_task(pthread_t * thread, void *(*func) (void *), void* arg, long period, int priority)
{
  this->_thread = thread;
  this->_rt_function = func;
  this->_arg = arg;
  this->_period_ns = period;
  this->_priority = priority;
  this->_signal_handler = true;
}

void rt_task::create_rt_task()
{
  struct sched_param param;
  pthread_attr_t attr;

  int ret;

  /* Initialize pthread attributes (default values) */
  ret = pthread_attr_init(&attr);
  if (ret != 0)
  handle_error_en(ret, "pthread_attr_init");
  

  /* Set a specific stack size  */
  ret = pthread_attr_setstacksize(&attr, PTHREAD_STACK_MIN);
  if (ret != 0)
  handle_error_en(ret, "pthread_attr_setstacksize");

  /* Set scheduler policy and priority of pthread */
  ret = pthread_attr_setschedpolicy(&attr, SCHED_FIFO);
  if (ret != 0)
  handle_error_en(ret, "pthread_attr_setschedpolicy");

  param.sched_priority = this->_priority;
  ret = pthread_attr_setschedparam(&attr, &param);
  if (ret != 0)
  handle_error_en(ret, "pthread_attr_setschedparam");
  
  /* Use scheduling parameters of attr */
  ret = pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);
  if (ret != 0)
  handle_error_en(ret, "pthread_attr_setinheritsched");

  ret = pthread_create(this->_thread, &attr, this->helper,  this);
  if (ret != 0)
  handle_error_en(ret, "pthread_create");


}
void rt_task::set_signal_handler(bool flag)
{
  this->_signal_handler = flag;
}
void rt_task::end_periodic_task()
{
  /* wait for the thread to exit and join */
  void * res;
  int s = pthread_join(*this->_thread, &res);  
  if (s != 0)
  handle_error_en(s, "pthread_join");
  else
  printf("Cleaned up thread successfully\n");
  free(res);      /* Free memory allocated by thread */

}

void *rt_task::helper(void *arg)
{
  ((rt_task *)arg)->periodic_task();
   return NULL;
} 

void *rt_task::periodic_task()
{
 struct period_info pinfo;
 periodic_task_init(&pinfo,this->_period_ns);
 while(_signal_handler)
 {
  _rt_function(this->_arg);
  wait_rest_of_period(&pinfo);

 }
 printf("Exiting the RT loop\n");
 return NULL;
    
}

void rt_task::periodic_task_init(struct period_info *pinfo, long int setperiod= 100000000)
{
  /* for simplicity, default is a 100ms period */
  printf("period %ld set\n", setperiod);
  pinfo->period_ns = setperiod; 
  clock_gettime(CLOCK_MONOTONIC, &(pinfo->next_period));
}

void rt_task::wait_rest_of_period(struct period_info *pinfo)
{
  inc_period(pinfo);
  /* for simplicity, ignoring possibilities of signal wakes */
  clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &pinfo->next_period, NULL);
}

void rt_task::inc_period(struct period_info *pinfo) 
{
  pinfo->next_period.tv_nsec += pinfo->period_ns;

  while (pinfo->next_period.tv_nsec >= 1000000000) {
    /* timespec nsec overflow */
    pinfo->next_period.tv_sec++;
    pinfo->next_period.tv_nsec -= 1000000000;
  }
}





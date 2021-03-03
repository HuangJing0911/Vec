/*
  Copyright (c) 2014, Detection Technology Inc.
  All rights reserved.

  This file defines some thread related classes.

  Author: Zhang Xu, 2014-3-10
*/

#ifndef XTHREAD_LIU_H
#define XTHREAD_LIU_H

#include <stdint.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>
/*
  This class wrapps a mutex object
*/
class XLock
{
public:
     XLock()
     {
	  pthread_mutex_init(&_mutex, NULL);
     }
     ~XLock()
     {
	  pthread_mutex_destroy(&_mutex);
     };
     void Lock()
     {
	  pthread_mutex_lock(&_mutex);
     };

     void Unlock()
     {
	  pthread_mutex_unlock(&_mutex);
     };
private:
     pthread_mutex_t _mutex;
  
};

/*
  This wrapps sem_t object
*/
class XEvent
{
public:
     XEvent()
     {
	  sem_init(&_sem_obj, 0, 0);
     }
     ~XEvent()
     {
	  sem_destroy(&_sem_obj);
     }
     void Set()
     {
	  sem_post(&_sem_obj);
     }
     void WaitTime(int32_t millisecond)
     {
	  struct timespec tv;
	  clock_gettime(CLOCK_REALTIME, &tv);
	  tv.tv_nsec += millisecond*1000000;
	  sem_init(&_sem_obj,0,0);
	  sem_timedwait(&_sem_obj,&tv);
     }
     void Wait()
     {
	  sem_init(&_sem_obj,0,0);
	  sem_wait(&_sem_obj);
     }
     /* void Reset() */
     /* { */
     /* 	  sem_init(&_sem_obj,0,0); */
     /* } */
private:
     sem_t _sem_obj;
};

/*
  This class wraps basic thread functions. Use thrad function and arguments
  as parameters when claiming.
  If circulation in the thread function needs to check flag, do like this
  "while(!thread_obj.IsStopped())"
  At the end of thread function, XThread::Exit() must be called.
*/

#define XTERMINATION_WAIT_INTERVAL 3 /*Wait for 3s, then force to cancel
				      * thread */

class XThread
{
public:
     typedef void* (*ThreadFunc) (void*);
XThread(ThreadFunc func_, void* arg_)
     :_thread_func_(func_)
	  ,_thread_arg_(arg_)
	  ,_thread_id(0)
     {
	  sem_init(&_sem_stop, 0, 0);
	  sem_init(&_sem_exit, 0, 0);
     };
     ~XThread()
     {
	  Stop();
	  sem_destroy(&_sem_stop);
	  sem_destroy(&_sem_exit);
     };
     /*
       Start thread function, 
      */
     bool Start(bool high_priority=0)
     {
	  //Already started
	  // if(_thread_id != 0)
	  //   return 1;
	  int32_t err;
	  if(!high_priority)
	       err = pthread_create(&_thread_id, NULL, _thread_func_, _thread_arg_);
	  else
	  {
	       //High priority
	       /* pthread_attr_t thread_attr; */
	       /* struct sched_param schedule_param; */
	       /* pthread_attr_init(&thread_attr); */
	       /* schedule_param.sched_priority = 99; */
	       /* pthread_attr_setinheritsched(&thread_attr, PTHREAD_EXPLICIT_SCHED); */
	       /* pthread_attr_setschedpolicy(&thread_attr, SCHED_RR); */
	       /* pthread_attr_setschedparam(&thread_attr, &shedule_param); */
	       // err = pthread_create(&_thread_id, &thread_attr, _thread_func_, _thread_arg_);
	       err = pthread_create(&_thread_id, NULL, _thread_func_, _thread_arg_);
	       struct sched_param schedule_param;
	       schedule_param.sched_priority = 90;
	       pthread_setschedparam(_thread_id, SCHED_RR, &schedule_param);
	  }

	  //If succeed, return 0
	  if(0 != err)
	       return 0;
	  
	  return 1;
     };
     /*
       Stop thread with stop event. If it doesn't exit within timeout, then
       force it to terminate. If it is forced to terminate, return 0 and can't
       start again.
      */
     bool Stop()
     {
	  bool ret = 1;
	  if(_thread_id)
	  {
	       //Send stop event
	       sem_post(&_sem_stop);
	       struct timespec tv;
	       clock_gettime(CLOCK_REALTIME, &tv);
	       tv.tv_sec += XTERMINATION_WAIT_INTERVAL;
	      
	       //Exit event not happen
	       if(0 != sem_timedwait(&_sem_exit, &tv))
	       {
		    //Force it to terminate
		    pthread_cancel(_thread_id);
	       	    ret = 0;
	       	    printf("Force to terminate...\n");
	       }
	
	       //Wait for thread exit
	       pthread_join(_thread_id, NULL);
	
	  }
	  _thread_id = 0;
	  return ret;
     };
     /*
       This function must be called at the end of thread function
      */
     void Exit()
     {
	  sem_post(&_sem_exit);
     };
   
     /*
       Check whether the stop event happens. If stop happens, return 1.
      */
     bool IsStopped()
     {
	  struct timespec tv;
	  clock_gettime(CLOCK_REALTIME,&tv);
	  //Stop event happens, sem_timedwait() return 0
	  return (0 == sem_timedwait(&_sem_stop, &tv));
     };

     uint32_t GetThreadId()
     {
	  return (uint32_t) pthread_self();
     };

private:
     ThreadFunc _thread_func_;
     void* _thread_arg_;
     pthread_t _thread_id;
     sem_t _sem_stop;
     sem_t _sem_exit;
};

#endif //XTHREAD_LIU_H

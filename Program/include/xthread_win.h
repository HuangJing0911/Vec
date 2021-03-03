/*
Copyright (c) 2014, Detection Technology Inc.
All rights reserved.

This file defines some thread related classes.

Author: Zhang Xu, 2014-3-5
*/

#ifndef XTHREAD_WIN_H
#define XTHREAD_WIN_H

#include <windows.h>
#include <stdint.h>
#include <process.h>

/*
This class simplely wraps the critical_section object.
It provides the basic locking function.
*/
class XDLL_EXPORT XLock
{
public:
	XLock()
	{
		InitializeCriticalSection(&_critical_sect);
	};

	~XLock()
	{
		DeleteCriticalSection(&_critical_sect);
	};

	void Lock()
	{
		EnterCriticalSection(&_critical_sect);
	};

	void Unlock()
	{
		LeaveCriticalSection(&_critical_sect);
	};

private:
	CRITICAL_SECTION	_critical_sect;
};

class XDLL_EXPORT XEvent
{
public:
	XEvent()
	{
		_event_obj = CreateEvent(NULL, TRUE, FALSE, NULL);
	}
	~XEvent()
	{
		CloseHandle(_event_obj);
	}

	void Set()
	{
		SetEvent(_event_obj);
	}
	void WaitTime(int32_t millisecond)
	{
		ResetEvent(_event_obj);
		WaitForSingleObject(_event_obj, millisecond);
	}
private:
	HANDLE _event_obj;
};
/*
This calss wraps basic thread funcitons. Use thread function and arguments
as parameters when claiming.
If circulation in thread function needs to checkstop flag, do like this
"while(!thread_obj.IsStopped())". 
*/

#define XTERMINATION_WAIT_INTERVAL	3000		//Force to terminate the thread after interval

typedef unsigned * PBEGINTHREADEX_THREADID;
typedef unsigned (WINAPI *PBEGINTHREADEX_THREADFUNC) (LPVOID lpThreadParameter);

class XDLL_EXPORT XThread
{
public:
	typedef uint32_t (__stdcall *ThreadFunc) (void *);

	XThread(ThreadFunc func_, void* arg_)
		:_thread_func_(func_)
		,_thread_arg_(arg_)
		,_thread_handle(NULL)
		,_thread_id(0)
	{
		_stop_event = CreateEvent(NULL, TRUE, FALSE, NULL);
	};

	~XThread()
	{
		Stop();
		CloseHandle(_stop_event);
	};

	bool Start(bool high_priority=0)
	{
		//Already started
		//if(_thread_handle)
		//	return 1;

		//If fail, return NULL
		_thread_handle = (HANDLE) _beginthreadex(NULL, 0, 
												(PBEGINTHREADEX_THREADFUNC)_thread_func_,
												_thread_arg_, 0, &_thread_id);
	    //Make stop event object active
		if(_thread_handle)
		{
			ResetEvent(_stop_event);
			if(high_priority)
			{
				SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS);
				SetThreadPriority(_thread_handle, THREAD_PRIORITY_TIME_CRITICAL);
			}
		}
		else
			return 0;
		return  1;
	}

	/*
	Stop thread with stop event. If it doesn't exit within XTERMINATION_WAIT_INTERVAL seconds,
	then force it to terminate. If it is forced to terminate, it'll return 0 and can't start 
	again correctly.
	*/
	bool Stop()
	{
		bool ret = 1;
		if(_thread_handle)
		{
			if(IsActive())
			{
				SetEvent(_stop_event);
				//Wait "XTERMINAL_WAIT_INTERVAL" seconds for thread to terminate
				if(WaitForSingleObject(_thread_handle, XTERMINATION_WAIT_INTERVAL) != WAIT_OBJECT_0)
				{
					if(IsActive())
					{
						//Force to terminate it
						TerminateThread(_thread_handle, int32_t(-1));
						//This yield allows the sheduler to do additional clean up
						Sleep(0);
						ret = 0;
					}
				}
			}
			//Clean up
			CloseHandle(_thread_handle);
			_thread_handle = NULL;
			_thread_id = 0;
		}
		return ret;
	}
	//Useless in windows
	void Exit()
	{
	
	};
	
	bool IsStopped()
	{
		uint32_t result;
		result = WaitForSingleObject(_stop_event,0);
		return (WAIT_OBJECT_0 == result); 
	}
	
	uint32_t GetThreadId()
	{
		return GetCurrentThreadId();
	}

private:	
	XThread(const XThread&);
	XThread& operator = (const XThread&);

	bool IsActive()
	{
		uint32_t exit_code = -1;
		GetExitCodeThread(_thread_handle, (LPDWORD)&exit_code);
		return (STILL_ACTIVE == exit_code);
	}
	
	HANDLE _thread_handle;
	HANDLE _stop_event;
	uint32_t _thread_id;

	ThreadFunc	_thread_func_;	//Thread function pointer
	void*		_thread_arg_;	//Thread fucntion argument 

};

#endif //XTHREAD_WIN_H

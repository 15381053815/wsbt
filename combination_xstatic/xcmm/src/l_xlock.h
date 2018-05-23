#ifndef __L_XLOCK_H_
#define __L_XLOCK_H_

#ifdef WIN32
#include <Windows.h>
#endif

#ifdef __cplusplus
class lxlock
{
#ifdef WIN32
	CRITICAL_SECTION cs;
public:
	lxlock(){ 
		InitializeCriticalSection( & cs); 
	} 
	~ lxlock(){ 
		DeleteCriticalSection( & cs); 
	} 

public:
	void lock(){ 
		EnterCriticalSection( & cs); 
	} 
	void unlock(){ 
		LeaveCriticalSection( & cs); 
	} 

#elif LINUX
	pthread_mutex_t _mutex;
#endif
}; 

class lxcond
{
#ifdef WIN32
	HANDLE _handle;
public:
	lxcond(){ 
		_handle = CreateEvent(NULL, FALSE, FALSE, 0);
	} 
	~ lxcond(){ 
		CloseHandle(_handle);
	} 

	void wait(){
		WaitForSingleObject(_handle, INFINITE);
	}

	int  wait_t(unsigned int ms){
		DWORD d=WaitForSingleObject(_handle, ms);
		if(d==WAIT_TIMEOUT)return 1;
		else if(d==WAIT_OBJECT_0)return 0;
		else return -1;
	}

	void notify(){
		SetEvent(_handle);
	}

#elif LINUX
	pthread_cond_t _cond;
#endif
};
extern "C" {
#endif


#ifdef __cplusplus
}
#endif
#endif
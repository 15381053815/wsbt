#include "l_xthread.h"
#include "l_xxx.h"
#include "l_xcmm.h"

u32		ltx_getcurrentthreadid()
{
#ifdef WIN32
	return ::GetCurrentThreadId();
#endif
}

void	ltx_assert(int bexp)
{
#ifdef WIN32
	(!!(bexp)) || (_wassert(L"xthd", _CRT_WIDE(__FILE__), __LINE__), 0);
#endif
}

//////////////////////////////////////////////////////////////////////////
int		lxthread::thread_func(lxthread *ths)
{
	if (ths->_fh[1].fp)
		lxexec(&ths->_fh[1]);

	while(1){
		int ret=lxexec(&ths->_fh[0]);
		if (ret<0)break;
		else if (ret==0){
			if (ths->_exit)
				break;
			lcx_sleep(ths->_sleeptm);
		}
	}
	
	if (ths->_fh[2].fp)
		lxexec(&ths->_fh[2]);
	return 0;
}

int		lxthread::start()
{	
	ltm_assertinthread(_thdno_self);

	for (int i=0;i<3;i++){
		_fh[i].m[_fh[i].mc++]=(void*)_thid;
		if (_fh[i].ths)_fh[i].clway=cmcw_thiscall;
		else _fh[i].clway=cmcw_cdecl;
	}

#ifdef WIN32
	_thdhdl = (u64)CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)thread_func,this, 0, (LPDWORD)&_thdno_thrd);
	if(!_thdhdl)
		return -1;
#endif
	return 0;
}

void	lxthread::end()
{
	ltm_assertinthread(_thdno_self);
	_exit = 1;

#ifdef WIN32
	if(_thdhdl){
		WaitForSingleObject((HANDLE)_thdhdl,INFINITE);
		CloseHandle((HANDLE)_thdhdl);
	}
#endif
}
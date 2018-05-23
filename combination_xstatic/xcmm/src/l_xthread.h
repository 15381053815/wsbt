#ifndef __L_XTHREAD_H_
#define __L_XTHREAD_H_

#ifndef u64
#ifdef  WIN32
#define u64 __int64
#endif
#endif

#ifndef u32
#ifdef  WIN32
#define u32 unsigned int
#endif
#endif

#ifdef  WIN32
#include <Windows.h>
#include <assert.h>
#endif

enum ecm_callway{//no change anything
	cmcw_stdcall = 1,	//win32 api
	cmcw_cdecl,			//c method
	cmcw_thiscall,		//class call
};

typedef struct __scm_funchandle
{//no change anything
	void*	fp;
	void*	ths;
	void*	ud;
	void*	m[3];

	u32		clway;
	u32		mc;
}cm_funchandle;

typedef	void	(*ltcb_xthreadenter)(void*,int thid);//id,ud
typedef	void	(*ltcb_xthreadleave)(void*,int thid);

u32		ltx_getcurrentthreadid();
void	ltx_assert(int bexp);
#define ltm_assertinthread(x) \
	ltx_assert(x==ltx_getcurrentthreadid())

class lxthread
{
public:
	lxthread(u32 thid,void* fp,void* ths,void* ud,u32 sleeptm):
	  _sleeptm(sleeptm),_exit(0),_thdhdl(0),_thdno_thrd(0),_thid(thid),
	  _thdno_self(ltx_getcurrentthreadid()){//2 call method,cmcw_cdecl/cmcw_thiscall,follow ths
		_fh[0].fp=fp;
		_fh[0].mc=1;
		_fh[0].m[0]=ud;
		_fh[0].ths=ths;
		_fh[1].fp=0;
		_fh[2].fp=0;
		_fh[1].mc=0;
		_fh[2].mc=0;
	}
	~lxthread(){;}
public:
	void	ltset_threadenter(ltcb_xthreadenter fp,void* ths,void* ud){
		_fh[1].fp=fp;
		_fh[1].mc=1;
		_fh[1].m[0]=ud;
		_fh[1].ths=ths;
	}
	void	ltset_threadleave(ltcb_xthreadenter fp,void* ths,void* ud){
		_fh[2].fp=fp;
		_fh[2].mc=1;
		_fh[2].m[0]=ud;
		_fh[2].ths=ths;
	}
	static int	thread_func(lxthread *ths);

	int		start();
	void	end();
private:
	cm_funchandle	_fh[3];

	u32		_exit;
	u32		_sleeptm;
	u32		_thdno_self;
	u32		_thdno_thrd;
	u32		_thid;

	u64		_thdhdl;
};

#endif
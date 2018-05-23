#ifndef __L_XLOGEX_H_
#define __L_XLOGEX_H_

#include "l_xmquu.h"
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

#ifndef ulong
#ifdef  WIN32
#define ulong unsigned long
#endif
#endif

enum	__ellx_threadmode{
	llx_tm_sync = 1,
	llx_tm_async,
};

typedef	int (*llxcb_logmsg)(void* u,char* l,int len);
class lxlog : public lxquu
{
public:
	lxlog(char *path,int tmode,llxcb_logmsg cbmsg,void* cbu);
	~lxlog(void);
private:
	typedef struct __slxlog_file
	{
		u64  tim;
		char name[64];
	}lxlog_file;
#define llx_logfileptr(x)	((lxlog_file*)x->data)
public:
	int				run();
	int				getc();
	int				close();

	int				addlogfile(char* name,u64 t);
	int				createlogfile(const char* name);
	int				deletelogfile(const char* name);

	int				putlog(char *l);
	int				putlogex(char *fmt, ...);
private:
	int				loadlogfile();
	int				openlogfile();
	int				checklogfile();
	int				deletehistroylogfile();

	int				generatelogfilename(char* buf,int len,u64 t);
	int				outlog(char *l, int len);
private:
	void*			_filelist;
	ulong			_hfile;

	lxlog_file		_cur;

	u32				_filekeepsecond;
	int				_tmode;
	int				_close;

	char			_path[256];

	llxcb_logmsg	_cbmsg;
	void*			_cbu;
};


#endif
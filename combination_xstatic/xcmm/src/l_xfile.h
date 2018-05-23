#ifndef __L_XFILE_H_
#define __L_XFILE_H_

#ifndef ulong
#ifdef  WIN32
#define ulong unsigned long
#endif
#endif

#ifdef  WIN32
#include <Windows.h>
#include <shlobj.h>
#endif

#ifndef u64
#ifdef  WIN32
#define u64 __int64
#endif
#endif

#ifdef __cplusplus
extern "C" {
#endif
#ifdef  WIN32
	enum __elfx_seekmode{
		elfsm_begin = FILE_BEGIN,
		elfsm_end   = FILE_END,
		elfsm_cur   = FILE_CURRENT,
	};
#endif

	typedef struct __slfx_fileinfo{
		char	path [512];
		char	fname[32];

		int		isdir;
		u64		size;
	}lfx_fileinfo;
typedef int (*lfxcb_loopdir)(lfx_fileinfo &fi,void *ud);

int			lfx_loopfile(char *path,lfxcb_loopdir fcb,void *ud);
int			lfx_loopdir (char *path,lfxcb_loopdir fcb,void *ud);

int			lfx_isexistdirectory(const char *path);
int			lfx_isexistfile(const char *path);
int 		lfx_deletefile(const char *name);
int			lfx_createdirectory(const char *path);

ulong		lfx_createfile(const char *name,int nobuff);
ulong		lfx_openfile(const char *name,int nobuff);
void		lfx_closefile(ulong h);

int			lfx_writefile(ulong h,char* buf,int len);
int			lfx_readfile(ulong h,char* buf,int len);
int			lfx_seekfile(ulong h,ulong o,int m);
int			lfx_tellfile(ulong h);
#ifdef __cplusplus
}
#endif
#endif
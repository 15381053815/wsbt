
#include "l_xlog.h"
#include "l_xcmm.h"
void			llx_log(char *fmt, ...)
{
	char logbuf[1024];

	va_list args;
	va_start(args, fmt);
	vsprintf(logbuf, fmt, args);
	va_end(args);

#ifdef WIN32
	OutputDebugString(logbuf);//only single thread
#elif LINUX

#endif
	printf(logbuf);
}

void			llx_log_t(char *fmt, ...)
{
	char logbuf[1024];
	lcx_gettimestr_ms(logbuf);

	va_list args;
	va_start(args, fmt);
	vsprintf(logbuf+strlen(logbuf), fmt, args);
	va_end(args);

#ifdef WIN32
	OutputDebugString(logbuf);//only single thread
#elif LINUX

#endif
	printf(logbuf);
}

void			llx_log_rcall(llxcb_puttxt ptf,void *u,u32 cl,char *fmt, ...)
{
	char llg[512];

	va_list args;
	va_start(args, fmt);
	vsprintf(llg, fmt, args);
	va_end(args);

	ptf(u,llg,cl);
}

void			llx_log_rcall_t(llxcb_puttxt ptf,void *u,u32 cl,char *fmt, ...)
{
	char llg[512];
	lcx_gettimestr_ms(llg);

	va_list args;
	va_start(args, fmt);
	vsprintf(llg+strlen(llg), fmt, args);
	va_end(args);

	ptf(u,llg,cl);
}

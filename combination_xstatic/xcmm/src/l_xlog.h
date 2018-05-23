#ifndef __L_XLOG_H_
#define __L_XLOG_H_

#ifndef u32
#ifdef  WIN32
#define u32 unsigned int
#endif
#include <stdio.h>
#include <stdarg.h>
#endif
#ifdef __cplusplus
extern "C" {
#endif

typedef	void	(*llxcb_puttxt)(void* u,char *,u32);
void		llx_log   (char *fmt, ...);
void		llx_log_t (char *fmt, ...);
void		llx_logs  (char *fmt, ...);	//thread safe
void		llx_logs_t(char *fmt, ...);
void		llx_log_rcall  (llxcb_puttxt ptf,void *u,u32 cl,char *fmt, ...);
void		llx_log_rcall_t(llxcb_puttxt ptf,void *u,u32 cl,char *fmt, ...);

#ifdef __cplusplus
}
#endif
#endif
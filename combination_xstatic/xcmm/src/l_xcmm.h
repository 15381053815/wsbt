#ifndef __L_XCMM_H_
#define __L_XCMM_H_

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

#ifndef u16
#ifdef  WIN32
#define u16 unsigned short
#endif
#endif

#ifndef ulong
#ifdef  WIN32
#define ulong unsigned long
#endif
#endif

#ifdef  WIN32
#include <Windows.h>
#include <time.h>
#include <stdio.h>
#include <assert.h>
#endif

#define	 lxx_assert		(1>2)
#ifdef __cplusplus
extern "C" {
#endif
//common function
void			lcx_sleep(u32 ms);
void			lcx_xorexch(ulong* s,ulong* d);
void			lcx_assert(int bexp,wchar_t *msg);
void			lcx_getmodulepath(char* s,int l);
void			lcx_combpath(char* d,const char* s);

int				lcx_compare(const char* src,const char* dst);
int				lcx_dividestr(char* s,char c,char d[][32],int n);
int				lcx_dividestrex(char* s,char c,char* d,int n,int m);
//time
typedef struct __scm_time {
	u16 year;
	u16 month;
	u16 day;
	u16 hour;
	u16 minute;
	u16 second;
	u16 milliseconds;
} cm_time;
u64				lcx_getcurtime(cm_time* lct);	
u64				lcx_time2i(cm_time* lct);
void			lcx_i2time(u64 t,cm_time* lct);

void			lcx_gettimestr_ms(char* s);
//image
int				lcx_savecolorbmpfile(int bitcount,int width,int height,
	char *data,char *dir);//32bits realcolor,the scan line must be times of 4bytes
int				lcx_savegraybmpfile(int width,int height,
	char *data,char *dir);//8bits  gray,...
int				lcx_convcolor2gray(char *psrc,int w,int h,char *pdst);//psrc:bgra
//str
char*			lcx_sstrcpy(char* d,const char* s,u32 dl);
char*			lcx_sstrcat(char* d,const char* s,u32 dl);
#ifdef __cplusplus
}
#endif
#endif
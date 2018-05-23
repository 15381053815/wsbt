#include "l_xcmm.h"
#include "l_xfile.h"

void			lcx_sleep(u32 ms)
{
#ifdef WIN32
	Sleep(ms);
#endif	
}

void			lcx_xorexch(ulong* s,ulong* d)
{
	*s^=*d;
	*d^=*s;
	*s^=*d;
}

void			lcx_assert(int bexp,wchar_t *msg)
{
#ifdef WIN32
	(!!(bexp)) || (_wassert(msg?msg:L"ammm", _CRT_WIDE(__FILE__), __LINE__), 0);
#endif
}

void			lcx_getmodulepath(char* s,int l)
{
#ifdef WIN32
	GetModuleFileName(NULL,s,l);
	char* pos = strrchr(s,'\\');
	if (!pos)return;

	*(pos+0) = '\\';
	*(pos+1) = 0;
#endif
}

void			lcx_combpath(char* d,const char* s)
{
	int l=strlen(d);
	if (*(d+l-1)!='\\'&&*(d+l-1)!='/'){
#ifdef WIN32
		*(d+l)='\\';
#endif
		*(d+l+1)=0;
	}
	if (*s=='\\'||*s=='/')
		s+=1;
	lcx_sstrcat(d,s,-1);
}

int				lcx_compare(const char* src,const char* dst)
{
	while(*src&&*dst&&(*src==*dst)){
		src++;dst++;
	}
	return *src-*dst;
}

int				lcx_dividestr(char* s,char c,char d[][32],int n)
{
	char  t[1024] = {0};
	lcx_sstrcpy(t,s,sizeof(t));

	char* p = t;
	char* l = 0;

	int count = 0;
	do { l = strchr(p,c);
		if (l)*l = 0;

		lcx_sstrcpy(d[count++],p,32);
		if (count==n)break;

		if (!l)break;
		else p = ++l;

	} while(l);
	return count;
}

int				lcx_dividestrex(char* s,char c,char* d,int n,int m)
{
	if (!s || n<=0 || m<=0)return 0;

	char  t[1024] = {0};
	lcx_sstrcpy(t,s,sizeof(t));

	char* p = t;
	char* l = 0;

	int count = 0;
	do { l = strchr(p,c);
		if (l)*l = 0;

		lcx_sstrcpy(d+count++*m,p,m);
		if (count==n)break;

		if (!l)break;
		else p = ++l;

	} while(l);
	return count;
}
//time
u64				lcx_getcurtime(cm_time* lct)
{
#ifdef WIN32
	SYSTEMTIME st = {0};
	GetLocalTime(&st);

	tm t = {0};
	t.tm_year = st.wYear - 1900;
	t.tm_mon = st.wMonth - 1;
	t.tm_mday = st.wDay;
	t.tm_hour = st.wHour;
	t.tm_min = st.wMinute;
	t.tm_sec = st.wSecond;
	t.tm_wday = 0;

	if (lct){
		lct->year = st.wYear;
		lct->month = st.wMonth;
		lct->day = st.wDay;
		lct->hour = st.wHour;
		lct->minute = st.wMinute;
		lct->second = st.wSecond;
		lct->milliseconds = st.wMilliseconds;
	}
	return mktime(&t);
#endif

	return 0;
}

u64				lcx_time2i(cm_time* lct)
{
#ifdef WIN32
	tm t;
	t.tm_year = lct->year - 1900;
	t.tm_mon = lct->month - 1;
	t.tm_mday = lct->day;
	t.tm_hour = lct->hour;
	t.tm_min = lct->minute;
	t.tm_sec = lct->second;
	t.tm_wday = 0;

	return mktime(&t);
#endif	
}

void			lcx_i2time(u64 t,cm_time* lct)
{
#ifdef WIN32
	time_t ts = t;
	tm tmp = *localtime((time_t*)&ts);

	lct->year = tmp.tm_year + 1900;
	lct->month = tmp.tm_mon + 1;
	lct->day = tmp.tm_mday;
	lct->hour = tmp.tm_hour;
	lct->minute =  tmp.tm_min;
	lct->second = tmp.tm_sec;
	lct->milliseconds = 0;
#endif	
}

void			lcx_gettimestr_ms(char* s)
{
#ifdef WIN32
	SYSTEMTIME st = {0};
	GetLocalTime(&st);
	sprintf(s,"%d-%d-%d %d:%d:%d:%d",st.wYear,st.wMonth,st.wDay,st.wHour,st.wMinute,st.wSecond,st.wMilliseconds);
#endif	
}

int				lcx_savecolorbmpfile(int bitcount,int width,int height,
	char *data,char *dir)
{
#ifdef WIN32
	BITMAPFILEHEADER bfh;  
	BITMAPINFOHEADER bih; 

	int channels=bitcount/8;
	bfh.bfType=0x4d42;             
	bfh.bfReserved1 = 0;  
	bfh.bfReserved2 = 0;  
	bfh.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)+width*height*channels;  
	bfh.bfOffBits = 0x36;  

	bih.biSize = sizeof(BITMAPINFOHEADER);  
	bih.biWidth = width;  
	bih.biHeight = -height;  
	bih.biPlanes = 1;  
	bih.biBitCount = bitcount;  
	bih.biCompression = 0;  
	bih.biSizeImage = 0;  
	bih.biXPelsPerMeter = 5000;  
	bih.biYPelsPerMeter = 5000;  
	bih.biClrUsed = 0;  
	bih.biClrImportant = 0; 

	lfx_deletefile(dir);
	ulong hfile=lfx_createfile(dir,0);
	lcx_assert(hfile!=0,0);

	lfx_writefile(hfile,(char*)&bfh,sizeof(BITMAPFILEHEADER));
	lfx_writefile(hfile,(char*)&bih,sizeof(BITMAPINFOHEADER));
	lfx_writefile(hfile,(char*)data,width*height*channels);
	lfx_closefile(hfile);
	return 0;
#endif	
}

int				lcx_savegraybmpfile(int width,int height,
	char *data,char *dir)
{
#ifdef WIN32
	char s[4]={0};
	int  alignwidth=0;
	int  sl=0;

	BITMAPFILEHEADER bfh;  
	BITMAPINFOHEADER bih; 
	bfh.bfOffBits = sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+sizeof(RGBQUAD)*256;
	bfh.bfSize=bfh.bfOffBits+width*height; 
	bfh.bfReserved1=0;
	bfh.bfReserved2=0;
	bfh.bfType=0x4d42;

	bih.biSize = sizeof(BITMAPINFOHEADER);  
	bih.biWidth = width;  
	bih.biHeight = -height;  
	bih.biPlanes = 1;  
	bih.biBitCount = 8;  
	bih.biCompression = BI_RGB;  
	bih.biSizeImage = 0;  
	bih.biXPelsPerMeter = 0;  
	bih.biYPelsPerMeter = 0;  
	bih.biClrUsed = 0;  
	bih.biClrImportant = 0;

	if (width%4!=0){
		alignwidth=(width+3)/4*4;
		bfh.bfSize=bfh.bfOffBits+height*alignwidth;
		bih.biSizeImage=height*alignwidth; 
		sl=alignwidth-width;
	}

	RGBQUAD rgbquad[256];
	for(int i=0;i<256;i++)
	{
		rgbquad[i].rgbBlue=i;
		rgbquad[i].rgbGreen=i;
		rgbquad[i].rgbRed=i;
		rgbquad[i].rgbReserved=0;
	} 

	lfx_deletefile(dir);
	ulong hfile=lfx_createfile(dir,0);
	lcx_assert(hfile!=0,0);

	lfx_writefile(hfile,(char*)&bfh,sizeof(BITMAPFILEHEADER));
	lfx_writefile(hfile,(char*)&bih,sizeof(BITMAPINFOHEADER));
	lfx_writefile(hfile,(char*)rgbquad,sizeof(RGBQUAD)*256);
	if (alignwidth==0)
		lfx_writefile(hfile,(char*)data,width*height);
	else{
		lcx_assert(sl>0,0);
		for (int j=0;j<height;j++){
			lfx_writefile(hfile,&data[j*width],width);
			lfx_writefile(hfile,s,sl);
		}
	}
	lfx_closefile(hfile);
	return 0;
#endif
}

int				lcx_convcolor2gray(char *psrc,int w,int h,char *pdst)
{
	//Y = 0.299R+0.587G+0.114B
	for (int j=0;j<h;j++){
		for (int i=0;i<w;i++){
			char *pos=&psrc[j*w*4+i*4];
			pdst[j*w+i]=0.114*pos[0]+0.587*pos[1]+0.299*pos[2];
		}
	}
	return 0;
}

char*			lcx_sstrcpy(char* d,const char* s,u32 dl)
{
	char *t=d;dl--;
	while(*s&&dl--)*d++=*s++;*d=0;
	return t;
}

char*			lcx_sstrcat(char* d,const char* s,u32 dl)
{
	u32 l=strlen(d);
	lcx_sstrcpy(d+l,s,dl-l);
	return d;
}
#include "l_xlogex.h"
#include "l_xcmm.h"
#include "am_xarith.h"
#include "l_xfile.h"
#include "mm_xmmy.h"

lxlog::lxlog(char *path,int tmode,llxcb_logmsg cbmsg,void* cbu):
	_filelist(0),_hfile(0),_filekeepsecond(60*60*24*7),_tmode(tmode),_cbmsg(cbmsg),_cbu(cbu),_close(0)
{
	lcx_sstrcpy(_path,
		path,sizeof(_path));
	_cur.tim=0;
	_cur.name[0]=0;

	am_parem pm={0};
	pm.umode=amdm_nomem;
	pm.type=amot_doubly_list;
	pm.unitsize=sizeof(lxlog_file);
	_filelist=am_createobject_m(&pm);

	if (!lfx_isexistdirectory(_path)){
		if (lfx_createdirectory(_path) != 0)
			lcx_assert(lxx_assert,L"log");
	}

	if (loadlogfile() != 0)
		lcx_assert(lxx_assert,L"log");
	if (openlogfile() != 0)
		lcx_assert(lxx_assert,L"log");
}

lxlog::~lxlog()
{
	am_destryobject(_filelist,
		amot_doubly_list);
}

int			lxlog::run()
{
	deletehistroylogfile();

	if (_tmode != llx_tm_async)
		return 0;
	checklogfile();

	lqmsg msg;
	if (lqgetinmsg(msg)){
		outlog((char*)msg.pm[0],msg.im[0]);
		lm_xfree(msg.pm[0]);
		return 1;
	}
	return 0;
}

int			lxlog::getc()
{
	return lqgetinmsgsize();
}

int			lxlog::close()
{
	_close=1;
	return _close;
}

int			lxlog::addlogfile(char* name,u64 t)
{
	lxlog_file	lf;
	lf.tim = t;
	lcx_sstrcpy(lf.name,name,sizeof(lf.name));

	if(lf.tim>_cur.tim)
		_cur = lf;

	amx_list_insert_cpy(_filelist,
		&lf,sizeof(lf),0);
	return 0;
}

int			lxlog::deletelogfile(const char* name)
{
	char szPath[256];
	lcx_sstrcpy(szPath,_path,sizeof(szPath));
	lcx_combpath(szPath,name);
	lfx_deletefile(szPath);
	return 0;
}

int			lxlog::createlogfile(const char* name)
{
	char szPath[256];
	lcx_sstrcpy(szPath,_path,sizeof(szPath));
	lcx_combpath(szPath,name);

	if(_hfile){
		lfx_closefile(_hfile);
		_hfile = 0;
	}

	_hfile = lfx_createfile(szPath,0);
	if (_hfile == 0)return -1;

	return 0;
}

int			lxlog::putlog(char *l)
{
	if(l == 0||_close)
		return -1;

	int len=strlen(l)+64;
	char* p = (char*)lm_xmalloc(len);
	*p=0;

	cm_time t;
	lcx_getcurtime(&t);
	sprintf(p, "<%4d-%02d-%02d %02d:%02d:%02d %04d>", \
		t.year, t.month, t.day, t.hour, t.minute, t.second,t.milliseconds);
	
	strcat(p,l);
	strcat(p,"\r\n");

	if (_tmode == llx_tm_async){
		lqmsg msg;
		msg.im[0] = strlen(p);
		msg.pm[0] = p;

		lqpshinmsg(msg);
		return 0;
	}

	deletehistroylogfile();
	if (checklogfile() == -1)
		return -1;

	outlog(p,strlen(p));
	lm_xfree(p);
	return 0;
}

int			lxlog::putlogex(char *fmt, ...)
{
	if(_close)
		return -1;

	char p[2018];

	va_list args;
	va_start(args, fmt);
	vsprintf(p, fmt, args);
	va_end(args);
	putlog(p);
	return 0;
}
//////////////////////////////////////////////////////////////////////////
int			analyzelogfilename(lfx_fileinfo &fi,void *ud)
{
	//file format : 1og20090501010101.txt
	if(fi.fname == 0)
		return -1;
	if(*fi.fname == 0)
		return -1;
	if(strlen(fi.fname) != 21)
		return -1;

	char szBuf[12] = {0};

	memcpy(szBuf, fi.fname + 3, 4);
	int y = atoi(szBuf);
	if(y < 2000)
		return -1;

	memcpy(szBuf, fi.fname + 7, 2);
	szBuf[2] = 0;
	int m = atoi(szBuf);
	if(m < 1 || m > 12)
		return -1;

	memcpy(szBuf, fi.fname + 9, 2);
	szBuf[2] = 0;
	int d = atoi(szBuf);
	if(d < 1 || d > 31)
		return -1;

	memcpy(szBuf, fi.fname + 11, 2);
	szBuf[2] = 0;
	int h = atoi(szBuf);
	if(h < 0 || h > 23)
		return -1;

	memcpy(szBuf, fi.fname + 13, 2);
	szBuf[2] = 0;
	int mt = atoi(szBuf);
	if(mt < 0 || mt > 59)
		return -1;

	memcpy(szBuf, fi.fname + 15, 2);
	szBuf[2] = 0;
	int s = atoi(szBuf);
	if(s < 0 || s > 59)
		return -1;

	cm_time tt = {0};
	tt.year = y;
	tt.month = m;
	tt.day = d;
	tt.hour = h;
	tt.minute = mt;
	tt.second = s;

	u64 ttt = lcx_time2i(&tt);

	lxlog* lg = (lxlog*)ud;
	if (lg)
		lg->addlogfile( fi.fname,ttt);
	
	return 0;
}

int			lxlog::loadlogfile()
{
	int c = lfx_loopfile(_path,analyzelogfilename,this);
	if (c == -1)
		return -1;
	return 0;
}

int			lxlog::openlogfile()
{
	if (!*_cur.name)
		return 0;

	char szPath[256];

	lcx_sstrcpy(szPath,_path,sizeof(szPath));
	lcx_combpath(szPath,_cur.name);

	if(_hfile){
		lfx_closefile(_hfile);
		_hfile = 0;
	}

	_hfile = lfx_openfile(szPath,0);
	if (_hfile == 0)
		return -1;

	lfx_seekfile(_hfile,0,elfsm_end);
	return 0;
}

int			lxlog::checklogfile()
{
	char buf[256];
	char name[64] = {0};

	u64 curtime = lcx_getcurtime(0);
	if (amx_list_count(_filelist) == 0){
		generatelogfilename(buf,sizeof(256),curtime);
		if (createlogfile(buf) == -1)
			return -1;

		lcx_sstrcpy(name,buf,sizeof(name));
	}else if (_cur.tim > 0){
		if (curtime - _cur.tim > 3600){
			generatelogfilename(buf,sizeof(256),curtime);
			if (createlogfile(buf) == -1)
				return -1;
			lcx_sstrcpy(name,buf,sizeof(name));
		}
	}

	if (*name)
		addlogfile(name,curtime);
	return 0;
}

int			lxlog::deletehistroylogfile()
{
	u64 curtime = lcx_getcurtime(0);
	am_node* node = amx_list_frst(_filelist);
	while (node){
		if (curtime - llx_logfileptr(node)->tim >= _filekeepsecond){
			if (_cur.tim != llx_logfileptr(node)->tim){
				deletelogfile(llx_logfileptr(node)->name);
				amx_list_delete(_filelist,node);
			}
			break;
		}
		node = amx_list_next(_filelist,node);
	}
	return 0;
}

int			lxlog::generatelogfilename(char* buf,int len,u64 t)
{
	cm_time ts;
	lcx_i2time(t,&ts);

	sprintf(buf, "log%d%02d%02d%02d%02d%02d.txt", 
		ts.year, ts.month, ts.day, ts.hour, ts.minute, ts.second);
	return 0;
}

int			lxlog::outlog(char *l, int len)
{
#ifdef WIN32
	OutputDebugString(l);
#endif

	int r=lfx_writefile(_hfile,(char*)l,len);
	if (_cbmsg)
		_cbmsg(_cbu,(char*)l,len);
	return r;
}

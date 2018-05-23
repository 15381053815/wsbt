#include "l_xfile.h"
#include "l_xcmm.h"

int				lfx_loopfile(char *path,lfxcb_loopdir fcb,void *ud)
{
	lfx_fileinfo fi;
	lcx_sstrcpy(fi.path,path,sizeof(fi.path));

#ifdef WIN32
	char filepath[512];
	lcx_sstrcpy(filepath,path,sizeof(filepath));
	lcx_combpath(filepath,"*.*");

	WIN32_FIND_DATA fd = {0};
	HANDLE hfile = FindFirstFile(filepath, &fd);
	if (hfile == INVALID_HANDLE_VALUE)
		return -1;

	int fcnt=0;
	do 
	{
		if (strcmp(fd.cFileName, ".") == 0 
			|| strcmp(fd.cFileName, "..") == 0)
			continue;

		lcx_sstrcpy(fi.fname,fd.cFileName,sizeof(fi.fname));
		fi.isdir=(fd.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY);
		fi.size =fd.nFileSizeLow;
		if (fcb(fi,ud)!=0)
			break;
		fcnt++;
	}while (FindNextFile(hfile, &fd));
	if(hfile)FindClose(hfile);
#endif
	return fcnt;
}

int				lfx_loopdir (char *path,lfxcb_loopdir fcb,void *ud)
{
	return 0;
}

int				lfx_isexistdirectory(const char *path)
{
	if (0==path || 0==*path)
		return 0;

#ifdef WIN32
	DWORD ret = GetFileAttributes(path);
	if (ret != -1)
	{
		if (FILE_ATTRIBUTE_DIRECTORY == (FILE_ATTRIBUTE_DIRECTORY & ret))
		{
			return 1;
		}
	}
#endif

	return 0;
}

int				lfx_isexistfile(const char *path)
{
	if (0==path || 0==*path)
		return 0;

#ifdef WIN32
	DWORD ret = GetFileAttributes(path);
	if (ret != -1)
	{
		if (!(FILE_ATTRIBUTE_DIRECTORY & ret))
		{
			return 1;
		}
	}
#endif

	return 0;
}

int				lfx_deletefile(const char *name)
{
#ifdef WIN32
	if (DeleteFile(name))
		return 0;
	else 
		return -1;
#endif
}

int				lfx_createdirectory(const char *path)
{
	if (0 == path || 0 == *path)
		return -1;

#ifdef WIN32
	int nret = SHCreateDirectoryEx(NULL,path,NULL);// create in recursion
	if (ERROR_SUCCESS == nret){
		return 0;
	}
#endif
	return -1;
}

ulong			lfx_createfile(const char *name,int nobuff)
{
	ulong h = 0;
#ifdef WIN32
	DWORD flag = FILE_FLAG_WRITE_THROUGH;
	if (nobuff)flag |= FILE_FLAG_NO_BUFFERING;

	HANDLE hf = CreateFileA(name, GENERIC_WRITE|GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL, CREATE_NEW, flag, NULL);

	DWORD err = GetLastError();
	if(err == ERROR_ALREADY_EXISTS)
		return 0;

	if (hf == INVALID_HANDLE_VALUE)
		return 0;

	h = (ulong)hf;
#endif
	return h;
}

ulong			lfx_openfile(const char *name,int nobuff)
{
	ulong h = 0;
#ifdef WIN32
	DWORD flag = FILE_FLAG_WRITE_THROUGH;
	if (nobuff)flag |= FILE_FLAG_NO_BUFFERING;

	HANDLE hf = CreateFileA(name, GENERIC_WRITE|GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL, OPEN_EXISTING, flag, NULL);

	if (hf == INVALID_HANDLE_VALUE)
		return 0;

	h = (ulong)hf;
#endif
	return h;
}

void			lfx_closefile(ulong h)
{
	if (h){
#ifdef WIN32
		CloseHandle((HANDLE)h);
#endif
	}
}

int				lfx_writefile(ulong h,char* buf,int len)
{
	int writeLen = 0;
#ifdef WIN32
	if(!WriteFile((HANDLE)h, buf, len, (DWORD*)&writeLen, NULL)){
		int r = GetLastError();
		return -1;
	}
#endif
	return writeLen;
}

int				lfx_readfile(ulong h,char* buf,int len)
{
	int readLen = 0;
#ifdef WIN32
	if(!ReadFile((HANDLE)h, buf, len, (DWORD*)&readLen, NULL)){
		unsigned int r=GetLastError() ;
		return -1;
	}
#endif
	return readLen;
}

int				lfx_seekfile(ulong h,ulong o,int m)
{
	if(0 == h)
		return -1;

#ifdef WIN32
	if(SetFilePointer((HANDLE)h, o, NULL, m) == -1L){
		return -1;
	}
#endif

	int pos =  lfx_tellfile(h);
	lcx_assert(pos != -1,L"file");
	return pos;
}

int				lfx_tellfile(ulong h)
{
	if(0 == h)
		return -1;

	long pos ;
#ifdef WIN32
	pos = SetFilePointer((HANDLE)h, 0, 0, FILE_CURRENT);
	if(pos  == -1L){
		int r = GetLastError();
		return -1;
	}
#endif
	return pos;
}
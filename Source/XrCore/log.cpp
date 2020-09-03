#include "stdafx.h"
#pragma hdrstop

#include <time.h>
#include "resource.h"
#include "log.h"
#if 0
	#include "malloc.h"
#endif

extern BOOL					LogExecCB		= TRUE;
static string_path			logFName		= "engine.log";
static string_path			log_file_name	= "engine.log";
static BOOL 				no_log			= TRUE;
#ifdef PROFILE_CRITICAL_SECTIONS
	static xrCriticalSection	logCS(MUTEX_PROFILE_ID(log));
#else // PROFILE_CRITICAL_SECTIONS
	static xrCriticalSection	logCS;
#endif // PROFILE_CRITICAL_SECTIONS
xr_vector<shared_str>*		LogFile			= NULL;
static LogCallback			LogCB			= 0;

void FlushLog			()
{
	if (!no_log){
		logCS.Enter			();
		IWriter *f			= FS.w_open(logFName);
        if (f) {
            for (u32 it=0; it<LogFile->size(); it++)	{
				LPCSTR		s	= *((*LogFile)[it]);
				f->w_string	(s?s:"");
			}
            FS.w_close		(f);
        }
		logCS.Leave			();
    }
}

void AddOne				(const char *split) 
{
	if(!LogFile)		
						return;

	logCS.Enter			();

#ifdef DEBUG
	OutputDebugString	(split);
	OutputDebugString	("\n");
#endif

//	DUMP_PHASE;
	{
		shared_str			temp = shared_str(split);
//		DUMP_PHASE;
		LogFile->push_back	(temp);
	}

	//exec CallBack
	if (LogExecCB&&LogCB)LogCB(split);

	logCS.Leave				();
}

void Log				(const char *s) 
{
	int		i,j;

	u32			length = xr_strlen( s );
#if 1    
	PSTR split  = (PSTR)_alloca( (length + 1) * sizeof(char) );
#else
	PSTR split  = (PSTR)alloca( (length + 1) * sizeof(char) );
#endif
	for (i=0,j=0; s[i]!=0; i++) {
		if (s[i]=='\n') {
			split[j]=0;	// end of line
			if (split[0]==0) { split[0]=' '; split[1]=0; }
			AddOne(split);
			j=0;
		} else {
			split[j++]=s[i];
		}
	}
	split[j]=0;
	AddOne(split);
}

void __cdecl Msg		( const char *format, ...)
{
	va_list		mark;
	string2048	buf;
	va_start	(mark, format );
	int sz		= _vsnprintf(buf, sizeof(buf)-1, format, mark ); buf[sizeof(buf)-1]=0;
    va_end		(mark);
	if (sz)		Log(buf);
}

void Log				(const char *msg, const char *dop) {
	if (!dop) {
		Log		(msg);
		return;
	}

	u32			buffer_size = (xr_strlen(msg) + 1 + xr_strlen(dop) + 1) * sizeof(char);
	PSTR buf	= (PSTR)_alloca( buffer_size );
	strconcat	(buffer_size, buf, msg, " ", dop);
	Log			(buf);
}

void Log				(const char *msg, u32 dop) {
	u32			buffer_size = (xr_strlen(msg) + 1 + 10 + 1) * sizeof(char);
	PSTR buf	= (PSTR)_alloca( buffer_size );

	xr_sprintf	(buf, buffer_size, "%s %d", msg, dop);
	Log			(buf);
}

void Log				(const char *msg, int dop) {
	u32			buffer_size = (xr_strlen(msg) + 1 + 11 + 1) * sizeof(char);
	PSTR buf	= (PSTR)_alloca( buffer_size );

	xr_sprintf	(buf, buffer_size, "%s %i", msg, dop);
	Log			(buf);
}

void Log				(const char *msg, float dop) {
	// actually, float string representation should be no more, than 40 characters,
	// but we will count with slight overhead
	u32			buffer_size = (xr_strlen(msg) + 1 + 64 + 1) * sizeof(char);
	PSTR buf	= (PSTR)_alloca( buffer_size );

	xr_sprintf	(buf, buffer_size, "%s %f", msg, dop);
	Log			(buf);
}

void Log				(const char *msg, const Fvector &dop) {
	u32			buffer_size = (xr_strlen(msg) + 2 + 3*(64 + 1) + 1) * sizeof(char);
	PSTR buf	= (PSTR)_alloca( buffer_size );

	xr_sprintf	(buf, buffer_size,"%s (%f,%f,%f)",msg, VPUSH(dop) );
	Log			(buf);
}

void Log				(const char *msg, const Fmatrix &dop)	{
	u32			buffer_size = (xr_strlen(msg) + 2 + 4*( 4*(64 + 1) + 1 ) + 1) * sizeof(char);
	PSTR buf	= (PSTR)_alloca( buffer_size );
	Fvector4 Matrix[4];
	Matrix[0] = dop.get_row(0);
	Matrix[1] = dop.get_row(1);
	Matrix[2] = dop.get_row(2);
	Matrix[3] = dop.get_row(3);
	xr_sprintf	(buf, buffer_size,"%s:\n%f,%f,%f,%f\n%f,%f,%f,%f\n%f,%f,%f,%f\n%f,%f,%f,%f\n",
		msg,
		Matrix[0].x, Matrix[0].y, Matrix[0].z, Matrix[0].w,
		Matrix[1].x, Matrix[1].y, Matrix[1].z, Matrix[1].w,
		Matrix[2].x, Matrix[2].y, Matrix[2].z, Matrix[2].w,
		Matrix[3].x, Matrix[3].y, Matrix[3].z, Matrix[3].w
	);
	Log			(buf);
}

void LogWinErr			(const char *msg, long err_code)	{
	Msg					("%s: %s",msg,Debug.error2string(err_code)	);
}

LogCallback SetLogCB	(LogCallback cb)
{
	LogCallback	result	= LogCB;
	LogCB				= cb;
	return				(result);
}

LPCSTR log_name			()
{
	return				(log_file_name);
}

void InitLog()
{
	R_ASSERT			(LogFile==NULL);
	LogFile				= xr_new< xr_vector<shared_str> >();
	LogFile->reserve	(1000);
}

void CreateLog			(BOOL nl)
{
    no_log				= nl;
	strconcat			(sizeof(log_file_name),log_file_name,Core.ApplicationName,"_",Core.UserName,".log");
	if (FS.path_exist("$logs$"))
		FS.update_path	(logFName,"$logs$",log_file_name);
	if (!no_log){
        IWriter *f		= FS.w_open	(logFName);
        if (f==NULL){
        	MessageBox	(NULL,"Can't create log file.","Error",MB_ICONERROR);
        	abort();
        }
        FS.w_close		(f);
    }
}

void CloseLog(void)
{
	FlushLog		();
 	LogFile->clear	();
	xr_delete		(LogFile);
}
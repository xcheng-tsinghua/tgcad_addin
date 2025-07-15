#pragma once
#include <string>
#include "log_interface.h"
#include "utils.h"

#define WRITELOG_STRING(TYPE, FORMAT, ...) CMyLog::Instance()->writelog_string(__FILE__,__LINE__,TYPE,FORMAT,__VA_ARGS__)
#define WRITELOG_STRING_F(TYPE, FORMAT, ...) CMyLog::Instance()->writelog_string_f(__FILE__,__LINE__,__FUNCTION__,TYPE,FORMAT,__VA_ARGS__)

class ILog;
class CMyLog
{
public:
	~CMyLog(void);

	static CMyLog *Instance();

	int InitLog(const std::string &strRoot);

	void writelog_string(const char *sFileName,int nFileLine,const char* strTitle,const char* strContent, ...);
	void writelog_string_f(const char *sFileName,int nFileLine,const char* strFunction,const char* strTitle,const char* strContent, ...);

	void clear_log();

protected:
	CMyLog(void);

	int log_open(bool bOpen);

	int clear_files(const char* szDir, int nDays);

protected:
	//
	bool m_bWriteLog;
	//
	int m_nMaxLen;
	//
	std::string m_strRoot;

	//
	ILog *m_pLog;
	ILog *m_pFunctionLog;
};

class MyLog {
public:
	static MyLog* instance() {
		static MyLog log;
		return &log;
	}
protected:
	MyLog() {
		//InitializeCriticalSection(&m_section);
		m_curDir = "";
	}

public:

	virtual ~MyLog() {
		//DeleteCriticalSection(&m_section);
	}

	void writeLog(const char* info, const char* logFile) {

		//MyAutoLock logLock(&m_section);

		if (_access(logFile, 0) != 0)
			NDSUtils::createDirsA(logFile, true);

		FILE* pFile = fopen(logFile, "ab");
		if (pFile == NULL)
			return;

		fwrite(info, 1, strlen(info), pFile);

		fclose(pFile);
	}

private:
	//CRITICAL_SECTION m_section;
	CStringA m_curDir;
};


class FuncLog {

public:
	FuncLog(const char* info, const char* logFilePath) {

		//if(!m_toLog)
		//	return;

		static unsigned long index = 1;
		m_index = index;
		index++;

		if (index > 99999999)
			index = 1;

		now = 0;endTime = 0;
		m_funcName = info;
		start();

		std::string strDateTime;
		NDSUtils::getDateTime(strDateTime);

		CStringA cInfo;
		cInfo.Format("%s<<<[%lu]开始时间 %s\r\n", m_funcName.c_str(), m_index, strDateTime.c_str());

		m_logPath = logFilePath;

		MyLog::instance()->writeLog(cInfo, m_logPath);
	}

	~FuncLog() {

		//if(!m_toLog)
		//	return;

		stop();

		std::string strDateTime;
		NDSUtils::getDateTime(strDateTime);

		CStringA cInfo;
		///cInfo.Format("%s>>>%s[%lu]time interval: %d milliseconds\r\n",strDateTime.c_str(),m_funcName.c_str(),m_index,getTime());

		cInfo.Format("%s<<<[%lu]结束时间 %s,时间间隔（毫秒）: %d milliseconds\r\n\r\n", m_funcName.c_str(), m_index, strDateTime.c_str(), getTime());

		MyLog::instance()->writeLog(cInfo, m_logPath);
	}

	static void setFlag(bool toLog) {
		//m_toLog = toLog;
	}

protected:
	void start() {
		now = GetTickCount();
	}
	void stop() {
		endTime = GetTickCount();
	}
	unsigned long getTime() {
		return endTime - now;
	}
private:
	unsigned long now;
	unsigned long endTime;
	std::string m_funcName;
	unsigned long m_index;
	CStringA m_logPath;
	//static bool m_toLog;
};
//bool FuncLog::m_toLog;
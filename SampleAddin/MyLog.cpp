#include "stdafx.h"
#include "MyLog.h"
#include "log_implement.h"
#include "character.h"

CMyLog::CMyLog(void)
	: m_bWriteLog(false)
	, m_nMaxLen(1024)
	, m_pLog(NULL)
	, m_pFunctionLog(NULL)
{
}

CMyLog::~CMyLog(void)
{
}

CMyLog * CMyLog::Instance()
{
	static CMyLog obj;
	return &obj;
}

int CMyLog::InitLog(const std::string &strRoot)
{
	m_strRoot = strRoot;

	m_bWriteLog = true;
	m_nMaxLen = 1024;
	if (m_pLog != NULL)
	{
		m_pLog->Log_setFileMaxLen(m_nMaxLen * 1024);
	}
	if (m_pLog == NULL && m_bWriteLog)
	{
		log_open(true);
	}

	return 0;
}

int CMyLog::log_open(bool bOpen)
{
	if (m_pLog == NULL && bOpen)
	{
		m_pLog = Log_CreateObj(LOG_MODE_S,true);
		m_pLog->Log_setFileMaxLen(m_nMaxLen * 1024);

		char szDir[1024] = {0};
		sprintf(szDir, "%s\\log", m_strRoot.c_str());
		::SHCreateDirectoryExA(NULL, szDir, NULL);

		m_pLog->Log_open(szDir,"w");

		Sleep(500);
	}

	if (m_pLog != NULL && !bOpen)
	{
		Log_DeleteObj(m_pLog);
		m_pLog = NULL;
	}

	return 0;
}

void CMyLog::writelog_string(const char *sFileName,int nFileLine,const char* strTitle,const char* strContent, ...)
{
	if (m_bWriteLog/* || 0 == strcmp(strTitle, LOG_ERROR) || 0 == strcmp(strTitle, LOG_EXCEPTION) || 0 == strcmp(strTitle, LOG_SQL)*/)
	{
		if (m_pLog == NULL)
		{
			log_open(true);
		}

		char szBuffer[1024*10 + 1] = {0};
		va_list args;
		va_start(args, strContent);
		::_vsnprintf(szBuffer, 1024*10, strContent, args);
		va_end(args);

		CStringA strTmp = szBuffer;
		strTmp.Replace("\r", "");
		strTmp.Replace("\t", "");
		strTmp.Replace("\n", "");
		m_pLog->Log_write_string(sFileName,nFileLine,strTitle,strTmp);
	}
}

void CMyLog::writelog_string_f(const char *sFileName,int nFileLine,const char* strFunction,const char* strTitle,const char* strContent, ...)
{
	if (m_bWriteLog/* || 0 == strcmp(strTitle, LOG_ERROR) || 0 == strcmp(strTitle, LOG_EXCEPTION) || 0 == strcmp(strTitle, LOG_SQL)*/)
	{
		if (m_pLog == NULL)
		{
			log_open(true);
		}

		char szBuffer[1024*10 + 1] = {0};
		va_list args;
		va_start(args, strContent);
		::_vsnprintf(szBuffer, 1024*10, strContent, args);
		va_end(args);

		CStringA strTmp = szBuffer;
		strTmp.Replace("\r", "");
		strTmp.Replace("\t", "");
		strTmp.Replace("\n", "");
		m_pLog->Log_write_string(sFileName,nFileLine,strFunction,strTitle,strTmp);
	}
}

void CMyLog::clear_log()
{
	int nDays = GetPrivateProfileInt(_T("Log"), _T("days"), 7, _T("Log.ini")); 

	clear_files((m_strRoot + "\\log").c_str(), nDays);
}

int CMyLog::clear_files(const char* szDir, int nDays)
{
	int nLen = (int)strlen(szDir);
	if (szDir == NULL || nLen <= 0)
	{
		return -1;
	}	

	char szPath[1024] = {0};
	strcpy(szPath, szDir);
	if (szDir[nLen-1] != '\\') strcat(szPath, "\\");
	strcat(szPath, "*");

	WIN32_FIND_DATAA fd;
	HANDLE hFindFile = FindFirstFileA(szPath, &fd);
	if (hFindFile == INVALID_HANDLE_VALUE)
	{
		::FindClose(hFindFile);
		return -2;
	}

	char szTmp[1024] = {0}; 
	bool bFinish = false;
	while (!bFinish)
	{
		strcpy(szTmp, szDir);
		if (szDir[nLen-1] != '\\') strcat(szTmp, "\\");
		strcat(szTmp, fd.cFileName);

		bool bDirectory = ((fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0);

		if (bDirectory && (strcmp(fd.cFileName, ".") == 0 || strcmp(fd.cFileName, "..") == 0)) 
		{		
			bFinish = (FindNextFileA(hFindFile, &fd) == FALSE);
			continue;
		}

		if (bDirectory)
		{
			//是子目录
			clear_files(szTmp, nDays);
		}
		else
		{
			//1.求nDays天之前的日期和时间
			CTime ctPrev = CTime::GetCurrentTime() - CTimeSpan(nDays, 0, 0, 0);

			//2.求文件的最后修改时间
			CTime ctFile(fd.ftLastWriteTime);

			//
			if (ctFile < ctPrev)
			{
				::DeleteFileA(szTmp);
			}
		}

		bFinish = (FindNextFileA(hFindFile, &fd) == FALSE);
	}

	::FindClose(hFindFile);

	return 0;
}

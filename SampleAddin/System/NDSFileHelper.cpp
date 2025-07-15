#include "stdafx.h"
#include "NDSFileHelper.h"
#include "NDSStringHelper.h"
#include "./SolidEdgeHelper/SolidEdgeHelper.h"

#include "imagehlp.h"
#pragma comment(lib,"imagehlp.lib")
#include <string>

#include "afxdialogex.h"
#include "./NDSDefine.h"

CString NDSFileHelper::GetWindowsTempFolder()
{
	TCHAR lpTempPathBuffer[MAX_PATH];

	DWORD dwRetVal = ::GetTempPath(MAX_PATH,lpTempPathBuffer);

	CString tempFolderPath = lpTempPathBuffer;
	return tempFolderPath;
}

bool NDSFileHelper::CopyFile(const CString& originalFilePath, const CString& destFilePath)
{
	NDSFileHelper::CreateSubFolder(destFilePath);
	BOOL bRes = ::CopyFile(originalFilePath, destFilePath, FALSE);
	ASSERT(bRes);
	return bRes ? true : false;
}



void NDSFileHelper::CreateSubFolder(const CString& dirPath)
{
	if (dirPath.IsEmpty())
		return;

	MakeSureDirectoryPathExists(NDSStringHelper::ToString(dirPath).c_str());
}


CString NDSFileHelper::ToCfgFilePath(const CString& strAsmFilePath, bool bExist/* = true*/)
{
	CString cfgFilePath;
	if (!SolidEdgeHelper::FileNameIsAsm(strAsmFilePath))
		return cfgFilePath;

	if (bExist && !PathFileExists(strAsmFilePath))
		return cfgFilePath;

	int iFind = strAsmFilePath.ReverseFind('.');
	if (iFind != -1)
	{
		cfgFilePath = strAsmFilePath.Left(iFind);
		cfgFilePath += ".cfg";
		if (bExist && !PathFileExists(cfgFilePath))
		{
			cfgFilePath.Empty();
		}
	}
	return cfgFilePath;
}


CString NDSFileHelper::GetRelativePath(const CString& fileNamePath, const CString& fileNameParentPath)
{
	CString relatePath;
	if (!PathFileExists(fileNamePath) || !PathFileExists(fileNameParentPath))
		return relatePath;

	int iFind = fileNamePath.Find(fileNameParentPath);
	if (iFind == -1)
		return relatePath;

	relatePath = fileNamePath.Right(fileNamePath.GetLength() - fileNameParentPath.GetLength());
	if (0 != relatePath.Find(_T("\\")))
	{
		CString preStr = _T("\\");
		relatePath = preStr + relatePath;
	}

	return relatePath;
}

CString NDSFileHelper::GetFileCurFolder(const CString& fileNamePath)
{
	CString folder;
	int iFind = fileNamePath.ReverseFind(_T('\\'));
	if (iFind == -1)
		return folder;

	folder = fileNamePath.Left(iFind + 1);
	return folder;
}

CString NDSFileHelper::GetFileName(const CString& fileNamePath, bool bIncludeExt/* = true*/, bool bExist/* = true*/)
{
	CString strfileName;
	if (bExist && !PathFileExists(fileNamePath))
		return strfileName;

	TCHAR drive[MAX_PATH];
	TCHAR dir[MAX_PATH];
	TCHAR fileName[MAX_PATH];
	TCHAR extName[MAX_PATH];
	_tsplitpath(fileNamePath, drive, dir, fileName, extName);

	strfileName = fileName;
	if (bIncludeExt)
	{
		strfileName += extName;
	}

	return strfileName;
}

bool NDSFileHelper::IsValidFilePath(const CString& filePath, bool bExist/* = true*/)
{
	if (bExist && !PathFileExists(filePath))
		return false;

	TCHAR drive[MAX_PATH];
	TCHAR dir[MAX_PATH];
	TCHAR fileName[MAX_PATH];
	TCHAR extName[MAX_PATH];
	_tsplitpath(filePath, drive, dir, fileName, extName);
	if (CString(drive).IsEmpty())
		return false;



	return true;
}

CString NDSFileHelper::GetRelativePathByTwoFile(const CString& filePath, const CString& seedFilePath)
{
	string strConsultPath = NDSStringHelper::ToString(GetFileCurFolder(filePath));
	string strSeedPath = NDSStringHelper::ToString(GetFileCurFolder(seedFilePath));

	TCHAR drive[MAX_PATH];
	TCHAR dir[MAX_PATH];
	TCHAR fileName[MAX_PATH];
	TCHAR extName[MAX_PATH];
	_tsplitpath(filePath, drive, dir, fileName, extName);
	CString curDirvie = drive;


	string samePath = NDSStringHelper::FindMaxSubString(strConsultPath, strSeedPath);

	string subConsultPath = NDSStringHelper::ToString(NDSFileHelper::GetRelativePath(NDSStringHelper::ToCString(strConsultPath), NDSStringHelper::ToCString(samePath)));
	string subSeedPath = NDSStringHelper::ToString(NDSFileHelper::GetRelativePath(NDSStringHelper::ToCString(strSeedPath), NDSStringHelper::ToCString(samePath)));

	std::string strRet;
	while (1)
	{
		//当前程序的上一级目录
		int nPos = subSeedPath.rfind('\\');
		if (-1 != nPos)
		{
			subSeedPath = subSeedPath.substr(0, nPos);
			strRet += "..\\";
		}
		else
		{
			break;
		}
	}
	strRet += subConsultPath;

	CString retPath = NDSStringHelper::ToCString(strRet);
	return retPath;
}

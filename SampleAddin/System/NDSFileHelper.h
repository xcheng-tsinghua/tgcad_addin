#pragma once

#include <string>

class NDSFileHelper
{
private:
	NDSFileHelper(void){};
	~NDSFileHelper(void){};

public:

	static CString GetWindowsTempFolder();

	static bool CopyFile(const CString& originalFilePath, const CString& destFilePath);



	static void CreateSubFolder(const CString& dirPath);


	static CString ToCfgFilePath(const CString& strAsmFilePath, bool bExist = true);

	static CString GetRelativePath(const CString& fileNamePath, const CString& fileNameParentPath);
	static CString GetFileCurFolder(const CString& fileNamePath);
	static CString GetFileName(const CString& fileNamePath, bool bIncludeExt = true, bool bExist = true);
	static bool IsValidFilePath(const CString& filePath, bool bExist = true);

	static CString GetRelativePathByTwoFile(const CString& filePath, const CString& seedFilePath);


private:

};


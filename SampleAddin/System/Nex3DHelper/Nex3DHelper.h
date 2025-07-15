#pragma once

#include <vector>
#include "../NDSDefine.h"

class Nex3DHelper
{
public:
	Nex3DHelper();
	virtual ~Nex3DHelper();

public:

	static bool IsCurActiveDocIsTemp();
	static CString GetCurActiveDocName();
	static CString GetCurActiveDocFullPath();
	static DocumentTypeConstants Nex3DHelper::GetCurActiveDocType();
	static bool CurActiveDocTypeIsAsm();
	static bool CurActiveDocTypeIsPar();
	static bool CurActiveDocTypeIsDft();
	static bool CurActiveDocTypeIsPsm();

	static bool FileNameIsAsm(const CString& fileNamePath);
	static bool FileNameIsPar(const CString& fileNamePath);
	static bool FileNameIsDft(const CString& fileNamePath);
	static bool FileNameIsCfg(const CString& fileNamePath);
	static bool FileNameIsPsm(const CString& fileNamePath);

	static SolidEdgeFramework::SolidEdgeDocumentPtr GetCurActiveDoc();
	static bool SaveAsCurActiveDocToJpg(const CString& jpgFilePath);
	static bool SaveAsActiveWindowToJpg(ApplicationPtr AppPtr, const CString& jpgFilePath, bool bFitView = true);

	static bool OpenFile(const CString& filePath);
	static bool CloseFile(const CString& filePath);
	static bool CloseCurActiveFile();

	static bool SaveCurActiveDocToFilePath(const CString& saveFilePath);

	static bool GetAllCurOpenDocs(std::vector<CString>& getFiles);

	static bool DocIsDirty(const CString& fileNamePath);

	static bool QuitCAD();

	static HWND GetCADHwnd();

	static bool ExportCurActiveDoc(const CString& exportDocFileNamePath);


	static bool CallCadCommand(long cmdId);



};
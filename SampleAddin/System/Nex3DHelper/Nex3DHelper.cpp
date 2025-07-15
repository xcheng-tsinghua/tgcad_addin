

#include "stdafx.h"
#include "Nex3DHelper.h"
#include "../../SampleAddin.h"
#include "../../SEAddIn.h"
#include "../NDSFileHelper.h"
#include "../NDSStringHelper.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


Nex3DHelper::Nex3DHelper()
{

}

Nex3DHelper::~Nex3DHelper()
{

}

bool Nex3DHelper::IsCurActiveDocIsTemp()
{
	SEAddin* pAddin = CSampleAddinApp::GetSEApp()->GetSEAddin();
	if (!pAddin)
		return true;

	ApplicationPtr AppPtr = pAddin->GetApplication();
	DocumentsPtr pDocs = AppPtr->Documents;
	if (pDocs->GetCount() <= 0)
		return true;

	SolidEdgeDocumentPtr pActiveDoc = AppPtr->ActiveDocument;
	if (!pActiveDoc)
		return true;

	CString fileName = (LPCTSTR)pActiveDoc->GetName();
	CString fullname = (LPCTSTR)pActiveDoc->GetFullName();
	if (PathFileExists(fullname))
	{
		return false;
	}


	return true;
}

CString Nex3DHelper::GetCurActiveDocName()
{
	CString fileName;

	SEAddin* pAddin = CSampleAddinApp::GetSEApp()->GetSEAddin();
	if (!pAddin)
		return fileName;

	ApplicationPtr AppPtr = pAddin->GetApplication();
	DocumentsPtr pDocs = AppPtr->Documents;
	if (pDocs->GetCount() <= 0)
		return fileName;
	SolidEdgeDocumentPtr pActiveDoc = AppPtr->ActiveDocument;
	if (pActiveDoc)
	{
		fileName = (LPCTSTR)pActiveDoc->GetName();
		CString fullname = (LPCTSTR)pActiveDoc->GetFullName();
	}

	return fileName;
}

CString Nex3DHelper::GetCurActiveDocFullPath()
{
	CString fullname;

	SEAddin* pAddin = CSampleAddinApp::GetSEApp()->GetSEAddin();
	if (!pAddin)
		return fullname;

	ApplicationPtr AppPtr = pAddin->GetApplication();
	DocumentsPtr pDocs = AppPtr->Documents;
	if (pDocs->GetCount() <= 0)
		return fullname;

	SolidEdgeDocumentPtr pActiveDoc = AppPtr->ActiveDocument;
	if (pActiveDoc)
	{
		fullname = (LPCTSTR)pActiveDoc->GetFullName();
	}

	return fullname;
}

DocumentTypeConstants Nex3DHelper::GetCurActiveDocType()
{
	DocumentTypeConstants docType = DocumentTypeConstants::igUnknownDocument;

	SEAddin* pAddin = CSampleAddinApp::GetSEApp()->GetSEAddin();
	if (!pAddin)
		return docType;

	ApplicationPtr AppPtr = pAddin->GetApplication();
	DocumentsPtr pDocs = AppPtr->Documents;
	if (pDocs->GetCount() <= 0)
		return docType;

	SolidEdgeDocumentPtr pActiveDoc = AppPtr->ActiveDocument;
	if (pActiveDoc)
	{
		docType = pActiveDoc->GetType();
	}

	return docType;
}

bool Nex3DHelper::CurActiveDocTypeIsAsm()
{
	if (DocumentTypeConstants::igAssemblyDocument == GetCurActiveDocType())
		return true;

	return false;
}

bool Nex3DHelper::CurActiveDocTypeIsPar()
{
	if (DocumentTypeConstants::igPartDocument == GetCurActiveDocType())
		return true;

	return false;
}

bool Nex3DHelper::CurActiveDocTypeIsDft()
{
	if (DocumentTypeConstants::igDraftDocument == GetCurActiveDocType())
		return true;

	return false;
}

bool Nex3DHelper::CurActiveDocTypeIsPsm()
{
	if (DocumentTypeConstants::igSheetMetalDocument == GetCurActiveDocType())
		return true;

	return false;
}

bool Nex3DHelper::FileNameIsAsm(const CString& fileNamePath)
{
	TCHAR drive[MAX_PATH];
	TCHAR dir[MAX_PATH];
	TCHAR fileName[MAX_PATH];
	TCHAR extName[MAX_PATH];
	_tsplitpath(fileNamePath, drive, dir, fileName, extName);

	CString strExtName = extName;
	if (strExtName.IsEmpty())
		return false;

	strExtName.MakeLower();
	if (strExtName == _T(".asm"))
		return true;

	return false;
}

bool Nex3DHelper::FileNameIsPar(const CString& fileNamePath)
{
	TCHAR drive[MAX_PATH];
	TCHAR dir[MAX_PATH];
	TCHAR fileName[MAX_PATH];
	TCHAR extName[MAX_PATH];
	_tsplitpath(fileNamePath, drive, dir, fileName, extName);

	CString strExtName = extName;
	if (strExtName.IsEmpty())
		return false;

	strExtName.MakeLower();
	if (strExtName == _T(".par"))
		return true;

	return false;
}

bool Nex3DHelper::FileNameIsDft(const CString& fileNamePath)
{
	TCHAR drive[MAX_PATH];
	TCHAR dir[MAX_PATH];
	TCHAR fileName[MAX_PATH];
	TCHAR extName[MAX_PATH];
	_tsplitpath(fileNamePath, drive, dir, fileName, extName);

	CString strExtName = extName;
	if (strExtName.IsEmpty())
		return false;

	strExtName.MakeLower();
	if (strExtName == _T(".dft"))
		return true;

	return false;
}

bool Nex3DHelper::FileNameIsCfg(const CString& fileNamePath)
{
	TCHAR drive[MAX_PATH];
	TCHAR dir[MAX_PATH];
	TCHAR fileName[MAX_PATH];
	TCHAR extName[MAX_PATH];
	_tsplitpath(fileNamePath, drive, dir, fileName, extName);

	CString strExtName = extName;
	if (strExtName.IsEmpty())
		return false;

	strExtName.MakeLower();
	if (strExtName == _T(".cfg"))
		return true;

	return false;
}

bool Nex3DHelper::FileNameIsPsm(const CString& fileNamePath)
{
	TCHAR drive[MAX_PATH];
	TCHAR dir[MAX_PATH];
	TCHAR fileName[MAX_PATH];
	TCHAR extName[MAX_PATH];
	_tsplitpath(fileNamePath, drive, dir, fileName, extName);

	CString strExtName = extName;
	if (strExtName.IsEmpty())
		return false;

	strExtName.MakeLower();
	if (strExtName == _T(".psm"))
		return true;

	return false;
}



SolidEdgeFramework::SolidEdgeDocumentPtr Nex3DHelper::GetCurActiveDoc()
{
	SEAddin* pAddin = CSampleAddinApp::GetSEApp()->GetSEAddin();
	if (!pAddin)
		return NULL;

	ApplicationPtr AppPtr = pAddin->GetApplication();
	return AppPtr->ActiveDocument;
}


bool Nex3DHelper::SaveAsCurActiveDocToJpg(const CString& jpgFilePath)
{
	if (jpgFilePath.IsEmpty())
		return false;


	SEAddin* pAddin = CSampleAddinApp::GetSEApp()->GetSEAddin();
	if (!pAddin)
		return false;

	ApplicationPtr AppPtr = pAddin->GetApplication();
	DocumentsPtr pDocs = AppPtr->Documents;
	if (pDocs->GetCount() <= 0)
		return false;

	SolidEdgeDocumentPtr pActiveDoc = AppPtr->ActiveDocument;
	if (!pActiveDoc)
		return false;

	bool bCurActiveDocIsDirty = pActiveDoc->GetDirty() == VARIANT_TRUE ? true : false;

	bool bRet = SaveAsActiveWindowToJpg(AppPtr, jpgFilePath, false);

	if (!bCurActiveDocIsDirty)
	{
		bool curIsDirty = pActiveDoc->GetDirty() == VARIANT_TRUE ? true : false;
		if (curIsDirty)
		{
	//		pActiveDoc->Save();
		}
	}
	return bRet;

	////copy file
	//CString currentFileFolder = NDSFileHelper::GetWindowsTempFolder();
	//CString tempfile = currentFileFolder;
	//tempfile += NDSStringHelper::ToCString(NDSStringHelper::Guid());

	//CString extName;
	//if (Nex3DHelper::GetCurActiveDocType() == igAssemblyDocument)
	//{
	//	extName = _T(".asm");
	//}
	//else if (Nex3DHelper::GetCurActiveDocType() == igPartDocument)
	//{
	//	extName = _T(".par");
	//}
	//else if (Nex3DHelper::GetCurActiveDocType() == igDraftDocument)
	//{
	//	extName = _T(".dft");
	//}
	//else if (Nex3DHelper::GetCurActiveDocType() == igSheetMetalDocument)
	//{
	//	extName = _T(".psm");
	//}
	//else
	//	return false;


	//tempfile = tempfile + extName;

	//HRESULT hr = pActiveDoc->SaveCopyAs((_bstr_t)tempfile);
	//if (hr != S_OK)
	//	return false;

	//bool bRes = false;
	//int iDocRelationAutoServer = 8; //Currently implemented partially. Users can use "8" to open a document in hidden state, for don't added recent file list
	//SolidEdgeDocumentPtr pDoc = pDocs->Open((_bstr_t)tempfile, iDocRelationAutoServer);
	//if (pDoc)
	//{
	//	pDoc->Activate();
	//	bRes = SaveAsActiveWindowToJpg(AppPtr, jpgFilePath);

	//	pDoc->Save();
	//	pDoc->Close();

	//	if (pActiveDoc)
	//	{
	//		pActiveDoc->Activate();
	//		if (!bCurActiveDocIsDirty)
	//		{
	//			bool curIsDirty = pActiveDoc->GetDirty();
	//			if (curIsDirty)
	//			{
	//				pActiveDoc->Save();
	//			}
	//		}
	//	}
	//}

	//::DeleteFile(tempfile);

	//return bRes;
}

bool Nex3DHelper::SaveAsActiveWindowToJpg(ApplicationPtr AppPtr, const CString& jpgFilePath, bool bFitView/* = true*/)
{
	if (!AppPtr)
		return false;

	SolidEdgeDocumentPtr pActiveDoc = AppPtr->ActiveDocument;
	if (pActiveDoc)
	{
		WindowPtr win = AppPtr->GetActiveWindow();
		SolidEdgeDraft::SheetWindowPtr sheetwin = AppPtr->GetActiveWindow();
		ViewPtr view = NULL;
		double resolution = 1;  // DPI - Larger values have better quality but also lead to larger file.
		int colorDepth = 24;
		if (win)
		{
			int width = win->GetUsableWidth();
			int height = win->GetUsableHeight();

			view = win->GetView();
			if (bFitView && view)
			{
				view->Fit();
			}
			HRESULT hr = view->SaveAsImage((_bstr_t)jpgFilePath, width, height, NULL, resolution, colorDepth, SolidEdgeFramework::SeImageQualityType::seImageQualityHigh, false);
			if (hr != S_OK)
				return false;
		}
		else if (sheetwin)
		{
			if (bFitView)
				sheetwin->FitEx(SolidEdgeDraft::igFitAll);

			int width = sheetwin->GetUsableWidth();
			int height = sheetwin->GetUsableHeight();

			HRESULT hr = sheetwin->SaveAsImage((_bstr_t)jpgFilePath, width, height, resolution, colorDepth, SolidEdgeFramework::SeImageQualityType::seImageQualityHigh, false);
			if (hr != S_OK)
				return false;
		}
		return true;
	}
	return false;
}

bool Nex3DHelper::OpenFile(const CString& filePath)
{

	SEAddin* pAddin = CSampleAddinApp::GetSEApp()->GetSEAddin();
	if (!pAddin)
		return false;


	ApplicationPtr AppPtr = pAddin->GetApplication();
	DocumentsPtr pDocs = AppPtr->Documents;
	SolidEdgeDocumentPtr pDoc = pDocs->Open((_bstr_t)filePath);

	return true;
}

bool Nex3DHelper::CloseFile(const CString& filePath)
{

	SEAddin* pAddin = CSampleAddinApp::GetSEApp()->GetSEAddin();
	if (!pAddin)
		return false;

	ApplicationPtr AppPtr = pAddin->GetApplication();
	DocumentsPtr pDocs = AppPtr->Documents;
	if (pDocs->GetCount() <= 0)
		return false;

	HRESULT hr = pDocs->CloseDocument((_bstr_t)filePath);

	if (hr == S_OK)
		return true;

	return false;
}

bool Nex3DHelper::CloseCurActiveFile()
{

	SEAddin* pAddin = CSampleAddinApp::GetSEApp()->GetSEAddin();
	if (!pAddin)
		return false;

	ApplicationPtr AppPtr = pAddin->GetApplication();
	DocumentsPtr pDocs = AppPtr->Documents;
	SolidEdgeDocumentPtr pActiveDoc = NULL;

	if (pDocs->GetCount() > 0)
	{
		pActiveDoc = AppPtr->GetActiveDocument();
		if (pActiveDoc)
		{
			pActiveDoc->Close();
			return true;
		}
	}
	return false;
}

bool Nex3DHelper::SaveCurActiveDocToFilePath(const CString& saveFilePath)
{

	SEAddin* pAddin = CSampleAddinApp::GetSEApp()->GetSEAddin();
	if (!pAddin)
		return false;

	ApplicationPtr AppPtr = pAddin->GetApplication();
	DocumentsPtr pDocs = AppPtr->Documents;
	if (pDocs->GetCount() <= 0)
		return false;

	SolidEdgeDocumentPtr pActiveDoc = AppPtr->ActiveDocument;
	if (!pActiveDoc)
		return false;

	HRESULT hr = pActiveDoc->SaveAs((_bstr_t)saveFilePath);
	if (hr != S_OK)
		return false;

	return true;
}


bool Nex3DHelper::GetAllCurOpenDocs(std::vector<CString>& getFiles)
{
	DocumentTypeConstants docType = DocumentTypeConstants::igUnknownDocument;

	SEAddin* pAddin = CSampleAddinApp::GetSEApp()->GetSEAddin();
	if (!pAddin)
		return false;

	ApplicationPtr AppPtr = pAddin->GetApplication();
	DocumentsPtr pDocs = AppPtr->Documents;
	if (pDocs->GetCount() <= 0)
		return false;

	for (long i = 1; i <= pDocs->GetCount(); ++i)
	{
		SolidEdgeDocumentPtr pDoc = pDocs->Item(i);
		CString fileName = (LPCTSTR)pDoc->GetFullName();
		getFiles.push_back(fileName);
	}
	if (getFiles.empty())
		return false;

	return true;
}


bool Nex3DHelper::DocIsDirty(const CString& fileNamePath)
{
	if (fileNamePath.IsEmpty())
		return false;


	SEAddin* pAddin = CSampleAddinApp::GetSEApp()->GetSEAddin();
	if (!pAddin)
		return false;

	ApplicationPtr AppPtr = pAddin->GetApplication();
	DocumentsPtr pDocs = AppPtr->Documents;
	if (pDocs->GetCount() <= 0)
		return false;

	for (long i = 1; i <= pDocs->GetCount(); ++i)
	{
		SolidEdgeDocumentPtr pDoc = pDocs->Item(i);
		CString fileName = (LPCTSTR)pDoc->GetFullName();
		if (fileNamePath == fileName)
		{
			VARIANT_BOOL bDirty = pDoc->GetDirty();
			return bDirty == VARIANT_TRUE ? true : false;;
		}
	}
	return false;
}

HWND Nex3DHelper::GetCADHwnd()
{
	CWnd* pWnd = CSampleAddinApp::GetSEApp()->GetMainWnd();
	if (pWnd)
	{
		return pWnd->m_hWnd;
	}

	return NULL;
}

bool Nex3DHelper::QuitCAD()
{

	SEAddin* pAddin = CSampleAddinApp::GetSEApp()->GetSEAddin();
	if (!pAddin)
		return false;

	ApplicationPtr AppPtr = pAddin->GetApplication();
	AppPtr->Quit();
	return true;
}

bool Nex3DHelper::ExportCurActiveDoc(const CString& exportDocFileNamePath)
{

	SEAddin* pAddin = CSampleAddinApp::GetSEApp()->GetSEAddin();
	if (!pAddin)
		return false;

	ApplicationPtr AppPtr = pAddin->GetApplication();
	DocumentsPtr pDocs = AppPtr->Documents;
	if (pDocs->GetCount() <= 0)
		return false;

	SolidEdgeDocumentPtr pActiveDoc = AppPtr->ActiveDocument;

	HRESULT hr = pActiveDoc->SaveAs((_bstr_t)exportDocFileNamePath);
	if (hr != S_OK)
		return false;

	return true;
}

bool Nex3DHelper::CallCadCommand(long cmdId)
{
	try
	{
		SEAddin* pAddin = CSampleAddinApp::GetSEApp()->GetSEAddin();
		if (!pAddin)
			return false;

		ApplicationPtr AppPtr = pAddin->GetApplication();
		HRESULT hr = AppPtr->StartCommand((SolidEdgeFramework::SolidEdgeCommandConstants)cmdId);
		if (hr == S_OK)
			return true;
	}
	catch (...)
	{

	}


	return false;
}

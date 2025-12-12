#include "stdafx.h"
#include "SEUtils.h"
#include "AddinApp.h"
#include "SKUtils.h"


namespace SEUtils
{

	SKPnt ScreenToModel(SKPnt_2d screen_pnt, WindowPtr wnd)
	{
		double WorldX = 0, WorldY = 0, WorldZ = 0;

		wnd->View->TransformDCToModel(screen_pnt.X(), screen_pnt.Y(), &WorldX, &WorldY, &WorldZ);

		return SKPnt(WorldX, WorldY, WorldZ);
	}

	SKPnt ScreenToModel(const double& sc_x, const double& sc_y, WindowPtr wnd)
	{
		return ScreenToModel(SKPnt_2d(sc_x, sc_y), wnd);
	}

	void UpdateScreen(WindowPtr wnd)
	{
		// 当指针为空时出现提示
		if (!wnd)
		{
			MesgBox("更新显示时输入了空窗口指针！");
		}
		else
		{
			wnd->View->Update();
		}

	}

	double ModelDistToScreen(SKPnt model_pnt1, SKPnt model_pnt2, WindowPtr wnd)
	{
		SKPnt_2d ScreenPnt1 = ModelToScreen(model_pnt1, wnd);
		SKPnt_2d ScreenPnt2 = ModelToScreen(model_pnt2, wnd);

		return ScreenPnt1.Distance(ScreenPnt2);

	}

	double ModelDistToScreen(const double& SapceDist, WindowPtr wnd)
	{
		return ModelDistToScreen(SKPnt(0, 0, 0), SKPnt(SapceDist, 0, 0), wnd);
	}

	double RefDistToScreen(SKPnt_2d ref_pnt1, SKPnt_2d ref_pnt2, WindowPtr wnd)
	{
		SKPnt_2d screen_pnt1 = RefToScreen(ref_pnt1, wnd);
		SKPnt_2d screen_pnt2 = RefToScreen(ref_pnt2, wnd);

		return screen_pnt1.Distance(screen_pnt2);

	}

	double RefDistToScreen(const double& ref_dist, WindowPtr wnd)
	{
		return RefDistToScreen(SKPnt_2d(0.0, 0.0), SKPnt_2d(ref_dist, 0.0), wnd);

	}

	SKPnt RefToModel(SKPnt_2d ref_pnt)
	{
		double model_x = 0.0, model_y = 0.0, model_z = 0.0;
		ProfilePtr(PartDocumentPtr(CSampleAddinApp::GetApplication()->GetActiveDocument())->ActiveSketch)->Convert2DCoordinate(ref_pnt.X(), ref_pnt.Y(), &model_x, &model_y, &model_z);

		return SKPnt(model_x, model_y, model_z);

	}

	SKPnt_2d RefToScreen(SKPnt_2d ref_pnt, WindowPtr wnd)
	{
		// 先将参考面上的点转化为模型空间中的点
		SKPnt model_pnt = RefToModel(ref_pnt);

		// 再将模型空间中的点转化为屏幕上的点
		SKPnt_2d screen_pnt = ModelToScreen(model_pnt, wnd);

		return screen_pnt;

	}

	vector<SKPnt_2d> RefToScreen(vector<SKPnt_2d> ref_pnt, WindowPtr wnd)
	{
		vector<SKPnt_2d> screen_pnts;
		for (const auto& c_ref_pnt : ref_pnt)
		{
			screen_pnts.push_back(RefToScreen(c_ref_pnt, wnd));
		}

		return screen_pnts;

	}


	void ViewTo(double vx, double vy, double vz, double tx, double ty, double tz, double ux, double uy, double uz, double scale, WindowPtr wnd)
	{
		double EyeX, EyeY, EyeZ, TargetX, TargetY, TargetZ, UpX, UpY, UpZ, ScaleOrAngle;
		VARIANT_BOOL Perspective;

		wnd->View->GetCamera(&EyeX, &EyeY, &EyeZ, &TargetX, &TargetY, &TargetZ, &UpX, &UpY, &UpZ, &Perspective, &ScaleOrAngle);

		double EyeXSt = EyeX, EyeYSt = EyeY, EyeZSt = EyeZ, TargetXSt = TargetX, TargetYSt = TargetY, TargetZSt = TargetZ, UpXSt = UpX, UpYSt = UpY, UpZSt = UpZ, ScaleOrAngleSt = ScaleOrAngle;
		double EyeXEd = vx, EyeYEd = vy, EyeZEd = vz, TargetXEd = tx, TargetYEd = ty, TargetZEd = tz, UpXEd = ux, UpYEd = uy, UpZEd = uz, ScaleOrAngleEd = scale;

		int ItaNum = 20;
		for (int i = 0; i < ItaNum; i++)
		{
			double currEyeX = EyeXSt + ((EyeXEd - EyeXSt) / (ItaNum - 1)) * i;
			double currEyeY = EyeYSt + ((EyeYEd - EyeYSt) / (ItaNum - 1)) * i;
			double currEyeZ = EyeZSt + ((EyeZEd - EyeZSt) / (ItaNum - 1)) * i;
			double currTarX = TargetXSt + ((TargetXEd - TargetXSt) / (ItaNum - 1)) * i;
			double currTarY = TargetYSt + ((TargetYEd - TargetYSt) / (ItaNum - 1)) * i;
			double currTarZ = TargetZSt + ((TargetZEd - TargetZSt) / (ItaNum - 1)) * i;
			double currUpX = UpXSt + ((UpXEd - UpXSt) / (ItaNum - 1)) * i;
			double currUpY = UpYSt + ((UpYEd - UpYSt) / (ItaNum - 1)) * i;
			double currUpZ = UpZSt + ((UpZEd - UpZSt) / (ItaNum - 1)) * i;
			double currScale = ScaleOrAngleSt + ((ScaleOrAngleEd - ScaleOrAngleSt) / (ItaNum - 1)) * i;

			static_cast<WindowPtr>(CSampleAddinApp::GetApplication()->GetWindows()->Item(1))->View->SetCamera(currEyeX, currEyeY, currEyeZ, currTarX, currTarY, currTarZ, currUpX, currUpY, currUpZ, 0, currScale);

			static_cast<WindowPtr>(CSampleAddinApp::GetApplication()->GetWindows()->Item(1))->View->Update();

			//Sleep(2000.0 / ItaNum);

		}


	}

	void MesgBox(const char* _mesg)
	{
		CString ErrorMessage = _mesg;
		MessageBox(NULL, ErrorMessage, TEXT("information"), MB_OK | MB_YESNO);

	}

	SKPnt_2d ScreenToRef(double fpCursorX, double fpCursorY, WindowPtr wnd)
	{
		// 先将鼠标坐标转化为模型三维坐标系坐标
		double WorldX = 0, WorldY = 0, WorldZ = 0;
		SKPnt model_pnt = ScreenToModel(fpCursorX, fpCursorY, wnd);

		// 再将模型三维坐标系坐标转化为参考面二维坐标
		double refX = 0, refY = 0;
		ProfilePtr(PartDocumentPtr(CSampleAddinApp::GetApplication()->GetActiveDocument())->ActiveSketch)->Convert3DCoordinate(model_pnt.X(), model_pnt.Y(), model_pnt.Z(), &refX, &refY);

		return SKPnt_2d(refX, refY);

	}

	SKPnt_2d ScreenToRef(SKPnt_2d screen_pnt, WindowPtr wnd)
	{
		return ScreenToRef(screen_pnt.X(), screen_pnt.Y(), wnd);

	}

	double ScreenDistToRef(SKPnt_2d screen_pnt1, SKPnt_2d screen_pnt2, WindowPtr wnd)
	{
		// 先将屏幕坐标转化为参考面坐标
		SKPnt_2d ref_pnt1 = ScreenToRef(screen_pnt1, wnd);
		SKPnt_2d ref_pnt2 = ScreenToRef(screen_pnt2, wnd);

		return ref_pnt1.Distance(ref_pnt2);

	}

	double ScreenDistToRef(const double& scren_dist, WindowPtr wnd)
	{
		return ScreenDistToRef(SKPnt_2d(0.0, 0.0), SKPnt_2d(0.0, scren_dist), wnd);
	}

	SKPnt_2d ViewToRef(const double& view_x, const double& view_y, const double& view_z, WindowPtr wnd)
	{
		// 先将当前视图xyz转化到模型坐标系
		double model_x = 0, model_y = 0, model_z = 0;
		wnd->View->TransformViewToModel(view_x, view_y, view_z, &model_x, &model_y, &model_z);

		// 再将模型坐标系xyz转化为草图参考面局部坐标
		double refX = 0, refY = 0;
		ProfilePtr(PartDocumentPtr(CSampleAddinApp::GetApplication()->GetActiveDocument())->ActiveSketch)->Convert3DCoordinate(model_x, model_y, model_z, &refX, &refY);

		return SKPnt_2d(refX, refY);

	}

	SKPnt_2d ViewToScreen(const double& view_x, const double& view_y, const double& view_z, WindowPtr wnd)
	{
		// 先将当前视图xyz转化到模型坐标系
		double model_x = 0, model_y = 0, model_z = 0;
		wnd->View->TransformViewToModel(view_x, view_y, view_z, &model_x, &model_y, &model_z);

		// 再将模型坐标系xyz转化为屏幕像素坐标
		long dc_x = 0, dc_y = 0;
		wnd->View->TransformModelToDC(model_x, model_y, model_z, &dc_x, &dc_y);

		return SKPnt_2d(dc_x, dc_y);

	}

	SKPnt_2d ModelToScreen(const double& model_x, const double& model_y, const double& model_z, WindowPtr wnd)
	{
		long dc_x = 0, dc_y = 0;
		wnd->View->TransformModelToDC(model_x, model_y, model_z, &dc_x, &dc_y);

		return SKPnt_2d(dc_x, dc_y);

	}

	SKPnt_2d ModelToScreen(SKPnt model_pnt, WindowPtr wnd)
	{
		return ModelToScreen(model_pnt.X(), model_pnt.Y(), model_pnt.Z(), wnd);

	}

}


HRESULT GetSolidEdgeVersion(DWORD& dwMajor, DWORD& dwMinor, ApplicationPtr& pEdge)
{
	HRESULT hr = S_OK;

	if (pEdge)
	{
		_bstr_t strVer = pEdge->Version;

		// Note: In stdafx.h, I have defined _ATL_CSTRING_EXPLICIT_CONSTRUCTORS. That caused the
		// assignment of sVer = strVer.GetBSTR() to generate an error. So I pass the BSTR to the
		// CString via the constructor ( sVer(strVer.GetBSTR()). I am now building with Visual Studio 2008
		// (MFC ver is 9). If compiling with a previous version of MFC, the code may need to be
		// changed back if it fails to compile.
		CString sVer(strVer.GetBSTR());

		if (!sVer.IsEmpty())
		{
			CString sMajor, sMinor, sTmp;

			// Brute force but straight-forward parsing of the string to get the major and minor version.
			int index = sVer.Find(_T("."));
			if (-1 != index)
			{
				sMajor = sVer.Left(index);

				sTmp = sVer.Right(sVer.GetLength() - index - 1);

				index = sTmp.Find(_T("."));
				if (-1 != index)
				{
					sTmp = sTmp.Right(sTmp.GetLength() - index - 1);

					index = sTmp.Find(_T("."));

					if (-1 != index)
					{
						sMinor = sTmp.Left(index);
					}
				}
			}
			if (!sMajor.IsEmpty())
			{
				dwMajor = _tstoi(sMajor);
			}
			if (!sMinor.IsEmpty())
			{
				dwMinor = _tstoi(sMinor);
			}
		}
	}
	else
	{
		hr = E_POINTER;
	}

	return hr;
}

// This routine returns the document type given the dispatch of a Solid Edge document.
// Returns E_INVALIDARG if pDocDispatch is NULL. Otherwise, returns the result from the Invoke
// method on that dispatch.
HRESULT GetDocType(LPDISPATCH pDocDispatch, DocumentTypeConstants& nDocType)
{
	HRESULT hr = NOERROR;

	nDocType = igUnknownDocument;

	try
	{
		if (NULL != pDocDispatch)
		{
			DISPID rgDispId = 0;

			OLECHAR* Names[1] = { L"Type" };

			hr = pDocDispatch->GetIDsOfNames(IID_NULL,
				Names,
				1,
				LOCALE_USER_DEFAULT,
				&rgDispId);

			if (SUCCEEDED(hr))
			{
				VARIANT varResult;
				VariantInit(&varResult);
				V_VT(&varResult) = VT_I4;

				DISPPARAMS disp = { NULL, NULL, 0, 0 };

				// Get the document type property.
				hr = pDocDispatch->Invoke(rgDispId,
					IID_NULL,
					LOCALE_USER_DEFAULT,
					DISPATCH_PROPERTYGET,
					&disp,
					&varResult,
					NULL,
					NULL);
				if (SUCCEEDED(hr))
				{
					nDocType = (DocumentTypeConstants)(V_I4(&varResult));
				}
				else
				{
					_com_issue_errorex(hr, pDocDispatch, __uuidof(pDocDispatch));
				}
			}

		}
		else
		{
			hr = E_INVALIDARG;
		}
	}
	catch (_com_error& e)
	{
		hr = e.Error();
	}

	return hr;
}

// This routine will return the type of located element given the dispatch interface of the element.
// If the type could not be determined, zero is returned (no element has a type value of zero)
// Returns E_INVALIDARG if pGraphicDispatch is NULL. Otherwise, returns the code from the Invoke
// method on the input dispatch interface.

// Note: The returned integer representing the object type should match one of the enumerated types
//       from either the SolidEdgeFramework::ObjectType or the SolidEdgeGeometry::GNTTypePropertyConstants
//       enumerations. Luckily, those two enumerated sets are defined such that there is no value
//       in common between the two. Which one of the two is expected to be returned from this routine
//       depends on the context in which the call is made.

HRESULT GetLocatedGraphicType(LPDISPATCH pGraphicDispatch, int& nObjectType)
{
	HRESULT hr = NOERROR;

	try
	{
		nObjectType = 0;

		if (pGraphicDispatch)
		{
			DISPID rgDispId = 0;

			OLECHAR* Names[1] = { L"Type" };

			hr = pGraphicDispatch->GetIDsOfNames(IID_NULL,
				Names,
				1,
				LOCALE_USER_DEFAULT,
				&rgDispId);

			if (SUCCEEDED(hr))
			{
				VARIANT varResult;
				VariantInit(&varResult);
				V_VT(&varResult) = VT_I4;

				DISPPARAMS disp = { NULL, NULL, 0, 0 };

				hr = pGraphicDispatch->Invoke(rgDispId,
					IID_NULL,
					LOCALE_USER_DEFAULT,
					DISPATCH_PROPERTYGET,
					&disp,
					&varResult,
					NULL,
					NULL);

				if (SUCCEEDED(hr))
				{
					nObjectType = V_I4(&varResult);
				}
			}
		}
		else
		{
			hr = E_INVALIDARG;
		}
	}
	catch (_com_error& e)
	{
		hr = e.Error();
	}

	return hr;
}
// In certain cases , the returned object is a Reference to the actual object that
// was located. This routine will return the type of located element behind the Reference object.
// If the type could not be determined, zero is returned (no element has a type value of zero).
// Returns E_INVALIDARG if the graphic dispatch is NULL or S_FALSE if the input graphic dispatch is
// not that of a Reference object. In the latter case, nObjectType will be the type of the input
// graphic object. Otherwise, returns the code from the Invoke method on the input 
// dispatch interface. In addition, if pObjectDispatch is not NULL, the dispatch of
// the object behind the Reference object is also returned. And yes, if returned, its up to the
// caller to Release the interface once it is no longer needed.

// Note: Some known cases where a Reference object is returned: 
//
//       Locates in the Assembly environment. 
//       Locates of objects in a drawing view when located from the Draft file
//       containing the drawing view.

HRESULT GetLocatedGraphicTypeFromReference(LPDISPATCH pGraphicDispatch,
	int& nObjectType,
	LPUNKNOWN* pObjectUnknown)
{
	HRESULT hr = NOERROR;

	try
	{
		hr = GetLocatedGraphicType(pGraphicDispatch, nObjectType);

		if (SUCCEEDED(hr))
		{
			// Assumed context of this call is from a locate performed in the Assembly environment.
			// Hence, the returned type should be enumerated in SolidEdgeFramework::ObjectType and
			// should be an igReference.

			if (nObjectType == igReference)
			{
				// Obtain the object behind the reference and get its type.

				ReferencePtr pRef = pGraphicDispatch;

				IDispatchPtr pObject = pRef->GetObject();

				if (NULL != pObject)
				{
					DISPID rgDispId = 0;

					OLECHAR* Names[1] = { L"Type" };

					// Assume nothing! Get the id to pass to Invoke in the most robust way rather than assume
					// the id will never change.

					hr = pObject->GetIDsOfNames(IID_NULL,
						Names,
						1,
						LOCALE_USER_DEFAULT,
						&rgDispId);

					if (SUCCEEDED(hr))
					{
						VARIANT varResult;
						VariantInit(&varResult);
						V_VT(&varResult) = VT_I4;

						DISPPARAMS disp = { NULL, NULL, 0, 0 };

						hr = pObject->Invoke(rgDispId,
							IID_NULL,
							LOCALE_USER_DEFAULT,
							DISPATCH_PROPERTYGET,
							&disp,
							&varResult,
							NULL,
							NULL);
						if (SUCCEEDED(hr))
						{
							nObjectType = V_I4(&varResult);
						}
					}

					if (NULL != pObjectUnknown)
					{
						// Return the dispatch pointer of the object.

						pObject->QueryInterface(IID_IUnknown, (LPVOID*)pObjectUnknown);
					}
				}
			}
			else
			{
				if (NULL != pObjectUnknown)
				{
					// Return the dispatch pointer of the object.

					pGraphicDispatch->QueryInterface(IID_IUnknown, (LPVOID*)pObjectUnknown);
				}

				hr = S_FALSE;
			}
		}
	}
	catch (_com_error& e)
	{
		hr = e.Error();
	}

	return hr;
}

HRESULT GetDocPtr(_bstr_t& strFilename, PartDocumentPtr& pPartDocument)
{
	HRESULT hr = S_OK;

	SolidEdgeDocumentPtr pDocument;

	DocumentTypeConstants nDocType = igUnknownDocument;

	hr = GetDocPtr(strFilename, pDocument, nDocType);

	pPartDocument = pDocument; // will be NULL if the pDocument is not a part document.

	return hr;
}

HRESULT GetDocPtr(_bstr_t& strFilename, DraftDocumentPtr& pDraftDocument)
{
	HRESULT hr = S_OK;

	SolidEdgeDocumentPtr pDocument;

	DocumentTypeConstants nDocType = igUnknownDocument;

	hr = GetDocPtr(strFilename, pDocument, nDocType);

	pDraftDocument = pDocument; // will be NULL if the pDocument is not a part document.

	return hr;
}

HRESULT GetDocPtr(_bstr_t& strFilename, SolidEdgeDocumentPtr& pSolidEdgeDocument, DocumentTypeConstants& nDocType)
{
	HRESULT hr = S_OK;

#if _MSC_VER < 1200   // 1200 or greater and compiler generates smart pointers everywhere
	IDispatch* pDocDispatch = NULL;
#else
	IDispatchPtr pDocDispatch;
#endif

	try
	{
		bool bGetName = false;

		// See if a name was passed in. If so, get the docs collection and attempt to get the
		// document using the name as the index.
		if (0 != strFilename.length())
		{
			DocumentsPtr pDocs = GetApplicationPtr()->GetDocuments();
			if (NULL != pDocs)
			{
				_variant_t vName(strFilename);

				pDocDispatch = pDocs->Item(vName);
			}
			else
			{
				hr = E_UNEXPECTED;
			}
		}
		else
		{
			pDocDispatch = GetApplicationPtr()->GetActiveDocument();
			bGetName = true;
		}

		pSolidEdgeDocument = pDocDispatch;

		if (pSolidEdgeDocument)
		{
			nDocType = pSolidEdgeDocument->Type;

			if (bGetName)
			{
				strFilename = pSolidEdgeDocument->GetFullName();
			}
		}
	}
	catch (_com_error& e)
	{
		// Probably not open in solid edge. No big deal.
		hr = e.Error();
	}

	return hr;
}

// Most common cause of failure for this routine: file is not open in the current session of Solid Edge. 
// S_FALSE if unknown document type (only handles assembly, part, sheetmetal and draft files).
// If strFilename is empty, this routine will grab the active document.

HRESULT GetDocPtr(_bstr_t& strFilename,
	AssemblyDocumentPtr& pAssemblyDoc,
	PartDocumentPtr& pPartDoc,
	SheetMetalDocumentPtr& pSheetMetalDoc,
	DraftDocumentPtr& pDraftDoc,
	WeldmentDocumentPtr& pWeldmentDoc,
	DocumentTypeConstants& nDocType)
{
	HRESULT hr = NOERROR;

	nDocType = igUnknownDocument;

	pAssemblyDoc = NULL;
	pPartDoc = NULL;
	pSheetMetalDoc = NULL;
	pDraftDoc = NULL;

	BOOL bGetName = FALSE;

	// Note: The Microsoft compiler's generate different signatures for the interfaces
	// generated from the typelibs depending on the compiler. Before VC6.0 (compiler ver 1200),
	// returned pure dispatch pointers were not smart pointers. That forces me to write conditionally
	// compiled code in order to avoid reference leaks that occur when assigning such an output
	// dispatch pointer to a smart pointer variable. The leak is due to the fact that the 
	// dispatch pointer is returned on the stack and is lost once the assignment to the smart
	// pointer is done.

#if _MSC_VER < 1200   // 1200 or greater and compiler generates smart pointers everywhere
	IDispatch* pDocDispatch = NULL;
#else
	IDispatchPtr pDocDispatch;
#endif

	try
	{
		// See if a name was passed in. If so, get the docs collection and attempt to get the
		// document using the name as the index.
		if (0 != strFilename.length())
		{
			DocumentsPtr pDocs = GetApplicationPtr()->GetDocuments();
			if (NULL != pDocs)
			{
				_variant_t vName(strFilename);

				pDocDispatch = pDocs->Item(vName);
			}
			else
			{
				hr = E_UNEXPECTED;
			}
		}
		else
		{
			pDocDispatch = GetApplicationPtr()->GetActiveDocument();
			bGetName = TRUE;
		}

		if (SUCCEEDED(hr) && NULL != pDocDispatch)
		{
			if (pDocDispatch)
			{
				VARIANT varResult;
				VariantInit(&varResult);
				V_VT(&varResult) = VT_I4;

				DISPPARAMS disp = { NULL, NULL, 0, 0 };

				hr = pDocDispatch->Invoke(0x46, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_PROPERTYGET, &disp, &varResult, NULL, NULL);
				if (SUCCEEDED(hr))
				{
					nDocType = (DocumentTypeConstants)(V_I4(&varResult));
				}
				else
				{
					_com_issue_errorex(hr, pDocDispatch, __uuidof(pDocDispatch));
				}
			}

			switch (nDocType)
			{
			case igAssemblyDocument:
			{
				pAssemblyDoc = pDocDispatch;
				if (NULL == pAssemblyDoc)
				{
					hr = E_UNEXPECTED;
				}
				else if (bGetName)
				{
					strFilename = pAssemblyDoc->GetFullName();
				}
				break;
			}
			case igPartDocument:
			{
				pPartDoc = pDocDispatch;
				if (NULL == pPartDoc)
				{
					hr = E_UNEXPECTED;
				}
				else if (bGetName)
				{
					strFilename = pPartDoc->GetFullName();
				}
				break;
			}
			case igSheetMetalDocument:
			{
				pSheetMetalDoc = pDocDispatch;
				if (NULL == pSheetMetalDoc)
				{
					hr = E_UNEXPECTED;
				}
				else if (bGetName)
				{
					strFilename = pSheetMetalDoc->GetFullName();
				}
				break;
			}
			case igDraftDocument:
			{
				pDraftDoc = pDocDispatch;
				if (NULL == pDraftDoc)
				{
					hr = E_UNEXPECTED;
				}
				else if (bGetName)
				{
					strFilename = pDraftDoc->GetFullName();
				}
				break;
			}
			case igWeldmentDocument:
			{
				pWeldmentDoc = pDocDispatch;
				if (NULL == pWeldmentDoc)
				{
					hr = E_UNEXPECTED;
				}
				else if (bGetName)
				{
					strFilename = pWeldmentDoc->GetFullName();
				}
				break;
			}
			case igUnknownDocument:
			{
				hr = S_FALSE;
				break;
			}
			default:
			{
				break;
			}
			}
		}
	}
	catch (_com_error& e)
	{
		// Probably not open in solid edge. No big deal.
		hr = e.Error();
	}

#if _MSC_VER < 1200
	C_RELEASE(pDocDispatch);
#endif

	return hr;
}

IUnknownPtr GetAddInStorage(LPDISPATCH pDocDispatch, BSTR Name, long grfMode)
{
	ASSERT(pDocDispatch);

	IUnknownPtr pUnkStorage;

	SolidEdgeDocumentPtr pSolidEdgeDocument = pDocDispatch;

	if (pSolidEdgeDocument)
	{
		IUnknownPtr pUnk = pSolidEdgeDocument->GetAddInsStorage(Name, grfMode);
	}

	return pUnkStorage;

	// This is what I did before the advent of SolidEdgeDocument API. I have since changed
	// the signature of the function from IUnknown* to IUnknownPtr.

	//IUnknown * _result;
	//_com_dispatch_method(pDocDispatch, 0x49, DISPATCH_PROPERTYGET, VT_UNKNOWN, (void*)&_result, 
	//	L"\x0008\x0003", Name, grfMode);
	//return _result;
}

HRESULT GetAddinStorage(LPDISPATCH pDocDispatch,
	long grfMode,
	_bstr_t StorageName,
	IStoragePtr& pStorage
)
{
	ASSERT(pDocDispatch);

	HRESULT hr = NOERROR;

	try
	{
		IUnknownPtr pUnkStorage = GetAddInStorage(pDocDispatch, (BSTR)StorageName, grfMode);

		if (pUnkStorage)
		{
			pStorage = pUnkStorage;
		}
	}
	catch (_com_error& e)
	{
		hr = e.Error();
	}

	return hr;
}



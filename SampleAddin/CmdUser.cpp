// samplloc.cpp : implementation file
//

#include "stdafx.h"
#include "CmdApp.h"
#include "resource.h"
#include "CmdUser.h"
#include "SEUtils.h"
#include "AddinMenu.h"
#include <iostream>
#include <fstream>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CmdUser::CmdUser() : CmdDevice()
{
	m_hWndRibbon = NULL;

	// Set up some default values for the two edit fields on the command's ribbon bar. These values will
	// be converted to database units using the UnitsOfMeasure interface when the ribbon bar is initialized.
	m_bstrLengthValue = "1.0";
	m_dLength = 1.0;

	m_bstrAngleValue = "180.0";
	m_dAngle = 180.0;

	m_is_draw = false;
}

CmdUser::~CmdUser()
{
}

HRESULT CmdUser::Activate()
{
	using namespace SolidEdgeConstants;

	HRESULT hr = S_OK;

	try
	{
		// Set up the mouse object. Getting the mouse setting correct can be a real
		// pain in the app. The type of environment can affect the mouse and of course
		// valid filters depend on what env the command is running in. For instance,
		// adding seLocateFace in a draft/drawing env doesn't make sense (but I can still
		// set the filter with no error). Meanwhile, while in say the Part env, using
		// seDynamicsRectangle doesn't work but does in draft.
		if (m_pSEMouse)
		{
			m_pSEMouse->put_ScaleMode(0);                // Design model coordinates.//-----------
			m_pSEMouse->put_WindowTypes(1);              // Graphic window's only please.
			m_pSEMouse->put_LocateMode(SolidEdgeConstants::seLocateModes::seLocateOff);
			// Enable the drag mode to get Edge to fire the MouseDrag event. This can
			// be useful for fence locate in draft or 2d (classic) part and asm environments.
			// See the code in MouseDrag to see how it can work.
			//m_pSEMouse->put_EnabledDrag( VARIANT_TRUE );//-----------
			m_pSEMouse->put_EnabledMove(VARIANT_TRUE);   // Request move events.
			_bstr_t strFilename; // empty so I get the active doc.

			DraftDocumentPtr pDraftDoc;
			hr = GetDocPtr(strFilename, pDraftDoc);

			if (NULL == pDraftDoc)
			{
				//m_pSEMouse->put_LocateMode(seLocateQuickPick);//-----------
			}
			else
			{
				// Note: Using seSmartLocate in part won't let me locate faces and edges. I said
				// this mouse was a pain in the App. I'm sure all this is documented in the
				// programmers reference guide. I'm just coding up what I see works.

				// Note: In order to obtain located objects during move events, set your locate
				//       filter to seSmartLocate.
				//m_pSEMouse->put_LocateMode(seLocateDrawingView);//-----------
			}
			//m_pSEMouse->put_LocateMode(seSmartLocate);//-----------
			//m_pSEMouse->put_LocateMode( seLocateSimple );//-----------
			//m_pSEMouse->AddToLocateFilter(seLocatePart);//-----------
			//m_pSEMouse->AddToLocateFilter(seLocateOccurrencesAndPartObjects);//-----------

			// Set up a few commonly located types in non-draft files.
			//m_pSEMouse->AddToLocateFilter( seLocateFace );	//Need to use both interaces combined//-----------
			//m_pSEMouse->AddToLocateFilter(seLocateMeshFacet); //Need to use both interaces combined//-----------

			//m_pSEMouse->AddToLocateFilter( seLocateEdge );//-----------
			//m_pSEMouse->AddToLocateFilter( seLocateVertex );//-----------

			// Some other filter settings that can be used.
			//m_pSEMouse->AddToLocateFilter( seLocateRefPlane );//-----------
			//m_pSEMouse->AddToLocateFilter( seLocatePart );//-----------
			//m_pSEMouse->AddToLocateFilter( seLocateCone );//-----------
			//m_pSEMouse->AddToLocateFilter( seLocateSphere );//-----------
			//m_pSEMouse->AddToLocateFilter( seLocateTorus );//-----------

			// Set up a few commonly located types in draft or part/assembly sketching environments.
			//m_pSEMouse->AddToLocateFilter( seLocateLine2d );//-----------
			//m_pSEMouse->AddToLocateFilter( seLocateCircle2d );//-----------
			//m_pSEMouse->AddToLocateFilter(seLocateCoordinateSystem);//-----------

			// Some other filter settings that can be used.
			//m_pSEMouse->AddToLocateFilter( seLocateLWRLine2d );//-----------

			// I believe this mode is for draft files. Edge will draw a rectangle for you.
			//m_pSEMouse->PutDynamicsMode( seDynamicsRectangle );//-----------

			// If you need to do inter document locates, Edge started supporting that in version ...
			// some version that I fail to recall.
			//m_pSEMouse->PutInterDocumentLocate(VARIANT_TRUE);//-----------

			// New for ST8. The mouse supports a locate "front to back", which is an ordering of located elements along the bore line.
			// For compatibility, a new interface was added. So if I can get the interface, I set the front to back locate mode.
			ISEMouseEx2Ptr pM2 = m_pSEMouse;

			VARIANT_BOOL vbFrontToBack = VARIANT_TRUE;

			if (pM2)
			{
				//pM2->PutLocateFrontToBack( vbFrontToBack );//-----------
			}

			ISEMouseEx3Ptr pM3 = m_pSEMouse;
			VARIANT_BOOL vbPathFinderLocate = VARIANT_TRUE;
			if (pM3)
			{
				//pM3->put_PathfinderLocate(vbPathFinderLocate);//-----------
			}
		}

		// ÉèÖÃopenglÏÔÊ¾²ÝÍ¼
		mp_gl_display = CSampleAddinApp::GetSEApp()->GetSEAddin()->GetMyViewOverlayObj();

		ApplicationPtr application = CSampleAddinApp::GetSEApp()->GetApplication();
		PartDocumentPtr partDocument = application->GetActiveDocument();
		WindowsPtr windows = partDocument->GetWindows();
		WindowPtr mainWnd = NULL;
		if (windows->Count >= 1)
		{
			mainWnd = windows->Item(1);
		}
		if (mp_gl_display)
		{
			mp_gl_display->SetView(mainWnd);
			mp_gl_display->ShowSkh();
		}


	}
	catch (_com_error& e)
	{
		hr = e.Error();
	}

	return hr;
}

// With older versions of Visual Studio, Microsoft manged to create every smart pointer based on
// the IDispatch interface instead of the acutual dispatch interface Edge defined that derived
// from IDispatch. That is, in the MS definition of the smart pointers, each one had 
// "__uuidof(IDispatch)", essentially making the smart pointers useless when it comes to
// identification (and most everything else). We quickly pointed that out to Microsoft and they actually 
// responded by making the suggested change, which you see below. I don't recall which version of Visual 
// Studio the bug was fixed in (6 perhaps). So I'm leaving these here but if you are using Visual Studio 
// 2005 or 2008 you will want to just rely on the compiler to import the typelib and do the right thing.
// Since I am using Visual Studio 2008, I no longer declare, for instance "::Circle2dPtr" and instead
// just use Circle2dPtr. All one has to do to see if they have the "fix" is to open any .tlh file
// and search for "_COM_SMARTPTR_TYPEDEF" and see if the uuidof has IDispatch or not.

//_COM_SMARTPTR_TYPEDEF(Line2d, __uuidof(Line2d));
//_COM_SMARTPTR_TYPEDEF(Circle2d, __uuidof(Circle2d));
//_COM_SMARTPTR_TYPEDEF(Occurrence, __uuidof(Occurrence));

// I am getting the reference key for some of the objects I locate. So I will just store the data
// here. Normally one would store the key (perhaps in add-in storage) and use it to try and bind
// back to the object at a later time. If the binding fails, the topology no longer exists.
SAFEARRAY* pCurRefKey = 0;

HRESULT CmdUser::MouseClick(short sButton, short sShift, double dX, double dY, double dZ, LPDISPATCH pWindowDispatch, long lKeyPointType, LPDISPATCH pGraphicDispatch)
{
	HRESULT hr = NOERROR;

	if (sButton == MK_LBUTTON)
	{
		//MessageBox(NULL, TEXT("Êó±ê×ó¼üµã»÷-- !"), TEXT("´íÎó£¡"), MB_OK | MB_YESNO);
		//m_is_draw = true;
	}
	else if (sButton == MK_RBUTTON)
	{
		//MessageBox(NULL, TEXT("Êó±êÓÒ¼üµã»÷-- !"), TEXT("´íÎó£¡"), MB_OK | MB_YESNO);
	}

	return hr;

}

// Create a part with say a cylinder in it and then run one of the two locate commands. Click on the face
// and then continue to move the mouse over the part, notice the data being updated in the status bar. 
// That's about all this add-in's MouseMove does.
HRESULT CmdUser::MouseMove(short sButton, short sShift, double dX, double dY, double dZ, LPDISPATCH pWindowDispatch, long lKeyPointType, LPDISPATCH pGraphicDispatch)
{
	HRESULT hr = NOERROR;
	if (m_is_draw)
	{
		mp_gl_display->Append(dX, dY);
		WindowPtr(pWindowDispatch)->View->Update();
	}

	return hr;
}

HRESULT CmdUser::MouseDown(short sButton, short sShift, double dX, double dY, double dZ, LPDISPATCH pWindowDispatch, long lKeyPointType, LPDISPATCH pGraphicDispatch)
{
	//[zheng.wentong][2024.7.16]
	HRESULT hr = NOERROR;
	//gDataShare->SetDrawingSrart(dX, dY, dZ);

	if (sButton == MK_LBUTTON)
	{
		m_is_draw = true;

	}
	else if (sButton == MK_RBUTTON)
	{
		mp_gl_display->ExportSketchPoints("sketchPnts.txt");
	}
	else
	{

	}

	return hr;

}

HRESULT CmdUser::MouseUp(short sButton, short sShift, double dX, double dY, double dZ, LPDISPATCH pWindowDispatch, long lKeyPointType, LPDISPATCH pGraphicDispatch)
{
	HRESULT hr = NOERROR;

	if (sButton == MK_LBUTTON)
	{
		//MessageBox(NULL, TEXT("Êó±ê×ó¼üµã»÷-- !"), TEXT("´íÎó£¡"), MB_OK | MB_YESNO);
		m_is_draw = false;
		mp_gl_display->DrawEnd();
	}
	else if (sButton == MK_RBUTTON)
	{
		//MessageBox(NULL, TEXT("Êó±êÓÒ¼üµã»÷-- !"), TEXT("´íÎó£¡"), MB_OK | MB_YESNO);
	}

	return hr;

}

HRESULT CmdUser::MouseDrag(short sButton, short sShift, double dX, double dY, double dZ, LPDISPATCH pWindowDispatch, short DragState, long lKeyPointType, LPDISPATCH pGraphicDispatch)
{
	using namespace SolidEdgeConstants;

	HRESULT hr = NOERROR;

	if (DragState == seMouseExitDrag && NULL != pGraphicDispatch)
	{
		short sLastDownEventShift = m_pSEMouse->LastDownEventShift;
		short sLastDownEventButton = m_pSEMouse->LastDownEventButton;

		if (seLEFT == sLastDownEventButton)
		{
			::SelectSetPtr pSelectSet;

			if (NULL != pGraphicDispatch)
			{
				pSelectSet = pGraphicDispatch;
			}

			if (NULL != pSelectSet)
			{
				long lCount = pSelectSet->Count;

				IDispatchPtr pElementDispatch;

				for (long Index = 1; Index <= lCount; ++Index)
				{
					pElementDispatch = pSelectSet->Item(Index);

					Line2dPtr pLine = pElementDispatch;

					Circle2dPtr pCircle = pElementDispatch;

					if (NULL != pLine)
					{
						TRACE("Input select set contains a line\n");
					}
					else if (NULL != pCircle)
					{
						TRACE("Input select set contains a circle\n");
					}
				}
			}
			else
			{
				Line2dPtr pLine = pGraphicDispatch;

				Circle2dPtr pCircle = pGraphicDispatch;

				if (NULL != pLine)
				{
					TRACE("Input select set contains a line\n");
				}
				else if (NULL != pCircle)
				{
					TRACE("Input select set contains a circle\n");
				}
			}
		}
	}

	return hr;

}

static int s_Valid = 1; // Delcare in function and VC++ debugger can't be used to change value when break pt is hit!
HRESULT CmdUser::Filter(LPDISPATCH pGraphicDispatch, VARIANT_BOOL* vbValid)
{
	// Demonstrate how an add-in can influence what the user locates. 
	HRESULT hr = NOERROR;

	if (s_Valid)
	{
		*vbValid = VARIANT_TRUE;
	}
	else
	{
		*vbValid = VARIANT_FALSE;
	}

	return hr;
}

// The KeyDown event is handles simply to show some of the ways the ribbon bar can be
// manipulated.

// The keys and their uses are as follows:
//
//  'r' - Remove the command bar entries (i.e., delete it)
//  'a' - Add the command bar entries
//  'h' - Hide the command bar group two entry
//  's' - Show the command bar group two entry
//  'g' - Set focus on the command bar group one entry
//  'v' - Ensure that the command bar group two entry is visible.
//  return/enter - Move focus among the edit fields defined in the ISolidEdgeRibbonBar::AddEditField call.

HRESULT CmdUser::KeyDown(short* KeyCode, short Shift)
{
	TRACE("Key down event\n");

	if (0xd == *KeyCode)
	{

	}
	else if (0x53 == *KeyCode) // The 's' key
	{
		MessageBox(NULL, TEXT("°´¼ü s !"), TEXT("´íÎó£¡"), MB_OK | MB_YESNO);
	}
	else if (0x48 == *KeyCode) // The 'h' key
	{
		MessageBox(NULL, TEXT("°´¼ü h !"), TEXT("´íÎó£¡"), MB_OK | MB_YESNO);
	}
	else if (0x52 == *KeyCode) // The 'r' key
	{
		MessageBox(NULL, TEXT("°´¼ü r !"), TEXT("´íÎó£¡"), MB_OK | MB_YESNO);
	}
	else if (0x41 == *KeyCode) // The 'a' key
	{
		MessageBox(NULL, TEXT("°´¼ü a !"), TEXT("´íÎó£¡"), MB_OK | MB_YESNO);
	}
	else if (0x47 == *KeyCode) // The 'g' key
	{
		MessageBox(NULL, TEXT("°´¼ü g !"), TEXT("´íÎó£¡"), MB_OK | MB_YESNO);
	}

	return S_OK;

}


HRESULT CmdUser::WindowProc(LPDISPATCH pUnkDoc, LPDISPATCH pUnkView, UINT nMsg, WPARAM wParam, LPARAM lParam, LRESULT* lResult)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	HRESULT hr = S_OK;
	return hr;
}

HRESULT CmdUser::Terminate()
{
	HRESULT hr = NOERROR;
	return hr;
}

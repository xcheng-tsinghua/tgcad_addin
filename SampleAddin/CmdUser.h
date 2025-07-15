#pragma once

#include "CmdApp.h"
#include "resource.h"
#include "CmdDevice.h"
#include "DrawGL.h"

// This is a trivial example of a command that connects up to the mouse
// to highlight certain graphics. This object inherits all the member
// functions of CmdDevice, choosing to only override Activate where it
// sets up the locate filter. The command also exercies the ISolidEdgeRibbonBar
// API.
class CmdUser : public CmdDevice
{
public:
	CmdUser();
	virtual ~CmdUser();

	ISEMouseExPtr GetMouse() { return m_pSEMouse; }

	// Override the virtual methods that this command cares about.

	STDMETHODIMP Activate();

	STDMETHODIMP MouseClick(short sButton,
		short sShift,
		double dX,
		double dY,
		double dZ,
		LPDISPATCH pWindowDispatch,
		long lKeyPointType,
		LPDISPATCH pGraphicDispatch);

	STDMETHODIMP MouseMove(short sButton,
		short sShift,
		double dX,
		double dY,
		double dZ,
		LPDISPATCH pWindowDispatch,
		long lKeyPointType,
		LPDISPATCH pGraphicDispatch);

	STDMETHOD(MouseDown)    (short sButton,
		short sShift,
		double dX,
		double dY,
		double dZ,
		LPDISPATCH pWindowDispatch,
		long lKeyPointType,
		LPDISPATCH pGraphicDispatch);

	STDMETHOD(MouseUp)      (short sButton,
		short sShift,
		double dX,
		double dY,
		double dZ,
		LPDISPATCH pWindowDispatch,
		long lKeyPointType,
		LPDISPATCH pGraphicDispatch);

	STDMETHOD(MouseDrag)    (short sButton,
		short sShift,
		double dX,
		double dY,
		double dZ,
		LPDISPATCH pWindowDispatch,
		short DragState,
		long lKeyPointType,
		LPDISPATCH pGraphicDispatch);

	STDMETHODIMP Filter(LPDISPATCH pGraphicDispatch,
		VARIANT_BOOL* vbValid);

	STDMETHOD(KeyDown)		(short* KeyCode,
		short Shift);

	STDMETHOD(Terminate)(void);

	// Override the WindowProc message so I can handle messages
	// emanating from the dialogs added to the command bar as well
	// as any messages sent to the command from Edge.
	STDMETHOD(WindowProc)   (LPDISPATCH pDocDispatch,
		LPDISPATCH pViewDispatch,
		UINT nMsg,
		WPARAM wParam,
		LPARAM lParam,
		LRESULT* lResult);

	/*CDemoPageHandler* GetPageHandler();*/

	// Store the handle to the command ribbon dialog.
	HWND m_hWndRibbon;

	// Command ribbon has a few controls, two of which show how to use the
	// UnitOfMeasure (UOM) API.

	// Declare the strings the user will see in the dialog. The unit of measure
	// object is used to format the string the user will see and to parse the
	// strings and return the corresponding data base units.
	_bstr_t m_bstrAngleValue;
	_bstr_t m_bstrLengthValue;

	// Declare the corresponding values. These values will always be in Solid
	// Edge data base units (expect these to differ from the above UI displayable
	// values that depend on the user's settings. For instance, the Solid Edge
	// data base unit for lengths is the meter but a user will typically work
	// in mili-meters. These are values that can be passed to various Edge APIs
	// that create or manipulate some entity (e.g., length of a line, angle of
	// an arc ...)
	double m_dAngle;
	double m_dLength;

private:
	CMyViewOverlayObj* mp_gl_display;
	bool m_is_draw;

};

// Use the following typedef when creating an instance of CmdDevice.
typedef CComObject<CmdUser> CDemoCommandObj;



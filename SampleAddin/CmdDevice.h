// Commands.h : header file
//
#pragma once


#include "stdafx.h"
#include "SampleAddin_i.h"
#include "CmdApp.h"


// Define a base class for an individual command. This class provides all the essential
// ingredients for writing a fully functional Solid Edge command. It pvovides default
// handling of all the various events which can be fired by the Solid Edge command control 
// (obtained by calling CreateCommand). When an event occurs, a virtual function on this base 
// class is called by embedded event handler classes. ALL ONE HAS TO DO IS SUBCLASS FROM 
// THIS OBJECT AND OVERRIDE ANY SPECIFIC EVENT THE SUBCLASS IS INTERESTED IN RECEIVING. 
// Users of the command and mouse controls should feel quite familiar with this object. 
// That's because the object returned from CreateCommand is essentially the command control 
// and its mouse property is the mouse control.

// Note: I choose to class an individual command from IUnknown, which I then add to
//       the COM_MAP because I am using the "GetUnknown" function that the BEGIN_COM_MAP
//       macro declares for the CmdDevice object. The reason for doing this is that once
//       the CmdApp object creates a CCommandObj (the templatized COM version of CmdDevice),
//       it addrefs the CCommandObj and then abandons the CmdDevice. From that point on, the
//       lifetime of CmdDevice is controlled by Solid Edge. Eventually, the XCommandEvents
//       object embedded in CmdApp (you can see that object below) receives a Terminate
//       call from Solid Edge. At that point, the command should perform all clean up code
//       and stop execution. Once the CmdDevice object disconnects from the Solid Edge
//       event sinks, there should be no reference to Solid Edge held by the command, and
//       there is, and never really was, any other external reference to the CmdDevice object.
//       Hence, the CmdDevice object must dereference itself so that its destructor will be called
//       and hence its allocated memory will be freed.
//
//       But theres a slight problem with that. There is a difference between a CCommandObj
//       object, and a CmdDevice object. The former is created by the use of the CComObject
//       ATL template, with CmdDevice as the template argument. That implicitly creates a
//       C++ COM object whose base class is CmdDevice object. Hence, member functions declared
//       on CmdDevice object cannot directly call IUnknown functions. They exist somewhere "up"
//       the class graph.
//
//       Apparently, ATL solves this "dilemma" by defining a GetUnknown function in BEGIN_COM_MAP.
//       Sounds simple but there is a caveat. The object have at least one "_ATL_SIMPLEMAP_ENTRY" in
//       the COM map for GetUnknown to work ( asserts without one ). Hence, the reason for adding
//       IUnknown to the object and the map.
//
//       I could not find any documentation on GetUnknown, so I wrote a GetMyUnknown which calls
//       GetUnknown (see code below) to provide some degree of protection in case that ever changes.
//       
//       Alternatively, one could use the fact that any event set that the object connects to results
//       in a reference on that object. Simply allow the disconnection process to delete the object.
//       I chose not to to strengthen my control over the lifetime of the object.


// The ever changing Solid Edge addin command object! An object derived from CmdDevice will usually
// be  created by the CmdApp object whenever its OnCommand function is called. The CmdApp object 
// AddRefs it and then calls SetCommandObject. CmdDevice must do the final Release in order to keep
// from leaking (remember that after creation by CmdApp, a CmdDevice is on its own).
// Use it to connect up any sinks a particular command needs in order to function successfully 
// such as the command or mouse events and to obtain the Command and Mouse interfaces if they 
// are needed.

class CmdDevice :
	IUnknown, 
	public CComObjectRoot,
	public CComCoClass<CmdDevice, &CLSID_ASMLOCCommand>
{
protected:
  // Do not delete m_pCommands! It exists as a mere convenience for accessing the CmdApp
  // object created when the addin is connected to Solid Edge. Use the access function
  // provided below (see GetCommandsObject()).
	CmdApp* m_pCommands;

	ISECommandPtr  m_pSECommand;
	ISEMouseExPtr    m_pSEMouse;

public:
	CmdDevice();
	virtual ~CmdDevice();

	LPUNKNOWN GetMyUnknown() { return GetUnknown(); }

	// Helper members to set and get the CmdApp object.
	void SetCommandsObject( CmdApp* pCommands ) { ASSERT( pCommands ); m_pCommands = pCommands; }
	CmdApp* GetCommandsObject()                 { ASSERT( m_pCommands ); return m_pCommands; }

	// CreateCommand helper member which calls the Solid Edge application object to obtain a command 
	// control from Solid Edge. If CommandType is seNoDeactivate, the mouse control is also obtained 
	// and both the mouse and window event sinks are connected. Use seNoDeactivate if your command
	// plans on processing user input obtained from Solid Edge. If seNoDeactivate is not specified,
	// the Solid Edge command control will Terminate the command immediately after invocation. That
	// is normally what one would want if for instance, the addin command simply takes some action
	// that needs no user input, or obtains all user input via a modal dialog box.
	virtual HRESULT CreateCommand( SolidEdgeConstants::seCmdFlag CommandType );

	// GetCommand returns the Solid Edge command control.
	ISECommandPtr GetCommand() { return m_pSECommand; }

	// UnadviseFromCommandEvents unadvises from the command event sets (command, mouse, window ...).
	virtual void UnadviseFromCommandEvents();

	// ReleaseInterfaces releases the command, mouse and window interfaces
	virtual void ReleaseInterfaces();

	BEGIN_COM_MAP(CmdDevice)
		COM_INTERFACE_ENTRY(IUnknown)
	END_COM_MAP()
	DECLARE_NOT_AGGREGATABLE(CmdDevice)

protected:

	//  Here is a base class, XEventHandler, for the various sinks that can be obtained from the
	//  command control Solid Edge returns from CreateCommand to subclass from. It handles connection
	//  and disconnection.

	template <class IEvents, const IID* piidEvents, const GUID* plibid,
		class XEvents, const CLSID* pClsidEvents>
	class XEventHandler :
		public CComDualImpl<IEvents, piidEvents, plibid>,
		public CComObjectRoot,
		public CComCoClass<XEvents, pClsidEvents>
	{
	public:
		BEGIN_COM_MAP(XEvents)
			COM_INTERFACE_ENTRY_IID(*piidEvents, IEvents)
		END_COM_MAP()
		DECLARE_NOT_AGGREGATABLE(XEvents)
		HRESULT Connect(IUnknown* pUnk)
		{ HRESULT hr; VERIFY(SUCCEEDED( hr = AtlAdvise(pUnk, this, *piidEvents, &m_dwAdvise))); return hr; }
		HRESULT Disconnect(IUnknown* pUnk)
		{ HRESULT hr; VERIFY(SUCCEEDED( hr = AtlUnadvise(pUnk, *piidEvents, m_dwAdvise))); return hr; }

		// Store a back pointer to the embedding command object (makes life easy).
		// Do not delete! Its merely a back pointer to the object that controls the
		// lifetime of your event handlers. Use this pointer in the event handling methods 
		// based on this object template to communicate with the embedding object (CCOmmand).
		CmdDevice* m_pCommand;
	protected:
		// cookie time
		DWORD m_dwAdvise;
	};

	// Now derive a class from XEventHandler to handle the ISECommandEvents interface.
	class XCommandEvents : public XEventHandler<ISECommandEvents, 
		&__uuidof(ISECommandEvents), &LIBID_SampleAddinLib,
		XCommandEvents, &CLSID_CommandEvents>
	{
	public:
		// ISECommandEvents methods
		STDMETHOD(raw_Activate)   ( void );

		STDMETHOD(raw_Deactivate) ( void );

		STDMETHOD(raw_Terminate)  ( void );

		STDMETHOD(raw_Idle)       ( long lCount, 
									VARIANT_BOOL* pbMore );

		STDMETHOD(raw_KeyDown)    ( short* KeyCode,
									short Shift );

		STDMETHOD(raw_KeyPress)   ( short* KeyAscii );

		STDMETHOD(raw_KeyUp)      ( short* KeyCode,
									short Shift );
	};
	typedef CComObject<XCommandEvents> XCommandEventsObj;
	XCommandEventsObj* m_pCommandEventsObj;

	// Now derive a class from XEventHandler to handle the ISEMouseEvents interface.
	class XMouseEvents : public XEventHandler<ISEMouseEvents, 
		&__uuidof(ISEMouseEvents), &LIBID_SampleAddinLib,
		XMouseEvents, &CLSID_MouseEvents>
	{
	public:
		// ISEMouseEvents methods
		STDMETHOD(raw_MouseDown)		( short sButton,
										  short sShift,
										  double dX,
										  double dY,
										  double dZ,
										  LPDISPATCH pWindowDispatch,
										  long lKeyPointType,
										  LPDISPATCH pGraphicDispatch );

		STDMETHOD(raw_MouseUp)			( short sButton,
										  short sShift,
										  double dX,
										  double dY,
										  double dZ,
										  LPDISPATCH pWindowDispatch,
										  long lKeyPointType,
										  LPDISPATCH pGraphicDispatch );

		STDMETHOD(raw_MouseMove)		( short sButton,
										  short sShift,
										  double dX,
										  double dY,
										  double dZ,
										  LPDISPATCH pWindowDispatch,
										  long lKeyPointType,
										  LPDISPATCH pGraphicDispatch );

		STDMETHOD(raw_MouseClick)		( short sButton,
										  short sShift,
										  double dX,
										  double dY,
										  double dZ,
										  LPDISPATCH pWindowDispatch,
										  long lKeyPointType,
										  LPDISPATCH pGraphicDispatch );

		STDMETHOD(raw_MouseDblClick)	( short sButton,
										  short sShift,
										  double dX,
										  double dY,
										  double dZ,
										  LPDISPATCH pWindowDispatch,
										  long lKeyPointType,
										  LPDISPATCH pGraphicDispatch );

		STDMETHOD(raw_MouseDrag)		( short sButton,
										  short sShift,
										  double dX,
										  double dY,
										  double dZ,
										  LPDISPATCH pWindowDispatch,
										  short DragState,
										  long lKeyPointType,
										  LPDISPATCH pGraphicDispatch );
	};
	typedef CComObject<XMouseEvents> XMouseEventsObj;
	XMouseEventsObj* m_pMouseEventsObj;

	// Now derive a class from XEventHandler to handle the ISELocateFilterEvents interface.
	class XLocateFilterEvents : public XEventHandler<ISELocateFilterEvents, 
		&__uuidof(ISELocateFilterEvents), &LIBID_SampleAddinLib,
		XLocateFilterEvents, &CLSID_LocateFilterEvents>
	{
	public:
		// ISEMouseEvents methods
		STDMETHOD(raw_Filter)         ( LPDISPATCH pGraphicDispatch,
										VARIANT_BOOL *vbValid );

	};
	typedef CComObject<XLocateFilterEvents> XLocateFilterEventsObj;
	XLocateFilterEventsObj* m_pLocateFilterEventsObj;

	// Now derive a class from XEventHandler to handle the ISECommandWindowEvents interface.
	class XWindowEvents : public XEventHandler<ISECommandWindowEvents, 
		&__uuidof(ISECommandWindowEvents), &LIBID_SampleAddinLib,
		XWindowEvents, &CLSID_CommandWindowEvents>
	{
	public:
	// ISEWindowEvents methods
    STDMETHOD(raw_WindowProc) ( LPDISPATCH pUnkDoc,
                                LPDISPATCH pUnkView,
                                UINT nMsg,
                                WPARAM wParam,
                                LPARAM lParam,
                                LRESULT *lResult );
	};
	typedef CComObject<XWindowEvents> XWindowEventsObj;
	XWindowEventsObj* m_pWindowEventsObj;

	// Wow. That's a lot of obtuse template definitions. Now to make life easier for writing an
	// individual command, this base class will define analogous virutal methods that the
	// templatized objects will use (via that m_pCommand back pointer).

	// Member functions below are called by the embedded event handlers whenever Solid Edge fires
	// an event to one of the handlers. Override the functions below that your CmdDevice derived
	// class cares to respond to. Default implementations provided for free.

	// The command control events.
	STDMETHOD(Activate)  ( void )                   {return S_OK;}

	STDMETHOD(Deactivate)( void )                   {return S_OK;}

	STDMETHOD(Terminate) ( void )                   {return S_OK;}

	STDMETHOD(Idle)      ( long lCount,
						   VARIANT_BOOL* pbMore )  { *pbMore = VARIANT_FALSE; return S_OK;};

	STDMETHOD(KeyDown)   ( short* KeyCode,
						   short Shift )            {return S_OK;} 

	STDMETHOD(KeyPress)  ( short* KeyAscii )        {return S_OK;}

	STDMETHOD(KeyUp)     ( short* KeyCode,
						   short Shift )            {return S_OK;}


	// The mouse control events
	STDMETHOD(MouseDown)    ( short sButton,
							  short sShift,
							  double dX,
							  double dY,
							  double dZ,
							  LPDISPATCH pWindowDispatch,
							  long lKeyPointType,
							  LPDISPATCH pGraphicDispatch ) {return S_OK;}

	STDMETHOD(MouseUp)      ( short sButton,
							  short sShift,
							  double dX,
							  double dY,
							  double dZ,
							  LPDISPATCH pWindowDispatch,
							  long lKeyPointType,
							  LPDISPATCH pGraphicDispatch ) {return S_OK;}

	STDMETHOD(MouseMove)    ( short sButton,
							  short sShift,
							  double dX,
							  double dY,
							  double dZ,
							  LPDISPATCH pWindowDispatch,
							  long lKeyPointType,
							  LPDISPATCH pGraphicDispatch ) {return S_OK;}

	STDMETHOD(MouseClick)   ( short sButton,
							  short sShift,
							  double dX,
							  double dY,
							  double dZ,
							  LPDISPATCH pWindowDispatch,
							  long lKeyPointType,
							  LPDISPATCH pGraphicDispatch ) {return S_OK;}

	STDMETHOD(MouseDblClick)( short sButton,
							  short sShift,
							  double dX,
							  double dY,
							  double dZ,
							  LPDISPATCH pWindowDispatch,
							  long lKeyPointType,
							  LPDISPATCH pGraphicDispatch ) {return S_OK;}

	STDMETHOD(MouseDrag)    ( short sButton,
							  short sShift,
							  double dX,
							  double dY,
							  double dZ,
							  LPDISPATCH pWindowDispatch,
							  short DragState,
							  long lKeyPointType,
							  LPDISPATCH pGraphicDispatch ) {return S_OK;}

	STDMETHOD(WindowProc)   ( LPDISPATCH pDocDispatch,
							  LPDISPATCH pViewDispatch,
							  UINT nMsg,
							  WPARAM wParam,
							  LPARAM lParam,
							  LRESULT *lResult )            {return S_OK;}

	STDMETHOD(Filter)       ( LPDISPATCH pGraphicDispatch,
							  VARIANT_BOOL *vbValid )      {return S_OK;}

	friend class XCommandEvents;
	friend class XMouseEvents;
	friend class XWindowEvents;
	friend class XLocateFilterEvents;

  public:
};

// Use the following typedef when creating an instance of CmdDevice.
typedef CComObject<CmdDevice> CCommandObj;
typedef CComAggObject<CmdDevice> CAggCommandObj;


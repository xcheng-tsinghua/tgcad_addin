// SEAddin.h : Declaration of the CSEAddin

#ifndef __SEADDIN_H_
#define __SEADDIN_H_

#include "resource.h"       // main symbols
#include "CmdApp.h"
#include "DrawGL.h"

using namespace SolidEdgeFramework;

#define COM_TYPELIBINTERFACE_ENTRY(x)\
	{&__uuidof(x), \
	offsetofclass(x, _ComMapClass), \
	_ATL_SIMPLEMAPENTRY},

// Define the GUID of this sample add-in. If you are copying this sample and
// creating your own real add-in, you REALLY NEED TO CHANGE THE GUID. Doing
// so will avoid any issue with clashing GUIDS on the same machine. Use
// the guidgen tool in the Visual Studio Tools menu to generate a guid. Simply
// changing some value below by hand will not guarantee uniqueness!!
DEFINE_GUID(CLSID_SEAddIn, 
0xA3DC5267,0xDD8B,0x4A7C, 0xB4,0xF4,0xE8,0x5E,0x39,0x05,0x66,0x80);

// Define the class that starts it all. This is the object that implements the
// ISolidEdgeAddIn interface. Edge will detect this COM class using the Microsoft
// registry APIs ( in particular EnumClassesOfCategories) because the add-in will
// register itself as implementing ISolidEdgeAddIn. 
/////////////////////////////////////////////////////////////////////////////
// SEAddin
class SEAddin :
	public ISolidEdgeAddIn,
	public CComObjectRoot,
	public ISupportErrorInfo,
	public CComCoClass<SEAddin, &CLSID_SEAddIn>
{
public:
	SEAddin()
	{
		m_pCommands = NULL;
		m_pMyViewOverlay = NULL;
	}

	~SEAddin()
	{
		m_pCommands = NULL;
		m_pMyViewOverlay = NULL;
	}

	DECLARE_REGISTRY_RESOURCEID(IDR_NEX3DMODELINGTOOL)

	BEGIN_COM_MAP(SEAddin)
		COM_TYPELIBINTERFACE_ENTRY(ISolidEdgeAddIn)
		COM_INTERFACE_ENTRY(ISupportErrorInfo)
	END_COM_MAP()

	BEGIN_CONNECTION_POINT_MAP(SEAddin)
	END_CONNECTION_POINT_MAP()


	// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

	// ISolidEdgeAddin
	STDMETHOD(raw_OnConnection)( THIS_ IDispatch* pAppDispatch, SeConnectMode ConnectMode, SolidEdgeFramework::AddIn* pUnkAddIn );
	STDMETHOD(raw_OnConnectToEnvironment)( BSTR EnvironmentCatid, LPDISPATCH pEnvironment, VARIANT_BOOL bFirstTime );
	STDMETHOD(raw_OnDisconnection)( THIS_ SeDisconnectMode DisconnectMode );

public:
	ApplicationPtr GetApplication();
	CCommandsObj* GetCommands() { return m_pCommands; }
	CMyViewOverlayObj* GetMyViewOverlayObj();

private:
	HRESULT CreateRibbon(GUID environmentGuid, EnvironmentPtr pEnvironment, VARIANT_BOOL bFirstTime);



private:
	ApplicationPtr m_pApplication;

protected:
	CCommandsObj* m_pCommands;
	CMyViewOverlayObj* m_pMyViewOverlay;

};

#endif //__SEADDIN_H_

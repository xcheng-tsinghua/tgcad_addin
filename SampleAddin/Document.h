
#pragma once

#include "stdafx.h"
#include "SampleAddin_i.h"
#include "CmdApp.h"
// This add-in will manipulate the edge bar. I have a dialog I will
// add to the edge bar. The document takes care of the details.

// Use this class to handle document events.
class ADDINDocument :
	public CComObjectRoot,
	public CComCoClass<ADDINDocument, &CLSID_ADDINDocument>
{
protected:

	// I store a pointer to the solid edge document.
	IDispatchPtr  m_pDocument;

	// I store a back pointer to the CmdApp object. Don't Release()!!
	CmdApp* m_pCommands;

	// I store a CEdgeBar object that helps with the creation of the
	// edge bar in Edge.


public:
	ADDINDocument();
	~ADDINDocument();


	HRESULT SetDocumentObject(LPDISPATCH pDocumentDispatch,
		BOOL bWithEvents = TRUE);
	IDispatchPtr GetDocument(void) { return m_pDocument; }

	HRESULT UnadviseFromEvents(void);
	HRESULT AdviseEvents(void);

	void SetCommandsObject(CmdApp* pCommands) { ASSERT(pCommands); m_pCommands = pCommands; }
	CmdApp* GetCommandsObject() { ASSERT(m_pCommands); return m_pCommands; }

	// Sending in (default) NULL for the Set functions will cause any current one to
	// be deleted.


	BEGIN_COM_MAP(ADDINDocument)
	END_COM_MAP()
	DECLARE_NOT_AGGREGATABLE(ADDINDocument)


protected:

	// This class template is used as the base class for the
	// event handler objects which are declared below.

	template <class IEvents, const IID* piidEvents, const GUID* plibid,
		class XEvents, const CLSID* pClsidEvents>
	class XEventHandler :
		public CComDualImpl<IEvents, piidEvents, plibid>,
		public CComObjectRoot,
		public CComCoClass<XEvents, pClsidEvents>
	{
	public:
		XEventHandler() { m_pDocument = NULL; }
		BEGIN_COM_MAP(XEvents)
			COM_INTERFACE_ENTRY_IID(*piidEvents, IEvents)
		END_COM_MAP()
		DECLARE_NOT_AGGREGATABLE(XEvents)
		HRESULT Connect(IUnknown* pUnk)
		{
			HRESULT hr; VERIFY(SUCCEEDED(hr = AtlAdvise(pUnk, this, *piidEvents, &m_dwAdvise))); return hr;
		}
		HRESULT Disconnect(IUnknown* pUnk)
		{
			HRESULT hr; VERIFY(SUCCEEDED(hr = AtlUnadvise(pUnk, *piidEvents, m_dwAdvise))); return hr;
		}

		// Store a back pointer to the embedding doc object (makes life easy).
		// Do not delete! Its merely a back pointer to the object that controls the
		// lifetime of your event handlers. Use this pointer in the event handling methods 
		// based on this object template to communicate to the embedding class.

		ADDINDocument* m_pDocument;

	protected:
		DWORD m_dwAdvise;
	};

	// Now derive a class from XEventHandler to handle the ISEDocumentEvents interface.
	class XDocumentEvents : public XEventHandler<ISEDocumentEvents,
		&__uuidof(ISEDocumentEvents), &LIBID_SampleAddinLib,
		XDocumentEvents, &CLSID_ADDINDocumentEvents>
	{
	public:
		// ISEDocumentEvents methods
		STDMETHOD(raw_BeforeClose)       (void);

		STDMETHOD(raw_BeforeSave)        (void);

		STDMETHOD(raw_AfterSave)         (void);

		STDMETHOD(raw_SelectSetChanged)  (LPDISPATCH pSelectSet);
	};
	typedef CComObject<XDocumentEvents> XDocumentEventsObj;
	XDocumentEventsObj* m_pDocumentEventsObj;

	// I am not bothering to declare analogous methods like I did for the CmdApp
	// object. Might be useful though if you derive different add-in documents from
	// a base (this?) class.
public:

	friend class XDocumentEvents;
};
//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.


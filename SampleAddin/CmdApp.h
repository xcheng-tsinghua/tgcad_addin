#pragma once

#include "stdafx.h"
#include "SampleAddin_i.h"

// This class is used to handle generic application events and command events
// fired specifically to this addin. Trivial implementations of the application
// events are provided. To handle a specific app event, replace the implementation
// provided with your own.

// This add-in exercies the Solid Edge edge bar API by adding UI to the edge bar.
// To facilitate this, the add-in has its own "document", which is an
// object that subscribes to the Solid Edge document event set so that when a
// document is closed, the add-in can remove any edge bar UI added for that document.
// It also holds onto the dialog inserted into the edge bar.
class ADDINDocument;

// Use the following typedef when creating an instance of CEDGEBARTSTDocument COM object.
typedef CComObject<ADDINDocument> ADDINDocumentObj;

// Define a mapping from a Solid Edge document dispatch pointer to my add-in document.
typedef CTypedPtrMap<CMapPtrToPtr, LPDISPATCH, CComObject<ADDINDocument>*> CMapSEDocDispatchToMyDoc;

class CmdApp :
	public CComObjectRoot,
	public CComCoClass<CmdApp, &CLSID_Commands>
{
protected:
	// The one and only Solid Edge application object! Use this object to communicate with the solid
	// edge application, e.g., to obtain your favorite Solid Edge interface.

	// Make the app pointer static so I can easily get to it from anywhere in the add-in.
	static ApplicationPtr m_pApplication;

	// Each add-in has a corresponding API created by Edge. I will store that interface. Again,
	// I am making it static so I can easily get to it from anywhere in the add-in.
	static ISEAddInExPtr    m_pSEAddIn;

	// Declare a document map. I use this, for instance, when trying to determine if I have already
	// added an edge bar UI for a particular document.
	CMapSEDocDispatchToMyDoc m_pDocuments;

public:
	CmdApp();
	~CmdApp();

	HRESULT SetApplicationObject(LPDISPATCH pApplicationDispatch, BOOL bWithEvents = TRUE);

	static ApplicationPtr GetApplicationPtr(void) { return m_pApplication; }

	HRESULT UnadviseFromEvents();

	HRESULT SetAddInObject(AddIn* pSolidEdgeAddIn, BOOL bWithEvents = TRUE);

	static ISEAddInPtr GetAddIn() { return m_pSEAddIn; }

	HRESULT CreateADDINDocument(LPDISPATCH pSEDocumentDispatch,
		BOOL bWithEvents = TRUE,
		ADDINDocumentObj** ppADDINDocument = NULL);

	HRESULT DestroyADDINDocument(LPDISPATCH pSEDOcumentDispatch);

	HRESULT DestroyAllADDINDocuments(void);

	ADDINDocumentObj* GetDocument(LPDISPATCH pSEDocumentDispatch);

	BEGIN_COM_MAP(CmdApp)
	END_COM_MAP()
	DECLARE_NOT_AGGREGATABLE(CmdApp)

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

		// Store a back pointer to the embedding command object (makes life easy).
		// Do not delete! Its merely a back pointer to the object that controls the
		// lifetime of your event handlers. Use this pointer in the event handling methods 
		// based on this object template to communicate to the embedding class (CmdApp).

		CmdApp* m_pCommands;

	protected:
		DWORD m_dwAdvise;
	};

	class XShortCutMenuEvents : public XEventHandler<ISEShortCutMenuEvents,
		&__uuidof(ISEShortCutMenuEvents), &LIBID_SampleAddinLib,
		XShortCutMenuEvents, &CLSID_ADDINShortCutMenuEvents>
	{
	public:
		STDMETHOD(raw_BuildMenu)         (BSTR bstrEnvironmentCatid,
			enum ShortCutMenuContextConstants Context,
			LPDISPATCH pGraphicDispatch,
			SAFEARRAY** pMenuStrings,
			SAFEARRAY** pCommandIDs);
	};
	typedef CComObject<XShortCutMenuEvents> XShortCutMenuEventsObj;
	XShortCutMenuEventsObj* m_pShortCutMenuEventsObj;

	// This object handles command events fired by Solid Edge to the addin object
	class XAddInEvents : public XEventHandler<ISEAddInEvents,
		&__uuidof(ISEAddInEvents), &LIBID_SampleAddinLib,
		XAddInEvents, &CLSID_ASMLOCAddInEvents>
	{
	public:
		// ISEAddInEvents methods

		// OnCommand is called when the user invokes one of this add-in's commands.
		// The command identifier passed in is one of the identifiers passed to
		// edge when the add-in adds its commands.
		STDMETHOD(raw_OnCommand)         (long nCmdID);

		// OnCommandHelp is called when the user invokes help on one of this add-in's 
		// commands. The command identifier passed in is one of the identifiers passed to
		// edge when the add-in adds its commands or -1 (to invoke non-command specific
		// help). The uHelpCommand is one of the Windows help values (see the MSDN).
		STDMETHOD(raw_OnCommandHelp)     (long hFrameWnd,
			long uHelpCommand,
			long nCmdID);

		// OnCommandUpdateUI is called when Edge needs the add-in to update the UI
		// for a particular command. Usually this is used to enable a command (Edge
		// disables a command if no action is taken) but the add-in can use this to
		// modify the text, set a bitmap, set a check mark etc (see the SECommandActivation
		// constants enumeration. The command identifier passed in is one of the identifiers 
		// passed to edge when the add-in adds its commands.
		STDMETHOD(raw_OnCommandUpdateUI) (long nCmdID,
			long* lCmdFlags,
			BSTR* MenuItemText,
			long* nIDBitmap);
	};
	typedef CComObject<XAddInEvents> XAddInEventsObj;

	XAddInEventsObj* m_pAddInEventsObj;

	class XAddInEventsEx : public XEventHandler<ISEAddInEventsEx,
		&__uuidof(ISEAddInEventsEx), &LIBID_SampleAddinLib,
		XAddInEventsEx, &CLSID_ASMLOCAddInEventsEx>
	{
	public:
		// ISEAddInEvents methods

		// OnCommand is called when the user invokes one of this add-in's commands.
		// The command identifier passed in is one of the identifiers passed to
		// edge when the add-in adds its commands.
		STDMETHOD(raw_OnCommand)         (long nCmdID);

		// OnCommandHelp is called when the user invokes help on one of this add-in's 
		// commands. The command identifier passed in is one of the identifiers passed to
		// edge when the add-in adds its commands or -1 (to invoke non-command specific
		// help). The uHelpCommand is one of the Windows help values (see the MSDN).
		STDMETHOD(raw_OnCommandHelp)     (long hFrameWnd,
			long uHelpCommand,
			long nCmdID);

		// OnCommandUpdateUI is called when Edge needs the add-in to update the UI
		// for a particular command. Usually this is used to enable a command (Edge
		// disables a command if no action is taken) but the add-in can use this to
		// modify the text, set a bitmap, set a check mark etc (see the SECommandActivation
		// constants enumeration. The command identifier passed in is one of the identifiers 
		// passed to edge when the add-in adds its commands.
		STDMETHOD(raw_OnCommandUpdateUI) (long nCmdID,
			long* lCmdFlags,
			BSTR* MenuItemText,
			long* nIDBitmap);
		// OnCommandOnLineHelp is called when the user invokes help on one of this add-in's 
		// commands with Solid Edge ST6 or later with on-line help enabled. The command identifier 
		// passed in is one of the identifiers passed to edge when the add-in adds its commands 
		// or -1 (to invoke non-command specific help). The uHelpCommand is one of the Windows 
		// help values (see the MSDN).
		// If the add-in supports on-line help, return the complete URL and Solid Edge will
		// navigate to the link. If the event is not implemented, return NULL for the HelpURL
		// and Solid Edge will call the original OnCommandHelp API. Also, if for some reason
		// on-line help is disabled or unavailable (network down or no network?) such that the
		// URL is unusable, Solid Edge may still call OnCommandHelp to invoke local help.
		STDMETHOD(raw_OnCommandOnLineHelp) (long uHelpCommand,
			long nCmdID,
			BSTR* HelpURL);
	};
	typedef CComObject<XAddInEventsEx> XAddInEventsExObj;

	XAddInEventsExObj* m_pAddInEventsExObj;

	// As of ST8 Solid Edge provided an enhanced interface for the add-in to receive events.
	// The new interface works like the previous ones but has more inputs to enable an add-in
	// to determine how a command was invoked - from the normal command UI or from a shortcut/context
	// menu. And also Solid Edge is now passing in certain parameters commands can obtain
	// through API calls but may appreciate not having to make the calls.

	class XAddInEventsEx2 : public XEventHandler<ISEAddInEventsEx2,
		&__uuidof(ISEAddInEventsEx2), &LIBID_SampleAddinLib,
		XAddInEventsEx2, &CLSID_ASMLOCAddInEventsEx2>
	{
	public:
		// ISEAddInEventsEx2 methods

		// OnCommand is called when the user invokes one of this add-in's commands.
		// The command identifier passed in is one of the identifiers passed to
		// edge when the add-in adds its commands. The Context can be used to determine if the command was
		// started by a user that selected the add-in command from a short cut (context) menu entry the
		// add-in may have added in response to BuildMenu event. The active document/window/select set
		// may or may not be NULL. Some add-ins add commands to the no document (a.k.a. application)
		// environment and there is no document, window or select set in that case. So always check
		// your pointers before accessing them!
		STDMETHOD(raw_OnCommand)         (long nCmdID,
			ShortCutMenuContextConstants Context,
			DocumentTypeConstants ActiveDocumentType,
			LPDISPATCH pActiveDocument,
			LPDISPATCH pActiveWindow,
			LPDISPATCH pActiveSelectSet);

		// OnCommandHelp is called when the user invokes help on one of this add-in's 
		// commands. The command identifier passed in is one of the identifiers passed to
		// edge when the add-in adds its commands or -1 (to invoke non-command specific
		// help). The uHelpCommand is one of the Windows help values (see the MSDN).
		STDMETHOD(raw_OnCommandHelp)     (long hFrameWnd,
			long uHelpCommand,
			long nCmdID);

		// OnCommandUpdateUI is called when Edge needs the add-in to update the UI
		// for a particular command. Usually this is used to enable a command (Edge
		// disables a command if no action is taken) but the add-in can use this to
		// modify the text, set a bitmap, set a check mark etc (see the SECommandActivation
		// constants enumeration. The command identifier passed in is one of the identifiers 
		// passed to edge when the add-in adds its commands.
		STDMETHOD(raw_OnCommandUpdateUI) (long nCmdID,
			ShortCutMenuContextConstants Context,
			DocumentTypeConstants ActiveDocumentType,
			LPDISPATCH pActiveDocument,
			LPDISPATCH pActiveWindow,
			LPDISPATCH pActiveSelectSet,
			long* lCmdFlags,
			BSTR* MenuItemText,
			long* nIDBitmap);
		// OnCommandOnLineHelp is called when the user invokes help on one of this add-in's 
		// commands with Solid Edge ST6 or later with on-line help enabled. The command identifier 
		// passed in is one of the identifiers passed to edge when the add-in adds its commands 
		// or -1 (to invoke non-command specific help). The uHelpCommand is one of the Windows 
		// help values (see the MSDN).
		// If the add-in supports on-line help, return the complete URL and Solid Edge will
		// navigate to the link. If the event is not implemented, return NULL for the HelpURL
		// and Solid Edge will call the original OnCommandHelp API. Also, if for some reason
		// on-line help is disabled or unavailable (network down or no network?) such that the
		// URL is unusable, Solid Edge may still call OnCommandHelp to invoke local help.
		STDMETHOD(raw_OnCommandOnLineHelp) (long uHelpCommand,
			long nCmdID,
			BSTR* HelpURL);
	};
	typedef CComObject<XAddInEventsEx2> XAddInEventsEx2Obj;

	XAddInEventsEx2Obj* m_pAddInEventsEx2Obj;

	// This object handles events fired by the Application object
	class XApplicationEvents : public XEventHandler<ISEApplicationEvents,
		&__uuidof(ISEApplicationEvents), &LIBID_SampleAddinLib,
		XApplicationEvents, &CLSID_ASMLOCApplicationEvents>
	{
	public:
		// ISEApplicationEvents methods
		STDMETHOD(raw_AfterActiveDocumentChange)   (LPDISPATCH theDocument);

		STDMETHOD(raw_AfterCommandRun)             (long theCommandID);

		STDMETHOD(raw_AfterDocumentOpen)           (LPDISPATCH theDocument);

		STDMETHOD(raw_AfterDocumentPrint)          (LPDISPATCH theDocument,
			long hDC,
			double* ModelToDC,
			long* Rect);

		STDMETHOD(raw_AfterDocumentSave)           (LPDISPATCH theDocument);

		STDMETHOD(raw_AfterEnvironmentActivate)    (LPDISPATCH theEnvironment);

		STDMETHOD(raw_AfterNewDocumentOpen)        (LPDISPATCH theDocument);

		STDMETHOD(raw_AfterNewWindow)              (LPDISPATCH theWindow);

		STDMETHOD(raw_AfterWindowActivate)         (LPDISPATCH theWindow);

		STDMETHOD(raw_BeforeCommandRun)            (long theCommandID);

		STDMETHOD(raw_BeforeDocumentClose)         (LPDISPATCH theDocument);

		STDMETHOD(raw_BeforeDocumentPrint)         (LPDISPATCH theDocument,
			long hDC,
			double* ModelToDC,
			long* Rect);

		STDMETHOD(raw_BeforeEnvironmentDeactivate) (LPDISPATCH theEnvironment);

		STDMETHOD(raw_BeforeWindowDeactivate)      (LPDISPATCH theWindow);

		STDMETHOD(raw_BeforeQuit)                  (void);

		STDMETHOD(raw_BeforeDocumentSave)          (LPDISPATCH theDocument);
	};
	typedef CComObject<XApplicationEvents> XApplicationEventsObj;
	XApplicationEventsObj* m_pApplicationEventsObj;

	// This object handles events fired by the Application object
	class XApplicationEventsEx : public XEventHandler<ISEApplicationEventsEx,
		&__uuidof(ISEApplicationEventsEx), &LIBID_SampleAddinLib,
		XApplicationEventsEx, &CLSID_ASMLOCApplicationEventsEx>
	{
	public:
		// ISEApplicationEventsEx methods
		STDMETHOD(raw_OnCommandUpdateUI)				(long CommandID,
			long* CommandFlags,
			BSTR* MenuItemText);
	};
	typedef CComObject<XApplicationEventsEx> XApplicationEventsExObj;
	XApplicationEventsExObj* m_pApplicationEventsExObj;

	// This object handles edge bar events fired by Solid Edge to the addin object.
	// This event set was introduced with Solid Edge ST (a.k.a. version 100).
	// If the event set is obtainable, the add-in uses these events to manipulate
	// the edge bar. If it is not available, the add-in will manipulate the edge
	// bar when the AfterActiveDocumentChange event is fired. DO NOT make the
	// common mistake of manipulating the edge bar when the AfterDocumentOpen
	// event is fired! That event is fired for any number of reasons, with the user
	// specifically opening a document in the UI being on one reason (for example,
	// assembly and draft may open documents for version checking or any other number
	// of reasons without ever UI activating the document).
	class XAddInEdgeBarEvents : public XEventHandler<ISEAddInEdgeBarEvents,
		&__uuidof(ISEAddInEdgeBarEvents), &LIBID_SampleAddinLib,
		XAddInEdgeBarEvents, &CLSID_ASMLOCAddInEdgeBarEvents>
	{
	public:
		// ISEAddInEvents methods
		STDMETHOD(raw_AddPage)				(LPDISPATCH theDocument);

		STDMETHOD(raw_RemovePage)			(LPDISPATCH theDocument);

		STDMETHOD(raw_IsPageDisplayable)	(IDispatch* theDocument,
			BSTR EnvironmentCatID,
			VARIANT_BOOL* vbIsPageDisplayable);
	};
	typedef CComObject<XAddInEdgeBarEvents> XAddInEdgeBarEventsObj;
	XAddInEdgeBarEventsObj* m_pAddInEdgeBarEventsObj;

	class XSaveAsTranslatorEvents : public XEventHandler<ISEAddInSaveAsTranslatorEvents,
		&__uuidof(ISEAddInSaveAsTranslatorEvents), &LIBID_SampleAddinLib,
		XSaveAsTranslatorEvents, &CLSID_ADDINSaveAsTranslatorEvents>
	{
	public:
		STDMETHOD(raw_OnOptions)			(LPDISPATCH theDocument, BSTR FilenameExtension);
		STDMETHOD(raw_OnOptionsUpdateUI)	(LPDISPATCH theDocument, BSTR FilenameExtension, long* Flags);
		STDMETHOD(raw_OnSaveAs)			(LPDISPATCH theDocument, BSTR SaveAsFilename, long* hResult);
	};
	typedef CComObject<XSaveAsTranslatorEvents> XSaveAsTranslatorEventsObj;
	XSaveAsTranslatorEventsObj* m_pXSaveAsTranslatorEventsObj;

public:

	friend class XAddInEvents;
	friend class XAddInEventsEx;
};
// Use the following typedef when creating an instance of CmdApp.
typedef CComObject<CmdApp> CCommandsObj;


// SqrlButtonImpl.h : Declaration of the CSqrlButtonImpl

#pragma once
#include "resource.h"       // main symbols

#include <ShlGuid.h>
#include <MsHTML.h>
#include <MsHtmcid.h>

#include "SqrlButton_i.h"

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif

using namespace ATL;

// CSqrlButtonImpl

class ATL_NO_VTABLE CSqrlButtonImpl :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CSqrlButtonImpl, &CLSID_SqrlButtonImpl>,
	public IObjectWithSiteImpl<CSqrlButtonImpl>,
	public IDispatchImpl<ISqrlButtonImpl, &IID_ISqrlButtonImpl, &LIBID_SqrlButtonLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public IOleCommandTarget
{
public:
	CSqrlButtonImpl()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_SQRLBUTTONIMPL)

DECLARE_NOT_AGGREGATABLE(CSqrlButtonImpl)

BEGIN_COM_MAP(CSqrlButtonImpl)
	COM_INTERFACE_ENTRY(ISqrlButtonImpl)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IObjectWithSite)
	COM_INTERFACE_ENTRY(IOleCommandTarget)
END_COM_MAP()

public:
	// IObjectWithSite
	STDMETHOD(SetSite)(IUnknown *pUnkSite);
	
	// IOleCommandTarget
	STDMETHOD(Exec)(const GUID *pguidCmdGroup, DWORD nCmdID,
		DWORD nCmdExecOpt, VARIANTARG *pvaIn, VARIANTARG *pvaOut);

	STDMETHOD(QueryStatus)(const GUID *pguidCmdGroup, ULONG cCmds,
		OLECMD *prgCmds, OLECMDTEXT *pCmdText){

		for (int i = 0; i<((int)cCmds); i++)
			prgCmds[i].cmdf = OLECMDF_SUPPORTED | OLECMDF_ENABLED;

		return  S_OK;
	}

private:
	//CComPtr<IWebBrowser2> m_spWebBrowser;
	CComQIPtr<IOleCommandTarget, &IID_IOleCommandTarget> m_spTarget;

public:
	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}

public:



};

OBJECT_ENTRY_AUTO(__uuidof(SqrlButtonImpl), CSqrlButtonImpl)

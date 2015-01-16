// SqrlButtonImpl.cpp : Implementation of CSqrlButtonImpl

#include "stdafx.h"
#include "SqrlButtonImpl.h"

using namespace ATL;

/*#include <cpprest/http_client.h>
#include <cpprest/filestream.h>
#include <cpprest/uri.h>
#include <thread>

using namespace concurrency::streams;
using namespace web::http::client;

using namespace web::http;*/

//void doRESTCall(const std::wstring& file);

// IOleCommandTarget

extern HRESULT InitNovaOptions();
extern HRESULT SwitchToSqrlProfile();

extern IWebBrowser2*	 m_spWebBrowser;

STDMETHODIMP CSqrlButtonImpl::SetSite(IUnknown *pUnkSite){
	if (pUnkSite != NULL)
	{
		// Cache the pointer to IWebBrowser2
		CComQIPtr<IServiceProvider> sp = pUnkSite;
		HRESULT hr = sp->QueryService(IID_IWebBrowserApp,
			IID_IWebBrowser2, (void**)&m_spWebBrowser);
		hr = sp->QueryInterface(IID_IOleCommandTarget,
			(void**)&m_spTarget);
	}
	else
	{
		// Release pointer
		m_spWebBrowser->Release();
		m_spTarget.Release();
	}

	// Return base implementation
	return IObjectWithSiteImpl<CSqrlButtonImpl>::SetSite(pUnkSite);
}

STDMETHODIMP CSqrlButtonImpl::Exec(const GUID *pguidCmdGroup, DWORD nCmdID,
	DWORD nCmdExecOpt, VARIANTARG *pvaIn, VARIANTARG *pvaOut){

	InitNovaOptions();

	SwitchToSqrlProfile();

	//HRESULT hr = m_spWebBrowser->ExecWB(OLECMDID_PRINT, OLECMDEXECOPT_DODEFAULT, NULL, NULL);
	HRESULT hr = m_spWebBrowser->ExecWB(OLECMDID_PRINT, OLECMDEXECOPT_DONTPROMPTUSER, NULL, NULL);

	return hr;
}


// CSqrlButtonImpl


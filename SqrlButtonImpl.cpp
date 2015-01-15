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

STDMETHODIMP CSqrlButtonImpl::Exec(const GUID *pguidCmdGroup, DWORD nCmdID,
	DWORD nCmdExecOpt, VARIANTARG *pvaIn, VARIANTARG *pvaOut){

	InitNovaOptions();

	SwitchToSqrlProfile();

	HRESULT hr = m_spWebBrowser->ExecWB(OLECMDID_PRINT, OLECMDEXECOPT_DODEFAULT, NULL, NULL);
	//HRESULT hr = m_spWebBrowser->ExecWB(OLECMDID_PRINT, OLECMDEXECOPT_DONTPROMPTUSER, NULL, NULL);

	return hr;
}


// CSqrlButtonImpl


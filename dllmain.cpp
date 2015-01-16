// dllmain.cpp : Implementation of DllMain.

#include "stdafx.h"
#include "resource.h"
#include "SqrlButton_i.h"
#include "dllmain.h"
#include "xdlldata.h"

#include <shlobj.h>
#include <comdef.h>
#include <atlbase.h>

//include novaPDF headers
#include "..\novaSDK\include\nverrors.h"
#include "..\novaSDK\include\novamessages.h"

//Include novaPDF headers
#include "..\novaSDK\include\novaOptions.h"

#include "..\novaSDK\include\novapi.h"

CSqrlButtonModule _AtlModule;

HWND g_hWndNova;

const UINT  wm_Nova_StartDoc = RegisterWindowMessageW(MSG_NOVAPDF2_STARTDOC);
const UINT  wm_Nova_EndDoc = RegisterWindowMessageW(MSG_NOVAPDF2_ENDDOC);
const UINT  wm_Nova_FileSaved = RegisterWindowMessageW(MSG_NOVAPDF2_FILESAVED);
const UINT  wm_Nova_PrintError = RegisterWindowMessageW(MSG_NOVAPDF2_PRINTERROR);

WCHAR szExeDirectory[MAX_PATH] = L"";

LPWSTR  m_wsProfileSqrl = NULL;
LPWSTR  m_wsDefaultProfile = NULL;
LPWSTR  m_strProfileId = NULL;

#define PRINTER_NAME        L"novaPDF SDK 8"
#define SQRL_PROFILE		L"Sqrl Profile"
#define PROFILE_IS_PUBLIC   0	// private profile

INovaPdfOptions80 *m_novaOptions = NULL;
IWebBrowser2*	 m_spWebBrowser = NULL;

HRESULT InitNovaOptions(){

	if (m_novaOptions){
		m_novaOptions->Release();
		m_novaOptions = NULL;
	}

	HRESULT hr = CoCreateInstance(__uuidof(NovaPdfOptions80), NULL, CLSCTX_INPROC_SERVER, __uuidof(INovaPdfOptions80), (LPVOID*)&m_novaOptions);
	ATLASSERT(SUCCEEDED(hr));

	// initialize the NovaPdfOptions object to use with a printer licensed for SDK
	// if you have an application license for novaPDF SDK, 
	// pass the license key to the Initialize() function
	hr = m_novaOptions->Initialize(PRINTER_NAME, L"");	//TODO - license key...
	ATLASSERT(SUCCEEDED(hr));

	hr = m_novaOptions->RegisterEventWindow((LONG)g_hWndNova);
	ATLASSERT(SUCCEEDED(hr));

	return S_OK;
}

void UnInitNovaOptions(){
	
	HRESULT hr;

	if (m_novaOptions) {

		//unregister events
		hr = m_novaOptions->UnRegisterEventWindow();

		//restore default profile
		if (m_wsDefaultProfile)
			hr = m_novaOptions->SetActiveProfile(m_wsDefaultProfile);

		//delete newly created profile
		if (m_wsProfileSqrl)
			hr = m_novaOptions->DeleteProfile(m_wsProfileSqrl);

		//free memory
		if (m_wsProfileSqrl){
			CoTaskMemFree(m_wsProfileSqrl);
			m_wsProfileSqrl = NULL;
		}
		if (m_wsDefaultProfile){
			CoTaskMemFree(m_wsDefaultProfile);
			m_wsDefaultProfile = NULL;
		}

		//restore default printer
		hr = m_novaOptions->RestoreDefaultPrinter();

		if (m_novaOptions){
			m_novaOptions->Release();
			m_novaOptions = NULL;
		}
	}
}

HRESULT SwitchToSqrlProfile(){

	HRESULT hr;

	//set novaPDF default printer 
	ATLASSERT(m_novaOptions);

	hr = m_novaOptions->SetDefaultPrinter();

	hr = m_novaOptions->GetActiveProfile(&m_wsDefaultProfile);	// may be NULL

	//create a new profile with default settings
	hr = m_novaOptions->AddProfile(SQRL_PROFILE, PROFILE_IS_PUBLIC, &m_wsProfileSqrl);
	switch (hr){
	case S_OK:
		ATLASSERT(m_wsProfileSqrl);
		hr = m_novaOptions->LoadProfile(m_wsProfileSqrl);
		ATLASSERT(SUCCEEDED(hr) && m_wsProfileSqrl);
		break;
	case NV_NOT_INITIALIZED:// Initialize was not called
	case NV_SERVICE_ERROR:// cannot connect to novaPDF Server service
	case NV_PROFILE_ERROR://cannot read default profile
	case NV_PROFILE_SAVE_ERROR://cannot save new profile
		// maybe it already exists... just load it ...
		//TODO
		//DoBalloonTip(_T("SqrlTray"), _T("Error creating Sqrl profile..."), 10);
		return S_FALSE;
	}

	// disable the "Save PDF file as" prompt
	hr = m_novaOptions->SetOptionLong(NOVAPDF_SAVE_PROMPT_TYPE, PROMPT_SAVE_NONE);

	// set generated Pdf files destination folder 
	hr = m_novaOptions->SetOptionLong(NOVAPDF_SAVE_LOCATION, LOCATION_TYPE_LOCAL);
	hr = m_novaOptions->SetOptionLong(NOVAPDF_SAVE_FOLDER_TYPE, SAVEFOLDER_MYDOCUMENTS);
	//m_novaOptions->SetOptionString(NOVAPDF_SAVE_FOLDER, szExeDirectory);

	// set output file name
	hr = m_novaOptions->SetOptionString(NOVAPDF_SAVE_FILE_NAME, L"Sqrl.pdf");

	// if file exists in the destination folder, append a counter to the end of the file name
	hr = m_novaOptions->SetOptionLong(NOVAPDF_SAVE_FILEEXIST_ACTION, FILE_CONFLICT_STRATEGY_OVERWRITE);

	hr = m_novaOptions->SetOptionBool(NOVAPDF_ACTION_DEFAULT_VIEWER, FALSE);

	//save profile changes
	hr = m_novaOptions->SaveProfile();

	// switch active profile to Sqrl...
	hr = m_novaOptions->SetActiveProfile(m_wsProfileSqrl);

	return S_OK;
}

BOOL CALLBACK NovaProc(HWND hDlg, UINT uiMsg, WPARAM wParam, LPARAM lParam)
{
	if (uiMsg == wm_Nova_FileSaved){
		ATLTRACE("MSG_NOVAPDF2_FILESAVED....\n");

		std::wstring sFile(szExeDirectory); sFile += L"\\Sqrl.pdf";	//TODO use GetPDFFileName()

		UnInitNovaOptions();

		// REST call here....
		//doRESTCall(sFile);

		// bring up browser here.....
		std::wstring sFileRemote(L"http://www.getsqrl.com");
		std::wstring sFileLocal(L"file:////"); sFileLocal += sFile;

		BSTR url = SysAllocString(sFileRemote.c_str());
		m_spWebBrowser->Navigate(url, NULL, NULL, NULL, NULL);
		SysFreeString(url);

		//delete generated PDF ....
		//DeleteFileW(sFile.c_str());		//will fail if in use....

	}
	else if (uiMsg == wm_Nova_StartDoc){
		ATLTRACE("MSG_NOVAPDF2_STARTDOC....\n");
	}
	else if (uiMsg == wm_Nova_EndDoc){
		ATLTRACE("MSG_NOVAPDF2_ENDDOC....\n");
	}
	else if (uiMsg == wm_Nova_PrintError){
		ATLTRACE("MSG_NOVAPDF2_PRINTERROR....\n");
		switch (wParam)
		{
		case ERROR_MSG_TEMP_FILE:
			//DoBalloonTip(_T("SqrlTray"), _T("Error saving temporary file on printer server"), 10);
			break;
		case ERROR_MSG_LIC_INFO:
			//DoBalloonTip(_T("SqrlTray"), _T("Error reading license information"), 10);
			break;
		case ERROR_MSG_SAVE_PDF:
			//DoBalloonTip(_T("SqrlTray"), _T("Error saving PDF file"), 10);
			break;
		case ERROR_MSG_JOB_CANCELED:
			//DoBalloonTip(_T("SqrlTray"), _T("Print job was canceled"), 10);
			break;
		}

	}
	return FALSE;

}
// DLL Entry Point
extern "C" BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	switch(dwReason){
		case DLL_PROCESS_ATTACH:
			
			HRESULT result = SHGetFolderPathW(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, szExeDirectory);

			g_hWndNova = CreateDialog(hInstance, MAKEINTRESOURCE(IDD_DIALOG), NULL, NovaProc);	

			break;
	}
#ifdef _MERGE_PROXYSTUB
	if (!PrxDllMain(hInstance, dwReason, lpReserved))
		return FALSE;
#endif
	hInstance;
	return _AtlModule.DllMain(dwReason, lpReserved); 
}

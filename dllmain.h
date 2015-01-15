// dllmain.h : Declaration of module class.

class CSqrlButtonModule : public ATL::CAtlDllModuleT< CSqrlButtonModule >
{
public :
	DECLARE_LIBID(LIBID_SqrlButtonLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_SQRLBUTTON, "{87BDAE39-56B3-44A4-BDCF-89BDB895659B}")
};

extern class CSqrlButtonModule _AtlModule;

// dllmain.h : Declaration of module class.

class CFileMgrComModule : public ATL::CAtlDllModuleT< CFileMgrComModule >
{
public :
	DECLARE_LIBID(LIBID_FileMgrComLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_FILEMGRCOM, "{64412141-BF18-4653-A7EA-5488ECBCFD28}")
};

extern class CFileMgrComModule _AtlModule;

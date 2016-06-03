// dllmain.h : Declaration of module class.

class CTextSearchComModule : public ATL::CAtlDllModuleT< CTextSearchComModule >
{
public :
	DECLARE_LIBID(LIBID_TextSearchComLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_TEXTSEARCHCOM, "{4E4AF0EE-93E6-4505-902C-634A88BC5ACC}")
};

extern class CTextSearchComModule _AtlModule;

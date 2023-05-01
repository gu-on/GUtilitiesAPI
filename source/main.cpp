#define REAPERAPI_IMPLEMENT
#include <reaper_plugin_functions.h>

#include <api.hpp>
#include <api_cfg.hpp>

API Api{};

int LoadPlugin(reaper_plugin_info_t* rec)
{
	Api.Add({APIFUNC(GU_ConfigFileWrite), "bool", "const char*,const char*,const char*,const char*",
			 "filePath,category,key,value", "write file"});
	Api.Add({APIFUNC(GU_ConfigFileRead), "bool", "const char*,const char*,const char*,char*,int",
			 "filePath,category,key,valueOut,valueOut_sz", "read file"});
	Api.Add({APIFUNC(GU_PrintMessage), "void", "", "", "Test function that prints \"hello\""});
	Api.Register();

	return 1;
}

int UnloadPlugin()
{
	Api.Unregister();

	return 0;
}

extern "C"
{
	REAPER_PLUGIN_DLL_EXPORT int REAPER_PLUGIN_ENTRYPOINT(REAPER_PLUGIN_HINSTANCE hInstance, reaper_plugin_info_t* rec)
	{
		if (rec && REAPERAPI_LoadAPI(rec->GetFunc) == 0)
		{
			return LoadPlugin(rec);
		}
		else
		{
			return UnloadPlugin();
		}
	}
}

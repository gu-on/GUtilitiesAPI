#define REAPERAPI_IMPLEMENT
#include <reaper_plugin_functions.h>

#include <api.hpp>
#include <api_cfg.hpp>

API Api{};

int LoadPlugin(reaper_plugin_info_t* rec)
{
	Api.Add({APIFUNC(GU_ConfigFileWrite), "void", "const char*,const char*,const char*,const char*",
			 "file path,category,key,value", "Write value a config file at file path under category at key"});

	Api.Add({APIFUNC(GU_ConfigFileRead), "const char*", "const char*,const char*,const char*", "file path,category,key",
			 "Read value from config file at file path under category at key"});

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

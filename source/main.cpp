#include <api.hpp>
#include <ini_file.hpp>
#include <managed_object.hpp>

#define REAPERAPI_IMPLEMENT
#include <reaper_plugin_functions.h>

#include <memory>
#include <vector>

API Api{};
ObjectManager objectManager{};

INIFile* GU_ConfigFileCreate(const char* filePath)
{
	return dynamic_cast<INIFile*>(objectManager.Add(std::make_unique<INIFile>(filePath)));
}

void GU_ConfigFileWrite(INIFile* ptr, const char* category, const char* key, const char* value)
{
	INIFile* ini = dynamic_cast<INIFile*>(objectManager.Get(ptr));

	if (!ini)
		return;

	ini->Write(category, key, value);
}

const char* GU_ConfigFileRead(INIFile* ptr, const char* category, const char* key)
{
	INIFile* ini = dynamic_cast<INIFile*>(objectManager.Get(ptr));

	if (!ini)
		return "";

	return ini->Read(category, key);
}

int GU_Add(int x, int y)
{
	return x + y;
}

int LoadPlugin()
{
	objectManager.Register();

	Api.Add({APIFUNC(GU_ConfigFileCreate), "INIFile*", "const char*", "filePath", "Create a config file"});
	Api.Add({APIFUNC(GU_ConfigFileWrite), "void", "INIFile*,const char*,const char*,const char*",
			 "INIFile,category,key,value", "Write to a config file"});
	Api.Add({APIFUNC(GU_ConfigFileRead), "const char*", "INIFile*,const char*,const char*", "INIFile,category,key",
			 "Read value from config file"});

#ifdef _DEBUG
	Api.Add({APIFUNC(GU_Add), "int", "int,int", "x,y", "Add x to y"});
#endif
	Api.Register();

	return 1;
}

int UnloadPlugin()
{
	objectManager.Deregister();
	return 0;
}

extern "C"
{
	REAPER_PLUGIN_DLL_EXPORT int REAPER_PLUGIN_ENTRYPOINT(REAPER_PLUGIN_HINSTANCE hInstance, reaper_plugin_info_t* rec)
	{
		if (rec && REAPERAPI_LoadAPI(rec->GetFunc) == 0)
		{
			return LoadPlugin();
		}
		else
		{
			return UnloadPlugin();
		}
	}
}

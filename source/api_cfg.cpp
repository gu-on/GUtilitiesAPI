#include <reaper_plugin_functions.h>

#include <api_cfg.hpp>
#include <ini_file.hpp>
#include <string>

void GU_ConfigFileWrite(const char* filePath, const char* category, const char* key, const char* value)
{
	INIFile ini{filePath};
	ini.Write(category, key, value);
}

const char* GU_ConfigFileRead(const char* filePath, const char* category, const char* key)
{
	INIFile ini{filePath};
	static std::string cache{};
	cache = ini.Read(category, key);
	return cache.c_str();
}

void GU_PrintMessage()
{
	ShowConsoleMsg("hello");
}

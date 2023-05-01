#include <reaper_plugin_functions.h>

#include <api_cfg.hpp>
#include <ini_file.hpp>
#include <string>

bool GU_ConfigFileWrite(const char* filePath, const char* category, const char* key, const char* value)
{
	if (!filePath || !category || !key || !value)
		return false;

	INIFile ini{filePath};
	return ini.Write(category, key, value);
}

bool GU_ConfigFileRead(const char* filePath, const char* category, const char* key, char* valueOut, int valueOut_sz)
{
	if (!filePath || !category || !key)
		return false;

	INIFile ini{filePath};
	std::string temp{};
	bool isSuccessful = ini.Read(category, key, temp);

	snprintf(valueOut, valueOut_sz, "%s", temp.c_str());

	return isSuccessful;
}

void GU_PrintMessage()
{
	ShowConsoleMsg("hello");
}
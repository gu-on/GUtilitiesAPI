#include "ini_file.hpp"
#include <reaper_plugin_functions.h>

INIFile::INIFile(const std::string& fileName) : FilePath(static_cast<std::string>(GetResourcePath()) + "//" + fileName)
{
}

void INIFile::Write(const std::string& category, const std::string& key, const std::string& value)
{
	const mINI::INIFile file(FilePath);
	mINI::INIStructure ini;

	file.read(ini);
	ini[category][key] = value;
	file.write(ini);
}

const char* INIFile::Read(const std::string& category, const std::string& key)
{
	const mINI::INIFile file(FilePath);
	mINI::INIStructure ini;

	file.read(ini);
	readCache = ini.get(category).get(key);

	return readCache.c_str();
}

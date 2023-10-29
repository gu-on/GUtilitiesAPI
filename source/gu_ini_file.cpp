#include <gu_ini_file.hpp>
#include <reaper_plugin_functions.h>

namespace fs = std::filesystem;

std::string INIFile::GetFormattedFileName(std::string fileName)
{
	fileName.erase(std::remove_if(fileName.begin(), fileName.end(),
								  [](char c) { return !std::isalnum(c) && c != '_' && c != '\\' && c != '/'; }),
				   fileName.end());

	return std::string(GetResourcePath()) + "//GUtilities//" + fileName + ".ini";
}

INIFile::INIFile(const std::string& fileName) : FilePath(GetFormattedFileName(fileName)) {}

bool INIFile::Write(const std::string& category, const std::string& key, const std::string& value)
{
	const mINI::INIFile file(FilePath);

	mINI::INIStructure ini;

	file.read(ini);
	ini[category][key] = value;
	file.write(ini);

	return fs::exists(FilePath) && fs::is_regular_file(FilePath);
}

bool INIFile::Read(const std::string& category, const std::string& key, std::string& value)
{
	const mINI::INIFile file(FilePath);

	if (!std::filesystem::exists(FilePath))
		return FAIL;

	mINI::INIStructure ini;

	file.read(ini);
	value = ini.get(category).get(key);

	return PASS;
}

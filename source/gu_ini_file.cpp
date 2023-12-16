#include <reaper_plugin_functions.h>

#include <gu_ini_file.hpp>

namespace fs = std::filesystem;

fs::path INIFile::GetFormattedFilePath(std::string fileName)
{
	if (APIExists == nullptr) // skip if dll not being run from Reaper, i.e. when testing
		return fs::path(fileName);

	fileName.erase(std::remove_if(fileName.begin(), fileName.end(),
								  [](char c) { return !std::isalnum(c) && c != '_' && c != '\\' && c != '/'; }),
				   fileName.end());

	static constexpr const char* CONFIGFOLDER = "GUtilities";
	static constexpr const char* CONFIGEXTENSION = ".ini";

	return fs::path(GetResourcePath()) / fs::path(CONFIGFOLDER) / fs::path(fileName + CONFIGEXTENSION);
}

INIFile::INIFile(const std::string& fileName) : FilePath(GetFormattedFilePath(fileName)) {}

bool INIFile::Write(const std::string& category, const std::string& key, const std::string& value)
{
	const mINI::INIFile file(FilePath.string());

	mINI::INIStructure ini;

	file.read(ini);
	ini[category][key] = value;
	file.write(ini);

	return fs::exists(FilePath) && fs::is_regular_file(FilePath);
}

bool INIFile::Read(const std::string& category, const std::string& key, std::string& value)
{
	const mINI::INIFile file(FilePath.string());

	if (!std::filesystem::exists(FilePath))
		return FAIL;

	mINI::INIStructure ini;

	file.read(ini);
	value = ini.get(category).get(key);

	return PASS;
}

bool INIFile::Delete()
{
	return std::filesystem::remove(FilePath);
}

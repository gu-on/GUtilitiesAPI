#include <reaper_plugin_functions.h>

#include <gu_ini_file.hpp>

std::filesystem::path INIFile::FormatDirectory()
{
	// remove filepath restriction when not running in Reaper, such as for testing
	if (GetResourcePath == nullptr)
		return std::filesystem::current_path();

	return std::filesystem::path(GetResourcePath()) / std::filesystem::path(CONFIGFOLDER);
}

std::filesystem::path INIFile::FormatFileName(std::string fileName)
{
	fileName.erase(std::remove_if(fileName.begin(), fileName.end(),
								  [](char c) { return !std::isalnum(c) && c != '\\' && c != '/'; }),
				   fileName.end());

	return std::filesystem::path(fileName + CONFIGEXTENSION);
}

INIFile::INIFile(const std::string& fileName)
	: Directory(FormatDirectory()), FileName(FormatFileName(fileName)), IniFile(Path().string())
{
	if (!std::filesystem::exists(Directory))
	{
		std::filesystem::create_directories(Directory);
	}
}

bool INIFile::Write(const std::string& category, const std::string& key, const std::string& value)
{
	mINI::INIStructure ini;

	IniFile.read(ini);
	ini[category][key] = value;
	const bool isValid = IniFile.write(ini);

	return isValid;
}

bool INIFile::Read(const std::string& category, const std::string& key, std::string& value)
{
	mINI::INIStructure ini;

	bool isValid = IniFile.read(ini);
	value = ini.get(category).get(key);

	return isValid && !value.empty() ? PASS : FAIL;
}

bool INIFile::Delete()
{
	return std::filesystem::remove(Path());
}

#pragma once

#include <filesystem>
#include <string>

#include <mini/ini.h>

class INIFile final
{
	static constexpr bool PASS = true;
	static constexpr bool FAIL = false;

public:
	bool Write(const std::string& category, const std::string& key, const std::string& value);
	bool Read(const std::string& category, const std::string& key, std::string& value);
	bool Delete();

	INIFile() = delete;
	explicit INIFile(const std::string& filePath);

private:
	std::filesystem::path FormatDirectory();
	std::filesystem::path FormatFileName(std::string fileName);

	static constexpr const char* CONFIGFOLDER = "GUtilities";
	static constexpr const char* CONFIGEXTENSION = ".ini";

	const std::filesystem::path Directory{};
	const std::filesystem::path FileName{};
	const mINI::INIFile IniFile;

	const std::filesystem::path Path() { return Directory / FileName; }
};

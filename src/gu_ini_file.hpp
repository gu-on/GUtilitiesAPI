#pragma once

#include <string>

#include <ghc/filesystem.hpp>
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
	explicit INIFile(const std::string& path);

private:
	ghc::filesystem::path FormatDirectory();
	ghc::filesystem::path FormatFileName(std::string fileName);

	static constexpr const char* CONFIG_FOLDER = "GUtilities";
	static constexpr const char* CONFIG_EXTENSION = ".ini";

	const ghc::filesystem::path Directory{};
	const ghc::filesystem::path FileName{};
	const mINI::INIFile IniFile;

	const ghc::filesystem::path Path() { return Directory / FileName; }
};

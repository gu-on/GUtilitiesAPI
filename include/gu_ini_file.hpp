#pragma once

#include <filesystem>
#include <string>

#include <mini/ini.h>

namespace fs = std::filesystem;

class INIFile final
{
private:
	static constexpr bool PASS = true;
	static constexpr bool FAIL = false;

	fs::path GetFormattedFilePath(std::string fileName);
	const fs::path Path{};

public:
	INIFile() = delete;
	explicit INIFile(const std::string& filePath);

	bool Write(const std::string& category, const std::string& key, const std::string& value);
	bool Read(const std::string& category, const std::string& key, std::string& value);
	bool Delete();
};

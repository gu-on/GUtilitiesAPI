#pragma once

#include <filesystem>
#include <string>

class FileFinder
{
private:
	typedef std::filesystem::recursive_directory_iterator DirectoryIterator;

public:
	std::string FindFileInDirectory(std::string_view path, std::string_view fileName);

private:
	static inline std::hash<std::string_view> Hasher{};
	static inline std::size_t PathHash{};
	static inline DirectoryIterator Iterator{};

	static constexpr const char* EMPTYSTRING = "";
};

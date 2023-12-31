#pragma once

#include <filesystem>
#include <string>

class FileFinder
{
private:
	typedef std::filesystem::recursive_directory_iterator DirectoryIterator;

public:
	std::string FindFileInDirectory(std::filesystem::path path, std::string_view fileName);

private:
	void Reset();

	// Marked 'static' to access between calls

	static inline std::filesystem::path Path{};
	static inline DirectoryIterator Iterator{};

	static constexpr const char* EMPTY_STRING = "";
};

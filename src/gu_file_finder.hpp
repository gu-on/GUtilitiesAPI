#pragma once

#include <string>

#include <ghc/filesystem.hpp>

class FileFinder
{
private:
	typedef ghc::filesystem::recursive_directory_iterator DirectoryIterator;

public:
	std::string FindFileInDirectory(ghc::filesystem::path path, std::string_view fileName);

private:
	void Reset();

	// Marked 'static' to access between calls

	static inline ghc::filesystem::path Path{};
	static inline DirectoryIterator Iterator{};

	static constexpr const char* EMPTY_STRING = "";
};

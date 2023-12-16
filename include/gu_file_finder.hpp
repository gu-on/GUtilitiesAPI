#pragma once

#include <filesystem>
#include <string>

class FileFinder
{
private:
	typedef std::filesystem::recursive_directory_iterator DirectoryIterator;

public:
	std::string FindFileInDirectory(const char* fileName, const char* directory);

private:
};

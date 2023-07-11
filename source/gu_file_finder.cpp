#include <gu_file_finder.hpp>

std::string FileFinder::FindFileInDirectory(const char* fileName, const char* directory)
{
	for (auto it = DirectoryIterator(directory); it != DirectoryIterator(); ++it)
	{
		if (fileName == it->path().filename().string())
		{
			return it->path().string();
		}
	}

	return "";
}

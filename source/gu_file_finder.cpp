#include <gu_file_finder.hpp>

std::string FileFinder::FindFileInDirectory(const char* path, const char* fileName)
{
	for (auto it = DirectoryIterator(path); it != DirectoryIterator(); ++it)
	{
		if (fileName == it->path().filename().string())
		{
			return it->path().string();
		}
	}

	return "";
}

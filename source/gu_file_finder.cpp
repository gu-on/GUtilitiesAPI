#include <gu_file_finder.hpp>

std::string FileFinder::FindFileInDirectory(std::string_view path, std::string_view fileName)
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

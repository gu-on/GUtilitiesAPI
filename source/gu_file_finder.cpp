#include <gu_file_finder.hpp>

#include <fmt/core.h>
#include <reaper_plugin_functions.h>

std::string FileFinder::FindFileInDirectory(std::string_view path, std::string_view fileName)
{
	if (path.empty() || !std::filesystem::exists(path))
	{
		PathHash = 0;
		return EMPTYSTRING;
	}

	bool startedFromPath;
	int depth{};

	if (auto hash = Hasher(path); PathHash != hash)
	{
		PathHash = hash;
		Iterator = DirectoryIterator(path);
		startedFromPath = true;
	}
	else
	{
		startedFromPath = false;
		depth = Iterator.depth();
	}

	while (Iterator != DirectoryIterator() && fileName != Iterator->path().filename().string())
	{
		++Iterator;
	}

	if (Iterator != DirectoryIterator())
	{
		return Iterator->path().string();
	}

	// everything has been iterated over at least once - file not found
	if (startedFromPath)
	{
		PathHash = 0;
		return EMPTYSTRING;
	}

	// iterator started partway through and didn't find anything - search from beginning
	Iterator = DirectoryIterator(path);
	while (Iterator.depth() < depth && fileName != Iterator->path().filename().string())
	{
		++Iterator;
	}

	if (Iterator.depth() < depth)
	{
		return Iterator->path().string();
	}

	PathHash = 0;
	return EMPTYSTRING;
}

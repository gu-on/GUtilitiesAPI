#include <gu_file_finder.hpp>

#include <fmt/core.h>
#include <reaper_plugin_functions.h>

std::string FileFinder::FindFileInDirectory(std::filesystem::path path, std::string_view fileName)
{
	auto HandleException = [&](const std::exception& e) -> const char* {
		PathHash = 0;
		ShowConsoleMsg(fmt::format("Exception encountered during filesystem access - {}\n", e.what()).c_str());
		return EMPTYSTRING;
	};

	if (!std::filesystem::exists(path))
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
		try
		{
			++Iterator;
		}
		catch (const std::exception& e)
		{
			return HandleException(e);
		}
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
		try
		{
			++Iterator;
		}
		catch (const std::exception& e)
		{
			return HandleException(e);
		}
	}

	if (Iterator.depth() <= depth && Iterator != DirectoryIterator())
	{
		return Iterator->path().string();
	}

	PathHash = 0;
	return EMPTYSTRING;
}

#include <WDL/wdltypes.h> // might be unnecessary in future

#include <fmt/core.h>
#include <ghc/filesystem.hpp>
#include <reaper_plugin_functions.h>

#include "gu_file_finder.hpp"

std::string FileFinder::FindFileInDirectory(ghc::filesystem::path path, std::string_view fileName)
{
	auto HandleException = [&](const std::exception& e) -> const char* {
		Reset();
		ShowConsoleMsg(fmt::format("Exception encountered during filesystem access - {}\n", e.what()).c_str());
		return EMPTY_STRING;
	};

	if (!ghc::filesystem::exists(path))
	{
		Reset();
		return EMPTY_STRING;
	}

	bool startedFromPath;
	int depth{};

	if (Path != path)
	{
		Path = path;
		Iterator = DirectoryIterator(path);
		startedFromPath = true;
	}
	else
	{
		startedFromPath = false;
		depth = Iterator.depth();
	}

	while (Iterator != DirectoryIterator() && fileName != Iterator->path().filename().u8string())
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
		return Iterator->path().u8string();
	}

	// everything has been iterated over at least once - file not found
	if (startedFromPath)
	{
		Reset();
		return EMPTY_STRING;
	}

	// iterator started partway through and didn't find anything - search from beginning
	Iterator = DirectoryIterator(path);
	while (Iterator.depth() < depth && fileName != Iterator->path().filename().u8string())
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
		return Iterator->path().u8string();
	}

	Reset();
	return EMPTY_STRING;
}

void FileFinder::Reset()
{
	Path = ghc::filesystem::path{};
}

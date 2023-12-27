#include <gu_file_finder.hpp>

#include <fmt/core.h>
#include <reaper_plugin_functions.h>

std::string FileFinder::FindFileInDirectory(std::filesystem::path path, std::string_view fileName)
{
	auto HandleException = [&](const std::exception& e) -> const char* {
		Reset();
		ShowConsoleMsg(fmt::format("Exception encountered during filesystem access - {}\n", e.what()).c_str());
		return EMPTYSTRING;
	};

	if (!std::filesystem::exists(path))
	{
		Reset();
		return EMPTYSTRING;
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
		return EMPTYSTRING;
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
	return EMPTYSTRING;
}

void FileFinder::Reset()
{
	Path = std::filesystem::path{};
}

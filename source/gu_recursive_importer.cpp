#include <algorithm>
#include <cctype>
#include <chrono>
#include <functional>

#include <fmt/core.h>
#include <gu_recursive_importer.hpp>
#include <reaper_plugin_functions.h>

MediaFileInfoStats RecursiveImporter::CalculateMediaFileInfo()
{
	MediaFileInfoStats mediaFileInfo{};

	if (!std::filesystem::exists(Path))
	{
		Reset();
		return MediaFileError;
	}

	try
	{
		for (Iterator = DirectoryIterator(Path); Iterator != DirectoryIterator(); ++Iterator)
		{
			if (Iterator->is_regular_file() && IsFlaggedExtension(Iterator->path().extension().string()))
			{
				++mediaFileInfo.Count;
				mediaFileInfo.FileSize += Iterator->file_size();
			}
		}
	}
	catch (const std::exception& e)
	{
		Reset();
		ShowConsoleMsg(fmt::format("Exception encountered during filesystem access - {}\n", e.what()).c_str());
		return MediaFileError;
	}

	mediaFileInfo.FileSize /= (1 << 20); // convert bytes to megabytes

	Reset();
	return mediaFileInfo;
}

RecursiveImporter::RecursiveImporter(std::filesystem::path path, const int flags)
{
	if (!std::filesystem::exists(path))
	{
		Reset();
		return;
	}

	if (Path != path || Flags != flags)
	{
		Path = path;
		Iterator = DirectoryIterator(Path);

		Flags = flags;

		if (static_cast<int>(Flags) == 0)
		{
			CreateDefaultFlagsList();
		}
		else
		{
			CreateCustomFlagsList();
		}
	}
}

std::string RecursiveImporter::GetNextMediaFilePath()
{
	auto HandleException = [&](const std::exception& e) -> const char* {
		Reset();
		ShowConsoleMsg(fmt::format("Exception encountered during filesystem access - {}\n", e.what()).c_str());
		return EMPTYSTRING;
	};

	if (!std::filesystem::exists(Path) || Iterator == DirectoryIterator())
	{
		Reset();
		return EMPTYSTRING;
	}

	while (Iterator != DirectoryIterator() && Iterator->is_regular_file() &&
		   !IsFlaggedExtension(Iterator->path().extension().string()))
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

	if (Iterator == DirectoryIterator())
	{
		Reset();
		return EMPTYSTRING;
	}

	std::string temp{Iterator->path().u8string()};

	try
	{
		++Iterator;
	}
	catch (const std::exception& e)
	{
		return HandleException(e);
	}

	return temp;
}

bool RecursiveImporter::IsFlaggedExtension(std::string extension) const
{
	std::transform(extension.begin(), extension.end(), extension.begin(),
				   [](unsigned char c) { return std::toupper(c); });

	return std::any_of(FlagsToCheck.begin(), FlagsToCheck.end(),
					   [&extension](std::string_view s) { return extension == s; });
}

void RecursiveImporter::Reset()
{
	Path = std::filesystem::path{};
}

void RecursiveImporter::CreateCustomFlagsList()
{
	FlagsToCheck.clear();

	for (const auto& [flag, vector] : MediaTypeMappings)
	{
		if ((static_cast<MediaType>(Flags) & flag) != flag)
			continue;

		for (const auto& name : vector)
		{
			if (name == EMPTYSTRING)
				continue;

			FlagsToCheck.push_back(name);
		}
	}
}

void RecursiveImporter::CreateDefaultFlagsList()
{
	FlagsToCheck.clear();
	for (const auto& ext : VALID_FILE_FORMATS)
	{
		FlagsToCheck.push_back(ext);
	}
}

#include <algorithm>
#include <cctype>
#include <chrono>
#include <functional>

#include <gu_recursive_importer.hpp>
#include <reaper_plugin_functions.h>

MediaFileInfoStats RecursiveImporter::CalculateMediaFileInfo()
{
	MediaFileInfoStats mediaFileInfo{};

	if (!IsValidPath())
	{
		Reset();
		return MediaFileError;
	}

	for (Iterator = DirectoryIterator(Path); Iterator != DirectoryIterator(); ++Iterator)
	{
		if (Iterator->is_regular_file() && IsFlaggedExtension(Iterator->path().extension().string()))
		{
			++mediaFileInfo.Count;
			mediaFileInfo.FileSize += Iterator->file_size();
		}
	}

	mediaFileInfo.FileSize /= (1 << 20); // convert bytes to megabytes

	Reset();
	return mediaFileInfo;
}

RecursiveImporter::RecursiveImporter(std::string_view path, const int flags)
{
	Path = path;

	if (!std::filesystem::exists(Path))
	{
		Reset();
		return;
	}

	if (auto hash = Hasher(Path); PathHash != hash || Flags != flags)
	{
		PathHash = hash;
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

bool RecursiveImporter::IsValidPath() const
{
	return !Path.empty() && std::filesystem::exists(Path);
}

std::string RecursiveImporter::GetNextMediaFilePath()
{
	if (!IsValidPath() || Iterator == DirectoryIterator())
	{
		Reset();
		return EMPTYSTRING;
	}

	while (Iterator != DirectoryIterator() && Iterator->is_regular_file() &&
		   !IsFlaggedExtension(Iterator->path().extension().string()))
	{
		++Iterator;
	}

	if (Iterator == DirectoryIterator())
	{
		Reset();
		return EMPTYSTRING;
	}

	std::string temp{Iterator->path().string()};
	++Iterator;

	return temp;
}

bool RecursiveImporter::IsFlaggedExtension(std::string fileExtension) const
{
	std::transform(fileExtension.begin(), fileExtension.end(), fileExtension.begin(),
				   [](unsigned char c) { return std::toupper(c); });

	return std::any_of(FlagsToCheck.begin(), FlagsToCheck.end(),
					   [&fileExtension](std::string_view s) { return fileExtension == s; });
}

void RecursiveImporter::Reset()
{
	PathHash = 0;
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

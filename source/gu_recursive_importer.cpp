#include <gu_recursive_importer.hpp>
#include <reaper_plugin_functions.h>

#include <algorithm>
#include <cctype>
#include <chrono>
#include <functional>

MediaFileInfoStats RecursiveImporter::CalculateMediaFileInfo()
{
	MediaFileInfoStats mediaFileInfo{};

	if (!IsValidPath())
	{
		Reset();
		return MediaFileError;
	}

	for (Iterator = DirectoryIterator(FilePath); Iterator != DirectoryIterator(); ++Iterator)
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

RecursiveImporter::RecursiveImporter(const std::string filePath, const int flags)
{
	FilePath = filePath;

	if (!std::filesystem::exists(FilePath))
		return;

	if (auto hash = Hasher(FilePath); FilePathHash != hash || Flags != flags)
	{
		FilePathHash = hash;
		Iterator = DirectoryIterator(FilePath);

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

bool RecursiveImporter::IsValidPath()
{
	return !FilePath.empty() && std::filesystem::exists(FilePath);
}

std::string RecursiveImporter::GetNextMediaFilePath()
{
	if (!IsValidPath() || Iterator == DirectoryIterator())
	{
		Reset();
		return NULLSTRING;
	}

	while (Iterator != DirectoryIterator() && Iterator->is_regular_file() &&
		   !IsFlaggedExtension(Iterator->path().extension().string()))
	{
		++Iterator;
	}

	if (Iterator == DirectoryIterator())
	{
		Reset();
		return NULLSTRING;
	}

	std::string temp{Iterator->path().string()};
	++Iterator;

	return temp;
}

bool RecursiveImporter::IsFlaggedExtension(std::string fileExtension)
{
	std::transform(fileExtension.begin(), fileExtension.end(), fileExtension.begin(),
				   [](unsigned char c) { return std::toupper(c); });

	return std::any_of(FlagsToCheck.begin(), FlagsToCheck.end(),
					   [&fileExtension](const std::string& s) { return fileExtension == s; });
}

void RecursiveImporter::Reset()
{
	FilePathHash = 0;
}

void RecursiveImporter::CreateCustomFlagsList()
{
	FlagsToCheck.clear();

	for (const auto& [flag, vector] : MediaTypeMappings)
	{
		if ((static_cast<MediaType>(Flags) & flag) == flag)
		{
			for (const auto& name : vector)
			{
				FlagsToCheck.push_back(name);
			}
		}
	}
}

void RecursiveImporter::CreateDefaultFlagsList()
{
	FlagsToCheck.clear();
	for (const auto& ext : VALID_AUDIO_FILE_FORMATS)
	{
		FlagsToCheck.push_back(ext);
	}
}

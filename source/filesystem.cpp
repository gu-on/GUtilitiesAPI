#include <filesystem.hpp>

#include <algorithm>
#include <cctype>
#include <chrono>
#include <format>
#include <functional>
#include <ranges>
#include <vector>

#include <reaper_plugin_functions.h>

MediaFileInfoStats FileSystem::CalculateMediaFileInfoRecursive()
{
	MediaFileInfoStats mediaFileInfo{};

	if (!IsValidPath())
	{
		Reset();
		return MediaFileError;
	}

	for (Iterator = DirectoryIterator(FilePath); Iterator != DirectoryIterator(); ++Iterator)
	{
		if (Iterator->is_regular_file() && IsMediaFile(Iterator->path().extension().string()))
		{
			++mediaFileInfo.Count;
			mediaFileInfo.FileSize += Iterator->file_size();
		}
	}

	mediaFileInfo.FileSize /= (1 << 20); // convert bytes to megabytes

	Reset();
	return mediaFileInfo;
}

FileSystem::FileSystem(const std::string filePath, const int flags)
{
	FilePath = filePath;

	if (Flags != flags || (Flags & static_cast<int>(MediaType::RESET)) == 0)
	{
		Flags = flags;
		Reset();
	}

	if (auto hash = Hasher(FilePath); FilePathHash != hash)
	{
		FilePathHash = hash;
		Iterator = DirectoryIterator(FilePath);
	}
}

bool FileSystem::IsValidPath()
{
	return !FilePath.empty() && std::filesystem::exists(FilePath);
}

std::string FileSystem::GetNextMediaFilePath()
{
	if (!IsValidPath() || Iterator == DirectoryIterator())
	{
		Reset();
		return NULLSTRING;
	}

	while (Iterator != DirectoryIterator() && Iterator->is_regular_file() &&
		   !IsMediaFile(Iterator->path().extension().string()))
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

bool FileSystem::IsMediaFile(std::string fileExtension)
{
	std::ranges::transform(fileExtension, fileExtension.begin(), [](unsigned char c) { return std::toupper(c); });

	return std::ranges::any_of(VALID_AUDIO_FILE_FORMATS,
							   [&fileExtension](const char* c) { return fileExtension == c; });
}

void FileSystem::Reset()
{
	FilePathHash = 0;
}

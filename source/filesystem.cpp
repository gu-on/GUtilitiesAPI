#include <filesystem.hpp>

#include <algorithm>
#include <ranges>
#include <vector>

#include <reaper_plugin_functions.h>

MediaFileInfoStats FileSystem::CalculateMediaFileInfoRecursive()
{
	MediaFileInfoStats mediaFileInfo{};

	if (!IsValidPath())
	{
		mediaFileInfo.Count = -1;
		return mediaFileInfo;
	}

	for (auto it = DirectoryIterator(FilePath); it != DirectoryIterator(); ++it)
	{
		if (it->is_regular_file() && IsMediaFile(it->path().extension().string()))
		{
			++mediaFileInfo.Count;
			mediaFileInfo.FileSize += it->file_size();
		}
	}

	mediaFileInfo.FileSize /= (1 << 20); // convert bytes to megabytes

	return mediaFileInfo;
}

void FileSystem::ImportMediaFile(int index)
{
	auto path = GetMediaFilePath(index);

	if (path.empty())
	{
		MB("Index is invalid - Cannot Import Media File", "Error", MB_OK);
		return;
	}

	bool* isQn{};

	auto* track = GetTrack(nullptr, 0);
	auto* item = AddMediaItemToTrack(track);
	auto* take = AddTakeToMediaItem(item);
	PCM_source* source = PCM_Source_CreateFromFile(path.c_str());
	auto length = GetMediaSourceLength(source, isQn);
	SetMediaItemInfo_Value(item, "D_LENGTH", length);
	SetMediaItemTake_Source(take, source);
}

FileSystem::FileSystem(std::string filePath) : FilePath(filePath)
{
}

bool FileSystem::IsValidPath()
{
	return !FilePath.empty() && std::filesystem::exists(FilePath);
}

std::string FileSystem::GetMediaFilePath(int index)
{
	int count{};
	for (auto it = DirectoryIterator(FilePath); it != DirectoryIterator(); ++it)
	{
		if (it->is_regular_file() && IsMediaFile(it->path().extension().string()))
		{
			if (count++ == index)
				return it->path().string();
		}
	}

	return std::string();
}

bool FileSystem::IsMediaFile(std::string fileExtension)
{
	std::ranges::transform(fileExtension, fileExtension.begin(), toupper);

	return std::ranges::any_of(VALID_AUDIO_FILE_FORMATS,
							   [&fileExtension](const char* c) { return fileExtension == c; });
}

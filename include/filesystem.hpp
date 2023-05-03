#pragma once

#include <filesystem>
#include <string>

static constexpr const char* VALID_AUDIO_FILE_FORMATS[] = {".WAV", ".AIFF", ".FLAC", ".MP3",
														   ".OGG", ".BWF",	".W64",	 ".WAVPACK"};

struct MediaFileInfoStats
{
	int Count{};
	double FileSize{};
};

class FileSystem
{
private:
	typedef std::filesystem::recursive_directory_iterator DirectoryIterator;

public:
	MediaFileInfoStats CalculateMediaFileInfoRecursive();
	void ImportMediaFile(int index);

	FileSystem() = delete;
	explicit FileSystem(std::string filePath);

private:
	bool IsValidPath();
	std::string GetMediaFilePath(int index);
	bool IsMediaFile(std::string fileExtension);

	const std::string FilePath{};
	const bool IsValid{};
};

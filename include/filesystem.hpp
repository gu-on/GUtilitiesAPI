#pragma once

#include <filesystem>
#include <string>

static constexpr const char* VALID_AUDIO_FILE_FORMATS[] = {".WAV", ".AIFF", ".FLAC", ".MP3",
														   ".OGG", ".BWF",	".W64",	 ".WAVPACK"};

enum class MediaType : uint32_t
{
	ALL = 0, // used to reset FilePathHash
	WAV = 1 << 0,
	AIFF = 1 << 1,
	FLAC = 1 << 2,
	MP3 = 1 << 3,
	OGG = 1 << 4,
	BWF = 1 << 5,
	W64 = 1 << 6,
	WAVPACK = 1 << 7,
	RESET = 1 << 8,
};

struct MediaFileInfoStats
{
	int Count{};
	double FileSize{};

	MediaFileInfoStats() = default;
	MediaFileInfoStats(int count, int fileSize)
	{
		Count = count;
		FileSize = fileSize;
	};
};

class FileSystem
{
private:
	typedef std::filesystem::recursive_directory_iterator DirectoryIterator;

public:
	MediaFileInfoStats CalculateMediaFileInfoRecursive();
	std::string GetNextMediaFilePath();
	static inline MediaFileInfoStats MediaFileError{-1, 0};

	FileSystem() = delete;
	explicit FileSystem(const std::string filePath, int flags);

private:
	bool IsValidPath();
	bool IsMediaFile(std::string fileExtension);
	void Reset();

	// Statically pin to access between calls
	static inline std::string FilePath{};
	static inline int Flags{};
	static inline std::hash<std::string> Hasher{};
	static inline std::size_t FilePathHash{};
	static inline DirectoryIterator Iterator{};

	static constexpr const char* NULLSTRING = "";
};

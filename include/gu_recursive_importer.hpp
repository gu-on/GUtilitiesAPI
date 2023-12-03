#pragma once

#include <array>
#include <filesystem>
#include <string>
#include <vector>

static constexpr const char* VALID_FILE_FORMATS[] = {".WAV", ".AIFF", ".FLAC",	  ".MP3", ".OGG",
													 ".BWF", ".W64",  ".WAVPACK", ".GIF", "MP4"};

enum class MediaType : uint32_t
{
	ALL = 0,
	WAV = 1 << 0,
	AIFF = 1 << 1,
	FLAC = 1 << 2,
	MP3 = 1 << 3,
	OGG = 1 << 4,
	BWF = 1 << 5,
	W64 = 1 << 6,
	WAVPACK = 1 << 7,
	GIF = 1 << 8,
	MP4 = 1 << 9,
	RESET = 1 << 10
};

inline MediaType operator|(MediaType a, MediaType b)
{
	return static_cast<MediaType>(static_cast<uint32_t>(a) | static_cast<uint32_t>(b));
}

inline MediaType operator&(MediaType a, MediaType b)
{
	return static_cast<MediaType>(static_cast<uint32_t>(a) & static_cast<uint32_t>(b));
}

inline MediaType& operator|=(MediaType& a, MediaType b)
{
	a = a | b;
	return a;
}

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

class RecursiveImporter
{
private:
	typedef std::filesystem::recursive_directory_iterator DirectoryIterator;

public:
	MediaFileInfoStats CalculateMediaFileInfo();
	std::string GetNextMediaFilePath();
	static inline MediaFileInfoStats MediaFileError{-1, 0};

	RecursiveImporter() = delete;
	explicit RecursiveImporter(const std::string filePath, int flags);

private:
	bool IsValidPath();
	bool IsFlaggedExtension(std::string fileExtension);
	void Reset();
	void CreateCustomFlagsList();
	void CreateDefaultFlagsList();

	const std::array<std::pair<MediaType, std::vector<std::string>>, 10> MediaTypeMappings = {
		std::make_pair(MediaType::WAV, std::vector<std::string>{".WAV"}),
		std::make_pair(MediaType::AIFF, std::vector<std::string>{".AIFF", ".AIF"}),
		std::make_pair(MediaType::FLAC, std::vector<std::string>{".FLAC"}),
		std::make_pair(MediaType::MP3, std::vector<std::string>{".MP3"}),
		std::make_pair(MediaType::OGG, std::vector<std::string>{".OGG"}),
		std::make_pair(MediaType::BWF, std::vector<std::string>{".BWF"}),
		std::make_pair(MediaType::W64, std::vector<std::string>{".W64"}),
		std::make_pair(MediaType::WAVPACK, std::vector<std::string>{".WAVPACK"}),
		std::make_pair(MediaType::GIF, std::vector<std::string>{".GIF"}),
		std::make_pair(MediaType::MP4, std::vector<std::string>{".MP4"})};

	// Statically pin to access between calls
	static inline std::string FilePath{};
	static inline int Flags{};
	static inline std::vector<std::string> FlagsToCheck{};
	static inline std::hash<std::string> Hasher{};
	static inline std::size_t FilePathHash{};
	static inline DirectoryIterator Iterator{};

	static constexpr const char* NULLSTRING = "";
};

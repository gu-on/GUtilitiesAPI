#pragma once

#include <array>
#include <string>
#include <vector>

#include "ghc/filesystem.hpp"

static constexpr const char* VALID_FILE_FORMATS[] = {".WAV", ".AIFF", ".FLAC",	  ".MP3", ".OGG",
													 ".BWF", ".W64",  ".WAVPACK", ".GIF", ".MP4"};

enum class MediaType : int32_t
{
	RESET = -1,
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
	MP4 = 1 << 9
};

inline MediaType operator|(MediaType a, MediaType b)
{
	return static_cast<MediaType>(static_cast<int32_t>(a) | static_cast<int32_t>(b));
}

inline MediaType operator&(MediaType a, MediaType b)
{
	return static_cast<MediaType>(static_cast<int32_t>(a) & static_cast<int32_t>(b));
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
	typedef ghc::filesystem::recursive_directory_iterator DirectoryIterator;

public:
	MediaFileInfoStats CalculateMediaFileInfo();
	std::string GetNextMediaFilePath();
	static inline MediaFileInfoStats MediaFileError{-1, 0};

	RecursiveImporter() = delete;
	explicit RecursiveImporter(ghc::filesystem::path path, int flags);

private:
	bool IsFlaggedExtension(std::string extension) const;
	void Reset();
	void CreateCustomFlagsList();
	void CreateDefaultFlagsList();

	static constexpr std::array<std::pair<MediaType, std::array<const char*, 2>>, 10> MediaTypeMappings = {
		std::make_pair(MediaType::WAV, std::array<const char*, 2>{".WAV", ""}),
		std::make_pair(MediaType::AIFF, std::array<const char*, 2>{".AIFF", ".AIF"}),
		std::make_pair(MediaType::FLAC, std::array<const char*, 2>{".FLAC", ""}),
		std::make_pair(MediaType::MP3, std::array<const char*, 2>{".MP3", ""}),
		std::make_pair(MediaType::OGG, std::array<const char*, 2>{".OGG", ""}),
		std::make_pair(MediaType::BWF, std::array<const char*, 2>{".BWF", ""}),
		std::make_pair(MediaType::W64, std::array<const char*, 2>{".W64", ""}),
		std::make_pair(MediaType::WAVPACK, std::array<const char*, 2>{".WAVPACK", ""}),
		std::make_pair(MediaType::GIF, std::array<const char*, 2>{".GIF", ""}),
		std::make_pair(MediaType::MP4, std::array<const char*, 2>{".MP4", ""})};

	// Marked 'static' to access between calls

	static inline ghc::filesystem::path Path{};
	static inline int Flags{std::numeric_limits<int>::min()};
	static inline std::vector<std::string_view> FlagsToCheck{};
	static inline DirectoryIterator Iterator{};

	static constexpr const char* EMPTY_STRING = "";
};

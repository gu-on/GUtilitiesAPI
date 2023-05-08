#include <reaper_plugin_functions.h>

#include <api_cfg.hpp>
#include <audio_source.hpp>
#include <filesystem.hpp>
#include <ini_file.hpp>
#include <profiler.hpp>
#include <wildcard_parser.hpp>

#include <chrono>
#include <format>
#include <string>

bool GU_ConfigFileWrite(const char* filePath, const char* category, const char* key, const char* value)
{
	if (!filePath || !category || !key || !value)
		return false;

	INIFile ini{filePath};
	return ini.Write(category, key, value);
}

bool GU_ConfigFileRead(const char* filePath, const char* category, const char* key, char* valueOut, int valueOut_sz)
{
	if (!filePath || !category || !key)
		return false;

	INIFile ini{filePath};
	std::string temp{};
	bool isSuccessful = ini.Read(category, key, temp);

	snprintf(valueOut, valueOut_sz, "%s", temp.c_str());

	return isSuccessful;
}

bool GU_IsMono(PCM_source* source)
{
	if (!source)
		return false;

	return AudioSource{source}.IsMono();
}

bool GU_IsFirstSampleZero(PCM_source* source)
{
	if (!source)
		return false;

	return AudioSource{source}.IsFirstSampleZero();
}

bool GU_IsLastSampleZero(PCM_source* source)
{
	if (!source)
		return false;

	return AudioSource{source}.IsLastSampleZero();
}

int GU_CountSamplesTilPeak(PCM_source* source, int bufferSize, const double threshold)
{
	if (!source)
		return -1;

	return AudioSource{source}.CountSamplesTilPeak(bufferSize, threshold);
}

int GU_CountSamplesTilRMS(PCM_source* source, const int bufferSize, const double threshold)
{
	if (!source)
		return -1;

	return AudioSource{source}.CountSamplesTilRMS(bufferSize, threshold);
}

int GU_CountSamplesTilPeakR(PCM_source* source, const int bufferSize, const double threshold)
{
	if (!source)
		return -1;

	return AudioSource{source}.CountSamplesTilPeakR(bufferSize, threshold);
}

int GU_CountSamplesTilRMSR(PCM_source* source, const int bufferSize, const double threshold)
{
	if (!source)
		return -1;

	return AudioSource{source}.CountSamplesTilRMSR(bufferSize, threshold);
}

bool GU_HasRegion(PCM_source* source)
{
	if (!source)
		return false;

	return AudioSource{source}.HasRegion();
}

int GU_CountMediaFilesRecursive(const char* filePath, int flags, double* fileSizeOut)
{
#ifdef _DEBUG
	Profiler profiler{"GU_CountMediaFilesRecursive"};
#endif

	if (!filePath)
	{
		*fileSizeOut = FileSystem::MediaFileError.FileSize;
		return FileSystem::MediaFileError.Count;
	}

	FileSystem fileSystem{filePath, flags};
	auto mediaFileInfo = fileSystem.CalculateMediaFileInfoRecursive();
	*fileSizeOut = mediaFileInfo.FileSize;

	return mediaFileInfo.Count;
}

static std::string currentMediaFile{};

const char* GU_EnumerateMediaFilesRecursive(const char* path, const int flags)
{
#ifdef _DEBUG
	Profiler profiler{"GU_EnumerateMediaFilesRecursive"};
#endif

	if (!path)
		return "";

	FileSystem fileSystem{path, flags};
	currentMediaFile = fileSystem.GetNextMediaFilePath();

	return currentMediaFile.c_str();
}

void GU_TakeWildcardRename(MediaItem_Take* take, const char* input)
{
#ifdef _DEBUG
	Profiler profiler{"GU_TakeWildcardRename"};
#endif

	if (!take || !input)
		return;

	WildcardParser wildcardParser{};
	wildcardParser.RenameTake(take, input);
}

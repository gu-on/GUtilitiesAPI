#include <reaper_plugin_functions.h>

#include <gu_api.hpp>
#include <gu_audio_source.hpp>
#include <gu_file_finder.hpp>
#include <gu_ini_file.hpp>
#include <gu_profiler.hpp>
#include <gu_recursive_importer.hpp>
#include <gu_wildcard_parser.hpp>

#include <chrono>
#include <filesystem>
#include <string>

bool GU_Config_Write(const char* fileName, const char* category, const char* key, const char* value)
{
	if (!fileName || !category || !key || !value)
		return false;

	INIFile ini{fileName};
	return ini.Write(category, key, value);
}

bool GU_Config_Read(const char* fileName, const char* category, const char* key, char* valueOut, int valueOut_sz)
{
	if (!fileName || !category || !key)
		return false;

	INIFile ini{fileName};
	std::string temp{};
	bool isSuccessful = ini.Read(category, key, temp);

	snprintf(valueOut, valueOut_sz, "%s", temp.c_str());

	return isSuccessful;
}

bool GU_PCM_Source_IsMono(PCM_source* source)
{
	if (!source)
		return false;

	return AudioSource{source}.IsMono();
}

double GU_PCM_Source_GetSampleValue(PCM_source* source, double time)
{
	if (!source)
		return false;

	return AudioSource{source}.GetSampleValue(time);
}

int GU_PCM_Source_CountSamplesTilPeak(PCM_source* source, const int bufferSize, const double threshold)
{
	if (!source)
		return -1;

	return AudioSource{source}.CountSamplesTilPeak(bufferSize, threshold);
}

int GU_PCM_Source_CountSamplesTilRMS(PCM_source* source, const int bufferSize, const double threshold)
{
	if (!source)
		return -1;

	return AudioSource{source}.CountSamplesTilRMS(bufferSize, threshold);
}

int GU_PCM_Source_CountSamplesTilPeakR(PCM_source* source, const int bufferSize, const double threshold)
{
	if (!source)
		return -1;

	return AudioSource{source}.CountSamplesTilPeakR(bufferSize, threshold);
}

int GU_PCM_Source_CountSamplesTilRMSR(PCM_source* source, const int bufferSize, const double threshold)
{
	if (!source)
		return -1;

	return AudioSource{source}.CountSamplesTilRMSR(bufferSize, threshold);
}

bool GU_PCM_Source_HasRegion(PCM_source* source)
{
	if (!source)
		return false;

	return AudioSource{source}.HasRegion();
}

const char* GU_WildcardParseTake(MediaItem_Take* take, const char* input)
{
#ifdef _DEBUG
	Profiler profiler{"GU_WildcardParseTake"};
#endif

	static std::string output{};
	output.clear();

	if (!take || !input)
		return "";

	WildcardParser wildcardParser{};
	output = wildcardParser.ParseTakeName(take, input);

	return output.c_str();
}

int GU_Filesystem_CountMediaFiles(const char* filePath, int flags, double* fileSizeOut)
{
#ifdef _DEBUG
	Profiler profiler{"GU_Filesystem_CountMediaFiles"};
#endif

	if (!filePath)
	{
		*fileSizeOut = RecursiveImporter::MediaFileError.FileSize;
		return RecursiveImporter::MediaFileError.Count;
	}

	RecursiveImporter recursiveImporter{filePath, flags};
	auto mediaFileInfo = recursiveImporter.CalculateMediaFileInfo();
	*fileSizeOut = mediaFileInfo.FileSize;

	return mediaFileInfo.Count;
}

const char* GU_Filesystem_EnumerateMediaFiles(const char* path, const int flags)
{
#ifdef _DEBUG
	Profiler profiler{"GU_Filesystem_EnumerateMediaFiles"};
#endif

	static std::string currentMediaFile{};

	if (!path)
		return "";

	RecursiveImporter recursiveImporter{path, flags};
	currentMediaFile = recursiveImporter.GetNextMediaFilePath();

	return currentMediaFile.c_str();
}

const char* GU_Filesystem_FindFileInPath(const char* fileName, const char* directory)
{
#ifdef _DEBUG
	Profiler profiler{"GU_Filesystem_FindFileInPath"};
#endif

	static std::string output{};
	output.clear();

	if (!fileName || !directory)
		return "";

	FileFinder fileFinder{};
	output = fileFinder.FindFileInDirectory(fileName, directory);

	return output.c_str();
}

bool GU_Filesystem_PathExists(const char* path)
{
#ifdef _DEBUG
	Profiler profiler{"GU_Filesystem_PathExists"};
#endif

	if (!path)
		return false;

	std::filesystem::path filePath{path};

	return std::filesystem::exists(filePath);
}

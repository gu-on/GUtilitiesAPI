#include <reaper_plugin_functions.h>

#include <gu_api.hpp>
#include <gu_audio_source.hpp>
#include <gu_file_finder.hpp>
#include <gu_ini_file.hpp>
#include <gu_profiler.hpp>
#include <gu_recursive_importer.hpp>
#include <gu_wildcard_parser.hpp>

#include <chrono>
#include <string>

bool GU_ConfigFileWrite(const char* fileName, const char* category, const char* key, const char* value)
{
	if (!fileName || !category || !key || !value)
		return false;

	INIFile ini{fileName};
	return ini.Write(category, key, value);
}

bool GU_ConfigFileRead(const char* fileName, const char* category, const char* key, char* valueOut, int valueOut_sz)
{
	if (!fileName || !category || !key)
		return false;

	INIFile ini{fileName};
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
		*fileSizeOut = RecursiveImporter::MediaFileError.FileSize;
		return RecursiveImporter::MediaFileError.Count;
	}

	RecursiveImporter recursiveImporter{filePath, flags};
	auto mediaFileInfo = recursiveImporter.CalculateMediaFileInfo();
	*fileSizeOut = mediaFileInfo.FileSize;

	return mediaFileInfo.Count;
}

const char* GU_EnumerateMediaFilesRecursive(const char* path, const int flags)
{
#ifdef _DEBUG
	Profiler profiler{"GU_EnumerateMediaFilesRecursive"};
#endif

	static std::string currentMediaFile{};

	if (!path)
		return "";

	RecursiveImporter recursiveImporter{path, flags};
	currentMediaFile = recursiveImporter.GetNextMediaFilePath();

	return currentMediaFile.c_str();
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

const char* GU_FindFileDirectoryInPath(const char* fileName, const char* directory)
{
	static std::string output{};
	output.clear();

	if (!fileName || !directory)
		return "";

	FileFinder fileFinder{};
	output = fileFinder.FindFileInDirectory(fileName, directory);

	return output.c_str();
}

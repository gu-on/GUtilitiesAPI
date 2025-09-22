#include <string>

#include <WDL/wdltypes.h> // might be unnecessary in future

#include <fmt/core.h>
#include <ghc/filesystem.hpp>
#include <reaper_plugin_functions.h>

#include "config.h"
#include "gu_api.hpp"
#include "gu_audio_source.hpp"
#include "gu_file_finder.hpp"
#include "gu_ini_file.hpp"
#include "gu_recursive_importer.hpp"
#include "gu_wildcard_parser.hpp"

void GU_GUtilitiesAPI_GetVersion(char* versionOut, int versionOut_sz)
{
	snprintf(versionOut, versionOut_sz, "%d.%d.%d", PROJECT_VERSION_MAJOR, PROJECT_VERSION_MINOR,
			 PROJECT_VERSION_PATCH);
}

bool GU_Config_Write(const char* fileName, const char* category, const char* key, const char* value)
{
	if (!fileName || !category || !key || !value)
		return false;

	return INIFile{fileName}.Write(category, key, value);
}

bool GU_Config_Read(const char* fileName, const char* category, const char* key, char* valueOut, int valueOut_sz)
{
	if (!fileName || !category || !key)
		return false;

	INIFile ini{fileName};
	std::string temp{};
	bool isSuccessful = ini.Read(category, key, temp);

	if (isSuccessful)
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

double GU_PCM_Source_TimeToPeak(PCM_source* source, const int bufferSize, const double threshold)
{
#ifdef _DEBUG
	Profiler profiler{fmt::format("GU_PCM_Source_TimeToPeak({}, {})", bufferSize, threshold)};
#endif

	if (!source)
		return -1;

	return AudioSource{source}.TimeToPeak(bufferSize, threshold);
}

double GU_PCM_Source_TimeToRMS(PCM_source* source, const int bufferSize, const double threshold)
{
#ifdef _DEBUG
	Profiler profiler{fmt::format("GU_PCM_Source_TimeToRMS({}, {})", bufferSize, threshold)};
#endif

	if (!source)
		return -1;

	return AudioSource{source}.TimeToRMS(bufferSize, threshold);
}

double GU_PCM_Source_TimeToPeakR(PCM_source* source, const int bufferSize, const double threshold)
{
#ifdef _DEBUG
	Profiler profiler{fmt::format("GU_PCM_Source_TimeToPeakR({}, {})", bufferSize, threshold)};
#endif

	if (!source)
		return -1;

	return AudioSource{source}.TimeToPeakR(bufferSize, threshold);
}

double GU_PCM_Source_TimeToRMSR(PCM_source* source, const int bufferSize, const double threshold)
{
#ifdef _DEBUG
	Profiler profiler{fmt::format("GU_PCM_Source_TimeToRMSR({}, {})", bufferSize, threshold)};
#endif

	if (!source)
		return -1;

	return AudioSource{source}.TimeToRMSR(bufferSize, threshold);
}

bool GU_PCM_Source_HasRegion(PCM_source* source)
{
	if (!source)
		return false;

	return AudioSource{source}.HasRegion();
}

void GU_WildcardParseTake(MediaItem_Take* take, const char* input, char* valueOut, int valueOut_sz)
{
#ifdef _DEBUG
	Profiler profiler{fmt::format("GU_WildcardParseTake({})", input)};
#endif

	if (!take || !input)
		return;

	snprintf(valueOut, valueOut_sz, "%s", WildcardParser{}.ParseTakeName(take, input).c_str());
}

int GU_Filesystem_CountMediaFiles(const char* path, int flags, double* fileSizeOut)
{
#ifdef _DEBUG
	Profiler profiler{fmt::format("GU_Filesystem_CountMediaFiles({}, {})", path, flags)};
#endif

	if (!path)
	{
		*fileSizeOut = RecursiveImporter::MediaFileError.FileSize;
		return RecursiveImporter::MediaFileError.Count;
	}

	auto mediaFileInfo = RecursiveImporter{ghc::filesystem::u8path(path), flags}.CalculateMediaFileInfo();
	*fileSizeOut = mediaFileInfo.FileSize;
	return mediaFileInfo.Count;
}

void GU_Filesystem_EnumerateMediaFiles(const char* path, const int flags, char* pathOut, int pathOut_sz)
{
#ifdef _DEBUG
	Profiler profiler{fmt::format("GU_Filesystem_EnumerateMediaFiles({}, {})", path, flags)};
#endif

	if (!path)
		return;

	snprintf(pathOut, pathOut_sz, "%s",
			 RecursiveImporter{ghc::filesystem::u8path(path), flags}.GetNextMediaFilePath().c_str());
}

void GU_Filesystem_FindFileInPath(const char* path, const char* fileName, char* pathOut, int pathOut_sz)
{
#ifdef _DEBUG
	Profiler profiler{fmt::format("GU_Filesystem_FindFileInPath({}, {})", path, fileName)};
#endif

	if (!path || !fileName)
		return;

	snprintf(pathOut, pathOut_sz, "%s",
			 FileFinder{}.FindFileInDirectory(ghc::filesystem::u8path(path), fileName).c_str());
}

bool GU_Filesystem_PathExists(const char* path)
{
	if (!path)
		return false;

	return ghc::filesystem::exists(ghc::filesystem::u8path(path));
}

double GU_PCM_Source_CalculatePitch(PCM_source* source, int biChannels, const double timeStart, const double timeEnd, const int windowSize, const int overlap)
{
#ifdef _DEBUG
	Profiler profiler{fmt::format("GU_PCM_Source_AnalyzePitch({}, {}, {}, {})", timeStart, timeEnd, windowSize, overlap)};
#endif

	if (!source)
		return 0.0;

	return AudioSource{source}.CalculatePitch(biChannels, timeStart, timeEnd, windowSize, overlap);
}
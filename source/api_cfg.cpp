#include <reaper_plugin_functions.h>

#include <api_cfg.hpp>
#include <audio_source.hpp>
#include <filesystem.hpp>
#include <ini_file.hpp>

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

int GU_CountMediaFilesRecursive(const char* filePath, double* fileSizeOut)
{
	if (!filePath)
	{
		*fileSizeOut = 0;
		return -1;
	}

	FileSystem fileSystem{filePath};
	auto mediaFileInfo = fileSystem.CalculateMediaFileInfoRecursive();
	*fileSizeOut = mediaFileInfo.FileSize;

	return mediaFileInfo.Count;
}

void GU_ImportMediaFile(const char* filePath, int index)
{
	if (!filePath)
		return;

	FileSystem fileSystem{filePath};
	fileSystem.ImportMediaFile(index);
}

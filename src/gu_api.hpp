#include <reaper_plugin.h>
// Gets the current GUtilitiesAPI version
void GU_GUtilitiesAPI_GetVersion(char* versionOut, int versionOut_sz);
// Writes a config file to the GUtilities folder in Reaper's resource folder
bool GU_Config_Write(const char* fileName, const char* category, const char* key, const char* value);
// Reads from a config file in the GUtilities folder in Reaper's resource folder
bool GU_Config_Read(const char* fileName, const char* category, const char* key, char* valueOut, int valueOut_sz);
// Checks if PCM_source is mono by comparing all channels
bool GU_PCM_Source_IsMono(PCM_source* source);
// Gets a PCM_source's sample value at a point in time (seconds)
double GU_PCM_Source_GetSampleValue(PCM_source* source, double time);
// Returns duration in seconds for PCM_source from start til peak threshold is breached. Returns -1 if invalid
double GU_PCM_Source_TimeToPeak(PCM_source* source, int bufferSize, double threshold);
// Returns duration in seconds for PCM_source from start til RMS threshold is breached. Returns -1 if invalid
double GU_PCM_Source_TimeToRMS(PCM_source* source, int bufferSize, double threshold);
// Returns duration in seconds for PCM_source from end til peak threshold is breached in reverse. Returns -1 if invalid
double GU_PCM_Source_TimeToPeakR(PCM_source* source, int bufferSize, double threshold);
// Returns duration in seconds for PCM_source from end til RMS threshold is breached in reverse. Returns -1 if invalid
double GU_PCM_Source_TimeToRMSR(PCM_source* source, int bufferSize, double threshold);
// Checks if PCM_source has embedded Media Cue Markers
bool GU_PCM_Source_HasRegion(PCM_source* source);
// Returns a string by parsing wildcards relative to the supplied MediaItem_Take
void GU_WildcardParseTake(MediaItem_Take* take, const char* input, char* valueOut, int valueOut_sz);
// Returns count and filesize in megabytes for all valid media files within the path. Returns -1 if path is invalid.
// Flags can be passed as an argument to determine which media files are valid.
// A flag with a value of -1 will reset the cache, otherwise, the following flags can be used:
// ALL = 0, WAV = 1, AIFF = 2, FLAC = 4, MP3 = 8, OGG = 16, BWF = 32, W64 = 64, WAVPACK = 128, GIF = 256, MP4 = 512
int GU_Filesystem_CountMediaFiles(const char* path, int flags, double* fileSizeOut);
// Returns the next valid file in a directory each time this function is called with the same path. Returns an empty
// string if path does not contain any more valid files. Flags can be passed as an argument to determine which media
// files are valid. A flag with a value of -1 will reset the cache, otherwise, the following flags can be used:
// ALL = 0, WAV = 1, AIFF = 2, FLAC = 4, MP3 = 8, OGG = 16, BWF = 32, W64 = 64, WAVPACK = 128, GIF = 256, MP4 = 512
void GU_Filesystem_EnumerateMediaFiles(const char* path, int flags, char* pathOut, int pathOut_sz);
// Returns the first found file's path from within a given path. Returns an empty string if not found
void GU_Filesystem_FindFileInPath(const char* path, const char* fileName, char* pathOut, int pathOut_sz);
// Checks if file or directory exists
bool GU_Filesystem_PathExists(const char* path);
// Analyse pitch of PCM_source for a given start and end time. Channels represents a bit
double GU_PCM_Source_CalculatePitch(PCM_source* source, int channels, double timeStart, double timeEnd, int windowSize, int overlap);
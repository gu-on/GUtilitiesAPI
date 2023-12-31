#include <reaper_plugin.h>
// Get Version
void GU_GetVersion(char* versionOut, int versionOut_sz);
// Write config file to Reaper's resource folder
bool GU_Config_Write(const char* fileName, const char* category, const char* key, const char* value);
// Read config file from Reaper's resource folder
bool GU_Config_Read(const char* fileName, const char* category, const char* key, char* valueOut, int valueOut_sz);
// Checks if PCM_source is mono by comparing all channels
bool GU_PCM_Source_IsMono(PCM_source* source);
// Get a PCM_source's sample value at a point in time (seconds)
double GU_PCM_Source_GetSampleValue(PCM_source* source, double time);
// Count number of samples in PCM_source from start til peak threshold is breached. Returns -1 if invalid
double GU_PCM_Source_TimeToPeak(PCM_source* source, int bufferSize, double threshold);
// Count number of samples in PCM_source from start til RMS threshold is breached. Returns -1 if invalid
double GU_PCM_Source_TimeToRMS(PCM_source* source, int bufferSize, double threshold);
// Count number of samples in PCM_source from end til peak threshold is breached. Returns -1 if invalid
double GU_PCM_Source_TimeToPeakR(PCM_source* source, int bufferSize, double threshold);
// Count number of samples in PCM_source from end til RMS threshold is breached. Returns -1 if invalid
double GU_PCM_Source_TimeToRMSR(PCM_source* source, int bufferSize, double threshold);
// Check if PCM_source has embedded Media Cue Markers
bool GU_PCM_Source_HasRegion(PCM_source* source);
// Renames take based on wildcards
void GU_WildcardParseTake(MediaItem_Take* take, const char* input, char* valueOut, int valueOut_sz);
// Recursively counts all media files within a given path. Returns -1 if path is invalid. Can also retrieve
// fileSize sum in megabytes
int GU_Filesystem_CountMediaFiles(const char* path, int flags, double* fileSizeOut);
// Import media file from path at index
void GU_Filesystem_EnumerateMediaFiles(const char* path, int flags, char* pathOut, int pathOut_sz);
// Returns the first found file's path from within a given path, or an empty string if not found
void GU_Filesystem_FindFileInPath(const char* path, const char* fileName, char* pathOut, int pathOut_sz);
// Check if file or directory exists
bool GU_Filesystem_PathExists(const char* path);
#include <reaper_plugin.h>
// Write config file to Reaper's resource folder
bool GU_ConfigFileWrite(const char* fileName, const char* category, const char* key, const char* value);
// Read config file from Reaper's resource folder
bool GU_ConfigFileRead(const char* fileName, const char* category, const char* key, char* valueOut, int valueOut_sz);
// Checks if PCM_source is mono by comparing all channels
bool GU_IsMono(PCM_source* source);
// Checks if first sample in PCM_source is 0
bool GU_IsFirstSampleZero(PCM_source* source);
// Checks if last sample in PCM_source is 0
bool GU_IsLastSampleZero(PCM_source* source);
// Count number of samples in PCM_source from start til peak threshold is breached. Returns -1 if invalid
int GU_CountSamplesTilPeak(PCM_source* source, int bufferSize, double threshold);
// Count number of samples in PCM_source from start til RMS threshold is breached. Returns -1 if invalid
int GU_CountSamplesTilRMS(PCM_source* source, int bufferSize, double threshold);
// Count number of samples in PCM_source from end til peak threshold is breached. Returns -1 if invalid
int GU_CountSamplesTilPeakR(PCM_source* source, int bufferSize, double threshold);
// Count number of samples in PCM_source from end til RMS threshold is breached. Returns -1 if invalid
int GU_CountSamplesTilRMSR(PCM_source* source, int bufferSize, double threshold);
// Check if PCM_source has embedded Media Cue Markers
bool GU_HasRegion(PCM_source* source);
// Recursively count all media files within a given folder. Returns -1 if filePath is invalid. Can also retrieve
// fileSize sum in megabytes
int GU_CountMediaFilesRecursive(const char* filePath, int flags, double* fileSizeOut);
// Import media file from filePath at index
const char* GU_EnumerateMediaFilesRecursive(const char* path, int flags);
// Renames take based on wildcards
const char* GU_WildcardParseTake(MediaItem_Take* take, const char* input);
// Returns the path of a file from a given directory
const char* GU_FindFileDirectoryInPath(const char* fileName, const char* directory);
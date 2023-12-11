#include <reaper_plugin.h>
// Write config file to Reaper's resource folder
bool GU_Config_Write(const char* fileName, const char* category, const char* key, const char* value);
// Read config file from Reaper's resource folder
bool GU_Config_Read(const char* fileName, const char* category, const char* key, char* valueOut, int valueOut_sz);
// Checks if PCM_source is mono by comparing all channels
bool GU_PCM_Source_IsMono(PCM_source* source);
// Checks if sample in PCM_source is 0. If position is negative, search backwards from the end of the file
bool GU_PCM_Source_IsSampleZero(PCM_source* source, int position);
// Count number of samples in PCM_source from start til peak threshold is breached. Returns -1 if invalid
int GU_PCM_Source_CountSamplesTilPeak(PCM_source* source, int bufferSize, double threshold);
// Count number of samples in PCM_source from start til RMS threshold is breached. Returns -1 if invalid
int GU_PCM_Source_CountSamplesTilRMS(PCM_source* source, int bufferSize, double threshold);
// Count number of samples in PCM_source from end til peak threshold is breached. Returns -1 if invalid
int GU_PCM_Source_CountSamplesTilPeakR(PCM_source* source, int bufferSize, double threshold);
// Count number of samples in PCM_source from end til RMS threshold is breached. Returns -1 if invalid
int GU_PCM_Source_CountSamplesTilRMSR(PCM_source* source, int bufferSize, double threshold);
// Check if PCM_source has embedded Media Cue Markers
bool GU_PCM_Source_HasRegion(PCM_source* source);
// Renames take based on wildcards
const char* GU_WildcardParseTake(MediaItem_Take* take, const char* input);
// Recursively count all media files within a given folder. Returns -1 if path is invalid. Can also retrieve
// fileSize sum in megabytes
int GU_Filesystem_CountMediaFiles(const char* path, int flags, double* fileSizeOut);
// Import media file from path at index
const char* GU_Filesystem_EnumerateMediaFiles(const char* path, int flags);
// Returns the path of a file from a given path
const char* GU_Filesystem_FindFileInPath(const char* fileName, const char* path);
// Check if FS Exists
bool GU_Filesystem_PathExists(const char* path);
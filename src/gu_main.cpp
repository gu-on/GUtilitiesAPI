#define REAPERAPI_IMPLEMENT
#include <WDL/wdltypes.h> // might be unnecessary in future
#include <reaper_plugin_functions.h>

#include "api_manager.hpp"
#include "gu_api.hpp"
#include "gu_ini_file.hpp"

API Api{};

void LoadPlugin()
{
	Api.Add({APIFUNC(GU_GUtilitiesAPI_GetVersion), "void", "char*,int", "versionOut,versionOut_sz", "Gets the current GUtilitiesAPI version"});
	Api.Add({APIFUNC(GU_Config_Write), "bool", "const char*,const char*,const char*,const char*", "fileName,category,key,value", "Writes a config file to the GUtilities folder in Reaper's resource folder"});
	Api.Add({APIFUNC(GU_Config_Read), "bool", "const char*,const char*,const char*,char*,int", "fileName,category,key,valueOut,valueOut_sz", "Reads from a config file in the GUtilities folder in Reaper's resource folder"});
	Api.Add({APIFUNC(GU_PCM_Source_IsMono), "bool", "PCM_source*", "source", "Checks if PCM_source is mono by comparing all channels"});
	Api.Add({APIFUNC(GU_PCM_Source_GetSampleValue), "double", "PCM_source*,double", "source,time", "Gets a PCM_source's sample value at a point in time (seconds)"});
	Api.Add({APIFUNC(GU_PCM_Source_TimeToPeak), "double", "PCM_source*,int,double", "source,bufferSize,threshold", "Returns duration in seconds for PCM_source from start til peak threshold is breached. Returns -1 if invalid"});
	Api.Add({APIFUNC(GU_PCM_Source_TimeToRMS), "double", "PCM_source*,int,double", "source,bufferSize,threshold", "Returns duration in seconds for PCM_source from start til RMS threshold is breached. Returns -1 if invalid"});
	Api.Add({APIFUNC(GU_PCM_Source_TimeToPeakR), "double", "PCM_source*,int,double", "source,bufferSize,threshold", "Returns duration in seconds for PCM_source from end til peak threshold is breached in reverse. Returns -1 if invalid"});
	Api.Add({APIFUNC(GU_PCM_Source_TimeToRMSR), "double", "PCM_source*,int,double", "source,bufferSize,threshold", "Returns duration in seconds for PCM_source from end til RMS threshold is breached in reverse. Returns -1 if invalid"});
	Api.Add({APIFUNC(GU_PCM_Source_HasRegion), "bool", "PCM_source*", "source", "Checks if PCM_source has embedded Media Cue Markers"});
	Api.Add({APIFUNC(GU_WildcardParseTake), "void", "MediaItem_Take*,const char*,char*,int", "take,input,valueOut,valueOut_sz", "Returns a string by parsing wildcards relative to the supplied MediaItem_Take"});
	Api.Add({APIFUNC(GU_Filesystem_CountMediaFiles), "int", "const char*,int,double*", "path,flags,fileSizeOut", "Returns count and filesize in megabytes for all valid media files within the path. Returns -1 if path is invalid. Flags can be passed as an argument to determine which media files are valid. A flag with a value of -1 will reset the cache, otherwise, the following flags can be used: ALL = 0, WAV = 1, AIFF = 2, FLAC = 4, MP3 = 8, OGG = 16, BWF = 32, W64 = 64, WAVPACK = 128, GIF = 256, MP4 = 512"});
	Api.Add({APIFUNC(GU_Filesystem_EnumerateMediaFiles), "void", "const char*,int,char*,int", "path,flags,pathOut,pathOut_sz", "Returns the next valid file in a directory each time this function is called with the same path. Returns an empty string if path does not contain any more valid files. Flags can be passed as an argument to determine which media files are valid. A flag with a value of -1 will reset the cache, otherwise, the following flags can be used: ALL = 0, WAV = 1, AIFF = 2, FLAC = 4, MP3 = 8, OGG = 16, BWF = 32, W64 = 64, WAVPACK = 128, GIF = 256, MP4 = 512"});
	Api.Add({APIFUNC(GU_Filesystem_FindFileInPath), "void", "const char*,const char*,char*,int", "path,fileName,pathOut,pathOut_sz", "Returns the first found file's path from within a given path. Returns an empty string if not found"});
	Api.Add({APIFUNC(GU_Filesystem_PathExists), "bool", "const char*", "path", "Checks if file or directory exists"});
	Api.Add({APIFUNC(GU_PCM_Source_AnalyzePitch), "double", "PCM_source*,double,double,int,int", "source,timeStart,timeEnd,windowSize,overlap", "Analyse pitch of PCM_source for a given start and end time"});
	Api.Register();
}

extern "C"
{
	REAPER_PLUGIN_DLL_EXPORT int REAPER_PLUGIN_ENTRYPOINT(REAPER_PLUGIN_HINSTANCE hInstance [[maybe_unused]], reaper_plugin_info_t* rec)
	{
		if (rec && REAPERAPI_LoadAPI(rec->GetFunc) == 0)
		{
			INIFile ini{"GUtilities"};
#ifdef _DEBUG
			ini.Write("debug", "enabled", "true");
#else
			ini.Write("debug", "enabled", "false");
#endif
			LoadPlugin();
			return 1;
		}
		else
		{
			Api.Unregister();
			return 0;
		}
	}
}

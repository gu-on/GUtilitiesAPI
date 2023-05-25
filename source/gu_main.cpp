#define REAPERAPI_IMPLEMENT
#include <reaper_plugin_functions.h>

#include <api.hpp>
#include <api_cfg.hpp>

API Api{};

int LoadPlugin()
{
	Api.Add({APIFUNC(GU_ConfigFileWrite), "bool", "const char*,const char*,const char*,const char*", "filePath,category,key,value", "write file"});
	Api.Add({APIFUNC(GU_ConfigFileRead), "bool", "const char*,const char*,const char*,char*,int", "filePath,category,key,valueOut,valueOut_sz", "read file"});
	Api.Add({APIFUNC(GU_IsMono), "bool", "PCM_source*", "source", "Checks if PCM_source is mono"});
	Api.Add({APIFUNC(GU_IsFirstSampleZero), "bool", "PCM_source*", "source", "Checks if first sample in PCM_source is 0"});
	Api.Add({APIFUNC(GU_IsLastSampleZero), "bool", "PCM_source*", "source", "Checks if last sample in PCM_source is 0"});
	Api.Add({APIFUNC(GU_CountSamplesTilPeak), "int", "PCM_source*,int,double", "source,bufferSize,threshold", "Count number of samples in PCM_source from start til peak threshold is breached. Returns -1 if invalid"});
	Api.Add({APIFUNC(GU_CountSamplesTilRMS), "int", "PCM_source*,int,double", "source,bufferSize,threshold", "Count number of samples in PCM_source from start til RMS threshold is breached. Returns -1 if invalid"});
	Api.Add({APIFUNC(GU_CountSamplesTilPeakR), "int", "PCM_source*,int,double", "source,bufferSize,threshold", "Count number of samples in PCM_source from end til peak threshold is breached. Returns -1 if invalid"});
	Api.Add({APIFUNC(GU_CountSamplesTilRMSR), "int", "PCM_source*,int,double", "source,bufferSize,threshold", "Count number of samples in PCM_source from end til RMS threshold is breached. Returns -1 if invalid"});
	Api.Add({APIFUNC(GU_HasRegion), "bool", "PCM_source*", "source", "Check if PCM_source has embedded Media Cue Markers"});
	Api.Add({APIFUNC(GU_CountMediaFilesRecursive), "int", "const char*,int,double*", "filePath,flags,fileSizeOut", "Recursively count all media files within a given folder. Returns -1 if filePath is invalid. Can also retrieve fileSize sum in megabytes"});
	Api.Add({APIFUNC(GU_EnumerateMediaFilesRecursive), "const char*", "const char*,int", "path,flags", "Import media file from filePath at index"});
	Api.Add({APIFUNC(GU_TakeWildcardRename), "void", "MediaItem_Take*,const char*", "take,input", "Renames take based on wildcards"});
	Api.Register();

	return 1;
}

int UnloadPlugin()
{
	Api.Unregister();

	return 0;
}

extern "C"
{
	REAPER_PLUGIN_DLL_EXPORT int REAPER_PLUGIN_ENTRYPOINT(REAPER_PLUGIN_HINSTANCE hInstance [[maybe_unused]],
														  reaper_plugin_info_t* rec)
	{
		if (rec && REAPERAPI_LoadAPI(rec->GetFunc) == 0)
		{
			return LoadPlugin();
		}
		else
		{
			return UnloadPlugin();
		}
	}
}

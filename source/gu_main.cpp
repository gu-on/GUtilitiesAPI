#define REAPERAPI_IMPLEMENT
#include <reaper_plugin_functions.h>

#include <api_manager.hpp>
#include <gu_api.hpp>
#include <gu_ini_file.hpp>

API Api{};

void LoadPlugin()
{
	Api.Add({APIFUNC(GU_Config_Write), "bool", "const char*,const char*,const char*,const char*", "fileName,category,key,value", "Write config file to Reaper's resource folder"});
	Api.Add({APIFUNC(GU_Config_Read), "bool", "const char*,const char*,const char*,char*,int", "fileName,category,key,valueOut,valueOut_sz", "Read config file from Reaper's resource folder"});
	Api.Add({APIFUNC(GU_PCM_Source_IsMono), "bool", "PCM_source*", "source", "Checks if PCM_source is mono by comparing all channels"});
	Api.Add({APIFUNC(GU_PCM_Source_GetSampleValue), "double", "PCM_source*,double", "source,time", "Get a PCM_source's sample value at a point in time (seconds)"});
	Api.Add({APIFUNC(GU_PCM_Source_TimeToPeak), "double", "PCM_source*,int,double", "source,bufferSize,threshold", "Count number of samples in PCM_source from start til peak threshold is breached. Returns -1 if invalid"});
	Api.Add({APIFUNC(GU_PCM_Source_TimeToRMS), "double", "PCM_source*,int,double", "source,bufferSize,threshold", "Count number of samples in PCM_source from start til RMS threshold is breached. Returns -1 if invalid"});
	Api.Add({APIFUNC(GU_PCM_Source_TimeToPeakR), "double", "PCM_source*,int,double", "source,bufferSize,threshold", "Count number of samples in PCM_source from end til peak threshold is breached. Returns -1 if invalid"});
	Api.Add({APIFUNC(GU_PCM_Source_TimeToRMSR), "double", "PCM_source*,int,double", "source,bufferSize,threshold", "Count number of samples in PCM_source from end til RMS threshold is breached. Returns -1 if invalid"});
	Api.Add({APIFUNC(GU_PCM_Source_HasRegion), "bool", "PCM_source*", "source", "Check if PCM_source has embedded Media Cue Markers"});
	Api.Add({APIFUNC(GU_WildcardParseTake), "void", "MediaItem_Take*,const char*,char*,int", "take,input,valueOut,valueOut_sz", "Renames take based on wildcards"});
	Api.Add({APIFUNC(GU_Filesystem_CountMediaFiles), "int", "const char*,int,double*", "path,flags,fileSizeOut", "Recursively count all media files within a given folder. Returns -1 if path is invalid. Can also retrieve fileSize sum in megabytes"});
	Api.Add({APIFUNC(GU_Filesystem_EnumerateMediaFiles), "void", "const char*,int,char*,int", "path,flags,pathOut,pathOut_sz", "Import media file from path at index"});
	Api.Add({APIFUNC(GU_Filesystem_FindFileInPath), "void", "const char*,const char*,char*,int", "path,fileName,pathOut,pathOut_sz", "Returns the first found file's path from within a given path, or an empty string if not found"});
	Api.Add({APIFUNC(GU_Filesystem_PathExists), "bool", "const char*", "path", "Check if file or directory exists"});
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

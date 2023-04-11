#include <reaper_plugin_functions.h>

#include <api.hpp>
#include <ini_file.hpp>
#include <managed_object.hpp>

void API::Add(structAPIdef apiDef)
{
	apiDefinitions.push_back(apiDef);
}

void API::Unregister()
{
	char temp[1 << 14];
	for (auto& f : apiDefinitions)
	{
		snprintf(temp, sizeof(temp), "-%s", f.regkey_func);
		plugin_register(temp, f.func);
	}
}

void API::Register()
{
	// Each function's defstring will temporarily be contructed in temp[]
	char temp[1 << 14];

	for (auto& f : apiDefinitions)
	{
		// REAPER uses a \0-separated string. sprintf cannot print \0, so must
		// temporarily print \r and replace later.
		snprintf(temp, 10000, "%s\n%s\n%s\n%s", f.ret_val, f.parm_types, f.parm_names, f.help);
		// Create permanent copy of temp string, so that REAPER can access it
		// later again.
		f.defstring = _strdup(temp);
		// Replace the three \n with \0.
		int i = 0;
		int countZeroes = 0;
		while (countZeroes < 3)
		{
			if (f.defstring[i] == '\n')
			{
				f.defstring[i] = 0;
				countZeroes++;
			}
			i++;
		}
		// Each function must be registered in three ways:
		// APIdef_... provides for converting parameters to vararg format, and
		// for documentation in the auto-generated REAPER API header and
		// ReaScript documentation.
		plugin_register(f.regkey_def, (void*)f.defstring);
		// API_... for exposing to other extensions, and for IDE to recognize
		// and color functions while typing .
		plugin_register(f.regkey_func, f.func);
		// APIvarag_... for exporting to ReaScript API.
		plugin_register(f.regkey_vararg, f.func_vararg);
	}
}
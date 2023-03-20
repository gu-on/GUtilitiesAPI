#include <api_creator.hpp>
#include <reaper_plugin_functions.h>
#include <stdio.h>

int TEST_Adder(int x, int y)
{
	return x + y;
}

int TEST_Multiplier(int x, int y)
{
	return x * y;
}

// Array of function info structs
structAPIdef arrayAPIdefs[] = {
	{APIFUNC(TEST_Adder), "int", "int,int", "x,y", "Add x to y"},
	{APIFUNC(TEST_Multiplier), "int", "int,int", "x,y", "Multiply x by y"},
};

void APICreator::Register()
{
	// Each function's defstring will temporarily be contructed in temp[]
	char temp[1 << 14];

	for (structAPIdef& f : arrayAPIdefs)
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
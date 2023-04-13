#define _CRT_NONSTDC_NO_DEPRECATE

#include <reaper_plugin_functions.h>

#include <api.hpp>
#include <ini_file.hpp>

#include <cstring>
#include <format>

void API::Add(structAPIdef apiDef)
{
	apiDefinitions.push_back(apiDef);
}

void API::Unregister()
{
	for (auto& def : apiDefinitions)
	{
		std::string str = std::format("-{}", def.regkey_func);
		plugin_register(str.c_str(), def.func);

		free(def.defstring);
	}

	apiDefinitions.clear();
}

void API::Register()
{
	for (auto& def : apiDefinitions)
	{
		std::string str = std::format("{}\n{}\n{}\n{}", def.ret_val, def.parm_types, def.parm_names, def.help);

		def.defstring = strdup(str.data());

		for (size_t i = 0; i < str.size(); i++)
		{
			if (def.defstring[i] == '\n')
				def.defstring[i] = 0;
		}

		plugin_register(def.regkey_def, (void*)def.defstring);
		plugin_register(def.regkey_func, def.func);
		plugin_register(def.regkey_vararg, def.func_vararg);
	}
}
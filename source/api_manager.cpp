#include <reaper_plugin_functions.h>

#include <api_manager.hpp>
#include <gu_ini_file.hpp>

#include <algorithm>
#include <cstring>
#include <fmt/core.h>

void API::Add(APIDefinition def)
{
	apiDefinitions.push_back(def);
}

void API::Unregister()
{
	for (auto& def : apiDefinitions)
	{
		std::string str = fmt::format("-{}", def.regkey_func);
		plugin_register(str.c_str(), def.func);
	}

	apiDefinitions.clear();
}

void API::Register()
{
	for (auto& def : apiDefinitions)
	{
		std::string str = fmt::format("{}\n{}\n{}\n{}", def.ret_val, def.parm_types, def.parm_names, def.help);

		def.defstring.assign(str.begin(), str.end());
		def.defstring.push_back('\0');
		std::replace(def.defstring.begin(), def.defstring.end(), '\n', '\0');

		plugin_register(def.regkey_def, (void*)def.defstring.data());
		plugin_register(def.regkey_func, def.func);
		plugin_register(def.regkey_vararg, def.func_vararg);
	}
}
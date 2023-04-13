#pragma once

#include "reascript_vararg.hpp"

#include <vector>

// https://forums.cockos.com/showthread.php?p=2028399
// (This struct is copied from SWS.)
// Struct to store info such as function name and help text for each function that the extensions intends to expose as
// API. This info will be used by REAPER's plugin_register functions to register the functions. NOTE: REAPER requires
// the return values, paramaters and help text to be one long \0-separated string.
//		The parm_names, parm_types and help fields will therefore have to be concatenated before registration, and
// stored in the defstring field.
//      If compatilibity with SWS is not an issue, I would remove parm_names, parm_types and help, and simply define one
//      \0-separated string from the start.
struct structAPIdef
{
	void* func; // pointer to the function that other extensions use
	const char* func_name;
	void* func_vararg;		   // pointer to the wrapper function that ReaScript API calls
	const char* regkey_vararg; // "APIvararg_funcName" - for
	const char* regkey_func;   // "API_funcName"
	const char* regkey_def;	   // "APIdef_funcName"
	const char* ret_val;	   // return value type, as string
	const char* parm_types;
	const char* parm_names;
	const char* help;
	char* defstring; // \0-separated string for APIdef... Will be concatenated and assigned while registering function
};

// (This macro is copied from SWS.)
// Macro to construct a comma-separated list of all the variants of a function name that are required for
// plugin_register(), in the order required by structAPIdef in which these variants are stored. APIFUNC(funcName)
// becomes (void*)funcName, "funcName", (void*)__vararg_funcName, "APIvararg_funcName", "API_funcName",
// "APIdef_funcName"
#define APIFUNC(x)                                                                                                     \
	(void*)x, #x, reinterpret_cast<void*>(&InvokeReaScriptAPI<&x>), "APIvararg_" #x "", "API_" #x "", "APIdef_" #x ""

class API
{
private:
	std::vector<structAPIdef> apiDefinitions{};

public:
	void Add(structAPIdef apiDef);
	void Unregister();
	void Register();
	
};
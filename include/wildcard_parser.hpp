#pragma once

#include <reaper_plugin_functions.h>
#include <reaper_wrapper.hpp>

#include <string>

class WildcardParser
{
public:
	void RenameTake(MediaItem_Take* take, std::string input);
};
#pragma once

#include <gu_reaper_wrapper.hpp>
#include <reaper_plugin_functions.h>

#include <string>

class WildcardParser
{
public:
	void RenameTake(MediaItem_Take* take, std::string input);
};
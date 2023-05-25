#pragma once

#include <gu_reaper_wrapper.hpp>
#include <reaper_plugin_functions.h>

#include <string>

class WildcardParser
{
public:
	std::string ParseTakeName(MediaItem_Take* take, std::string input);
};
#pragma once

#include <string>

#include <WDL/wdltypes.h> // might be unnecessary in future

#include <reaper_plugin_functions.h>

#include "gu_reaper_wrapper.hpp"

class WildcardParser
{
private:
	struct WildcardInfo
	{
		Take* take;
		Item* item;
		Track* track;
		AudioSource* source;
		Project* project;

		WildcardInfo(Take& t, Item& i, Track& tr, AudioSource& src, Project& proj)
		{
			take = &t;
			item = &i;
			track = &tr;
			source = &src;
			project = &proj;
		}
	};

public:
	std::string ParseTakeName(MediaItem_Take* take, std::string input);

	void ParseWildcardsBracketsInt(std::string& input, WildcardInfo& info);
	void ParseWildcardsBracketsString(std::string& input, WildcardInfo& info);
	void ParseWildcardsPlain(std::string& input, WildcardInfo& info);
};
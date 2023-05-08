#pragma once

#include <reaper_plugin_functions.h>

#include <string>

class WildcardParser
{
public:
	void RenameTake(MediaItem_Take* take, std::string input);

private:
	static constexpr ReaProject* THIS_PROJ = nullptr;
	static constexpr int CHAR_STRING_SIZE = 1 << 12;
	char OutString[CHAR_STRING_SIZE];

	std::string PrintProjectName();
	std::string PrintProjectAuthor();
	std::string PrintOutermostTrackParentName(MediaTrack* track);
	std::string PrintTrackName(MediaTrack* track);
	std::string PrintTrackNumber(MediaTrack* track);
	std::string PrintItemNotes(MediaItem* item);
	std::string PrintSelectedItemCount();
	std::string PrintActiveTakeName(MediaItem_Take* take);
};
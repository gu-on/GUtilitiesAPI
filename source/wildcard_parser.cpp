#include <wildcard_parser.hpp>

#include <audio_source.hpp>
#include <time_printer.hpp>

#include <chrono>
#include <format>
#include <functional>
#include <map>
#include <string>

void WildcardParser::RenameTake(MediaItem_Take* take, std::string input)
{
	std::string::size_type pos;

	MediaItem* item = GetMediaItemTake_Item(take);
	MediaTrack* track = GetMediaItemTake_Track(take);
	PCM_source* pcm_source = GetMediaItemTake_Source(take);
	AudioSource source{pcm_source};

	// clang-format off
	std::vector<std::pair<std::string, std::function<std::string()>>> wildcardReplacements =
	{
		{"$year2", [&] { return TimePrinter::PrintYear2(); }},
		{"$year", [&] { return TimePrinter::PrintYear(); }},
		{"$tracktop", [&] { return PrintOutermostTrackParentName(track); }},
		{"$tracknumber", [&] { return PrintTrackNumber(track); }},
		{"$track", [&] { return PrintTrackName(track); }},
		{"$timesig", [&] { return ""; }},
		{"$time", [&] { return TimePrinter::PrintTime(); }},
		{"$tempo", [&] { return ""; }},
		{"$second", [&] { return TimePrinter::PrintSeconds(); }},
		{"$rms", [&] { return std::format("{:.1f}", source.GetRMS()); }},
		{"$region", [&] { return ""; }},
		{"$project", [&] { return PrintProjectName(); }},
		{"$peak", [&] { return std::format("{:.1f}", source.GetPeak()); }},
		{"$monthname", [&] { return TimePrinter::PrintMonthName(); }},
		{"$month", [&] { return TimePrinter::PrintMonth(); }},
		{"$minute", [&] { return TimePrinter::PrintMinutes(); }},
		{"$marker", [&] { return ""; }},
		{"$lufs", [&] { return std::format("{:.1f}", source.GetLUFS()); }},
		{"$itemnumberontrack", [&] { return ""; }},
		{"$itemnumber", [&] { return ""; }},
		{"$itemnotes", [&] { return PrintItemNotes(item); }},
		{"$itemcount", [&] { return PrintSelectedItemCount(); }},
		{"$item", [&] { return PrintActiveTakeName(take); }},
		{"$hour12", [&] { return TimePrinter::PrintHours12(); }},
		{"$hour", [&] { return TimePrinter::PrintHours(); }},
		{"$fx", [&] { return ""; }},
		{"$dayname", [&] { return TimePrinter::PrintDayName(); }},
		{"$day", [&] { return TimePrinter::PrintDay(); }},
		{"$date", [&] { return TimePrinter::PrintDate(); }},
		{"$author", [&] { return PrintProjectAuthor(); }},
		{"$ampm", [&] { return TimePrinter::PrintAmPm(); }},
	};
	// clang-format on

	for (const auto& [wildcard, func] : wildcardReplacements)
	{
		while ((pos = input.find(wildcard)) != std::string::npos)
		{
			input.replace(pos, wildcard.length(), func());
		}
	}

	GetSetMediaItemTakeInfo_String(take, "P_NAME", const_cast<char*>(input.c_str()), true);
}

std::string WildcardParser::PrintProjectName()
{
	GetProjectName(THIS_PROJ, OutString, CHAR_STRING_SIZE);
	return OutString;
}

std::string WildcardParser::PrintProjectAuthor()
{
	GetSetProjectAuthor(THIS_PROJ, false, OutString, CHAR_STRING_SIZE);
	return OutString;
}

std::string WildcardParser::PrintOutermostTrackParentName(MediaTrack* track)
{
	MediaTrack* parent = track;

	while (GetParentTrack(parent))
		parent = GetParentTrack(parent);

	return PrintTrackName(parent);
}

std::string WildcardParser::PrintTrackName(MediaTrack* track)
{
	GetSetMediaTrackInfo_String(track, "P_NAME", OutString, false);
	return OutString;
}

std::string WildcardParser::PrintTrackNumber(MediaTrack* track)
{
	return std::to_string(GetMediaTrackInfo_Value(track, "IP_TRACKNUMBER"));
}

std::string WildcardParser::PrintItemNotes(MediaItem* item)
{
	GetSetMediaItemInfo_String(item, "P_NOTES", OutString, false);
	return OutString;
}

std::string WildcardParser::PrintSelectedItemCount()
{
	int count = CountSelectedMediaItems(THIS_PROJ);
	return std::to_string(count);
}

std::string WildcardParser::PrintActiveTakeName(MediaItem_Take* take)
{
	GetSetMediaItemTakeInfo_String(take, "P_NAME", OutString, false);
	return OutString;
}

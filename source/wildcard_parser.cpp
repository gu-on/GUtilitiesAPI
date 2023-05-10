#include <wildcard_parser.hpp>

#include <audio_source.hpp>
#include <time_printer.hpp>

#include <chrono>
#include <fmt/core.h>
#include <functional>
#include <map>
#include <string>

void WildcardParser::RenameTake(MediaItem_Take* takePtr, std::string input)
{
	Take take{takePtr};
	Item item = take.GetItem();
	Track track = take.GetTrack();
	AudioSource source = take.GetSource();
	Project project{};

	static int currentState{};
	currentState = project.GetState();

	static int itemCount{};
	static int itemNumber{};
	static int itemNumberOnTrack{};
	static MediaItem_Take* lastTakePtr{};

	if (static int LastState; LastState != currentState)
	{
		LastState = currentState;
		itemNumber = 0;
		itemNumberOnTrack = 0;
		lastTakePtr = nullptr;
	}

	++itemNumber;

	if (lastTakePtr)
	{
		Take lastTake{lastTakePtr};
		Track lastTrack = lastTake.GetTrack();

		if (lastTrack != track)
		{
			itemNumberOnTrack = 0;
		}
	}

	lastTakePtr = takePtr;
	++itemNumberOnTrack;

	// get project state

	// clang-format off
	std::vector<std::pair<std::string, std::function<std::string()>>> wildcardReplacements =
	{
		{"$year2", [&] { return TimePrinter::PrintYear2(); }},
		{"$year", [&] { return TimePrinter::PrintYear(); }},
		{"$tracktop", [&] { return std::get<0>(track.GetOutermostAncestor()).GetName(); }},
		{"$tracknumber", [&] { return std::to_string(track.GetNumber()); }},
		{"$track", [&] { return track.GetName(); }},
		{"$timesig", [&] { return project.GetTimeSig(item.GetPosition()); }},
		{"$time", [&] { return TimePrinter::PrintTime(); }},
		{"$tempo", [&] { return std::to_string(project.GetTempo(item.GetPosition())); }},
		{"$second", [&] { return TimePrinter::PrintSeconds(); }},
		{"$rms", [&] { return fmt::format("{:.1f}", source.GetRMS()); }},
		{"$region", [&] { return project.GetRegionName(item.GetPosition()); }},
		{"$project", [&] { return project.GetName(); }},
		{"$peak", [&] { return fmt::format("{:.1f}", source.GetPeak()); }},
		{"$monthname", [&] { return TimePrinter::PrintMonthName(); }},
		{"$month", [&] { return TimePrinter::PrintMonth(); }},
		{"$minute", [&] { return TimePrinter::PrintMinutes(); }},
		{"$marker", [&] { return project.GetMarkerName(item.GetPosition()); }},
		{"$lufs", [&] { return fmt::format("{:.1f}", source.GetLUFS()); }},
		{"$itemnumberontrack", [&] { return std::to_string(itemNumberOnTrack); }},
		{"$itemnumber", [&] { return std::to_string(itemNumber); }},
		{"$itemnotes", [&] { return item.GetNotes(); }},
		{"$itemcount", [&] { return std::to_string(project.CountSelectedItems()); }},
		{"$item", [&] { return take.GetName(); }},
		{"$hour12", [&] { return TimePrinter::PrintHours12(); }},
		{"$hour", [&] { return TimePrinter::PrintHours(); }},
		{"$fx", [&] { return take.GetFXNames(); }},
		{"$dayname", [&] { return TimePrinter::PrintDayName(); }},
		{"$day", [&] { return TimePrinter::PrintDay(); }},
		{"$date", [&] { return TimePrinter::PrintDate(); }},
		{"$author", [&] { return project.GetAuthor(); }},
		{"$ampm", [&] { return TimePrinter::PrintAmPm(); }},
	};
	// clang-format on

	std::string::size_type pos;

	for (const auto& [wildcard, func] : wildcardReplacements)
	{
		while ((pos = input.find(wildcard)) != std::string::npos)
		{
			auto temp = func();
			auto totalLength = pos + temp.length();

			if (totalLength > REAPER_NAMES_MAX_LENGTH)
			{
				// todo: return failure condition
				return;
			}

			input.replace(pos, wildcard.length(), temp);
		}
	}

	take.SetName(input);
}

void WildcardParser::RenameItem(MediaItem* item, std::string input)
{
}

#include <gu_wildcard_parser.hpp>

#include <gu_audio_source.hpp>
#include <gu_maths.hpp>
#include <gu_time_printer.hpp>

#include <chrono>
#include <fmt/core.h>
#include <functional>
#include <map>
#include <string>

std::string WildcardParser::ParseTakeName(MediaItem_Take* takePtr, std::string input)
{
	Take take{takePtr};
	Item item = take.GetItem();
	Track track = take.GetTrack();
	AudioSource source = take.GetSource();
	Project project{};

	WildcardInfo info{take, item, track, source, project};

	ParseWildcardsBracketsInt(input, info);
	ParseWildcardsBracketsString(input, info);
	ParseWildcardsPlain(input, info);

	return input;
}

void WildcardParser::ParseWildcardsBracketsInt(std::string& input, WildcardInfo& info)
{
	// clang-format off
	std::vector<std::pair<std::string, std::function<std::string(int)>>> wildcardReplacements = 
	{
		{"$track(", [&](const int i) { return info.track->GetTrackAbove(i).GetName(); }},
	};
	// clang-format on

	std::string::size_type findPos;

	for (const auto& [wildcard, func] : wildcardReplacements)
	{
		while ((findPos = input.find(wildcard)) != std::string::npos)
		{
			std::string::size_type endBracket = input.find(")", findPos);

			if (endBracket == std::string::npos)
				break;

			auto startPos = findPos + wildcard.length();
			std::string selector = input.substr(startPos, endBracket - startPos);

			if (Maths::IsNumber(selector))
			{
				input.replace(findPos, endBracket - findPos + 1, func(std::stoi(selector)));
				continue;
			}

			input.replace(findPos, endBracket - findPos + 1, "");
		}
	}
}

void WildcardParser::ParseWildcardsBracketsString(std::string& input, WildcardInfo& info)
{
	// clang-format off
	std::vector<std::pair<std::string, std::function<std::string(std::string)>>> wildcardReplacements =
	{
		{"$fx(", [&](const std::string s) { return info.take->GetFXNames(s); }},
		{"$region(", [&](const std::string s) { return info.project->GetRegionNameByKey(s, info.item->GetPosition()); }},
	};
	// clang-format on

	std::string::size_type findPos;

	for (const auto& [wildcard, func] : wildcardReplacements)
	{
		while ((findPos = input.find(wildcard)) != std::string::npos)
		{
			std::string::size_type endBracket = input.find(")", findPos);

			if (endBracket == std::string::npos)
				break;

			auto startPos = findPos + wildcard.length();

			std::string selector = input.substr(startPos, endBracket - startPos);
			std::string replacement{func(selector)};

			input.replace(findPos, endBracket - findPos + 1, replacement);
		}
	}
}

void WildcardParser::ParseWildcardsPlain(std::string& input, WildcardInfo& info)
{
	// clang-format off
	std::vector<std::pair<std::string, std::function<std::string()>>> wildcardReplacements =
	{
		{"$year2", [&] { return TimePrinter::PrintYear2(); }},
		{"$year", [&] { return TimePrinter::PrintYear(); }},
		{"$tracktop", [&] { return std::get<0>(info.track->GetOutermostAncestor()).GetName(); }},
		{"$tracknumber", [&] { return std::to_string(info.track->GetNumber()); }},
		{"$track", [&] { return info.track->GetName(); }},
		{"$timesignature", [&] { return info.project->GetTimeSig(info.item->GetPosition()); }},
		{"$time", [&] { return TimePrinter::PrintTime(); }},
		{"$tempo", [&] { return std::to_string(info.project->GetTempo(info.item->GetPosition())); }},
		{"$second", [&] { return TimePrinter::PrintSeconds(); }},
		{"$rms", [&] { return fmt::format("{:.1f}", info.source->GetRMS()); }},
		{"$region", [&] { return info.project->GetRegionName(info.item->GetPosition()); }},
		{"$project", [&] { return info.project->GetName(); }},
		{"$peak", [&] { return fmt::format("{:.1f}", info.source->GetPeak()); }},
		{"$monthname", [&] { return TimePrinter::PrintMonthName(); }},
		{"$month", [&] { return TimePrinter::PrintMonth(); }},
		{"$minute", [&] { return TimePrinter::PrintMinutes(); }},
		{"$marker", [&] { return info.project->GetMarkerName(info.item->GetPosition()); }},
		{"$lufs", [&] { return fmt::format("{:.1f}", info.source->GetLUFS()); }},
		{"$itemnotes", [&] { return info.item->GetNotes(); }},
		{"$item", [&] { return info.take->GetName(); }},
		{"$hour12", [&] { return TimePrinter::PrintHours12(); }},
		{"$hour", [&] { return TimePrinter::PrintHours(); }},
		{"$fx", [&] { return info.take->GetFXNames(); }},
		{"$dayname", [&] { return TimePrinter::PrintDayName(); }},
		{"$day", [&] { return TimePrinter::PrintDay(); }},
		{"$date", [&] { return TimePrinter::PrintDate(); }},
		{"$author", [&] { return info.project->GetAuthor(); }},
		{"$ampm", [&] { return TimePrinter::PrintAmPm(); }},
	};
	// clang-format on

	std::string::size_type findPos;

	for (const auto& [wildcard, func] : wildcardReplacements)
	{
		while ((findPos = input.find(wildcard)) != std::string::npos)
		{
			auto replacementString = func();
			auto totalLength = findPos + replacementString.length();

			if (totalLength > REAPER_NAMES_MAX_LENGTH)
			{
				// todo: return failure condition
				input.clear();
				return;
			}

			input.replace(findPos, wildcard.length(), replacementString);
		}
	}
}

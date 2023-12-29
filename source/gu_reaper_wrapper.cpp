#include <cassert>

#include <fmt/core.h>

#include <gu_reaper_wrapper.hpp>

std::string Track::GetName() const
{
	char outString[REAPER_NAMES_MAX_LENGTH];
	GetSetMediaTrackInfo_String(Ptr, "P_NAME", outString, false);
	return outString;
}

std::tuple<Track, int> Track::GetOutermostAncestor() const
{
	MediaTrack* track = Ptr;
	int depth = 0;

	while (GetParentTrack(track))
	{
		track = GetParentTrack(track);
		++depth;
	}

	return std::make_tuple(Track{track}, depth);
}

Track Track::GetTrackAbove(int level) const
{
	const int maxDepth = std::get<1>(this->GetOutermostAncestor());
	MediaTrack* track = Ptr;

	while (GetTrackDepth(track) != maxDepth - level && GetParentTrack(track))
		track = GetParentTrack(track);

	return Track{track};
}

int Track::GetDepth() const
{
	return GetTrackDepth(Ptr);
}

int Track::GetNumber() const
{
	return static_cast<int>(GetMediaTrackInfo_Value(Ptr, "IP_TRACKNUMBER"));
}

void Track::SetName(const std::string& name) const
{
	GetSetMediaTrackInfo_String(Ptr, "P_NAME", const_cast<char*>(name.c_str()), true);
}

void Track::SetDepth(const int depth) const
{
	SetMediaTrackInfo_Value(Ptr, "I_FOLDERDEPTH", depth);
}

Item Track::CreateNewItem(const std::string& filePath, const double position)
{
	Item item{AddMediaItemToTrack(Ptr)};
	auto take = item.CreateNewTake(filePath);
	item.SetPosition(position);
	item.SetLength(take.GetSourceLength());

	return item;
}

Track::Track(MediaTrack* track) : Ptr((assert(track != nullptr), track)) {}

std::string Item::GetNotes() const
{
	char out[REAPER_NOTES_MAX_LENGTH];
	GetSetMediaItemInfo_String(Ptr, "P_NOTES", out, false);
	return out;
}

double Item::GetPosition() const
{
	return GetMediaItemInfo_Value(Ptr, "D_POSITION");
}

Take Item::CreateNewTake(const std::string& filePath) const
{
	Take take(AddTakeToMediaItem(Ptr));
	take.SetAudioSource(filePath);

	const std::string fileName{Project::ExtractFileName(filePath)};
	take.SetName(fileName);

	return take;
}

void Item::SetPosition(double position) const
{
	SetMediaItemInfo_Value(Ptr, "D_POSITION", position);
}

void Item::SetLength(double length) const
{
	SetMediaItemInfo_Value(Ptr, "D_LENGTH", length);
}

Item::Item(MediaItem* item) : Ptr((assert(item != nullptr), item)) {}

PCM_source* Take::SetAudioSource(const std::string& filePath)
{
	return PCM_Source_CreateFromFile(filePath.c_str());
}

void Take::SetName(const std::string& name)
{
	GetSetMediaItemTakeInfo_String(Ptr, "P_NAME", const_cast<char*>(name.c_str()), true);
}

std::string Take::GetName()
{
	char out[REAPER_NAMES_MAX_LENGTH];
	GetSetMediaItemTakeInfo_String(Ptr, "P_NAME", out, false);
	return out;
}

double Take::GetSourceLength()
{
	auto source = GetMediaItemTake_Source(Ptr);

	if (source == nullptr)
		return 0.0;

	bool* lengthIsQNOut{};
	return GetMediaSourceLength(source, lengthIsQNOut);
}

std::string Take::GetFXNames(const std::string& separator)
{
	std::string output{};
	int count = TakeFX_GetCount(Ptr);

	for (int i = 0; i < count; ++i)
	{
		char fx[REAPER_NAMES_MAX_LENGTH];
		bool valid = TakeFX_GetFXName(Ptr, i, fx, REAPER_NAMES_MAX_LENGTH);

		if (!valid)
			continue;

		std::string fxName{fx};
		const auto start = fxName.find(':');
		const auto end = fxName.find('(');
		output += fxName.substr(start + 2, end - start - 3);

		if (i < count - 1)
			output += separator;
	}

	return output;
}

Track Take::GetTrack()
{
	return Track{GetMediaItemTake_Track(Ptr)};
}

Item Take::GetItem()
{
	return Item{GetMediaItemTake_Item(Ptr)};
}

AudioSource Take::GetSource()
{
	return AudioSource{GetMediaItemTake_Source(Ptr)};
}

Take::Take(MediaItem_Take* take) : Ptr((assert(take != nullptr), take)) {}

int Project::GetState() const
{
	return GetProjectStateChangeCount(Ptr);
}

std::string Project::GetName() const
{
	char outString[REAPER_NAMES_MAX_LENGTH];
	GetProjectName(Ptr, outString, REAPER_NAMES_MAX_LENGTH);
	return outString;
}

std::string Project::GetAuthor() const
{
	char outString[REAPER_NAMES_MAX_LENGTH];
	GetSetProjectAuthor(Ptr, false, outString, REAPER_NAMES_MAX_LENGTH);
	return outString;
}

std::string Project::ExtractFileName(const std::string& filePath)
{
	constexpr auto delimiter = "/\\";
	const auto pos = filePath.find_last_of(delimiter) + 1;
	return filePath.substr(pos, filePath.length() - pos);
}

std::string Project::GetRegionName(const double timelinePos)
{
	int regionIndex;
	GetLastMarkerAndCurRegion(Ptr, timelinePos, nullptr, &regionIndex);

	if (regionIndex < 0)
		return "";

	bool isRegion;
	const char* regionName;
	EnumProjectMarkers(regionIndex, &isRegion, nullptr, nullptr, &regionName, nullptr);

	if (strlen(regionName) <= 0 || !isRegion)
		return "";

	return regionName;
}

std::string Project::GetMarkerName(const double timelinePos)
{
	int markerIndex;
	GetLastMarkerAndCurRegion(Ptr, timelinePos, &markerIndex, nullptr);

	if (markerIndex < 0)
		return "";

	const char* markerName;
	EnumProjectMarkers(markerIndex, nullptr, nullptr, nullptr, &markerName, nullptr);

	if (strlen(markerName) <= 0)
		return "";

	return markerName;
}

int Project::GetTempo(const double timelinePos) const
{
	return static_cast<int>(GetTempoAndTimeSignature(timelinePos).BPM);
}

std::string Project::GetTimeSig(double timelinePos)
{
	const auto [BPM, TimeSigNum, TimeSigDen] = GetTempoAndTimeSignature(timelinePos);
	return fmt::format("{}-{}", TimeSigNum, TimeSigDen);
}

int Project::CountSelectedItems() const
{
	return CountSelectedMediaItems(Ptr);
}

std::vector<Region> Project::GetRegions()
{
	std::vector<Region> regions{};
	const int count = CountProjectMarkers(Ptr, nullptr, nullptr);

	for (int i = 0; i < count; ++i)
	{
		bool isRegion;
		double startPos;
		double endPos;
		const char* name;
		int index;
		EnumProjectMarkers(i, &isRegion, &startPos, &endPos, &name, &index);

		if (!isRegion)
			continue;

		Region region{index, name, startPos, endPos};
		regions.push_back(region);
	}

	return regions;
}

std::string Project::GetRegionNameByKey(const std::string& key, const double itemPos)
{
	std::vector<Region> regions = GetRegions();

	for (const auto& region : regions)
	{
		if (region.EndPos <= itemPos || region.StartPos > itemPos)
			continue;

		if (std::string tag = region.GetTag(); !tag.empty())
		{
			if (region.GetKey() != key)
				continue;

			return tag;
		}
	}

	return "";
}

MusicNotation Project::GetTempoAndTimeSignature(double timelinePos) const
{
	double bpm{};
	int numerator{};
	int denominator{};

	int index = CountTempoTimeSigMarkers(Ptr) - 1;
	double timePos;

	GetTempoTimeSigMarker(Ptr, index, &timePos, nullptr, nullptr, &bpm, &numerator, &denominator, nullptr);
	while (timePos > timelinePos && index > -1)
	{
		--index;
		GetTempoTimeSigMarker(Ptr, index, &timePos, nullptr, nullptr, &bpm, &numerator, &denominator, nullptr);
	}

	if (numerator <= 0)
	{
		double bpi;
		GetProjectTimeSignature2(Ptr, nullptr, &bpi);
		numerator = static_cast<int>(bpi);
	}

	if (denominator <= 0)
	{
		TimeMap_GetTimeSigAtTime(Ptr, 0, nullptr, &denominator, nullptr);
	}

	if (bpm <= 0)
	{
		bpm = Master_GetTempo();
	}

	const MusicNotation ms{bpm, numerator, denominator};

	return ms;
}

std::string Marker::GetTag() const
{
	const std::string::size_type startPos = Name.find("=");

	if (startPos == std::string::npos)
		return "";

	const size_t endPos = Name.length() - startPos + 1;
	return Name.substr(startPos + 1, endPos);
}

std::string Marker::GetKey() const
{
	const std::string::size_type endPos = Name.find("=");

	if (endPos == std::string::npos)
		return "";

	const size_t startPos = 0;
	return Name.substr(startPos, endPos);
}

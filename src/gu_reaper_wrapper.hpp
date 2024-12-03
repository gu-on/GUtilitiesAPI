#pragma once

#include <string>
#include <tuple>

#include <WDL/wdltypes.h> // might be unnecessary in future

#include <reaper_plugin_functions.h>

#include "gu_audio_source.hpp"

static constexpr int REAPER_NOTES_MAX_LENGTH = 1 << 15;
static constexpr int REAPER_NAMES_MAX_LENGTH = 1 << 9;

class Item;
class Take;

struct MusicNotation
{
	double BPM{};
	int TimeSigNum{};
	int TimeSigDen{};
};

struct Marker
{
	int Index{};
	std::string Name;
	double StartPos{};

	Marker() = delete;
	Marker(const int index, std::string name, const double startPos)
	{
		Index = index;
		Name = std::move(name);
		StartPos = startPos;
	}

	[[nodiscard]] std::string GetTag() const;
	[[nodiscard]] std::string GetKey() const;
};

struct Region : public Marker
{
	double EndPos{};

	Region() = delete;
	Region(const int index, std::string name, const double startPos, const double endPos)
		: Marker(index, name, startPos)
	{
		EndPos = endPos;
	}
};

class Project
{
public:
	[[nodiscard]] int GetState() const;
	[[nodiscard]] std::string GetName() const;
	[[nodiscard]] std::string GetAuthor() const;
	[[nodiscard]] static std::string ExtractFileName(const std::string& path);

	[[nodiscard]] std::string GetRegionName(const double timelinePos);
	[[nodiscard]] std::string GetMarkerName(const double timelinePos);
	[[nodiscard]] int GetTempo(double timelinePos) const;
	[[nodiscard]] std::string GetTimeSig(double timelinePos);

	[[nodiscard]] int CountSelectedItems() const;

	[[nodiscard]] std::vector<Region> GetRegions();
	[[nodiscard]] std::string GetRegionNameByKey(const std::string& key, const double timelinePos);

private:
	ReaProject* Ptr;

	MusicNotation GetTempoAndTimeSignature(double timelinePos) const;

public:
	Project() = default;
	explicit Project(ReaProject* project) : Ptr(project) {}
};

class Track
{
public:
	[[nodiscard]] std::string GetName() const;
	[[nodiscard]] std::tuple<Track, int> GetOutermostAncestor() const;
	[[nodiscard]] Track GetTrackAbove(int level) const;
	[[nodiscard]] int GetDepth() const;
	[[nodiscard]] int GetNumber() const;

	void SetName(const std::string& name) const;
	void SetDepth(const int depth) const;

	Item CreateNewItem(const std::string& path, double position);

private:
	MediaTrack* Ptr;

public:
	Track() = delete;
	Track(MediaTrack* track);

	bool operator==(const Track& other) const { return other.Ptr == this->Ptr; }
	bool operator!=(const Track& other) const { return other.Ptr != this->Ptr; }
};

class Item
{
public:
	[[nodiscard]] std::string GetNotes() const;
	[[nodiscard]] double GetPosition() const;

	Take CreateNewTake(const std::string& path) const;
	void SetPosition(double position) const;
	void SetLength(double length) const;

private:
	MediaItem* Ptr;

public:
	Item() = delete;
	explicit Item(MediaItem* item);
};

class Take
{
public:
	PCM_source* SetAudioSource(const std::string& path);
	void SetName(const std::string& name);

	[[nodiscard]] std::string GetName();
	[[nodiscard]] double GetSourceLength();
	[[nodiscard]] std::string GetFXNames(const std::string& separator = "-");

	[[nodiscard]] Track GetTrack();
	[[nodiscard]] Item GetItem();
	[[nodiscard]] AudioSource GetSource();

private:
	MediaItem_Take* Ptr;

public:
	Take() = delete;
	explicit Take(MediaItem_Take* take);
};
#pragma once

#include <gu_audio_source.hpp>
#include <reaper_plugin_functions.h>

#include <string>
#include <tuple>

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

class Project
{
public:
	[[nodiscard]] int GetState() const;
	[[nodiscard]] std::string GetName() const;
	[[nodiscard]] std::string GetAuthor() const;
	[[nodiscard]] static std::string ExtractFileName(const std::string& filePath);

	[[nodiscard]] std::string GetRegionName(const double timelinePos);
	[[nodiscard]] std::string GetMarkerName(const double timelinePos);
	[[nodiscard]] int GetTempo(double timelinePos) const;
	[[nodiscard]] std::string GetTimeSig(double timelinePos);

	[[nodiscard]] int CountSelectedItems() const;

private:
	ReaProject* Ptr;

	MusicNotation GetTempoAndTimeSignature(double timelinePos) const;

public:
	Project() = default;
	explicit Project(ReaProject* project);
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

	Item CreateNewItem(const std::string& filePath, double position);

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

	Take CreateNewTake(const std::string& filePath) const;
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
	[[nodiscard]] std::string GetFXNames();

	[[nodiscard]] Track GetTrack();
	[[nodiscard]] Item GetItem();
	[[nodiscard]] AudioSource GetSource();

private:
	MediaItem_Take* Ptr;

public:
	Take() = delete;
	explicit Take(MediaItem_Take* take);
};
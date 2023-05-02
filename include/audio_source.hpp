#pragma once

#include <reaper_plugin_functions.h>
#include <string>
#include <vector>

struct CueMarker
{
	int Index{};
	float StartTime{};
	float EndTime{};
	bool IsRegion{};
	std::string Name{};
};

class AudioSource
{
	friend class AudioBuffer;

public:
	[[nodiscard]] bool IsMono(int bufferSize = 1024) const;
	[[nodiscard]] bool IsFirstSampleZero() const;
	[[nodiscard]] bool IsLastSampleZero() const;
	[[nodiscard]] int CountSamplesTilPeak(int bufferSize, double threshold) const;
	[[nodiscard]] int CountSamplesTilPeakR(int bufferSize, double threshold) const;
	[[nodiscard]] int CountSamplesTilRMS(int bufferSize, double threshold) const;
	[[nodiscard]] int CountSamplesTilRMSR(int bufferSize, double threshold) const;
	[[nodiscard]] bool HasRegion() const;

	[[nodiscard]] std::vector<CueMarker> GetMediaCues() const;
	[[nodiscard]] int GetChannelCount() const
	{
		return AudioPtr->GetNumChannels();
	}
	void GetSamples(PCM_source_transfer_t& buffer) const
	{
		AudioPtr->GetSamples(&buffer);
	}
	[[nodiscard]] int GetSampleRate() const
	{
		return static_cast<int>(AudioPtr->GetSampleRate());
	}
	[[nodiscard]] int GetLengthInSamples() const
	{
		return static_cast<int>(AudioPtr->GetLength() * AudioPtr->GetSampleRate());
	}

private:
	PCM_source* AudioPtr{};

public:
	AudioSource() = delete;
	explicit AudioSource(PCM_source* source);
};

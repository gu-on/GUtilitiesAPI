#pragma once

#include <string>
#include <vector>

#include <reaper_plugin_functions.h>

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
	[[nodiscard]] double GetSampleValue(double time) const;
	[[nodiscard]] int CountSamplesTilPeak(int bufferSize, double threshold) const;
	[[nodiscard]] int CountSamplesTilPeakR(int bufferSize, double threshold) const;
	[[nodiscard]] int CountSamplesTilRMS(int bufferSize, double threshold) const;
	[[nodiscard]] int CountSamplesTilRMSR(int bufferSize, double threshold) const;
	[[nodiscard]] bool HasRegion() const;

	[[nodiscard]] std::vector<CueMarker> GetMediaCues() const;
	[[nodiscard]] int GetChannelCount() const { return AudioPtr->GetNumChannels(); }
	void GetSamples(PCM_source_transfer_t& buffer) const { AudioPtr->GetSamples(&buffer); }
	[[nodiscard]] int GetSampleRate() const { return static_cast<int>(AudioPtr->GetSampleRate()); }
	[[nodiscard]] int GetLengthInSamples() const
	{
		return static_cast<int>(AudioPtr->GetLength() * AudioPtr->GetSampleRate());
	}
	[[nodiscard]] double GetLengthInSeconds() const { return AudioPtr->GetLength(); }

private:
	PCM_source* AudioPtr{};

public:
	enum class NormalizationType
	{
		LUFS_I,
		RMS,
		PEAK,
		TRUE_PEAK,
		LUFS_M,
		LUFS_S
	};

	[[nodiscard]] double GetNormalization(int normalizationType) const;

	[[nodiscard]] double GetLUFS() const { return GetNormalization(static_cast<int>(NormalizationType::LUFS_I)); }
	[[nodiscard]] double GetRMS() const { return GetNormalization(static_cast<int>(NormalizationType::RMS)); }
	[[nodiscard]] double GetPeak() const { return GetNormalization(static_cast<int>(NormalizationType::PEAK)); }
	[[nodiscard]] double GetTruePeak() const
	{
		return GetNormalization(static_cast<int>(NormalizationType::TRUE_PEAK));
	}

	AudioSource() = delete;
	explicit AudioSource(PCM_source* source);
};

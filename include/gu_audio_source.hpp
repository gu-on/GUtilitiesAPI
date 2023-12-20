#pragma once

#include <string>
#include <vector>

#include <reaper_plugin_functions.h>

#include <gu_maths.hpp>

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
public:
	[[nodiscard]] bool IsMono(int bufferSize = 1024) const;
	[[nodiscard]] double GetSampleValue(double time) const;
	[[nodiscard]] double TimeToPeak(int bufferSize, double threshold) const;
	[[nodiscard]] double TimeToPeakR(int bufferSize, double threshold) const;
	[[nodiscard]] double TimeToRMS(int bufferSize, double threshold) const;
	[[nodiscard]] double TimeToRMSR(int bufferSize, double threshold) const;
	[[nodiscard]] bool HasRegion() const;

	[[nodiscard]] std::vector<CueMarker> GetMediaCues() const;
	[[nodiscard]] int GetChannelCount() const { return AudioPtr->GetNumChannels(); }
	void GetSamples(PCM_source_transfer_t& buffer) const { AudioPtr->GetSamples(&buffer); }
	[[nodiscard]] double GetSampleRate() const { return AudioPtr->GetSampleRate(); }
	[[nodiscard]] int GetLengthInSamples() const
	{
		return static_cast<int>(std::ceil(AudioPtr->GetLength() * AudioPtr->GetSampleRate()));
	}
	[[nodiscard]] double GetLengthInSeconds() const { return AudioPtr->GetLength(); }

private:
	[[nodiscard]] double GetFinalBufferTime(int bufferSize) const
	{
		return GetLengthInSeconds() - bufferSize / GetSampleRate();
	}

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

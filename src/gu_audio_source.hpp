#pragma once

#include <string>
#include <vector>

#include <WDL/wdltypes.h> // might be unnecessary in future

#include <reaper_plugin_functions.h>

#include "gu_maths.hpp"

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
	[[nodiscard]] double TimeToPeak(int bufferSize, double threshold) const;
	[[nodiscard]] double TimeToPeakR(int bufferSize, double threshold) const;
	[[nodiscard]] double TimeToRMS(int bufferSize, double threshold) const;
	[[nodiscard]] double TimeToRMSR(int bufferSize, double threshold) const;
	[[nodiscard]] bool HasRegion() const;

	[[nodiscard]] std::vector<CueMarker> GetMediaCues() const;
	[[nodiscard]] int GetChannelCount() const { return AudioPtr->GetNumChannels(); }
	[[nodiscard]] double GetSampleRate() const { return AudioPtr->GetSampleRate(); }
	[[nodiscard]] double GetLengthInSeconds() const { return AudioPtr->GetLength(); }
	[[nodiscard]] int GetLengthInSamples() const
	{
		return static_cast<int>(std::ceil(AudioPtr->GetLength() * AudioPtr->GetSampleRate()));
	}

	enum class NormalizationType
	{
		LUFS_I,
		RMS,
		PEAK,
		TRUE_PEAK,
		LUFS_M,
		LUFS_S
	};

	[[nodiscard]] double GetNormalization(NormalizationType type) const;

	[[nodiscard]] double GetLUFS() const { return GetNormalization(NormalizationType::LUFS_I); }
	[[nodiscard]] double GetRMS() const { return GetNormalization(NormalizationType::RMS); }
	[[nodiscard]] double GetPeak() const { return GetNormalization(NormalizationType::PEAK); }
	[[nodiscard]] double GetTruePeak() const { return GetNormalization(NormalizationType::TRUE_PEAK); }

	AudioSource() = delete;
	explicit AudioSource(PCM_source* source);

private:
	void GetSamples(PCM_source_transfer_t& buffer) const { AudioPtr->GetSamples(&buffer); }
	[[nodiscard]] double GetFinalBufferTime(int bufferSize) const
	{
		return (GetLengthInSamples() - 1 - bufferSize) / GetSampleRate();
	}

private:
	PCM_source* AudioPtr{};
};

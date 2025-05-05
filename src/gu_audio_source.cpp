#include <algorithm>
#include <cassert>

#include <fmt/core.h>
#include <iterator>

#include "gu_audio_analysis.hpp"
#include "gu_audio_buffer.hpp"
#include "gu_audio_source.hpp"
#include "gu_maths.hpp"
#include "reaper_plugin_functions.h"

bool AudioSource::IsMono() const
{
	AudioBuffer buffer(*this, 1, 0.0);

	if (buffer.ChannelCount() == 1)
		return true;

	while (buffer.IsValid())
		if (!buffer.IsMono())
			return false;

	return true;
}

double AudioSource::GetSampleValue(const double time) const
{
	static constexpr int BUFFER_LENGTH = 1;
	const AudioBuffer buffer(*this, BUFFER_LENGTH, std::clamp(time, 0.0, (GetDurationInSamples() - 1) / GetSampleRate()));

	return buffer.GetFirstSampleValue();
}

double AudioSource::TimeToPeak(const int bufferSize, const double threshold) const
{
	AudioBuffer buffer(*this, bufferSize, 0.0);

	while (buffer.IsValid())
		if (const double time = buffer.GetTimeToPeak(threshold); time > 0.0)
			return time;

	return 0;
}

double AudioSource::TimeToPeakR(const int bufferSize, const double threshold) const
{
	AudioBuffer buffer(*this, bufferSize, GetLengthInSeconds() - bufferSize / GetSampleRate());

	while (buffer.IsValid())
		if (const double time = buffer.GetTimeToPeakR(threshold); time > 0.0)
			return GetLengthInSeconds() - time;

	return 0;
}

double AudioSource::TimeToRMS(const int bufferSize, const double threshold) const
{
	AudioBuffer buffer(*this, bufferSize, 0.0);

	while (buffer.IsValid())
		if (const double rms = buffer.GetRMS(); rms > Maths::DB2VOL(threshold))
			return buffer.StartTime();

	return 0;
}

double AudioSource::TimeToRMSR(const int bufferSize, const double threshold) const
{
	AudioBuffer buffer{*this, bufferSize, GetFinalBufferStartTime(bufferSize)};

	while (buffer.IsValid())
		if (const double rms = buffer.GetRMSR(); rms > Maths::DB2VOL(threshold))
			return buffer.StartTime();

	return 0;
}

bool AudioSource::HasRegion() const
{
	auto mediaCues = GetMediaCues();

	return std::any_of(mediaCues.begin(), mediaCues.end(), [&](const auto& marker) { return marker.IsRegion; });
}

std::vector<CueMarker> AudioSource::GetMediaCues() const
{
	std::vector<CueMarker> temp{};

	REAPER_cue cue{};
	int index{};
	static constexpr int call{PCM_SOURCE_EXT_ENUMCUES_EX};
	while (AudioPtr->Extended(call, reinterpret_cast<void*>(static_cast<intptr_t>(index++)), &cue, nullptr) > 0)
	{
		CueMarker marker{
			index,			static_cast<float>(cue.m_time), static_cast<float>(cue.m_endtime),
			cue.m_isregion, cue.m_name ? cue.m_name : "",
		};

		temp.emplace_back(marker);
	}

	return temp;
}

double AudioSource::GetNormalization(const NormalizationType type, const double start, const double end) const
{
	return 20.0 * log10(CalculateNormalization(AudioPtr, static_cast<int>(type), 0, start, end)) * -1;
}

double AudioSource::CalculatePitch(int biChannels, double timeStart, double timeEnd, int windowSize, int overlap)
{
	static constexpr int windowMin = 1 << 6;
	static constexpr int windowMax = 1 << 14;
	static constexpr int overlapMin = 1;
	static constexpr int overlapMax = 8;

	static constexpr double err = -1.0;

	timeStart = std::clamp(timeStart, 0.0, GetLengthInSeconds());

	if (timeEnd <= 0.0 || timeEnd > GetLengthInSeconds())
		timeEnd = GetLengthInSeconds();

	if (Maths::IsNearlyEqual(timeStart, timeEnd) || timeEnd < timeStart)
		return err;

	windowSize = std::clamp(Maths::GetNextPowerOfTwo(windowSize), windowMin, windowMax);
	overlap = std::clamp(Maths::GetNextPowerOfTwo(overlap), overlapMin, overlapMax);

	// It is not necessary for bufferLength and windowSize to be the same value, it is just simpler if they are
	AudioBuffer buffer{*this, windowSize, timeStart};
	std::vector<double> tempBuffer(windowSize);
	std::vector<float> audio(0);
	while (buffer.IsValid())
	{
		buffer.OverwriteOutBuffer(tempBuffer, biChannels, AudioBuffer::Direction::Forward);
		audio.insert(audio.end(), std::make_move_iterator(tempBuffer.begin()), std::make_move_iterator(tempBuffer.end()));
	}

	if (audio.empty())
		return err;

	AcaPitch pitch{audio, CPitchIf::kPitchTimeAcf, static_cast<int>(GetSampleRate()), windowSize, overlap};

	return pitch.GetPitch();
}

AudioSource::AudioSource(PCM_source* source) : AudioPtr((assert(source != nullptr), source)) {}

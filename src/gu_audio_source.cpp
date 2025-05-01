#include <algorithm>
#include <cassert>

#include <AcaAll.h>
#include <fmt/core.h>

// #include "Pitch.h"
#include "Pitch.h"
#include "gu_audio_analysis.hpp"
#include "gu_audio_buffer.hpp"
#include "gu_audio_source.hpp"
#include "gu_maths.hpp"
#include "reaper_plugin_functions.h"

bool AudioSource::IsMono(const int bufferSize) const
{
	AudioBuffer buffer(*this, bufferSize, 0.0);

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
	const AudioBuffer buffer(*this, BUFFER_LENGTH, std::clamp(time, 0.0, (GetLengthInSamples() - 1) / GetSampleRate()));

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
			return buffer.Time();

	return 0;
}

double AudioSource::TimeToRMSR(const int bufferSize, const double threshold) const
{
	AudioBuffer buffer{*this, bufferSize, GetFinalBufferTime(bufferSize)};

	while (buffer.IsValid())
		if (const double rms = buffer.GetRMSR(); rms > Maths::DB2VOL(threshold))
			return buffer.Time();

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
	return 20 * log10(CalculateNormalization(AudioPtr, static_cast<int>(type), 0, start, end)) * -1;
}

double AudioSource::AnaylzePitch(double start, double end, int windowSize, int overlap)
{
	static constexpr double err = -1.0f;

	if (end < start)
		return err;

	if (start < 0.0)
		start = 0.0;

	if (end <= 0.0)
		end = this->GetLengthInSeconds();

	if (windowSize <= 0)
		windowSize = 64;

	if (overlap <= 0)
		overlap = 1;

	AudioBuffer buffer{*this, windowSize, start};
	const int endSample = static_cast<int>(end * this->GetSampleRate());

	const int channelCount = this->GetChannelCount();
	const double channelReciprocal = 1.0 / channelCount;

	int sameplePos{static_cast<int>(start * this->GetSampleRate())};
	double running{};
	std::vector<float> audio{};

	// fill audio vector by iteratively adding audio buffers
	while (buffer.IsValid())
	{
		if (buffer.SamplesOut() < windowSize) // hard limits to ensure full buffers for pitch detection
			break;

		if (sameplePos + windowSize > endSample) // ensure full buffer
			break;

		for (int i = 0; i < windowSize; i++)
		{
			// sums and averages channels into single mono source
			running += buffer.SampleAt(i);
			++sameplePos;

			if (i % channelCount != 0)
				continue;

			running *= channelReciprocal;
			audio.push_back(static_cast<double>(running));
			running = 0;
		}

		buffer.RefillSamples(AudioBuffer::Direction::Forward);
	}

	if (audio.size() <= 0)
		return err;

	AcaPitch pitch{audio, CPitchIf::kPitchTimeAcf, static_cast<int>(this->GetSampleRate()), windowSize, overlap};

	return pitch.GetPitch();
}

AudioSource::AudioSource(PCM_source* source) : AudioPtr((assert(source != nullptr), source)) {}

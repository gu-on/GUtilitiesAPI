#include <algorithm>
#include <cassert>

#include <fmt/core.h>

#include <gu_audio_buffer.hpp>
#include <gu_audio_source.hpp>

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

double AudioSource::GetNormalization(const NormalizationType type) const
{
	return 20 * log10(CalculateNormalization(AudioPtr, static_cast<int>(type), 0, 0, 0)) * -1;
}

AudioSource::AudioSource(PCM_source* source) : AudioPtr((assert(source != nullptr), source)) {}

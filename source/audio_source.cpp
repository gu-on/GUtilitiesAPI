#include <audio_source.hpp>

#include <algorithm>
#include <audio_buffer.hpp>
#include <cassert>
#include <maths.hpp>

bool AudioSource::IsMono(int bufferSize) const
{
	AudioBuffer buffer(*this, bufferSize, 0);

	if (buffer.ChannelCount() == 1)
		return true;

	int frame{};

	while (buffer.SamplesOut())
	{
		if (!buffer.IsMono())
			return false;

		buffer.RefillSamples(++frame);
	}

	return true;
}

bool AudioSource::IsFirstSampleZero() const
{
	static constexpr int BUFFER_LENGTH = 1;
	const AudioBuffer buffer(*this, BUFFER_LENGTH, 0);

	const double bufferVal = std::abs(buffer.SampleAt(0));

	return Maths::IsNearlyEqual(bufferVal, 0.0);
}

bool AudioSource::IsLastSampleZero() const
{
	static constexpr int BUFFER_LENGTH = 1;
	const AudioBuffer buffer(*this, BUFFER_LENGTH, GetLengthInSamples() - 1);

	const double bufferVal = std::abs(buffer.SampleAt(0));

	return Maths::IsNearlyEqual(bufferVal, 0.0);
}

int AudioSource::CountSamplesTilPeak(const int bufferSize, const double threshold) const
{
	AudioBuffer buffer(*this, bufferSize, 0);

	int frame{};

	while (buffer.SamplesOut())
	{
		if (const int sample = buffer.GetFirstSampleAboveThreshold(threshold); sample > 0)
			return sample;

		buffer.RefillSamples(++frame);
	}

	return 0;
}

int AudioSource::CountSamplesTilPeakR(const int bufferSize, const double threshold) const
{
	const int startSample{GetLengthInSamples() - bufferSize};
	AudioBuffer buffer(*this, bufferSize, startSample);

	int frame{};
	const int maxFrames = GetLengthInSamples() / bufferSize;

	while (buffer.SamplesOut() && frame < maxFrames)
	{
		if (const int sample = buffer.GetFirstSampleAboveThreshold(threshold); sample > 0)
			return sample;

		buffer.ReverseRefillSamples(++frame, maxFrames);
	}

	return 0;
}

int AudioSource::CountSamplesTilRMS(const int bufferSize, const double threshold) const
{
	AudioBuffer buffer(*this, bufferSize, 0);

	int frame{};

	while (buffer.SamplesOut())
	{
		if (const double rms = buffer.GetRMS(); rms > Maths::DB2VOL(threshold))
			return frame * bufferSize;

		buffer.RefillSamples(++frame);
	}

	return 0;
}

int AudioSource::CountSamplesTilRMSR(const int bufferSize, const double threshold) const
{
	const int startSample{GetLengthInSamples() - bufferSize};
	AudioBuffer buffer{*this, bufferSize, startSample};

	int frame{};
	const int maxFrames = GetLengthInSamples() / bufferSize;

	while (buffer.SamplesOut() && frame < maxFrames)
	{
		if (const double rms = buffer.GetRMS(); rms > Maths::DB2VOL(threshold))
			return frame * bufferSize;

		buffer.ReverseRefillSamples(++frame, maxFrames);
	}

	return 0;
}

bool AudioSource::HasRegion() const
{
	return std::ranges::any_of(GetMediaCues(), [&](const auto& marker) { return marker.IsRegion; });
}

std::vector<CueMarker> AudioSource::GetMediaCues() const
{
	std::vector<CueMarker> temp{};

	REAPER_cue cue{};
	int index{};
	while (AudioPtr->Extended(PCM_SOURCE_EXT_ENUMCUES_EX, reinterpret_cast<void*>(static_cast<intptr_t>(index++)), &cue,
							  nullptr) > 0)
	{
		CueMarker marker{
			index,			static_cast<float>(cue.m_time), static_cast<float>(cue.m_endtime),
			cue.m_isregion, cue.m_name ? cue.m_name : "",
		};

		temp.emplace_back(marker);
	}

	return temp;
}

AudioSource::AudioSource(PCM_source* source) : AudioPtr((assert(source != nullptr), source))
{
}

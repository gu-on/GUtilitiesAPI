#include <algorithm>
#include <cassert>

#include <fmt/core.h>

#include <gu_audio_buffer.hpp>
#include <gu_audio_source.hpp>

bool AudioSource::IsMono(int bufferSize) const
{
	AudioBuffer buffer(*this, bufferSize, 0);

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
	double startTime{time >= 0 ? time : GetLengthInSeconds() + time};

	const AudioBuffer buffer(*this, BUFFER_LENGTH, startTime);

	return buffer.SampleAt(0);
}

double AudioSource::TimeToPeak(const int bufferSize, const double threshold) const
{
	AudioBuffer buffer(*this, bufferSize, 0.0);

	while (buffer.IsValid())
	{
		if (const double time = buffer.GetTimeToPeak(threshold); time > 0)
		{
			ShowConsoleMsg(fmt::format("\ntotal refills: {}\n", buffer.totalSampleRefills).c_str());
			return time;
		}
	}

	ShowConsoleMsg(fmt::format("\ntotal refills: {}\n", buffer.totalSampleRefills).c_str());
	return 0;
}

double AudioSource::TimeToPeakR(const int bufferSize, const double threshold) const
{
	const double startTime{GetLengthInSeconds() - bufferSize / GetSampleRate()};
	AudioBuffer buffer(*this, bufferSize, startTime);

	while (buffer.IsValid())
	{
		if (const double time = buffer.GetTimeToPeakR(threshold); time > 0)
		{
			ShowConsoleMsg(fmt::format("\ntotal refills: {}\n", buffer.totalSampleRefills).c_str());
			return GetLengthInSeconds() - time;
		}
	}

	ShowConsoleMsg(fmt::format("\ntotal refills: {}\n", buffer.totalSampleRefills).c_str());
	return 0;
}

double AudioSource::TimeToRMS(const int bufferSize, const double threshold) const
{
	AudioBuffer buffer(*this, bufferSize, 0.0);
	while (buffer.IsValid())
	{
		if (const double rms = buffer.GetRMS(); rms > Maths::DB2VOL(threshold))
		{
			ShowConsoleMsg(fmt::format("\ntotal refills: {}\n", buffer.totalSampleRefills).c_str());
			return buffer.Time();
		}
	}

	ShowConsoleMsg(fmt::format("\ntotal refills: {}\n", buffer.totalSampleRefills).c_str());
	return 0;
}

double AudioSource::TimeToRMSR(const int bufferSize, const double threshold) const
{
	const double startTime{GetFinalBufferTime(bufferSize)};
	AudioBuffer buffer{*this, bufferSize, startTime};

	while (buffer.IsValid())
	{
		if (const double rms = buffer.GetRMSR(); rms > Maths::DB2VOL(threshold))
		{
			ShowConsoleMsg(fmt::format("\ntotal refills: {}\n", buffer.totalSampleRefills).c_str());
			return buffer.Time();
		}
	}

	ShowConsoleMsg(fmt::format("\ntotal refills: {}\n", buffer.totalSampleRefills).c_str());
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

double AudioSource::GetNormalization(int normalizationType) const
{
	return 20 * log10(CalculateNormalization(AudioPtr, normalizationType, 0, 0, 0)) * -1;
}

AudioSource::AudioSource(PCM_source* source) : AudioPtr((assert(source != nullptr), source)) {}

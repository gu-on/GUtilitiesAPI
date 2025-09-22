#include <algorithm>
#include <cassert>
#include <fmt/core.h>
#include <functional>
#include <vector>

#include "gu_audio_buffer.hpp"
#include "gu_maths.hpp"

AudioBuffer::AudioBuffer(const AudioSource& source, const int bufferSize, const double startTime, const int biChannels,
						 const Direction dir)
{
	Source = &source;

	const double sampleRate = Source->GetSampleRate();
	frame = static_cast<int>(std::floor(startTime * sampleRate));
	frameMax = Source->GetDurationInSamples() - 1;

	Buffer.time_s = startTime;
	Buffer.samplerate = sampleRate;
	Buffer.nch = Source->GetChannelCount();
	Buffer.length = std::clamp(bufferSize, 1, 1 << 14);
	Buffer.samples = new ReaSample[Buffer.length * Buffer.nch];

	direction = dir;
	channelsToSum = GetChannelsFromBinary(biChannels);
	assert(!channelsToSum.empty());
	sumChannelCountRecip = 1.f / channelsToSum.size();

	Source->GetSamples(Buffer);
}

void AudioBuffer::RefillSamples()
{
	Buffer.time_s += (direction == Direction::Forward ? SamplesOut() / SampleRate() : -SamplesOut() / SampleRate());

	frame += (direction == Direction::Forward ? SamplesOut() : -SamplesOut());

	Source->GetSamples(Buffer);
}

void AudioBuffer::Iterate(const std::function<bool(int, double)>& func) const
{
	int frameTemp = frame;
	if (direction == Direction::Forward)
	{
		for (int i = 0; i < SamplesOut(); ++i)
		{
			if (!IsFrameInRange())
				break;

			if (!func(i, SampleAt(i)))
				break;

			++frame;
		}
	}
	else
	{
		for (int i = SamplesOut() - 1; i >= 0; --i)
		{
			if (!IsFrameInRange())
				break;

			if (!func(i, SampleAt(i)))
				break;

			--frame;
		}
	}

	frame = frameTemp; // Only commit to adding after RefillSamples
}

double AudioBuffer::CalculateRMS()
{
	std::vector<double> tempBuffer(Buffer.length);
	OverwriteOutBuffer(tempBuffer);

	double totalAmp{};
	for (const auto& ampVal : tempBuffer)
		totalAmp += ampVal * ampVal;

	return std::sqrt(totalAmp / static_cast<int>(tempBuffer.size()));
}

double AudioBuffer::CalculateTimeToPeak(const double peakThreshold)
{
	double timeToPeak{};

	Iterate([&](const int index, const double value) {
		if (Maths::VOL2DB(value) > peakThreshold)
		{
			timeToPeak = StartTime() + (index / SampleRate());
			return false;
		}

		return true;
	});

	return timeToPeak;
}

double AudioBuffer::SampleAt(const int index) const
{
	double value{};
	for (const int channel : channelsToSum)
		value += AbsoluteSampleAt(index * ChannelCount() + (channel - 1));

	return value * sumChannelCountRecip;
}

bool AudioBuffer::IsMono()
{
	std::vector<double> values{};

	for (int i = 1; i <= Source->GetChannelCount(); i++)
		Iterate([&]([[maybe_unused]] const int index, const double value) {
			values.push_back(value);
			return true;
		});

	if (!std::equal(values.begin() + 1, values.end(), values.begin()))
		return false;

	return true;
}

void AudioBuffer::OverwriteOutBuffer(std::vector<double>& outBuffer)
{
	assert(outBuffer.size() >= static_cast<size_t>(Buffer.length));

	Iterate([&]([[maybe_unused]] const int index, const double value) { outBuffer[index] = value; return true; });
}

std::vector<int> AudioBuffer::GetChannelsFromBinary(int biChannels) const
{
	const unsigned long maxChannels = static_cast<unsigned long>(Source->GetChannelCount());
	std::vector<int> channels{};

	if (biChannels <= 0)
	{
		for (unsigned long ch = 1; ch <= maxChannels; ch++)
			channels.push_back(ch);

		return channels;
	}

	for (unsigned long ch = 1; ch < sizeof(int) && ch <= maxChannels; ch++)
	{
		int flag = 1 << (ch - 1);
		if ((biChannels & flag) != flag)
			continue;

		channels.push_back(ch);
	}

	return channels;
}
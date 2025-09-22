#include <algorithm>
#include <fmt/core.h>
#include <functional>
#include <vector>

#include "gu_audio_analysis.hpp"
#include "gu_audio_buffer.hpp"
#include "gu_maths.hpp"

AudioBuffer::AudioBuffer(const AudioSource& source, int bufferLength, double startTime)
{
	Source = &source;

	const double sampleRate = Source->GetSampleRate();
	frame = static_cast<int>(std::floor(startTime * sampleRate));
	frameMax = Source->GetLengthInSamples() - 1;

	Buffer.time_s = startTime;
	Buffer.samplerate = sampleRate;
	Buffer.nch = Source->GetChannelCount();
	Buffer.length = std::clamp(bufferLength, 1, 1 << 14);
	Buffer.samples = new ReaSample[static_cast<unsigned long long>(Buffer.length) * Buffer.nch];
	Buffer.absolute_time_s = 0.0;

	Source->GetSamples(Buffer);
}

void AudioBuffer::RefillSamples(const Direction dir)
{
	if (dir == Direction::Forward)
	{
		Buffer.time_s += SamplesOut() / SampleRate();
	}
	else
	{
		Buffer.time_s -= SamplesOut() / SampleRate();
	}
	Buffer.samples_out = 0;
	Source->GetSamples(Buffer);
}

void AudioBuffer::Iterate(const std::function<void(int, double)>& func, const Direction dir, int channelHex) const
{
	const std::vector<int> channels = GetChannelsFromHex(channelHex);

	if (dir == Direction::Forward)
	{
		for (int sample = 0; sample < SamplesOut(); ++sample)
		{
			if (!IsFrameInRange())
				return;

			double value{};
			for (const int channel : channels)
				value += SampleAt(sample * ChannelCount() + (channel - 1));

			value /= channels.size();
			func(sample, value);

			++frame;
		}
	}
	else
	{
		for (int sample = SamplesOut() - 1; sample >= 0; --sample)
		{
			if (!IsFrameInRange())
				return;

			double value{};
			for (const int channel : channels)
				value += SampleAt(sample * ChannelCount() + (channel - 1));

			value /= channels.size();
			func(sample, value);

			--frame;
		}
	}
}

double AudioBuffer::GetRMS()
{
	return CalculateRMS(Direction::Forward);
}

double AudioBuffer::GetRMSR()
{
	return CalculateRMS(Direction::Backward);
}

double AudioBuffer::GetTimeToPeak(const double peakThreshold)
{
	return CalculateTimeToPeak(peakThreshold, Direction::Forward);
}

double AudioBuffer::GetTimeToPeakR(const double peakThreshold)
{
	return CalculateTimeToPeak(peakThreshold, Direction::Backward);
}

double AudioBuffer::CalculateRMS(const Direction dir)
{
	std::vector<double> tempBuffer{};

	Iterate([&]([[maybe_unused]] const int index, const double value) { tempBuffer.push_back(value); }, dir, 0);

	double totalAmp{};
	for (const auto& ampVal : tempBuffer)
		totalAmp = ampVal * ampVal;

	RefillSamples(dir);

	return std::sqrt(totalAmp / static_cast<int>(tempBuffer.size()));
}

double AudioBuffer::CalculateTimeToPeak(const double peakThreshold, Direction dir)
{
	double timeToPeak{};

	Iterate(
		[&](const int index, const double value) {
			if (std::abs(value) > Maths::DB2VOL(peakThreshold))
			{
				timeToPeak = StartTime() + index / SampleRate();
				return;
			}
		},
		dir, 0);

	RefillSamples(dir);

	return timeToPeak;
}

bool AudioBuffer::IsMono()
{
	static constexpr Direction dir = Direction::Forward;
	std::vector<double> values{};

	for (int i = 1; i <= Source->GetChannelCount(); i++)
		Iterate([&]([[maybe_unused]] const int index, const double value) { values.push_back(value); }, dir, (1 << (i - 1)));

	if (!std::equal(values.begin() + 1, values.end(), values.begin()))
		return false;

	RefillSamples(dir);

	return true;
}

double AudioBuffer::CalculatePitch(int channels, double end, int overlap)
{
	static constexpr double err = 0.0;

	if (end < Buffer.time_s)
		return err;

	if (end <= 0.0 || end > Source->GetLengthInSeconds())
		end = Source->GetLengthInSeconds();

	if (overlap <= 0)
		overlap = 1;

	std::vector<float> tempBuffer{};
	while (IsValid())
	{
		Iterate([&]([[maybe_unused]] const int index, const double value) { tempBuffer.push_back(value); },
				Direction::Forward, channels);

		RefillSamples(Direction::Forward);
	}

	if (tempBuffer.empty())
		return err;

	AcaPitch pitch{tempBuffer, CPitchIf::kPitchTimeAcf, static_cast<int>(Source->GetSampleRate()), Buffer.length,
				   overlap};

	return pitch.GetPitch();
}

std::vector<int> AudioBuffer::GetChannelsFromHex(int channelHex) const
{
	const unsigned long maxChannels = static_cast<unsigned long>(Source->GetChannelCount());
	std::vector<int> channels{};

	if (channelHex <= 0)
		for (int ch = 1; ch == Source->GetChannelCount(); ch++)
			channels.push_back(ch);

	for (unsigned long ch = 1; ch < sizeof(int) && ch <= maxChannels; ch++)
	{
		int flag = 1 << (ch - 1);
		if ((channelHex & flag) != flag)
			continue;

		channels.push_back(ch);
	}

	return channels;
}
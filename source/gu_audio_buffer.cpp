#include <fmt/core.h>

#include <gu_audio_buffer.hpp>
#include <gu_maths.hpp>

AudioBuffer::AudioBuffer(const AudioSource& source, int bufferLength, double startTime)
{
	Source = &source;

	const double sampleRate = Source->GetSampleRate();
	frame = static_cast<int>(std::floor(startTime * sampleRate));
	frameMax = Source->GetLengthInSamples() - 1;

	Buffer.time_s = startTime;
	Buffer.samplerate = sampleRate;
	Buffer.nch = Source->GetChannelCount();
	Buffer.length = std::clamp(bufferLength, 0, 1 << 14);
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

void AudioBuffer::Iterate(const std::function<void(int)>& func, const Direction dir) const
{
	if (dir == Direction::Forward)
	{
		for (int sample = 0; sample < SamplesOut(); ++sample)
		{
			if (!IsFrameInRange())
				return;

			for (int chan = 0; chan < ChannelCount(); ++chan)
				func(sample * ChannelCount() + chan);

			++frame;
		}
	}
	else
	{
		for (int sample = SamplesOut() - 1; sample >= 0; --sample)
		{
			if (!IsFrameInRange())
				return;

			for (int chan = ChannelCount() - 1; chan >= 0; --chan)
				func(sample * ChannelCount() + chan);

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

	Iterate([&](const int i) { tempBuffer.push_back(SampleAt(i)); }, dir);

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
		[&](const int i) {
			if (std::abs(SampleAt(i)) > Maths::DB2VOL(peakThreshold))
			{
				timeToPeak = Time() + i / SampleRate() / ChannelCount();
				return;
			}
		},
		dir);

	RefillSamples(dir);

	return timeToPeak;
}

bool AudioBuffer::IsMono()
{
	static constexpr Direction dir = Direction::Forward;
	bool isMono = true;

	Iterate(
		[&](const int i) {
			if (const int index = i % ChannelCount(); index > 0)
			{
				if (!Maths::IsNearlyEqual(SampleAt(index - 1), SampleAt(index)))
				{
					isMono = false;
					return;
				}
			}
		},
		dir);

	RefillSamples(dir);

	return isMono;
}
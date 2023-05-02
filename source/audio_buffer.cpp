#include <audio_buffer.hpp>
#include <maths.hpp>

AudioBuffer::AudioBuffer(const AudioSource& source, int bufferLength, int startSample)
{
	Source = &source;
	const int sampleRate = Source->GetSampleRate();

	Buffer.time_s = static_cast<double>(startSample) / sampleRate;
	Buffer.samplerate = sampleRate;
	Buffer.nch = Source->GetChannelCount();
	Buffer.length = bufferLength;
	Buffer.samples = new ReaSample[static_cast<unsigned long long>(Buffer.length) * Buffer.nch];
	Buffer.absolute_time_s = 0.0;

	Source->GetSamples(Buffer);
}

AudioBuffer::~AudioBuffer()
{
	delete[] Buffer.samples;
}

void AudioBuffer::RefillSamples(const int frame)
{
	Frame = frame;

	Buffer.time_s = static_cast<double>(Buffer.length) * Frame / Buffer.samplerate;
	Buffer.samples_out = 0;
	Source->GetSamples(Buffer);
}

void AudioBuffer::ReverseRefillSamples(const int frame, const int maxFrames)
{
	Frame = frame;

	Buffer.time_s = static_cast<double>(Buffer.length) * (maxFrames - Frame) / Buffer.samplerate;
	Buffer.samples_out = 0;
	Source->GetSamples(Buffer);
}

void AudioBuffer::IterateOver(const std::function<void(int)>& func) const
{
	for (int sample = 0; sample < SamplesOut(); sample++)
	{
		for (int chan = 0; chan < ChannelCount(); chan++)
		{
			func(sample * ChannelCount() + chan);
		}
	}
}

double AudioBuffer::GetRMS() const
{
	std::vector<double> tempBuffer{};
	IterateOver([&](const int i) { tempBuffer.push_back(SampleAt(i)); });

	double totalAmp{};
	for (const auto& ampVal : tempBuffer)
		totalAmp = ampVal * ampVal;

	return std::sqrt(totalAmp / static_cast<int>(tempBuffer.size()));
}

int AudioBuffer::GetFirstSampleAboveThreshold(const double peakThreshold) const
{
	int samplesTilPeak{};
	IterateOver([&](const int i) {
		if (samplesTilPeak > 0)
			return;

		if (std::abs(SampleAt(i)) > Maths::DB2VOL(peakThreshold))
		{
			samplesTilPeak = Frame * Length() + i / ChannelCount();
		}
	});

	return samplesTilPeak;
}

bool AudioBuffer::IsMono() const
{
	bool isMono = true;
	IterateOver([&](const int i) {
		if (const int index = i % ChannelCount(); index > 0)
		{
			if (!Maths::IsNearlyEqual(SampleAt(index - 1), SampleAt(index)))
			{
				isMono = false;
			}
		}
	});

	return isMono;
}
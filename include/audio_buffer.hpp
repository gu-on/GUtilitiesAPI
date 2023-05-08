#pragma once

#include <audio_source.hpp>
#include <reaper_plugin.h>

#include <functional>

class AudioBuffer
{
public:
	void RefillSamples(int frame);
	void ReverseRefillSamples(int frame, int maxFrames);

	[[nodiscard]] int SamplesOut() const { return Buffer.samples_out; }
	[[nodiscard]] int ChannelCount() const { return Buffer.nch; }
	[[nodiscard]] int Length() const { return Buffer.length; }

	void IterateOver(const std::function<void(int)>& func) const;

	[[nodiscard]] double GetRMS() const;
	[[nodiscard]] int GetFirstSampleAboveThreshold(double peakThreshold) const;
	[[nodiscard]] bool IsMono() const;

	[[nodiscard]] ReaSample SampleAt(const int index) const { return Buffer.samples[index]; }

	AudioBuffer() = delete;
	explicit AudioBuffer(const AudioSource& source, int bufferLength, int startSample);
	~AudioBuffer();

	AudioBuffer(const AudioBuffer&) = default;
	AudioBuffer& operator=(const AudioBuffer&) = default;
	AudioBuffer(AudioBuffer&&) = default;
	AudioBuffer& operator=(AudioBuffer&&) = default;

private:
	const AudioSource* Source{};
	PCM_source_transfer_t Buffer{};
	int Frame{};
};

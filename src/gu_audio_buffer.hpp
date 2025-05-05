#pragma once

#include <cassert>
#include <functional>

#include <reaper_plugin.h>
#include <vector>

#include "gu_audio_source.hpp"

class AudioBuffer
{
public:
	enum class Direction
	{
		Forward,
		Backward,
	};

public:
	[[nodiscard]] bool IsMono();
	[[nodiscard]] bool IsFrameInRange() const { return frame >= 0 && frame <= frameMax; }
	[[nodiscard]] bool IsValid() const { return SamplesOut() && IsFrameInRange(); };
	// outBuffer size MUST be at least as big as Buffer.length
	void OverwriteOutBuffer(std::vector<double>& outBuffer);

	[[nodiscard]] int ChannelCount() const { return Buffer.nch; }
	// In samples
	[[nodiscard]] int Size() const { return Buffer.length; }
	[[nodiscard]] double SampleRate() const { return Buffer.samplerate; }
	// Start time of block relative to source, in seconds
	[[nodiscard]] double StartTime() const { return Buffer.time_s; }

	[[nodiscard]] double CalculateRMS();
	[[nodiscard]] double CalculateTimeToPeak(double peakThreshold);

	// Sums and averages for a given frame across channels
	[[nodiscard]] double SampleAt(const int frame) const;
	void RefillSamples();

private:
	[[nodiscard]] ReaSample AbsoluteSampleAt(const int index) const { return Buffer.samples[index]; }
	[[nodiscard]] int SamplesOut() const { return Buffer.samples_out; }
	// func returns a bool which controls whether iteration should continue
	void Iterate(const std::function<bool(int, double)>& func) const;

	[[nodiscard]] std::vector<int> GetChannelsFromBinary(int biChannels) const;

public:
	AudioBuffer() = delete;
	explicit AudioBuffer(const AudioSource& source, int bufferSize = 1024, double startTime = 0.0, int biChannels = 0,
						 Direction dir = Direction::Forward);
	~AudioBuffer() { delete[] Buffer.samples; };

	AudioBuffer(const AudioBuffer&) = delete;
	AudioBuffer& operator=(const AudioBuffer&) = delete;
	AudioBuffer(AudioBuffer&&) = delete;
	AudioBuffer& operator=(AudioBuffer&&) = delete;

private:
	const AudioSource* Source{};
	PCM_source_transfer_t Buffer{};

	Direction direction;
	std::vector<int> channelsToSum{};
	float sumChannelCountRecip{};
	mutable int frame{};
	int frameMax{};
};

#pragma once

#include <functional>

#include <reaper_plugin.h>

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
	[[nodiscard]] double GetRMS();
	[[nodiscard]] double GetRMSR();
	[[nodiscard]] double GetTimeToPeak(double peakThreshold);
	[[nodiscard]] double GetTimeToPeakR(double peakThreshold);
	[[nodiscard]] bool IsMono();
	[[nodiscard]] bool IsFrameInRange() const { return frame >= 0 && frame <= frameMax; }
	[[nodiscard]] bool IsValid() const { return SamplesOut() && IsFrameInRange(); };
	[[nodiscard]] double GetFirstSampleValue() const { return SampleAt(0); }

	[[nodiscard]] int ChannelCount() const { return Buffer.nch; }
	[[nodiscard]] int Length() const { return Buffer.length; }
	[[nodiscard]] double SampleRate() const { return Buffer.samplerate; }
	[[nodiscard]] double Time() const { return Buffer.time_s; }

	[[nodiscard]] double CalculateRMS(Direction dir);
	[[nodiscard]] double CalculateTimeToPeak(double peakThreshold, Direction dir);
	[[nodiscard]] ReaSample SampleAt(const int index) const { return Buffer.samples[index]; }
	void RefillSamples(Direction dir);
	[[nodiscard]] int SamplesOut() const { return Buffer.samples_out; }

private:
	void Iterate(const std::function<void(int)>& func, Direction dir) const;

public:
	AudioBuffer() = delete;
	explicit AudioBuffer(const AudioSource& source, int bufferLength, double startTime);
	~AudioBuffer() { delete[] Buffer.samples; };

	AudioBuffer(const AudioBuffer&) = delete;
	AudioBuffer& operator=(const AudioBuffer&) = delete;
	AudioBuffer(AudioBuffer&&) = delete;
	AudioBuffer& operator=(AudioBuffer&&) = delete;

private:
	const AudioSource* Source{};
	PCM_source_transfer_t Buffer{};

	mutable int frame{};
	static constexpr int FRAME_MIN{0};
	int frameMax{};
};

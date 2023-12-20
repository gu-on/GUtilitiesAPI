#pragma once

#include <functional>

#include <reaper_plugin.h>

#include <gu_audio_source.hpp>

class AudioBuffer
{
	friend class AudioSource;

private:
	enum class Direction
	{
		Forward,
		Backward,
	};

private:
	[[nodiscard]] double CalculateRMS(Direction dir);
	[[nodiscard]] double CalculateTimeToPeak(double peakThreshold, Direction dir);

public:
	void RefillSamples(Direction dir);

	[[nodiscard]] int SamplesOut() const { return Buffer.samples_out; }
	[[nodiscard]] int ChannelCount() const { return Buffer.nch; }
	[[nodiscard]] int Length() const { return Buffer.length; }
	[[nodiscard]] double SampleRate() const { return Buffer.samplerate; }
	[[nodiscard]] double Time() const { return Buffer.time_s; }

	void Iterate(const std::function<void(int)>& func, Direction dir) const;

	[[nodiscard]] double GetRMS() { return CalculateRMS(Direction::Forward); }
	[[nodiscard]] double GetRMSR() { return CalculateRMS(Direction::Backward); }
	[[nodiscard]] double GetTimeToPeak(const double peakThreshold)
	{
		return CalculateTimeToPeak(peakThreshold, Direction::Forward);
	}
	[[nodiscard]] double GetTimeToPeakR(const double peakThreshold)
	{
		return CalculateTimeToPeak(peakThreshold, Direction::Backward);
	}
	[[nodiscard]] bool IsMono();
	[[nodiscard]] bool IsFrameInRange() const { return frame >= 0 && frame <= frameMax; }
	[[nodiscard]] bool IsValid() const { return SamplesOut() && IsFrameInRange(); };

	[[nodiscard]] ReaSample SampleAt(const int index) const { return Buffer.samples[index]; }

	AudioBuffer() = delete;
	explicit AudioBuffer(const AudioSource& source, int bufferLength, double startTime);
	~AudioBuffer() { delete[] Buffer.samples; };

	AudioBuffer(const AudioBuffer&) = default;
	AudioBuffer& operator=(const AudioBuffer&) = default;
	AudioBuffer(AudioBuffer&&) = default;
	AudioBuffer& operator=(AudioBuffer&&) = default;

private:
	const AudioSource* Source{};
	PCM_source_transfer_t Buffer{};
	mutable int frame{};
	int frameMax{};
	static constexpr int FRAME_MIN{0};
	int totalSampleRefills{};
};

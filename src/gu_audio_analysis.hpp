#pragma once

#include <memory>
#include <vector>

#include <AcaAll.h>

class AcaPitch
{
public:
	AcaPitch() = delete;
	explicit AcaPitch(std::vector<float>& buffer, CPitchIf::PitchExtractors_t algorithm, const int sampleRate,
					  int windowSize, int overlap)
		: Buffer(std::move(buffer)), pfPitch(std::make_unique<float[]>(Buffer.size())), SampleRate(sampleRate), BlockSize(windowSize), HopLength(windowSize / overlap)
	{
		Error_t result = CPitchIf::create(pCInstance, algorithm, Buffer.data(), GetBufferLength(), SampleRate, BlockSize, HopLength);
		assert(result == Error_t::kNoError);
		assert(pCInstance != 0);
	}

	float GetPitch() const
	{		
		int numBlocks{};
		if (Error_t::kNoError != pCInstance->getNumBlocks(numBlocks))
			return -1.0;

		if (Error_t::kNoError != pCInstance->compF0(pfPitch.get()))
			return -1.0;

		return pfPitch.get()[0];
	}

	AcaPitch(const AcaPitch&) = delete;
	AcaPitch& operator=(const AcaPitch&) = delete;
	AcaPitch(AcaPitch&&) = delete;
	AcaPitch& operator=(AcaPitch&&) = delete;

	~AcaPitch() { CPitchIf::destroy(pCInstance); }

private:
	[[nodiscard]] int GetBufferLength() const { return Buffer.size(); }

private:
	CPitchIf* pCInstance{};
	std::vector<float> Buffer{};
	std::unique_ptr<float[]> pfPitch{};

	const float SampleRate{};
	const int BlockSize{};
	const int HopLength{};
};
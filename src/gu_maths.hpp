#pragma once

#include <algorithm>
#include <cmath>
#include <string>

class Maths
{
public:
	[[nodiscard]] static double DB2VOL(const double dB) { return pow(10.0, 0.05 * dB); }
	[[nodiscard]] static float DB2VOL(const float dB) { return powf(10.0f, 0.05f * dB); }
	[[nodiscard]] static double VOL2DB(const double volume) { return 20.0 * log10(volume); }
	[[nodiscard]] static float VOL2DB(const float volume) { return 20.0f * log10f(volume); }

	[[nodiscard]] static bool IsNearlyEqual(const float x, const float y, const float eps = 0.00001f)
	{
		return fabs(x - y) < eps;
	}

	[[nodiscard]] static bool IsNearlyEqual(const double x, const double y, const double eps = 0.00001)
	{
		return fabs(x - y) < eps;
	}

	[[nodiscard]] static bool IsNaturalNumber(const std::string& str)
	{
		if (str.empty())
			return false;

		return std::all_of(str.begin(), str.end(), [](char c) { return std::isdigit(static_cast<unsigned char>(c)); });
	}

	[[nodiscard]] static int GetNextPowerOfTwo(int value)
	{
		// https://graphics.stanford.edu/%7Eseander/bithacks.html#RoundUpPowerOf2
		--value;
		unsigned long i = 1;
		while (i <= sizeof(int))
			value |= value >> i++;

		return ++value;
	}
};

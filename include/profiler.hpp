#pragma once

#include <chrono>

class Profiler
{
public:
	Profiler() = delete;
	explicit Profiler(std::string name);
	~Profiler();

private:
	using microseconds = std::chrono::microseconds;
	using steady_clock = std::chrono::steady_clock;

	steady_clock::time_point StartTime;
	std::string Name;
};

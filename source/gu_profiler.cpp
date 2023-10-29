#include <gu_profiler.hpp>
#include <reaper_plugin_functions.h>

#include <fmt/core.h>

Profiler::Profiler(std::string name)
{
	Name = std::move(name);
	StartTime = steady_clock::now();
}

Profiler::~Profiler()
{
	auto elapsedTime = std::chrono::duration_cast<microseconds>(steady_clock::now() - StartTime).count() * 0.0001;
	ShowConsoleMsg(fmt::format("{}: took {:.3f} ms \n", Name, elapsedTime).c_str());
}

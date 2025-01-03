#include <WDL/wdltypes.h> // might be unnecessary in future

#include <fmt/core.h>
#include <reaper_plugin_functions.h>

#include "gu_profiler.hpp"

Profiler::Profiler(std::string name)
{
	Name = std::move(name);
	StartTime = steady_clock::now();
}

Profiler::~Profiler()
{
	auto elapsedTime = std::chrono::duration_cast<microseconds>(steady_clock::now() - StartTime).count() * 0.001;
	ShowConsoleMsg(fmt::format("GUtilitiesAPI - {}: took {:.3f} ms \n", Name, elapsedTime).c_str());
}

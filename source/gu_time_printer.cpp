#include <gu_time_printer.hpp>

#include <cassert>
#include <fmt/core.h>

YY_MM_DD TimePrinter::GetCurrentYMD()
{
	auto now = std::chrono::system_clock::now();
	std::time_t current_time = std::chrono::system_clock::to_time_t(now);
	std::tm local_time;
#ifdef _WIN32
	localtime_s(&local_time, &current_time);
#else
	localtime_r(&current_time, &local_time);
#endif

	YY_MM_DD ymd{};
	ymd.year = local_time.tm_year + 1900; // tm_year = years since 1900
	ymd.month = local_time.tm_mon + 1;	  // in range 0 - 11
	ymd.day = local_time.tm_mday;

	return ymd;
}

HH_MM_SS TimePrinter::GetCurrentHMS()
{
	auto now = std::chrono::system_clock::now();
	std::time_t current_time = std::chrono::system_clock::to_time_t(now);
	std::tm local_time;
#ifdef _WIN32
	localtime_s(&local_time, &current_time);
#else
	localtime_r(&current_time, &local_time);
#endif

	HH_MM_SS hms{};
	hms.hours = local_time.tm_hour;
	hms.minutes = local_time.tm_min;
	hms.seconds = local_time.tm_sec;

	return hms;
}

std::string TimePrinter::PrintAmPm()
{
	return (GetCurrentHMS().hours >= 12) ? "PM" : "AM";
}

std::string TimePrinter::PrintHMS()
{
	const auto hms = GetCurrentHMS();
	return fmt::format("{}-{}-{}", hms.hours, hms.minutes, hms.seconds);
}

std::string TimePrinter::PrintHours()
{
	const auto hour = GetCurrentHMS().hours;
	return fmt::format("{:02}", hour);
}

std::string TimePrinter::PrintHours12()
{
	auto hms = GetCurrentHMS();
	const auto hour = hms.hours >= 12 ? hms.hours - 12 : hms.hours;
	return fmt::format("{:02}", hour);
}

std::string TimePrinter::PrintMinutes()
{
	return fmt::format("{:02}", GetCurrentHMS().minutes);
}

std::string TimePrinter::PrintSeconds()
{
	return fmt::format("{:02}", GetCurrentHMS().seconds);
}

std::string TimePrinter::PrintYear()
{
	return std::to_string(GetCurrentYMD().year);
}

std::string TimePrinter::PrintYear2()
{
	return std::to_string(GetCurrentYMD().year % 100);
}

std::string TimePrinter::PrintMonth()
{
	return fmt::format("{:02}", GetCurrentYMD().month);
}

std::string TimePrinter::PrintMonthName()
{
	const unsigned month = static_cast<unsigned>(GetCurrentYMD().month);
	assert(month <= (sizeof(MONTH_NAMES) / sizeof(const char*)));
	return MONTH_NAMES[month];
}

std::string TimePrinter::PrintDay()
{
	return fmt::format("{:02}", GetCurrentYMD().day);
}

std::string TimePrinter::PrintDayName()
{
	const auto weekday = GetCurrentYMD().day;
	assert(static_cast<size_t>(weekday) <= (sizeof(DAY_NAMES) / sizeof(const char*)));
	return DAY_NAMES[weekday];
}

std::string TimePrinter::PrintDate()
{
	return fmt::format("{}-{}-{}", PrintYear(), PrintMonth(), PrintDay());
}

std::string TimePrinter::PrintTime()
{
	return fmt::format("{}-{}-{}", PrintHours(), PrintMinutes(), PrintSeconds());
}
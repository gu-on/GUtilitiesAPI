#include <time_printer.hpp>

#include <cassert>
#include <fmt/core.h>

std::chrono::year_month_day TimePrinter::GetCurrentYMD()
{
	using namespace std::chrono;

	const auto now = system_clock::now();
	const auto currentZone = current_zone();
	const auto zonedTime = zoned_time<std::chrono::system_clock::duration>{currentZone, now}.get_local_time();
	const auto ld = floor<days>(zonedTime);
	const auto ymd = year_month_day{ld};

	return ymd;
}

std::chrono::hh_mm_ss<std::chrono::duration<long long, std::ratio<1, 10000000>>> TimePrinter::GetCurrentHMS()
{
	using namespace std::chrono;

	const auto now = system_clock::now();
	const auto ld = floor<days>(now);
	const auto hms = hh_mm_ss{now - ld};

	return hms;
}

std::chrono::weekday TimePrinter::GetCurrentWeekday()
{
	using namespace std::chrono;

	const auto now = zoned_time{current_zone(), system_clock::now()}.get_local_time();
	const auto ld = floor<days>(now);
	const weekday weekday{ld};

	return weekday;
}

std::string TimePrinter::PrintAmPm()
{
	return is_am(GetCurrentHMS().hours()) ? "AM" : "PM";
}

std::string TimePrinter::PrintHMS()
{
	const auto hms = GetCurrentHMS();
	return fmt::format("{}-{}-{}", hms.hours().count(), hms.minutes().count(), hms.seconds().count());
}

std::string TimePrinter::PrintHours()
{
	const auto hour = GetCurrentHMS().hours().count();
	return fmt::format("{:02}", hour);
}

std::string TimePrinter::PrintHours12()
{
	const auto hour = make12(GetCurrentHMS().hours()).count();
	return fmt::format("{:02}", hour);
}

std::string TimePrinter::PrintMinutes()
{
	const auto minutes = GetCurrentHMS().minutes().count();
	return fmt::format("{:02}", minutes);
}

std::string TimePrinter::PrintSeconds()
{
	const unsigned seconds = static_cast<unsigned>(GetCurrentHMS().seconds().count());
	return fmt::format("{:02}", seconds);
}

std::string TimePrinter::PrintYear()
{
	return std::to_string(static_cast<int>(GetCurrentYMD().year()));
}

std::string TimePrinter::PrintYear2()
{
	return std::to_string(static_cast<int>(GetCurrentYMD().year()) % 100);
}

std::string TimePrinter::PrintMonth()
{
	const unsigned month = static_cast<unsigned>(GetCurrentYMD().month());
	return fmt::format("{:02}", month);
}

std::string TimePrinter::PrintMonthName()
{
	const unsigned month = static_cast<unsigned>(GetCurrentYMD().month());
	assert(month <= (sizeof(MONTH_NAMES) / sizeof(const char*)));
	return MONTH_NAMES[month];
}

std::string TimePrinter::PrintDay()
{
	const unsigned day = static_cast<unsigned>(GetCurrentYMD().day());
	return fmt::format("{:02}", day);
}

std::string TimePrinter::PrintDayName()
{
	const auto weekday = GetCurrentWeekday().c_encoding();
	assert(weekday <= (sizeof(DAY_NAMES) / sizeof(const char*)));
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
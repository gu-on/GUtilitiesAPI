#pragma once

#include <chrono>
#include <string>

constexpr const char* DAY_NAMES[] = {"sun", "mon", "tue", "wed", "thu", "fri", "sat"};
constexpr const char* MONTH_NAMES[] = {"_",	  "jan", "feb", "mar", "apr", "may", "jun",
									   "jul", "aug", "sep", "oct", "nov", "dec"};

class TimePrinter
{
private:
	static constexpr intmax_t MAX = 10'000'000;

public:
	[[nodiscard]] static std::chrono::year_month_day GetCurrentYMD();
	[[nodiscard]] static std::chrono::hh_mm_ss<std::chrono::duration<long long, std::ratio<1, MAX>>> GetCurrentHMS();
	[[nodiscard]] static std::chrono::weekday GetCurrentWeekday();

	[[nodiscard]] static std::string PrintAmPm();
	[[nodiscard]] static std::string PrintHMS();
	[[nodiscard]] static std::string PrintHours();
	[[nodiscard]] static std::string PrintHours12();
	[[nodiscard]] static std::string PrintMinutes();
	[[nodiscard]] static std::string PrintSeconds();
	[[nodiscard]] static std::string PrintYear();
	[[nodiscard]] static std::string PrintYear2();
	[[nodiscard]] static std::string PrintMonth();
	[[nodiscard]] static std::string PrintMonthName();
	[[nodiscard]] static std::string PrintDay();
	[[nodiscard]] static std::string PrintDayName();
	[[nodiscard]] static std::string PrintDate();
	[[nodiscard]] static std::string PrintTime();
};

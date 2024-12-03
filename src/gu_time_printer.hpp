#pragma once

#include <chrono>
#include <string>

constexpr const char* DAY_NAMES[] = {"sun", "mon", "tue", "wed", "thu", "fri", "sat"};
constexpr const char* MONTH_NAMES[] = {"_",	  "jan", "feb", "mar", "apr", "may", "jun",
									   "jul", "aug", "sep", "oct", "nov", "dec"};

struct HH_MM_SS
{
	int hours{};
	int minutes{};
	int seconds{};
};

struct YY_MM_DD
{
	int year{};
	int month{};
	int day{};
};

class TimePrinter
{
private:
	static constexpr intmax_t MAX = 10'000'000;

public:
	[[nodiscard]] static std::tm GetCurrentTime();
	[[nodiscard]] static YY_MM_DD GetCurrentYMD();
	[[nodiscard]] static HH_MM_SS GetCurrentHMS();
	[[nodiscard]] static int GetCurrentDayOfTheWeek();

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

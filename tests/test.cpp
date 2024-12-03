#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <cmath>
#include <fstream>
#include <iostream>
#include <string>

#include "gu_ini_file.hpp"
#include "gu_maths.hpp"

TEST_CASE("Check if string is a natural number")
{
	CHECK(Maths::IsNaturalNumber("0"));
	CHECK(Maths::IsNaturalNumber("2"));
	CHECK_FALSE(Maths::IsNaturalNumber("-1"));
	CHECK_FALSE(Maths::IsNaturalNumber("0.5f"));
	CHECK_FALSE(Maths::IsNaturalNumber("abc"));
	CHECK_FALSE(Maths::IsNaturalNumber("123xyz"));
}

TEST_CASE("Check that DB to volume conversions are correct")
{
	CHECK(Maths::DB2VOL(0.0) == 1.0);
	CHECK(Maths::VOL2DB(1.0) == 0.0);
	CHECK(std::round(Maths::VOL2DB(0.5)) == -6.0); // apprxomiately -6dB
}

TEST_CASE("Check INI File")
{
	using namespace std;

	const string fileName = "testing";
	const string category = "fruit";
	const string key = "tasty";
	const string value = "yes";
	bool success = false;

	INIFile ini{fileName};
	success = ini.Write(category, key, value);
	CHECK(success);

	std::string valueOut{};
	success = ini.Read(category, key, valueOut);

	CHECK(success);
	CHECK(value == valueOut);

	success = ini.Delete();
	CHECK(success);
}
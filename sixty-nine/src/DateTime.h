#pragma once

#include <string>
#include <stdint.h>

struct DateTime
{
	uint32_t Day = -1, Month = -1, Year = -1;
	uint32_t Hour = -1, Minute = -1;

	DateTime(uint32_t day, uint32_t month, uint32_t year, uint32_t hour, uint32_t minute)
		: Day(day), Month(month), Year(year), Hour(hour), Minute(minute)
	{}

	DateTime(uint32_t day, uint32_t month, uint32_t year)
		: Day(day), Month(month), Year(year)
	{}

	DateTime(uint32_t hour, uint32_t minute)
		: Hour(hour), Minute(minute)
	{}

	~DateTime() = default;
	DateTime(const DateTime&) = default;

	std::string GetTime() const
	{

	}

	std::string GetDateTime() const
	{

	}
};
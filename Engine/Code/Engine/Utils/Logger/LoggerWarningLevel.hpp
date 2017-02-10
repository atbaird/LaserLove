#pragma once

#ifndef LOGGERWARNINGLEVEL_HPP
#define LOGGERWARNINGLEVEL_HPP

enum LoggerWarningLevels
{
	W0 = 0, //disable all warnings
	W1, //displays level 1 (severe) warnings / W1 is default
	W2, //level 1 and level 2 (significant warnings)
	W3, //displays level 1, level 2, and level 3 (production quality) warnings
	W4, //displays level 1, level 2, level 3, and level 4 (informational) warnings that are not turned off by default
		//use this option only to provide lint-like warnings.
	NUM_OF_LOGGER_WARNINGS
};

#endif
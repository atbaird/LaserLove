#pragma once

#ifndef MIN_HPP
#define MIN_HPP

template <typename T>
static T Min(T const &a, T const &b)
{
	if (a < b) {
		return a;
	}
	else {
		return b;
	}
}

#endif
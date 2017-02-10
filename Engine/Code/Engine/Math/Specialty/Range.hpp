#pragma once

#ifndef RANGE_HPP
#define RANGE_HPP

template <typename T>
class Range
{
public:
	T _min_value;
	T _max_value;

	Range(T const &a)
		: _min_value(a),
		_max_value(a) {}

	Range(T const &a, T const &b)
	{
		_min_value = Min(a, b);
		_max_value = Max::Max(a, b);
	}

	T Lerp(T min, T max, float percentage)
	{
		T diff = max - min;
		return min + (diff * percentage);
	}

	T Get(float const t)
	{
		return Lerp(_min_value, _max_value, t);
	}

	static float RandomRange01()
	{
		float ran = ((float)rand() / (RAND_MAX)) + 1;
		return ran;
	}

	T Roll()
	{
		return Get(RandomRange01());
	}

	operator T()
	{
		return Roll();
	}
};

#endif
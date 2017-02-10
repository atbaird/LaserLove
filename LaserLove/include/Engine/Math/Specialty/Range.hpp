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
		_max_value = Max(a, b);
	}

	T Get(float const t)
	{
		return Lerp(a, b, t);
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
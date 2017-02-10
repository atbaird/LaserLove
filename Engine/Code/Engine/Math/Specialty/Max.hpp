#pragma once

#ifndef MAX_HPP
#define MAX_HPP

namespace Max
{
	template <typename T>
	static T Max(T const &a, T const &b)
	{
		if (a > b) {
			return a;
		}
		else {
			return b;
		}
	}
}



#endif
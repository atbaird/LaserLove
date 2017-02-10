#pragma once

#ifndef CLASSESARESAME_HPP
#define CLASSESARESAME_HPP

template<typename T_val1, typename T_val2>
struct ClassesAreSame
{
	bool val = false;
	ClassesAreSame() {};
};

template<typename T_val1>
struct ClassesAreSame<T_val1, T_val1>
{
	bool val = true;
	ClassesAreSame() {};
};

#endif
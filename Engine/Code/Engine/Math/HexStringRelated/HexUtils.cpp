#include "Engine/Math/HexStringRelated/HexUtils.hpp"


int ConvertOneOrTwoCharHexStrToInt(std::string str)
{
	if (str.length() <= 0 || str.length() > 2)
	{
		//if greater than length 2, then you broke the rules.
		return 0;
	}


	int val = ConvertCharHexValueToInt(str[0]);
	if (str.length() > 1)
	{
		val *= 16;
		val += ConvertCharHexValueToInt(str[1]);
	}
	return val;
}


int ConvertCharHexValueToInt(char val)
{
	if (val == '0')
	{
		return 0;
	}
	else if (val == '1')
	{
		return 1;
	}
	else if (val == '2')
	{
		return 2;
	}
	else if (val == '3')
	{
		return 3;
	}
	else if (val == '4')
	{
		return 4;
	}
	else if (val == '5')
	{
		return 5;
	}
	else if (val == '6')
	{
		return 6;
	}
	else if (val == '7')
	{
		return 7;
	}
	else if (val == '8')
	{
		return 8;
	}
	else if (val == '9')
	{
		return 9;
	}
	else if (val == 'a' || val == 'A')
	{
		return 10;
	}
	else if (val == 'b' || val == 'B')
	{
		return 11;
	}
	else if (val == 'c' || val == 'C')
	{
		return 12;
	}
	else if (val == 'd' || val == 'D')
	{
		return 13;
	}
	else if (val == 'e' || val == 'E')
	{
		return 14;
	}
	else if (val == 'f' || val == 'F')
	{
		return 15;
	}
	return 0;
}
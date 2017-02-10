#include "Engine/Core/StringUtils.hpp"
#include <stdarg.h>
#include <sstream>
#include <algorithm>
#include <regex>
#include <sys/stat.h>


//-----------------------------------------------------------------------------------------------
const int STRINGF_STACK_LOCAL_TEMP_LENGTH = 2048;


//-----------------------------------------------------------------------------------------------
const std::string Stringf( const char* format, ... )
{
	char textLiteral[ STRINGF_STACK_LOCAL_TEMP_LENGTH ];
	va_list variableArgumentList;
	va_start( variableArgumentList, format );
	vsnprintf_s( textLiteral, STRINGF_STACK_LOCAL_TEMP_LENGTH, _TRUNCATE, format, variableArgumentList );	
	va_end( variableArgumentList );
	textLiteral[ STRINGF_STACK_LOCAL_TEMP_LENGTH - 1 ] = '\0'; // In case vsnprintf overran (doesn't auto-terminate)

	return std::string( textLiteral );
}


//-----------------------------------------------------------------------------------------------
const std::string Stringf( const int& maxLength, const char* format, ... )
{
	char textLiteralSmall[ STRINGF_STACK_LOCAL_TEMP_LENGTH ];
	char* textLiteral = textLiteralSmall;
	if( maxLength > STRINGF_STACK_LOCAL_TEMP_LENGTH )
		textLiteral = new char[ maxLength ];

	va_list variableArgumentList;
	va_start( variableArgumentList, format );
	vsnprintf_s( textLiteral, maxLength, _TRUNCATE, format, variableArgumentList );	
	va_end( variableArgumentList );
	textLiteral[ maxLength - 1 ] = '\0'; // In case vsnprintf overran (doesn't auto-terminate)

	std::string returnValue( textLiteral );
	if( maxLength > STRINGF_STACK_LOCAL_TEMP_LENGTH )
		delete[] textLiteral;

	return returnValue;
}

//-------------------------------------------------------------------------------------------------

const std::vector<std::string> ParseString(const std::string& str, const char& splitBy)
{
	std::string strTemp = str;
	std::vector<std::string> strings;
	//std::stringstream ss = std::stringstream(strTemp);
	
	size_t lastIndexHit = 0;
	for (size_t i = 0; i < str.length(); i++)
	{
		char curChar = str.at(i);
		if (curChar == splitBy)
		{
			std::string word = str.substr(lastIndexHit, i - lastIndexHit);
			if (SimpleStrCmp(word, "") == false)
			{
				strings.push_back(word);
			}
			lastIndexHit = i + 1;
		}
	}
	if (lastIndexHit != str.length())
	{
		strings.push_back(str.substr(lastIndexHit, str.length() - lastIndexHit));
	}
	//while (ss >> temp)
	//{
	//	strings.push_back(temp);
	//	temp = "";
	//}

	return strings;
}

const std::string ReplaceCharInString(const std::string& original, const char& toReplace, const char& replaceBy)
{
	std::string copy = "";

	for (size_t i = 0; i < original.size(); i++)
	{
		if (SimpleStrCmp(std::to_string(original.at(i)), std::to_string(toReplace)))
		{
			copy += replaceBy;
		}
		else
		{
			copy += original.at(i);
		}
	}
	copy += '\0';

	return copy;
}
const std::string ReplaceStringWithString(const std::string& original, const std::string& toReplace, const std::string& replaceBy)
{
	if (original.size() < toReplace.size())
	{
		return original;
	}
	std::string toReturn = original;
	for (size_t i = 0; i < toReturn.size() - toReplace.size(); i++)
	{
		std::string cur = toReturn.substr(i, toReplace.size());
		if (strcmp(cur.c_str(), toReplace.c_str()) == 0)
		{
			toReturn = toReturn.substr(0, i) + replaceBy + toReturn.substr(i + toReplace.size(), toReturn.size());
		}
	}
	return toReturn;
}


//------------------------------------------------------------------------------------------------
const std::string StartFromBackAndChopOffAfterEncounterChar(const std::string& str, const char& chopOff)
{
	if (str.size() == 0)
	{
		return str;
	}
	std::string chopped;
	for (int i = (int)str.size() - 1; i >= 0; i--)
	{
		char a = str.at(i);
		if (strcmp(&a, &chopOff) == 0)
		{
			break;
		}
		else
		{
			chopped = a + chopped;
		}
	}
	return chopped;
}

const std::string StartFromFrontAndChopOffAfterEncounterChar(const std::string& str, const char& chopOff)
{
	if (str.size() == 0)
	{
		return str;
	}

	std::string chopped;
	for (int i = 0; i < (int)str.size(); i++)
	{
		char a = str.at(i);
		if (strcmp(&a, &chopOff) == 0)
		{
			break;
		}
		else
		{
			chopped = chopped + a;
		}
	}
	return chopped;
}

//-------------------------------------------------------------------------------------------------
const std::string MakeLower(const std::string& str)
{
	std::string copy = str;
	for (size_t i = 0; i < copy.length(); i++)
	{
		unsigned char car = copy.at(i);
		copy[i] = unsigned char(tolower(int(car)));
	}
	return copy;
}

//-------------------------------------------------------------------------------------------------

const std::string ToString(const int& intVal)
{
	int copy = intVal;
	std::string strval;
	if (copy == 0)
	{
		strval = "0";
	}
	while (copy > 0)
	{
		int remainder = copy % 10;
		switch (remainder)
		{
		case 0:
			strval = "0" + strval;
			break;
		case 1:
			strval = "1" + strval;
			break;
		case 2:
			strval = "2" + strval;
			break;
		case 3:
			strval = "3" + strval;
			break;
		case 4:
			strval = "4" + strval;
			break;
		case 5:
			strval = "5" + strval;
			break;
		case 6:
			strval = "6" + strval;
			break;
		case 7:
			strval = "7" + strval;
			break;
		case 8:
			strval = "8" + strval;
			break;
		case 9:
			strval = "9" + strval;
			break;
		default:
			break;
		}
		copy = copy / 10;
	}
	return strval;
}
const bool SimpleStrCmp(const std::string& str1, const std::string& str2)
{
	if (strcmp(str1.c_str(), str2.c_str()) == 0)
	{
		return true;
	}
	return false;
}
const bool SimpleStrCmpLower(const std::string& str1, const std::string& str2)
{
	std::string lstr1 = MakeLower(str1);
	std::string lstr2 = MakeLower(str2);
	return SimpleStrCmp(lstr1, lstr2);
}


//-----------------------------------------------------------------------------------------------------
void StringCopy(char* copyTo, const size_t& numOfCharsToCopyUpTo, const char* toCopy)
{
	if (toCopy == nullptr)
	{
		return;
	}
	size_t len = strlen(toCopy);
	for (size_t i = 0; i < numOfCharsToCopyUpTo && i < len; i++)
	{
		char temp = toCopy[i];
		copyTo[i] = temp;
		//strcpy_s(&copyTo[i], 1, &toCopy[i]);
	}
}

//------------------------------------------------------------------------------------------------
const bool VerifyDirectoryExists(const std::string& directoryPath)
{
	struct stat sb;
	if (stat(directoryPath.c_str(), &sb) != 0)
	{
		return false;
	}
	else if (sb.st_mode & S_IFDIR)
	{
		return true;
	}
	else
	{
		return false;
	}
}


const int GetCountOfCharInstanceInString(const std::string& str, const char car)
{
	int count = 0;
	std::string c2 = "" + car;
	for (size_t i = 0; i < str.length(); i++)
	{
		std::string c1 = "" + str[i];
		if (strcmp(c1.c_str(), c2.c_str()) == 0)
		{
			count++;
		}
	}
	return count;
}

const bool DoesStringHaveThisChar(const std::string& str, const char car)
{
	std::string c2 = "" + car;
	for (size_t i = 0; i < str.length(); i++)
	{
		std::string c1 = "" + str[i];
		if (strcmp(c1.c_str(), c2.c_str()) == 0)
		{
			return true;
		}
	}
	return false;
}

const bool DoesStringHaveThisCharAndStopIfSeeOtherChar(const std::string& str, const char car, const char stopHere)
{
	std::string c2 = "" + car;
	std::string c3 = "" + stopHere;
	for (size_t i = 0; i < str.length(); i++)
	{
		std::string c1 = "" + str[i];
		if (strcmp(c1.c_str(), c2.c_str()) == 0)
		{
			return true;
		}
		if (strcmp(c1.c_str(), c3.c_str()) == 0)
		{
			return false;
		}
	}
	return false;
}


std::string IterateInReverseUntilHitChar(const std::string& str, const char car, bool includeStoppingChar)
{
	std::string strOut = "";
	char charToCareAbout[2];
	charToCareAbout[0] = car;
	charToCareAbout[1] = '\0';

	char currentChar[2];
	currentChar[0] = '\0';
	currentChar[1] = '\0';
	for (int i = str.length() - 1; i >= 0; i--)
	{
		currentChar[0] = str[i];
		if (strcmp(charToCareAbout, currentChar) == 0)
		{
			if (includeStoppingChar)
			{
				strOut = str[i] + strOut;
			}
			return strOut;
		}
		strOut = str[i] + strOut;
	}

	return strOut;
}


std::string IterateInReverseUntilHitNthChar(const std::string& str, const char car, int numToHit)
{
	std::string strOut = "";
	int encounterAmt = 0;
	char charToCareAbout[2];
	charToCareAbout[0] = car;
	charToCareAbout[1] = '\0';

	char currentChar[2];
	currentChar[0] = '\0';
	currentChar[1] = '\0';

	for (int i = str.length()- 1; i >= 0; i--)
	{
		currentChar[0] = str[i];
		if (strcmp(charToCareAbout, currentChar) == 0)
		{
			encounterAmt++;
			if (encounterAmt >= numToHit)
			{
				return strOut;
			}
		}
		strOut = str[i] + strOut;
	}

	return strOut;
}


int GetNthEncounterIdxOfCharInStr(const std::string& str, const char car, int numToHit)
{
	char charToCareAbout[2];
	charToCareAbout[0] = car;
	charToCareAbout[1] = '\0';

	char currentChar[2];
	currentChar[0] = '\0';
	currentChar[1] = '\0';
	int numEncountered = 0;

	for (size_t i = 0; i < str.length(); i++)
	{
		currentChar[0] = str[i];
		if (strcmp(charToCareAbout, currentChar) == 0)
		{
			numEncountered++;
			if (numEncountered >= numToHit)
			{
				return (int)i;
			}
		}
	}

	return -1;
}

int GetNthEncounterIdxOfCharInStrReverse(const std::string& str, const char car, int numToHit)
{
	char charToCareAbout[2];
	charToCareAbout[0] = car;
	charToCareAbout[1] = '\0';

	char currentChar[2];
	currentChar[0] = '\0';
	currentChar[1] = '\0';
	int numEncountered = 0;

	for (size_t i = str.length() - 1; i >= 0; i--)
	{
		currentChar[0] = str[i];
		if (strcmp(charToCareAbout, currentChar) == 0)
		{
			numEncountered++;
			if (numEncountered >= numToHit)
			{
				return (int)i;
			}
		}
	}
	return -1;
}
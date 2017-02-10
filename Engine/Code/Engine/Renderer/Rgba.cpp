#include "Engine/Renderer/Rgba.hpp"
#include <sstream>

//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
//Static Variables
const Rgba Rgba::s_White = Rgba(1.f,1.f,1.f,1.f);
const Rgba Rgba::s_Black = Rgba(0.f, 0.f, 0.f, 1.f);
const Rgba Rgba::s_Blue = Rgba(0.f, 0.f, 1.f, 1.f);
const Rgba Rgba::s_Gray = Rgba(0.5f, 0.5f, 0.5f, 1.f);
const Rgba Rgba::s_Gray6 = Rgba(0.6f, 0.6f, 0.6f, 1.f);
const Rgba Rgba::s_lightGray = Rgba(0.7f, 0.7f, 0.7f, 1.f);
const Rgba Rgba::s_Red = Rgba(1.f, 0.f, 0.f, 1.f);
const Rgba Rgba::s_Green = Rgba(0.f, 1.f, 0.f, 1.f);
const Rgba Rgba::s_Yellow = Rgba(1.f, 1.f, 0.f, 1.f);

//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
//constructors
Rgba::Rgba(const float& red, const float& green, const float& blue, const float& alpha)
{
	//constructor 2
	Red = red;
	Green = green;
	Blue = blue;
	Alpha = alpha;
}

Rgba::Rgba(const Vector3& rgb, const float& alpha)
{
	Red = rgb.x;
	Green = rgb.y;
	Blue = rgb.z;
	Alpha = alpha;
}

//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
//setters
void Rgba::SetRGBA(const float& in_r, const float& in_g, const float& in_b, const float& in_a)
{
	//sets Red, Green, Blue and Alpha values
	Red = in_r;
	Green = in_g;
	Blue = in_b;
	Alpha = in_a;
}

void Rgba::SetRGB(const float& in_r, const float& in_g, const float& in_b)
{
	//sets just the Red, Green and Blue colors
	Red = in_r;
	Green = in_g;
	Blue = in_b;
}
void Rgba::setAlpha(const float& in_a)
{
	//sets the alpha
	Alpha = in_a;
}

//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
//getters
void Rgba::GetRGBA(float& out_r, float& out_g, float& out_b, float& out_a) const
{
	out_r = Red;
	out_g = Green;
	out_b = Blue;
	out_a = Alpha;
}

const std::string Rgba::ToString() const
{
	std::string red = "";
	std::string green = "";
	std::string blue = "";
	std::string alpha = "";
	std::stringstream str("");
	str << Red; //float
	str >> red; //string
	str = std::stringstream("");
	str << Green; //float
	str >> green; //string
	str = std::stringstream("");
	str << Blue; //float
	str >> blue; //string
	str = std::stringstream("");
	str << Alpha; //float
	str >> alpha; //string
	std::string toReturn = "color(";
	toReturn += (red + ",");
	toReturn += (green + ",");
	toReturn += (blue + ",");
	toReturn += (alpha + ") ");
	return toReturn;
}
const Vector3 Rgba::GetRGB() const
{
	return Vector3(Red, Green, Blue);
}



//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
//operators
const bool Rgba::operator==(const Rgba& other) const
{
	if (Red == other.Red
		&& Green == other.Green
		&& Blue == other.Blue
		&& Alpha == other.Alpha)
	{
		return true;
	}
	return false;
}
const bool Rgba::operator!=(const Rgba& other) const
{
	if (Red != other.Red
		|| Green != other.Green
		|| Blue != other.Blue
		|| Alpha != other.Alpha)
	{
		return true;
	}
	return false;
}


const Rgba Rgba::operator-(const Rgba& other) const
{
	return Rgba(Red - other.Red, Green - other.Green, Blue - other.Blue, Alpha - other.Alpha);
}

const Rgba Rgba::operator+(const Rgba& other) const
{
	return Rgba(Red + other.Red, Green + other.Green, Blue + other.Blue, Alpha + other.Alpha);
}

const Rgba Rgba::operator*(const float& mult) const
{
	return Rgba(Red * mult, Green * mult, Blue * mult, Alpha * mult);
}

const Rgba Rgba::operator/(const float& div) const
{
	return Rgba(Red / div, Green / div, Blue / div, Alpha / div);
}

//------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------
//Static Functions
STATIC int Rgba::ConvertPercentageFloatTo255Int(float percentage)
{
	//This function does not clamp!
	float roundedOutVal = round((percentage) * 255.f);
	return (int)roundedOutVal;
}

STATIC float Rgba::Convert255IntToPercentageFloat(int val)
{
	//This function does not clamp!
	float outVal = ((float)val) / 255.f;
	return outVal;
}

int Rgba::ClampedConvertPercentageFloatTo255Int(float percentage)
{
	int val = ConvertPercentageFloatTo255Int(percentage);
	if (val < 0)
	{
		val = 0;
	}
	else if (val > 255)
	{
		val = 255;
	}
	return val;
}

float Rgba::ClampedConvert255IntToPercentageFloat(int val)
{
	float percentage = Convert255IntToPercentageFloat(val);
	if (percentage < 0.f)
	{
		percentage = 0.f;
	}
	else if (percentage > 1.f)
	{
		percentage = 1.f;
	}
	return percentage;
}


#include "Engine/Core/StringUtils.hpp"
#include "Engine/Math/HexStringRelated/HexUtils.hpp"
Rgba ReadColorOutOfString(const std::string& str)
{
	Rgba val;
	std::string clearedString = ReplaceCharInString(str, ',', ' ');
	std::vector<std::string> parsedString = ParseString(clearedString, ' ');
	if (parsedString.size() > 0)
	{
		std::string valTesting = parsedString[0];
		int hitAPeriod = GetNthEncounterIdxOfCharInStr(valTesting, '.', 1);
		if (hitAPeriod != -1)
		{
			//Assume it's a percentage!
			val.Red = (float)atof(valTesting.c_str());
		}
		else
		{
			int hitAHash = GetNthEncounterIdxOfCharInStr(valTesting, '#', 1);
			int hitAnx = GetNthEncounterIdxOfCharInStr(valTesting, 'x', 1); //lower case
			int hitAnX = GetNthEncounterIdxOfCharInStr(valTesting, 'X', 1); //upper case
			if (hitAHash != -1
				|| hitAnX != -1
				|| hitAnx != -1)
			{
				//Its a hex value.
				int commonHitForHex = -1;
				if (hitAHash != -1)
				{
					commonHitForHex = hitAHash;
				}
				if (hitAnX != -1)
				{
					commonHitForHex = hitAnX;
				}
				if (hitAnx != -1)
				{
					commonHitForHex = hitAnx;
				}
				std::string hexStr = valTesting.substr(commonHitForHex + 1, valTesting.length() - (commonHitForHex + 1));
				std::string redHex = hexStr.substr(0, 2);
				std::string greenHex = hexStr.substr(2, 2);
				std::string blueHex = hexStr.substr(4, 2);
				std::string alphaHex = hexStr.substr(6, 2);
				if (hexStr.length() > 0)
				{
					int valTest = ConvertOneOrTwoCharHexStrToInt(redHex);
					val.Red = Rgba::ClampedConvert255IntToPercentageFloat(valTest);
					
				}
				else
				{
					val.Red = 0.f;
				}

				if (hexStr.length() > 2)
				{
					int valTest = ConvertOneOrTwoCharHexStrToInt(greenHex);
					val.Green = Rgba::ClampedConvert255IntToPercentageFloat(valTest);
				}
				else
				{
					val.Green = 0.f;
				}
				if (hexStr.length() > 4)
				{
					int valTest = ConvertOneOrTwoCharHexStrToInt(blueHex);
					val.Blue = Rgba::ClampedConvert255IntToPercentageFloat(valTest);
				}
				else
				{
					val.Blue = 0.f;
				}
				if (hexStr.length() > 6)
				{
					int valTest = ConvertOneOrTwoCharHexStrToInt(alphaHex);
					val.Alpha = Rgba::ClampedConvert255IntToPercentageFloat(valTest);
				}
				else
				{
					val.Alpha = 1.f;
				}

			}
			else
			{
				int valTest = atoi(valTesting.c_str());
				val.Red = Rgba::ClampedConvert255IntToPercentageFloat(valTest);
			}
		}
	}
	if (parsedString.size() > 1)
	{
		// can safely assume not Hex.
		std::string valTesting = parsedString[1];
		int hitAPeriod = GetNthEncounterIdxOfCharInStr(valTesting, '.', 1);
		if (hitAPeriod != -1)
		{
			//Assume it's a percentage!
			val.Green = (float)atof(valTesting.c_str());
		}
		else
		{
			int valTest = atoi(valTesting.c_str());
			val.Green = Rgba::ClampedConvert255IntToPercentageFloat(valTest);
		}
	}
	if (parsedString.size() > 2)
	{
		// can safely assume not Hex.
		std::string valTesting = parsedString[2];
		int hitAPeriod = GetNthEncounterIdxOfCharInStr(valTesting, '.', 1);
		if (hitAPeriod != -1)
		{
			//Assume it's a percentage!
			val.Blue = (float)atof(valTesting.c_str());
		}
		else
		{
			int valTest = atoi(valTesting.c_str());
			val.Blue = Rgba::ClampedConvert255IntToPercentageFloat(valTest);
		}
	}
	if (parsedString.size() > 3)
	{
		// can safely assume not Hex.
		std::string valTesting = parsedString[3];
		int hitAPeriod = GetNthEncounterIdxOfCharInStr(valTesting, '.', 1);
		if (hitAPeriod != -1)
		{
			//Assume it's a percentage!
			val.Alpha = (float)atof(valTesting.c_str());
		}
		else
		{
			int valTest = atoi(valTesting.c_str());
			val.Alpha = Rgba::ClampedConvert255IntToPercentageFloat(valTest);
		}
	}


	return val;
}
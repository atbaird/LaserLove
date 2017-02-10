#pragma once

#ifndef STRXMLATTR_HPP
#define STRXMLATTR_HPP
#include <string>

struct XMLAttribute;
struct StrXMLAttr
{
public:
	//Variables
	std::string name = "";
	std::string val = "";

	//Constructors
	StrXMLAttr(const std::string& nam = "", const std::string& va = "");
	StrXMLAttr(XMLAttribute attr);
	virtual ~StrXMLAttr();
};

#endif
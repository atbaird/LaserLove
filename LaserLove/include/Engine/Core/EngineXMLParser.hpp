#pragma once

#ifndef XMLPARSER_HPP
#define XMLPARSER_HPP

#include <string>
#include <vector>
#include "Thirdparty/xmlParser/xmlParser.h"


struct StrXMLAttr
{
	StrXMLAttr(const std::string& nam = "", const std::string& va = "");
	StrXMLAttr(XMLAttribute attr);
	std::string name;
	std::string val;
};


class EngineXMLParser
{
public:
	EngineXMLParser();
	~EngineXMLParser();

	static const XMLNode ParseXMLFile(const std::string& xmlFileLocation, const char* tag, bool& errorOccurred);
	static const XMLNode TraverseDownNodeChildren(XMLNode node, std::vector<std::string> nodeNames, bool& successfullyFound);

private:
};

#endif

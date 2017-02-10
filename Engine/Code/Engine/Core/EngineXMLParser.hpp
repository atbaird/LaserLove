#pragma once

#ifndef XMLPARSER_HPP
#define XMLPARSER_HPP

#include <string>
#include <vector>
#include "Thirdparty/xmlParser/xmlParser.h"
#include "Engine/Core/XMLRelated/XMLNodeWrapper.hpp"

#define STATIC


class EngineXMLParser
{
public:
	EngineXMLParser();
	~EngineXMLParser();

	static const XMLNode ParseXMLFile(const std::string& xmlFileLocation, const char* tag, bool& errorOccurred);
	static const XMLNode TraverseDownNodeChildren(const XMLNode& node, std::vector<std::string> nodeNames, bool& successfullyFound);
	static XMLNodeWrapper* WrapUpNodeAndRegister(const XMLNode& node, int idToListenForWhenUnregistering = 0);
	static void CleanUpAndRemoveAllRegisteredWrapperNodes();
	static void CleanUpAndRemoveAllRegisteredWrapperNodesBasedOnID(int id);
	static std::vector<StrXMLAttr> GetAttributesOffOfNode(const XMLNode& node);
	static std::string GetAttributeOffNodeAsString(const XMLNode& node, const char* attributeName, const std::string& defaultVal = "");

private:
};

#endif

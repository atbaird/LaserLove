#pragma once

#ifndef XMLNODEWRAPPER_HPP
#define XMLNODEWRAPPER_HPP
#include "Engine/Core/XMLRelated/StrXMLAttr.hpp"
#include <vector>
#define STATIC
struct XMLNode;
struct XMLNodeWrapper
{
private:
protected:
	//Static Variables
	static std::vector<XMLNodeWrapper> s_AllXMLNodeWrappersCurrentlyExisting;

	//Variables
	std::vector<StrXMLAttr> m_attributes;
	std::vector<XMLNodeWrapper*> m_children;
	XMLNodeWrapper* m_parent = nullptr;
	std::string m_name = "";
	int m_IDWaitingForInOrderToClear = 0;
public:
	//Constructors
	XMLNodeWrapper();
	XMLNodeWrapper(const XMLNode& node);
	virtual ~XMLNodeWrapper();

	//Setters
	void AddChild(XMLNodeWrapper* child);
	void RemoveChild(XMLNodeWrapper* child);
	void SetParent(XMLNodeWrapper* parent);
	void SetName(const std::string& name);
	void SetIDToListenForWhenClaring(int id);
	void AddOrSetAttribute(const StrXMLAttr& attr);
	bool SetAttribute(const StrXMLAttr& attr);
	void AddAttribute(const StrXMLAttr& attr);

	//Getters
	const XMLNodeWrapper* GetParent() const;
	XMLNodeWrapper* GetEditableParent();
	const XMLNodeWrapper* GetChildAtIdx(int idx) const;
	XMLNodeWrapper* GetEditableChildAtIdx(int idx);
	const StrXMLAttr* GetAttributeAtIdx(int idx) const;
	StrXMLAttr* GetEditableAttributeAtIdx(int idx);
	size_t GetNumberOfChildren() const;
	size_t GetNumberOfAttributes() const;
	std::string GetName() const;
	int GetIDWaitingForInOrderToClear() const;

	//Functions
	static XMLNodeWrapper* FullyRegisterXMLNodeIntoTree(const XMLNode& node, int idToListenForWhenUnregistering = 0);
	static void EmptyAllRegisteredXMLNodes();
	static void EmptyAllRegisteredXMLNodesThatHaveID(int id);
};

#endif
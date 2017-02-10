#include "Engine/Core/XMLRelated/XMLNodeWrapper.hpp"
#include "Thirdparty/xmlParser/xmlParser.h"

//-------------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------
//Static Variables
STATIC std::vector<XMLNodeWrapper> XMLNodeWrapper::s_AllXMLNodeWrappersCurrentlyExisting;

//-------------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------
//Constructors
XMLNodeWrapper::XMLNodeWrapper()
{

}

XMLNodeWrapper::XMLNodeWrapper(const XMLNode& node)
{
	m_name = std::string(node.getName());

	for (int attrIdx = 0; attrIdx < node.nAttribute(); attrIdx++)
	{
		XMLAttribute nodeAttr = node.getAttribute(attrIdx);
		StrXMLAttr attr;
		attr.name = std::string(nodeAttr.lpszName);
		attr.val = std::string(nodeAttr.lpszValue);
		m_attributes.push_back(attr);
	}
}

XMLNodeWrapper::~XMLNodeWrapper()
{
}


//-------------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------
//Setters
void XMLNodeWrapper::AddChild(XMLNodeWrapper* child)
{
	if (child == nullptr)
	{
		return;
	}
	m_children.push_back(child);
}

void XMLNodeWrapper::RemoveChild(XMLNodeWrapper* child)
{
	for (size_t childIdx = 0; childIdx < m_children.size(); childIdx++)
	{
		if (m_children[childIdx] == child)
		{
			m_children.erase(m_children.begin() + childIdx);
			return;
		}
	}
}

void XMLNodeWrapper::SetParent(XMLNodeWrapper* parent)
{
	m_parent = parent;
}

void XMLNodeWrapper::SetName(const std::string& name)
{
	m_name = name;
}

void XMLNodeWrapper::SetIDToListenForWhenClaring(int id)
{
	m_IDWaitingForInOrderToClear = id;
}

void XMLNodeWrapper::AddOrSetAttribute(const StrXMLAttr& attr)
{
	if (SetAttribute(attr) == false)
	{
		AddAttribute(attr);
	}
}

bool XMLNodeWrapper::SetAttribute(const StrXMLAttr& attr)
{
	for (size_t attrIdx = 0; attrIdx < m_attributes.size(); attrIdx++)
	{
		StrXMLAttr* curAttr = &m_attributes[attrIdx];
		if (attr.name == curAttr->name)
		{
			curAttr->val = attr.val;
			return true;
		}
	}
	return false;
}

void XMLNodeWrapper::AddAttribute(const StrXMLAttr& attr)
{
	m_attributes.push_back(attr);
}

//-------------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------
//Getters
const XMLNodeWrapper* XMLNodeWrapper::GetParent() const
{
	return m_parent;
}

XMLNodeWrapper* XMLNodeWrapper::GetEditableParent()
{
	return m_parent;
}

const XMLNodeWrapper* XMLNodeWrapper::GetChildAtIdx(int idx) const
{
	if (idx < 0 || (size_t)idx >= m_children.size())
	{
		return nullptr;
	}
	return m_children[idx];
}

XMLNodeWrapper* XMLNodeWrapper::GetEditableChildAtIdx(int idx)
{
	if (idx < 0 || (size_t)idx >= m_children.size())
	{
		return nullptr;
	}
	return m_children[idx];
}

const StrXMLAttr* XMLNodeWrapper::GetAttributeAtIdx(int idx) const
{
	if (idx < 0 || (size_t)idx >= m_attributes.size())
	{
		return nullptr;
	}
	return &m_attributes[idx];
}

StrXMLAttr* XMLNodeWrapper::GetEditableAttributeAtIdx(int idx)
{
	if (idx < 0 || (size_t)idx >= m_attributes.size())
	{
		return nullptr;
	}
	return &m_attributes[idx];
}

size_t XMLNodeWrapper::GetNumberOfChildren() const
{
	return m_children.size();
}

size_t XMLNodeWrapper::GetNumberOfAttributes() const
{
	return m_attributes.size();
}

std::string XMLNodeWrapper::GetName() const
{
	return m_name;
}

int XMLNodeWrapper::GetIDWaitingForInOrderToClear() const
{
	return m_IDWaitingForInOrderToClear;
}

//-------------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------
//Functions
STATIC XMLNodeWrapper* XMLNodeWrapper::FullyRegisterXMLNodeIntoTree(const XMLNode& node, int idToListenForWhenUnregister)
{
	XMLNodeWrapper wrapper(node);
	size_t idxAt = s_AllXMLNodeWrappersCurrentlyExisting.size();
	s_AllXMLNodeWrappersCurrentlyExisting.push_back(wrapper);
	XMLNodeWrapper* wrapperPtr = &s_AllXMLNodeWrappersCurrentlyExisting[idxAt];
	wrapperPtr->m_IDWaitingForInOrderToClear = idToListenForWhenUnregister;

	for (int childIdx = 0; childIdx < node.nChildNode(); childIdx++)
	{
		XMLNode child = node.getChildNode(childIdx);
		XMLNodeWrapper* wrapperChild = FullyRegisterXMLNodeIntoTree(child, idToListenForWhenUnregister);
		wrapperChild->SetParent(wrapperPtr);
		wrapperPtr->AddChild(wrapperChild);

	}
	return wrapperPtr;
}

STATIC void XMLNodeWrapper::EmptyAllRegisteredXMLNodes()
{
	s_AllXMLNodeWrappersCurrentlyExisting.clear();
	s_AllXMLNodeWrappersCurrentlyExisting.shrink_to_fit();
}

STATIC void XMLNodeWrapper::EmptyAllRegisteredXMLNodesThatHaveID(int id)
{
	for (size_t XMLNodeWrapperIdx = 0; XMLNodeWrapperIdx < s_AllXMLNodeWrappersCurrentlyExisting.size(); XMLNodeWrapperIdx++)
	{
		if (s_AllXMLNodeWrappersCurrentlyExisting[XMLNodeWrapperIdx].m_IDWaitingForInOrderToClear == id)
		{
			s_AllXMLNodeWrappersCurrentlyExisting.erase(s_AllXMLNodeWrappersCurrentlyExisting.begin() + XMLNodeWrapperIdx);
			XMLNodeWrapperIdx--;
			continue;
		}
	}
	s_AllXMLNodeWrappersCurrentlyExisting.shrink_to_fit();
}
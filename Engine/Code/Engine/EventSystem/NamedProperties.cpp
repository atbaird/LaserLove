#include "Engine/EventSystem/NamedProperties.hpp"

//----------------------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------------------
//Constructors
NamedProperties::NamedProperties() 
{
}

NamedProperties::~NamedProperties() 
{ 
	ClearAll();
}

//----------------------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------------------
//Setters, Adders, Remove
void NamedProperties::Set(const std::string& propertyName, const char* typedDataValue)
{
	std::string val(typedDataValue);
	Set(propertyName, val);
}

void NamedProperties::Set(const char* propertyName, const char* typedDataValue)
{
	std::string propName(propertyName);
	Set(propName, typedDataValue);
}

void NamedProperties::Remove(const std::string& propertyName)
{
	NamedPropertyIterator it = m_properties.find(propertyName);
	if (it != m_properties.end())
	{
		delete it->second;
		m_properties.erase(it);
	}
}

void NamedProperties::Remove(const char* propertyName)
{
	std::string propNameStr(propertyName);
	Remove(propNameStr);
}

void NamedProperties::CopyProperties(NamedProperties* props)
{
	if (props != nullptr)
	{
		return;
	}
	for (NamedPropertyIterator it = props->m_properties.begin(); it != props->m_properties.end(); it++)
	{
		PropertyName name(it->first);
		NamedPropertyBase* copyOfVar = it->second->CloneProperty();
		m_properties.insert(std::pair<PropertyName, NamedPropertyBase*>(name, copyOfVar));
	}
}

void NamedProperties::ClearAll()
{
	for (NamedPropertyIterator it = m_properties.begin(); it != m_properties.end(); it++)
	{
		if (it->second != nullptr)
		{
			delete it->second;
			it->second = nullptr;
		}
	}
	m_properties.clear();
}

//----------------------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------------------
//Getters
PropertyGetResult NamedProperties::Get(const std::string& propertyName, std::string& out_typedDataValue) const
{
	if (m_properties.size() == 0)
	{
		return PROPERTY_FETCH_FAILED_NO_PROPERTIES;
	}
	ConstNamedPropertyIterator it = m_properties.find(propertyName);
	if (it != m_properties.end())
	{
		TypedNamedProperty<std::string>* prop = dynamic_cast<TypedNamedProperty<std::string>*>(it->second);
		if (prop == nullptr)
		{
			TypedNamedProperty<char*>* prop1 = dynamic_cast<TypedNamedProperty<char*>*>(it->second);
			if (prop1 != nullptr)
			{
				out_typedDataValue = std::string(prop1->m_data);
				return PROPERTY_FETCH_SUCCESS;
			}
			TypedNamedProperty<const char*>* prop2 = dynamic_cast<TypedNamedProperty<const char*>*>(it->second);
			if (prop2 != nullptr)
			{
				out_typedDataValue = std::string(prop2->m_data);
				return PROPERTY_FETCH_SUCCESS;
			}
			return PROPERTY_FETCH_FAILED_WRONG_TYPE;

		}
		out_typedDataValue = prop->m_data;
		return PROPERTY_FETCH_SUCCESS;
	}
	return PROPERTY_FETCH_FAILED_NO_SUCH_PROPERTY;
}


PropertyGetResult NamedProperties::Get(const char* propertyName, std::string& out_typedDataValue) const
{
	std::string propName(propertyName);
	return Get(propName, out_typedDataValue);
}
#pragma once

#ifndef NAMEDPROPERTIES_HPP
#define NAMEDPROPERTIES_HPP
#include "Engine/Core/ClassesAreSame.hpp"
#include "Engine/EventSystem/PropertyGetResult.hpp"
#include "Engine/EventSystem/TypedNamedProperty.hpp"
#include <cstdint>
#include <string>
#include <map>

#define PropertyName std::string

class NamedProperties
{
private:
protected:
	std::map<PropertyName, NamedPropertyBase*> m_properties;
	#define NamedPropertyIterator std::map<PropertyName, NamedPropertyBase*>::iterator
	#define ConstNamedPropertyIterator std::map<PropertyName, NamedPropertyBase*>::const_iterator
public:
	//----------------------------------------------------------------------------------------------------------------------------------------------
	//Constructors
	NamedProperties();
	virtual ~NamedProperties();

	template<typename T_prop1>
	NamedProperties(const std::string& propertyName, const T_prop1& typedDataValue);
	template<typename T_prop1, typename T_prop2>
	NamedProperties(const std::string& propertyName, const T_prop1& typedDataValue,
		const std::string& propertyName2, const T_prop2& typedDataValue2);
	template<typename T_prop1, typename T_prop2, typename T_prop3>
	NamedProperties(const std::string& propertyName, const T_prop1& typedDataValue,
		const std::string& propertyName2, const T_prop2& typedDataValue2,
		const std::string& propertyName3, const T_prop3& typedDataValue3);

	//----------------------------------------------------------------------------------------------------------------------------------------------
	//Setters, Adders, Remove
	void Set(const std::string& propertyName, const char* typedDataValue);
	void Set(const char* propertyName, const char* typedDataValue);

	template<typename T_prop>
	void Set(const std::string& propertyName, const T_prop& typedDataValue);

	template<typename T_prop>
	void Set(const char* propertyName, const T_prop& typedDataValue);


	void Remove(const std::string& propertyName);
	void Remove(const char* propertyName);

	void CopyProperties(NamedProperties* props);
	void ClearAll();

	//----------------------------------------------------------------------------------------------------------------------------------------------
	//Getters
	template<typename T_prop>
	PropertyGetResult Get(const std::string& propertyName, T_prop& out_typedDataValue) const;

	template<typename T_prop>
	PropertyGetResult Get(const char* propertyName, T_prop& out_typedDataValue) const;

	template<typename T_prop>
	PropertyGetResult Get(const std::string& propertyName, T_prop& out_typedDataValue, const T_prop& defaultValue) const;

	template<typename T_prop>
	PropertyGetResult Get(const char* propertyName, T_prop& out_typedDataValue, const T_prop& defaultValue) const;

	PropertyGetResult Get(const std::string& propertyName, std::string& out_typedDataValue) const;
	PropertyGetResult Get(const char* propertyName, std::string& out_typedDataValue) const;

};


//-------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------
//Constructors

//----------------------------------------------------------------------------------------------------------------------------------------------
template<typename T_prop1>
NamedProperties::NamedProperties(const std::string& propertyName, const T_prop1& typedDataValue)
{
	Set<T_prop1>(propertyName, typedDataValue);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------
template<typename T_prop1, typename T_prop2>
NamedProperties::NamedProperties(const std::string& propertyName, const T_prop1& typedDataValue,
	const std::string& propertyName2, const T_prop2& typedDataValue2)
{
	Set<T_prop1>(propertyName, typedDataValue);
	Set<T_prop2>(propertyName2, typedDataValue2);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------
template<typename T_prop1, typename T_prop2, typename T_prop3>
NamedProperties::NamedProperties(const std::string& propertyName, const T_prop1& typedDataValue,
	const std::string& propertyName2, const T_prop2& typedDataValue2,
	const std::string& propertyName3, const T_prop3& typedDataValue3)
{
	Set<T_prop1>(propertyName, typedDataValue);
	Set<T_prop2>(propertyName2, typedDataValue2);
	Set<T_prop3>(propertyName3, typedDataValue3);
}



//-------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------
//Setters, Adders, Remove
//-------------------------------------------------------------------------------------------------------------------------------------------------------
template<typename T_prop>
void NamedProperties::Set(const std::string& propertyName, const T_prop& typedDataValue)
{
	TypedNamedProperty<T_prop>* newProp = new TypedNamedProperty<T_prop>(typedDataValue);

	NamedPropertyIterator it = m_properties.find(propertyName);
	if (it != m_properties.end())
	{
		delete it->second;
		it->second = newProp;
	}
	else
	{
		m_properties.insert(std::pair<PropertyName, NamedPropertyBase*>(propertyName, newProp));
	}
}


//-------------------------------------------------------------------------------------------------------------------------------------------------------
template<typename T_prop>
void NamedProperties::Set(const char* propertyName, const T_prop& typedDataValue)
{
	std::string propNameStr(propertyName);
	Set(propNameStr, typedDataValue);
}


//-------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------
//Getters
#include <type_traits>
//-------------------------------------------------------------------------------------------------------------------------------------------------------
template<typename T_prop>
PropertyGetResult NamedProperties::Get(const std::string& propertyName, T_prop& out_typedDataValue) const
{
	if (m_properties.size() == 0)
	{
		return PROPERTY_FETCH_FAILED_NO_PROPERTIES;
	}
	ConstNamedPropertyIterator it = m_properties.find(propertyName);
	if (it != m_properties.end())
	{
		TypedNamedProperty<T_prop>* prop = dynamic_cast<TypedNamedProperty<T_prop>*>(it->second);
		if (prop == nullptr)
		{

			return PROPERTY_FETCH_FAILED_WRONG_TYPE;
		}

		out_typedDataValue = prop->m_data;
		return PROPERTY_FETCH_SUCCESS;
	}
	return PROPERTY_FETCH_FAILED_NO_SUCH_PROPERTY;
}


//-------------------------------------------------------------------------------------------------------------------------------------------------------
template<typename T_prop>
PropertyGetResult NamedProperties::Get(const char* propertyName, T_prop& out_typedDataValue) const
{
	std::string propNameStr(propertyName);
	return Get<T_prop>(propNameStr, out_typedDataValue);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------
template<typename T_prop>
PropertyGetResult NamedProperties::Get(const std::string& propertyName, T_prop& out_typedDataValue, const T_prop& defaultValue) const
{
	PropertyGetResult successful = Get(propertyName, out_typedDataValue);
	if (successful != PROPERTY_FETCH_SUCCESS)
	{
		out_typedDataValue = defaultValue;
	}
	return successful;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------
template<typename T_prop>
PropertyGetResult NamedProperties::Get(const char* propertyName, T_prop& out_typedDataValue, const T_prop& defaultValue) const
{
	std::string propNameStr(propertyName);
	return Get<T_prop>(propNameStr, out_typedDataValue, defaultValue);
}

#endif
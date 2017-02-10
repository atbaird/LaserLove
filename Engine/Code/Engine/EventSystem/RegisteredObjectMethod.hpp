#pragma once

#ifndef REGISTEREDOBJECTMETHOD_HPP
#define REGISTEREDOBJECTMETHOD_HPP
#include "Engine/EventSystem/RegisteredObjectBase.hpp"

template<typename T_object, typename T_methodPtr>
struct RegisteredObjectMethod : public RegisteredObjectBase
{
	T_object* m_object = nullptr;
	T_methodPtr m_method = nullptr;

	RegisteredObjectMethod(T_object* obj, T_methodPtr meth, const std::string& usage = "", bool consoleCallable = false)
		: RegisteredObjectBase(usage, consoleCallable),
		m_object(obj),
		m_method(meth)
	{
	};

	virtual ~RegisteredObjectMethod() { RegisteredObjectBase::~RegisteredObjectBase(); };

	virtual void Execute(NamedProperties& params) override;
	virtual bool DoesMatchObject(void* objPtr) const override;
	virtual void* GetObjectAddress() const override;
};

template<typename T_object, typename T_methodPtr>
void RegisteredObjectMethod<T_object, T_methodPtr>::Execute(NamedProperties& params)
{
	if (m_object == nullptr)
	{
		return;
	}
	(m_object->*m_method)(params);
}

template<typename T_object, typename T_methodPtr>
bool RegisteredObjectMethod<T_object, T_methodPtr>::DoesMatchObject(void* objPtr) const
{
	if (m_object == objPtr)
	{
		return true;
	}
	return false;
}

template<typename T_object, typename T_methodPtr>
void* RegisteredObjectMethod<T_object, T_methodPtr>::GetObjectAddress() const
{
	return m_object;
}
#endif
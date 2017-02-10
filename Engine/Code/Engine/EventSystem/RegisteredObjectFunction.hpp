#pragma once

#ifndef REGISTEREDOBJECTFUNCTION_HPP
#define REGISTEREDOBJECTFUNCTION_HPP
#include "Engine/EventSystem/RegisteredObjectBase.hpp"
#include "Engine/EventSystem/NamedProperties.hpp"

typedef void(EventCallbackFunc)(NamedProperties& params); //This is a function type.

struct RegisteredObjectFunction : public RegisteredObjectBase
{
	EventCallbackFunc* m_function = nullptr;

	RegisteredObjectFunction(EventCallbackFunc* function,  std::string usage = "", bool consoleCallable =false);

	virtual ~RegisteredObjectFunction();

	virtual void Execute(NamedProperties& params) override;
};

#endif
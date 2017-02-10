#include "Engine/EventSystem/RegisteredObjectFunction.hpp"

RegisteredObjectFunction::RegisteredObjectFunction(EventCallbackFunc* function,  std::string usage, bool consoleCallable)
	: RegisteredObjectBase(usage, consoleCallable),
	m_function(function)
{
}

RegisteredObjectFunction::~RegisteredObjectFunction() 
{
	RegisteredObjectBase::~RegisteredObjectBase(); 
}

void RegisteredObjectFunction::Execute(NamedProperties& params)
{
	(*m_function)(params);
}

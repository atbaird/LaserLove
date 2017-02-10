#pragma once

#ifndef NAMEDPROPERTYBASE_HPP
#define NAMEDPROPERTYBASE_HPP

struct NamedPropertyBase
{
	NamedPropertyBase();
	virtual ~NamedPropertyBase();

	virtual NamedPropertyBase* CloneProperty() = 0;
};

//------------------------------------------------------------------------------------------------------------------------------------


#endif
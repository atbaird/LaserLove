#pragma once

#ifndef ENTITY_HPP
#define ENTITY_HPP

class Entity
{
private:
public:
	Entity();
	virtual ~Entity();

	virtual const float Update(const float& deltaSeconds) = 0;
	virtual void Render() const = 0;
};
#endif
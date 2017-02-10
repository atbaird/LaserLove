#pragma once

#ifndef MATRIX44STACK_HPP
#define MATRIX44STACK_HPP
#include "Engine/Math/Matrix44.hpp"
#include <vector>

class Matrix44Stack
{
public:
	std::vector<Matrix44> matrices;

	Matrix44Stack();
	const bool IsEmpty() const;
	const Matrix44 GetTop() const;
	void Clear();
	void Push(const Matrix44& transform);
	void Pop();
};
#endif // !MATRIX44STACK_HPP

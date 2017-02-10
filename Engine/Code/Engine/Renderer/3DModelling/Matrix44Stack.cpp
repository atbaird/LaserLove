#include "Engine/Renderer/3DModelling/Matrix44Stack.hpp"

Matrix44Stack::Matrix44Stack()
{
	matrices.push_back(Matrix44::mat44_identity);
}
const bool Matrix44Stack::IsEmpty() const
{
	return (matrices.size() == 1);
}
const Matrix44 Matrix44Stack::GetTop() const
{
	return matrices.back();
}

void Matrix44Stack::Clear()
{
	matrices.clear();
	matrices.push_back(Matrix44::mat44_identity);
}

void Matrix44Stack::Push(const Matrix44& transform)
{
	Matrix44 top = GetTop();
	Matrix44 new_top;
	new_top = transform * top;
	matrices.push_back(new_top);
}

void Matrix44Stack::Pop()
{
	if (!IsEmpty())
	{
		matrices.pop_back();
	}
}
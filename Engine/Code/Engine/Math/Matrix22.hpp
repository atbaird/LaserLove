#pragma once

#ifndef MATRIX22_HPP
#define MATRIX22_HPP
#include "Engine/Math/Vector2.hpp"

class Matrix22
{
public:
	union
	{
		Vector2 rows[2];
		float data[2];
	};
public:
	//static variables
	static const Matrix22 mat22_identity;
	static const Matrix22 mat22_zeros;
	static const int mat22_numOfVals;
	static const int mat22_numOfRows;

	//Constructors
	Matrix22(const bool& makeIdentity = true);
	Matrix22(const float& v0, const float& v1,
		const float& v2, const float& v3);
	Matrix22(const Vector2& row1, const Vector2& row2);
	Matrix22(const Matrix22& mat);


	//Setters
	void MatrixMakeIdentity();
	void MatrixMakeZeros();
	void SetRow(const Vector2& row, unsigned int rowIndex);
	void SetCol(const Vector2& col, unsigned int colIndex);

	//Operators
	void MakeTranspose();
	void MakeNormalized();
	void MatrixAdjoint();
	static void MatrixInvertOrthogonal(Matrix22& mat);
	void MatrixInvert();
	const Vector2 operator*(const Vector2&);
	static void MatrixMultiply(const Matrix22* left, const Matrix22* right, Matrix22* out);
	const Vector2 TransformOnLeft(const Vector2& toMult) const;


	//Getters
	const float MatrixGetDeterminant() const;
	const bool MatrixIsOrthogonal() const;
	const Vector2 GetColumn(const unsigned int& colIndex) const;

};
const Vector2 operator*(const Vector2& v, const Matrix22& mat);
#endif
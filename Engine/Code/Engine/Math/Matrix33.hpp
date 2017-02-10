#pragma once
#include "Engine/Math/Vector2.hpp"
#include "Engine/Math/Vector4.hpp"

#ifndef MATRIX33_HPP
#define MATRIX33_HPP

class Matrix33
{
public:
	union
	{
		Vector3 rows[3];
		float data[9];
	};
public:

	//Static variables
	static const Matrix33 mat33_identity;
	static const Matrix33 mat33_zeros;

	//Constructors
	Matrix33(const bool& makeIdentity = true);
	Matrix33(const float& v0, const float& v1, const float& v2,
		const float& v3, const float& v4, const float& v5,
		const float& v6, const float& v7, const float& v8);
	Matrix33(const Vector3& row1, const Vector3& row2, const Vector3& row3);
	Matrix33(const Matrix33& mat);

	//Setters
	void MatrixMakeIdentity();
	void MatrixMakeZeros();
	void MatrixSetBasis(const Vector3& right, const Vector3& up, const Vector3& forward);
	void MatrixSetRows(const Vector3& r0, const Vector3& r1, const Vector3& r2);
	void MatrixSetCols(const Vector3& c0, const Vector3& c1, const Vector3& c2);
	void SetRow(const Vector3& row, unsigned int rowIndex);
	void SetCol(const Vector3& col, unsigned int colIndex);

	//Operators
	void MakeTranspose();
	void MakeNormalized();
	static void MatrixInvertOrthogonal(Matrix33& mat);
	void MatrixAdjoint();
	void MatrixInvert();
	static void MatrixGenerateBasis(Matrix33& mat, const Vector3& forward);
	const Vector3 operator*(const Vector3&);
	static void LookAt(Matrix33& makeLookAt, const Vector3& matrixPos, const Vector3& targetPosition, const Vector3& direction, const Vector3& up);
	static void MakeEulerRotation(Matrix33& mat, const float& roll, const float& pitch, const float& yaw, const Vector3& o);
	static void MatrixMultiply(const Matrix33* left, const Matrix33* right, Matrix33* out);
	const Vector3 TransformOnLeft(const Vector3& toMult) const;


	//Getters
	const float MatrixGetDeterminant() const;
	const bool MatrixIsOrthogonal() const;
	void MatrixGetBasis(const Vector3* r, const Vector3* u, const Vector3* f);
	const Vector3 GetColumn(const unsigned int& colIndex) const;
	const Vector3 MatrixGetRight() const;
	const Vector3 MatrixGetUp() const;
	const Vector3 MatrixGetForward() const;



};
const Vector3 operator*(const Vector3& v, const Matrix33& mat);
#endif

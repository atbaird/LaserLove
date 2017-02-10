#pragma once
#include "Engine/Math/Vector2.hpp"
#include "Engine/Math/Vector4.hpp"

#ifndef MATRIX44_HPP
#define MATRIX44_HPP

class Matrix44
{
public:
	union
	{
		Vector4 rows[4];
		float data[16];
	};
public:
	static const Matrix44 mat44_commonProjChangeOfBasis;
	static const Matrix44 mat44_commonWorldTransform;
	static const Matrix44 mat44_identity;
	static const Matrix44 mat44_zeros;

	//Constructors
	Matrix44(const bool& makeIdentity = true);
	Matrix44(const float& v0, const float& v1, const float& v2, const float& v3,
		const float& v4, const float& v5, const float& v6, const float& v7,
		const float& v8, const float& v9, const float& v10, const float& v11,
		const float& v12, const float& v13, const float& v14, const float& v15);
	Matrix44(const Vector4& row1, const Vector4& row2, const Vector4& row3, const Vector4& row4);
	Matrix44(const Matrix44& mat);

	//Setters
	static const Matrix44 GetEngineCommonEngineBasis();

	void MatrixMakeIdentity();
	void MatrixMakeZeros();
	void MatrixSetBasis(const Vector3& right, const Vector3& up, const Vector3& forward, const Vector3& pos);
	void MatrixSetRows(const Vector4& r0, const Vector4& r1, const Vector4& r2, const Vector4& r3);
	void MatrixSetCols(const Vector4& c0, const Vector4& c1, const Vector4& c2, const Vector4& c3);
	void SetRow(const Vector4& row, unsigned int rowIndex);
	void SetCol(const Vector4& col, unsigned int colIndex);
	void MatrixSetOffset(const Vector4&);
	void SetTranslate(const Vector3& position);

	//Operators
	void MakeTranspose();
	void MakeNormalized();
	static void MatrixInvertOrthogonal(Matrix44& mat);
	void MatrixAdjoint();
	void MatrixInvert();
	static void MatrixGenerateBasis(Matrix44& mat, const Vector3& forward);
	const Vector4 operator*(const Vector4&) const;
	const Matrix44 operator*(const float&) const;
	const Matrix44 operator*(const Matrix44&) const;
	const Matrix44 operator+(const Matrix44& other) const;
	const Matrix44 operator-(const Matrix44& other) const;
	static void MatrixMakePerspective(Matrix44& mat, const float& fov_degrees_y, const float& aspect, const float& nz, const float& fz);
	static void MatrixMakeOrthogonal(Matrix44& mat, const Vector3& n, const Vector3& f);
	static void MatrixMakeOrthogonal(Matrix44& mat, const Vector2& widthHeight, const float& nearZ, const float& farZ);
	static void LookAt(Matrix44& makeLookAt, const Vector3& matrixPos, const Vector3& targetPosition, const Vector3& direction, const Vector3& up);
	static void MakeEulerRotation(Matrix44& mat, const float& roll, const float& pitch, const float& yaw, const Vector3& o);
	void MatrixSetRight(const Vector3& right);
	void MatrixSetUp(const Vector3& up);
	void MatrixSetForward(const Vector3& forward);
	const Vector4 TransformOnLeft(const Vector4& toMult) const;


	//Getters
	const float MatrixGetDeterminant() const;
	const bool MatrixIsOrthogonal() const;
	static void MatrixGetBasis(const Matrix44& mat, Vector3& r, Vector3& u, Vector3& f, Vector3& o);
	static const Matrix44 MatrixFromBasis(const Vector3& r, const Vector3& u, const Vector3& f, const Vector3& t);
	const Vector4 GetColumn(const unsigned int& colIndex) const;
	const Vector4 MatrixGetOffset() const;
	const Vector3 MatrixGetRight() const;
	const Vector3 MatrixGetUp() const;
	const Vector3 MatrixGetForward() const;
	const Vector3 MatrixGetPosition() const;
	static const Matrix44 MatrixLerp(const Matrix44& a, const Matrix44& b, const float& time);

};
const Vector4 operator*(const Vector4& v, const Matrix44& mat);
const Matrix44 operator*(const float& f, const Matrix44& mat);
#endif

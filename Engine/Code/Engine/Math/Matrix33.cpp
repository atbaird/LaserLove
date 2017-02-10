#include "Engine/Math/Matrix33.hpp"
#include "Engine/Math/Matrix22.hpp"
#include "Engine/Math/ExtendedMath.hpp"
#include <cmath>
#include <memory.h>

const Matrix33 Matrix33::mat33_identity = Matrix33(true);
const Matrix33 Matrix33::mat33_zeros = Matrix33(false);

//-----------------------------------------------------------------------
//Constructors
Matrix33::Matrix33(const bool& makeIdentity)
{
	if (makeIdentity)
	{
		MatrixMakeIdentity();
	}
	else
	{
		MatrixMakeZeros();
	}
}
Matrix33::Matrix33(const float& v0, const float& v1, const float& v2,
	const float& v4, const float& v5, const float& v6,
	const float& v8, const float& v9, const float& v10)
{
	rows[0] = Vector3(v0, v1, v2);
	rows[1] = Vector3(v4, v5, v6);
	rows[2] = Vector3(v8, v9, v10);
}
Matrix33::Matrix33(const Vector3& row1, const Vector3& row2, const Vector3& row3)
{
	rows[0] = row1;
	rows[1] = row2;
	rows[2] = row3;
}

Matrix33::Matrix33(const Matrix33& mat)
{
	rows[0] = mat.rows[0];
	rows[1] = mat.rows[1];
	rows[2] = mat.rows[2];
}

//-----------------------------------------------------------------------
//Setters
void Matrix33::MatrixMakeIdentity()
{
	rows[0] = Vector3(1.f, 0.f, 0.f);
	rows[1] = Vector3(0.f, 1.f, 0.f);
	rows[2] = Vector3(0.f, 0.f, 1.f);
}

void Matrix33::MatrixMakeZeros()
{
	rows[0] = Vector3(0.f, 0.f, 0.f);
	rows[1] = Vector3(0.f, 0.f, 0.f);
	rows[2] = Vector3(0.f, 0.f, 0.f);
}

void Matrix33::MatrixSetBasis(const Vector3& r, const Vector3& u, const Vector3& f)
{
	rows[0] = Vector3(r.x, r.y, r.z);
	rows[1] = Vector3(u.x, u.y, u.z);
	rows[2] = Vector3(f.x, f.y, f.z);
}
//------------------------------------------------------------------------
void Matrix33::MatrixSetRows(const Vector3& c0, const Vector3& c1, const Vector3& c2)
{
	rows[0] = c0;
	rows[1] = c1;
	rows[2] = c2;
}
void Matrix33::MatrixSetCols(const Vector3& c0, const Vector3& c1, const Vector3& c2)
{
	rows[0] = Vector3(c0.x, c1.x, c2.x);
	rows[1] = Vector3(c0.y, c1.y, c2.y);
	rows[2] = Vector3(c0.z, c1.z, c2.z);
}
void Matrix33::SetRow(const Vector3& row, unsigned int rowIndex)
{
	if (rowIndex >= 2)
	{
		return;
	}
	rows[rowIndex] = row;
}

void Matrix33::SetCol(const Vector3& col, unsigned int colIndex)
{
	switch (colIndex)
	{
	case 0:
		rows[0].x = col.x;
		rows[1].x = col.y;
		rows[2].x = col.z;
		break;
	case 1:
		rows[0].y = col.x;
		rows[1].y = col.y;
		rows[2].y = col.z;
		break;
	case 2:
		rows[0].z = col.x;
		rows[1].z = col.y;
		rows[2].z = col.z;
		break;
	default:
		break;
	}
}
//------------------------------------------------------
//operators

const Vector3 Matrix33::operator*(const Vector3& vec4)
{
	Vector3 toReturn;
	toReturn.x = (rows[0].x * vec4.x) + (rows[0].y *vec4.y) + (rows[0].z * vec4.z);
	toReturn.y = (rows[1].x * vec4.x) + (rows[1].y *vec4.y) + (rows[1].z * vec4.z);
	toReturn.z = (rows[2].x * vec4.x) + (rows[2].y *vec4.y) + (rows[2].z * vec4.z);
	return toReturn;
}
void Matrix33::MatrixInvertOrthogonal(Matrix33& mat)
{
	mat.MakeTranspose();
}
void Matrix33::MatrixAdjoint()
{
	Vector3 r1, r2, r3, r4;
	r1 = Vector3(rows[0].x, rows[1].x, rows[2].x);
	r2 = Vector3(rows[0].y, rows[1].y, rows[2].y);
	r3 = Vector3(rows[0].z, rows[1].z, rows[2].z);
	rows[0] = r1;
	rows[1] = r2;
	rows[2] = r3;
}
void Matrix33::MatrixInvert()
{
	float determinant = MatrixGetDeterminant();

	if (determinant == 0.f)
	{
		return;
	}
	
	MatrixAdjoint();

	rows[0] = rows[0] / determinant;
	rows[1] = rows[1] / determinant;
	rows[2] = rows[2] / determinant;


}
void Matrix33::MatrixGenerateBasis(Matrix33& mat, const Vector3& forward)
{
	Vector3 f = forward;
	f.normalize();
	Vector3 r;
	Vector3 u;

	if (true)//IsColinear(forward, Vector3(0.f, 0.f, 1.f)) 
	{
		u = Vector3(1.f, 0.f, 0.f).crossProduct(f);
		r = f.crossProduct(u);
	}
	else
	{
		r = Vector3(0.f, 0.f, 1.f).crossProduct(f);
		u = r.crossProduct(f);
	}

	mat.MatrixSetBasis(r, u, f);
}
void Matrix33::MakeTranspose()
{
	float temp[9];
	for (unsigned int col = 0; col < 3; col++)
	{
		temp[col] = rows[col].x;
		temp[col + 3] = rows[col].y;
		temp[col + 6] = rows[col].z;
	}
	rows[0] = Vector3(temp[0], temp[1], temp[2]);
	rows[1] = Vector3(temp[3], temp[4], temp[5]);
	rows[2] = Vector3(temp[6], temp[7], temp[8]);
}
void Matrix33::MakeNormalized()
{
	Vector3 f, r, u, o;

	MatrixGetBasis(&r, &u, &f);

	f.normalize();
	u.normalize();
	//Normalize3(&f);
	//Normalize3(&u);

	r = f.crossProduct(u);
	u = r.crossProduct(f);

	MatrixSetBasis(r, u, f);
}

const Vector3 Matrix33::MatrixGetRight() const
{
	return rows[0];
}
const Vector3 Matrix33::MatrixGetUp() const
{
	return rows[1];
}
const Vector3 Matrix33::MatrixGetForward() const
{
	return rows[2];
}
void Matrix33::LookAt(Matrix33& makeLookAt, const Vector3& matrixPos, const Vector3& targetPosition, const Vector3& direction, const Vector3& up)
{
	direction;
	Vector3 dir = targetPosition - matrixPos;
	dir.normalize();
	Vector3 worldUp = up;
	worldUp.normalize();
	Vector3 right = dir.crossProduct(worldUp);
	Vector3 lUp = right.crossProduct(dir);
	makeLookAt.MatrixSetBasis(right, lUp, dir);
}
void Matrix33::MakeEulerRotation(Matrix33& mat, const float& roll, const float& pitch, const float& yaw, const Vector3& o)
{
	o;
	// yaw = y
	// pitch = x
	// roll = z

	float sx = sin(pitch);
	float cx = cos(pitch);

	float sy = sin(-yaw);
	float cy = cos(-yaw);

	float sz = sin(roll);
	float cz = cos(roll);

	// 	float const values[] = {
	// 		cy*cz + sx*sy*sz,		-cx*sz,		-cz*sy + cy*sx*sz,		0.0f,
	// 		cy*sz - cz*sx*sy ,		cx*cz,		-sy*sz - cy*cz*sx,		0.0f,
	// 		cx*sy,					sx ,		cx*cy,					0.0f,
	// 		o.x,					o.y,		o.z,					1.0f
	// 	};
	const float values[] = {
		cx*cy,					(-cx*sy),				sx,		
		-(-cz*sy + cy*sx*sz),	cy*cz + sx*sy*sz,		(cx*sz),
		-sy*sz - cy*cz*sx,		-(cy*sz - cz*sx*sy),	cx*cz,	
	};

	memcpy(mat.data, values, sizeof(values));
}
const Vector3 Matrix33::TransformOnLeft(const Vector3& toMult) const
{
	Vector3 col0, col1, col2;
	col0 = GetColumn(0);
	col1 = GetColumn(1);
	col2 = GetColumn(2);
	Vector3 ret = Vector3(col0.dotProduct(toMult), col1.dotProduct(toMult), col2.dotProduct(toMult));

	return ret;
}

//-----------------------------------------------------------------------------
//Getters

void Matrix33::MatrixGetBasis(const Vector3* r, const Vector3* u, const Vector3* f)
{
	r = &rows[0];
	u = &rows[1];
	f = &rows[2];
}

const float Matrix33::MatrixGetDeterminant() const
{
	//float inv[3];

	Matrix22 first = Matrix22(rows[1].y, rows[1].z, rows[2].y, rows[2].z);
	Matrix22 second = Matrix22(rows[1].x, rows[1].z, rows[2].x, rows[2].z);
	Matrix22 third = Matrix22(rows[1].x, rows[1].y, rows[2].x, rows[2].y);

	float det;

	//inv[0] = ((rows[1].y * rows[2].z) - (rows[1].z * rows[2].y));
	//inv[1] = ((rows[1].x * rows[2].z) - (rows[1].z * rows[2].x));
	//inv[2] = ((rows[1].x * rows[2].y) - (rows[1].y * rows[2].x));

	det = (rows[0].x * first.MatrixGetDeterminant()) - (rows[0].y * second.MatrixGetDeterminant()) + (rows[0].z * third.MatrixGetDeterminant());
	return det;
}
const bool Matrix33::MatrixIsOrthogonal() const
{
	Vector3 row0 = rows[0];
	Vector3 row1 = rows[1];
	Vector3 row2 = rows[2];

	return ((row1.dotProduct(row1) == 0.f)
		&& (row2.dotProduct(row0) == 0.0f)
		&& (row2.dotProduct(row1) == 0.0f));
}
const Vector3 Matrix33::GetColumn(const unsigned int& colIndex) const
{
	Vector3 col = Vector3();
	switch (colIndex)
	{
	case 0:
		col.x = rows[0].x;
		col.y = rows[1].x;
		col.z = rows[2].x;
		break;
	case 1:
		col.x = rows[0].y;
		col.y = rows[1].y;
		col.z = rows[2].y;
		break;
	case 2:
		col.x = rows[0].z;
		col.y = rows[1].z;
		col.z = rows[2].z;
		break;
	default:
		break;
	}
	return col;
}


//----------------------------------------------------------------------------
//Global

const Vector3 operator*(const Vector3& v, const Matrix33& mat)
{
	return mat.TransformOnLeft(v);
}
#include "Engine/Math/Matrix44.hpp"
#include "Engine/Math/ExtendedMath.hpp"
#include <cmath>
#include <memory.h>
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Engine/Math/Matrix33.hpp"

/*

rows[0] = Vector4(r.x, r.y, r.z, rows[0].w);
rows[1] = Vector4(u.x, u.y, u.z, rows[1].w);
rows[2] = Vector4(f.x, f.y, f.z, rows[2].w);
rows[3] = Vector4(o.x, o.y, o.z, rows[3].w);
*/

const Matrix44 Matrix44::mat44_commonProjChangeOfBasis = Matrix44(Vector4(1.f, 0.f, 0.f, 0.f), 
																	Vector4(0.f, 0.f, 1.f, 0.f),
																	Vector4(0.f, 1.f, 0.f, 0.f),
																	Vector4(0.f, 0.f, 0.f, 1.f));

const Matrix44 Matrix44::mat44_commonWorldTransform = Matrix44(0.f, -1.f, 0.f, 0.f, 
															0.f, 0.f, 1.f, 0.f,
															1.f, 0.f, 0.f, 0.f,
															0.f, 0.f, 0.f, 1.f);
const Matrix44 Matrix44::mat44_identity = Matrix44(true);
const Matrix44 Matrix44::mat44_zeros = Matrix44(false);

//-----------------------------------------------------------------------
//Constructors
Matrix44::Matrix44(const bool& makeIdentity)
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
Matrix44::Matrix44(const float& v0, const float& v1, const float& v2, const float& v3,
	const float& v4, const float& v5, const float& v6, const float& v7,
	const float& v8, const float& v9, const float& v10, const float& v11,
	const float& v12, const float& v13, const float& v14, const float& v15)
{
	rows[0] = Vector4(v0, v1, v2, v3);
	rows[1] = Vector4(v4, v5, v6, v7);
	rows[2] = Vector4(v8, v9, v10, v11);
	rows[3] = Vector4(v12, v13, v14, v15);
}
Matrix44::Matrix44(const Vector4& row1, const Vector4& row2, const Vector4& row3, const Vector4& row4)
{
	rows[0] = row1;
	rows[1] = row2;
	rows[2] = row3;
	rows[3] = row4;
}

Matrix44::Matrix44(const Matrix44& mat)
{
	rows[0] = mat.rows[0];
	rows[1] = mat.rows[1];
	rows[2] = mat.rows[2];
	rows[3] = mat.rows[3];
}

const Matrix44 Matrix44::GetEngineCommonEngineBasis()
{
	Matrix44 mat;
	mat.MatrixSetRight(Vector3(0.f, -1.f, 0.f));
	mat.MatrixSetUp(Vector3(0.f, 0.f, 1.f));
	mat.MatrixSetForward(Vector3(-1.f, 0.f, 0.f));
	mat.SetRow(Vector4(0.f, 0.f, 0.f, 1.f), 3);
	return mat;
}
//-----------------------------------------------------------------------
//Setters
void Matrix44::MatrixMakeIdentity()
{
	rows[0] = Vector4(1.f, 0.f, 0.f, 0.f);
	rows[1] = Vector4(0.f, 1.f, 0.f, 0.f);
	rows[2] = Vector4(0.f, 0.f, 1.f, 0.f);
	rows[3] = Vector4(0.f, 0.f, 0.f, 1.f);
}

void Matrix44::MatrixMakeZeros()
{
	rows[0] = Vector4(0.f, 0.f, 0.f, 0.f);
	rows[1] = Vector4(0.f, 0.f, 0.f, 0.f);
	rows[2] = Vector4(0.f, 0.f, 0.f, 0.f);
	rows[3] = Vector4(0.f, 0.f, 0.f, 0.f);
}

void Matrix44::MatrixSetBasis(const Vector3& r, const Vector3& u, const Vector3& f, const Vector3& o)
{
	rows[0] = Vector4(r.x, r.y, r.z, rows[0].w);
	rows[1] = Vector4(u.x, u.y, u.z, rows[1].w);
	rows[2] = Vector4(f.x, f.y, f.z, rows[2].w);
	rows[3] = Vector4(o.x, o.y, o.z, rows[3].w);
}
//------------------------------------------------------------------------
void Matrix44::MatrixSetRows(const Vector4& c0, const Vector4& c1, const Vector4& c2, const Vector4& c3)
{
	rows[0] = c0;
	rows[1] = c1;
	rows[2] = c2;
	rows[3] = c3;
}
void Matrix44::MatrixSetCols(const Vector4& c0, const Vector4& c1, const Vector4& c2, const Vector4& c3)
{
	rows[0] = Vector4(c0.x, c1.x, c2.x, c3.x);
	rows[1] = Vector4(c0.y, c1.y, c2.y, c3.y);
	rows[2] = Vector4(c0.z, c1.z, c2.z, c3.z);
	rows[3] = Vector4(c0.w, c1.w, c2.w, c3.w);
}
void Matrix44::SetRow(const Vector4& row, unsigned int rowIndex)
{
	if (rowIndex >= 3)
	{
		return;
	}
	rows[rowIndex] = row;
}

void Matrix44::SetCol(const Vector4& col, unsigned int colIndex)
{
	switch (colIndex)
	{
	case 0:
		rows[0].x = col.x;
		rows[1].x = col.y;
		rows[2].x = col.z;
		rows[3].x = col.w;
		break;
	case 1:
		rows[0].y = col.x;
		rows[1].y = col.y;
		rows[2].y = col.z;
		rows[3].y = col.w;
		break;
	case 2:
		rows[0].z = col.x;
		rows[1].z = col.y;
		rows[2].z = col.z;
		rows[3].z = col.w;
		break;
	case 3:
		rows[0].w = col.x;
		rows[1].w = col.y;
		rows[2].w = col.z;
		rows[3].w = col.w;
		break;
	default:
		break;
	}
}
void Matrix44::MatrixSetOffset(const Vector4& offset)
{
	rows[3] = offset;
	rows[3].w = 1.f;
}

void Matrix44::SetTranslate(const Vector3& position)
{
	data[12] = position.x;
	data[13] = position.y;
	data[14] = position.z;
}

//------------------------------------------------------
//operators

const Vector4 Matrix44::operator*(const Vector4& vec4) const
{
	Vector4 toReturn;
	toReturn.x = (rows[0].x * vec4.x) + (rows[0].y *vec4.y) + (rows[0].z * vec4.z) + (rows[0].w * vec4.w);
	toReturn.y = (rows[1].x * vec4.x) + (rows[1].y *vec4.y) + (rows[1].z * vec4.z) + (rows[1].w * vec4.w);
	toReturn.z = (rows[2].x * vec4.x) + (rows[2].y *vec4.y) + (rows[2].z * vec4.z) + (rows[2].w * vec4.w);
	toReturn.w = (rows[3].x * vec4.x) + (rows[3].y *vec4.y) + (rows[3].z * vec4.z) + (rows[3].w * vec4.w);
	return toReturn;
}
const Matrix44 Matrix44::operator*(const float& mult) const
{
	Matrix44 toReturn;
	for (size_t i = 0; i < 16; i++)
	{
		toReturn.data[i] = data[i] * mult;
	}
	return toReturn;
}
const Matrix44 Matrix44::operator*(const Matrix44& other) const
{
	Matrix44 out;
	for (int row = 0; row < 4; row++)
	{
		for (int col = 0; col < 4; col++)
		{
			out.data[(row * 4) + col] = rows[row] * other.GetColumn(col);
		}
	}
	return out;
}
const Matrix44 Matrix44::operator+(const Matrix44& other) const
{
	Matrix44 out;
	for (int row = 0; row < 4; row++)
	{
		out.rows[row] = rows[row] + other.rows[row];
	}
	return out;
}
const Matrix44 Matrix44::operator-(const Matrix44& other) const
{
	Matrix44 out;
	for (int row = 0; row < 4; row++)
	{
		out.rows[row] = rows[row] - other.rows[row];
	}
	return out;
}
void Matrix44::MatrixInvertOrthogonal(Matrix44& mat)
{
	Vector4 translation = mat.MatrixGetOffset();
	mat.MakeTranspose();
	mat.SetCol(Vector4(0.0f, 0.0f, 0.0f, 1.0f), 3); //originally was setCol
	translation = (translation * -1.f) * mat;
	mat.MatrixSetOffset(translation);
}
void Matrix44::MatrixAdjoint()
{

	Vector4 r1, r2, r3, r4;

	r1 = Vector4(rows[0].x, rows[1].x, rows[2].x, rows[3].x);
	r2 = Vector4(rows[0].y, rows[1].y, rows[2].y, rows[3].y);
	r3 = Vector4(rows[0].z, rows[1].z, rows[2].z, rows[3].z);
	r4 = Vector4(rows[0].w, rows[1].w, rows[2].w, rows[3].w);
	rows[0] = r1;
	rows[1] = r2;
	rows[2] = r3;
	rows[3] = r4;
}
void Matrix44::MatrixInvert()
{

	//for (i = 0; i < 16; i++) {
	//	m[i] = inv[i] * det;
	//}

	float det;
	float inv[16];
	const float* m = data;

	inv[0] = (m[5] * m[10] * m[15]) -
		(m[5] * m[11] * m[14]) -
		(m[9] * m[6] * m[15]) +
		(m[9] * m[7] * m[14]) +
		(m[13] * m[6] * m[11]) -
		(m[13] * m[7] * m[10]);

	inv[4] = (-m[4] * m[10] * m[15]) +
		(m[4] * m[11] * m[14]) +
		(m[8] * m[6] * m[15]) -
		(m[8] * m[7] * m[14]) -
		(m[12] * m[6] * m[11]) +
		(m[12] * m[7] * m[10]);

	inv[8] = (m[4] * m[9] * m[15]) -
		(m[4] * m[11] * m[13]) -
		(m[8] * m[5] * m[15]) +
		(m[8] * m[7] * m[13]) +
		(m[12] * m[5] * m[11]) -
		(m[12] * m[7] * m[9]);

	inv[12] = (-m[4] * m[9] * m[14]) +
		(m[4] * m[10] * m[13]) +
		(m[8] * m[5] * m[14]) -
		(m[8] * m[6] * m[13]) -
		(m[12] * m[5] * m[10]) +
		(m[12] * m[6] * m[9]);

	inv[1] = (-m[1] * m[10] * m[15]) +
		(m[1] * m[11] * m[14]) +
		(m[9] * m[2] * m[15]) -
		(m[9] * m[3] * m[14]) -
		(m[13] * m[2] * m[11]) +
		(m[13] * m[3] * m[10]);

	inv[5] = (m[0] * m[10] * m[15]) -
		(m[0] * m[11] * m[14]) -
		(m[8] * m[2] * m[15]) +
		(m[8] * m[3] * m[14]) +
		(m[12] * m[2] * m[11]) -
		(m[12] * m[3] * m[10]);

	inv[9] = (-m[0] * m[9] * m[15]) +
		(m[0] * m[11] * m[13]) +
		(m[8] * m[1] * m[15]) -
		(m[8] * m[3] * m[13]) -
		(m[12] * m[1] * m[11]) +
		(m[12] * m[3] * m[9]);

	inv[13] = (m[0] * m[9] * m[14]) -
		(m[0] * m[10] * m[13]) -
		(m[8] * m[1] * m[14]) +
		(m[8] * m[2] * m[13]) +
		(m[12] * m[1] * m[10]) -
		(m[12] * m[2] * m[9]);

	inv[2] = (m[1] * m[6] * m[15]) -
		(m[1] * m[7] * m[14]) -
		(m[5] * m[2] * m[15]) +
		(m[5] * m[3] * m[14]) +
		(m[13] * m[2] * m[7]) -
		(m[13] * m[3] * m[6]);

	inv[6] = (-m[0] * m[6] * m[15]) +
		(m[0] * m[7] * m[14]) +
		(m[4] * m[2] * m[15]) -
		(m[4] * m[3] * m[14]) -
		(m[12] * m[2] * m[7]) +
		(m[12] * m[3] * m[6]);

	inv[10] = (m[0] * m[5] * m[15]) -
		(m[0] * m[7] * m[13]) -
		(m[4] * m[1] * m[15]) +
		(m[4] * m[3] * m[13]) +
		(m[12] * m[1] * m[7]) -
		(m[12] * m[3] * m[5]);

	inv[14] = (-m[0] * m[5] * m[14]) +
		(m[0] * m[6] * m[13]) +
		(m[4] * m[1] * m[14]) -
		(m[4] * m[2] * m[13]) -
		(m[12] * m[1] * m[6]) +
		(m[12] * m[2] * m[5]);

	inv[3] = (-m[1] * m[6] * m[11]) +
		(m[1] * m[7] * m[10]) +
		(m[5] * m[2] * m[11]) -
		(m[5] * m[3] * m[10]) -
		(m[9] * m[2] * m[7]) +
		(m[9] * m[3] * m[6]);

	inv[7] = (m[0] * m[6] * m[11]) -
		(m[0] * m[7] * m[10]) -
		(m[4] * m[2] * m[11]) +
		(m[4] * m[3] * m[10]) +
		(m[8] * m[2] * m[7]) -
		(m[8] * m[3] * m[6]);

	inv[11] = (-m[0] * m[5] * m[11]) +
		(m[0] * m[7] * m[9]) +
		(m[4] * m[1] * m[11]) -
		(m[4] * m[3] * m[9]) -
		(m[8] * m[1] * m[7]) +
		(m[8] * m[3] * m[5]);

	inv[15] = (m[0] * m[5] * m[10]) -
		(m[0] * m[6] * m[9]) -
		(m[4] * m[1] * m[10]) +
		(m[4] * m[2] * m[9]) +
		(m[8] * m[1] * m[6]) -
		(m[8] * m[2] * m[5]);

	det = (m[0] * inv[0]) + (m[1] * inv[4]) + (m[2] * inv[8]) + (m[3] * inv[12]);
	if (det == 0.f)
	{
		return;
	}

	for (size_t i = 0; i < 16; i++)
	{
		data[i] = inv[i] * det;
	}
}		
void Matrix44::MatrixGenerateBasis(Matrix44& mat, const Vector3& forward)
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

	mat.MatrixSetBasis(r, u, f, Vector3());
}
void Matrix44::MakeTranspose()
{
	float temp[16];
	for (unsigned int col = 0; col < 4; col++)
	{
		temp[col] =		rows[col].x;
		temp[col + 4] = rows[col].y;
		temp[col + 8] = rows[col].z;
		temp[col + 12] = rows[col].w;
	}
	rows[0] = Vector4(temp[0], temp[1], temp[2], temp[3]);
	rows[1] = Vector4(temp[4], temp[5], temp[6], temp[7]);
	rows[2] = Vector4(temp[8], temp[9], temp[10], temp[11]);
	rows[3] = Vector4(temp[12], temp[13], temp[14], temp[15]);
}
void Matrix44::MakeNormalized()
{
	Vector3 f, r, u, o;

	MatrixGetBasis(*this, r, u, f, o);

	f.normalize();
	u.normalize();
	//Normalize3(&f);
	//Normalize3(&u);

	r = f.crossProduct(u);
	u = r.crossProduct(f);

	MatrixSetBasis(r, u, f, o);
}

const Vector3 Matrix44::MatrixGetRight() const
{
	return rows[0].XYZ();
}
const Vector3 Matrix44::MatrixGetUp() const
{
	return rows[1].XYZ();
}
const Vector3 Matrix44::MatrixGetForward() const
{
	return rows[2].XYZ();
}
const Vector3 Matrix44::MatrixGetPosition() const
{
	return rows[3].XYZ();
}
void Matrix44::MatrixMakePerspective(Matrix44& mat, const float& fov_degrees_y, const float& aspect, const float& nz, const float& fz)
{
	//can't pass in a z thats <= 0.f
	float rads = DegreesToRadians(fov_degrees_y);
	float size = 1 / tan(rads / 2.0f);


	float w_ = size;
	float h_ = size;
	if (aspect > 1.0f)
	{
		w_ /= aspect;
	}
	else
	{
		h_ *= aspect;
	}
	float q = 1.0f / (fz - nz);
// 	const float values[] =
// 	{
// 		w_,		0.f,		0.f,							0.f,
// 		0.f,	0.f,		(fz + nz) * q,					1.f,
// 		0.f,	h_,			0.f,							0.f,
// 		0.f,	0.f,		-2.0f * nz * fz * q,			0.f
// 	};
	const float values[] =
	{
		0.f,	0.f,	(fz + nz) * q,			1.f,
		-w_,	0.f,	0.f,					0.f,
		0.f,	h_,		0.f,					0.f,
		0.f,	0.f,	-2.0f * nz * fz * q,	0.f
	};
	memcpy(mat.data, values, sizeof(values));
}
void Matrix44::LookAt(Matrix44& makeLookAt, const Vector3& matrixPos, const Vector3& targetPosition,  const Vector3& up)
{
	Vector3 dir = targetPosition - matrixPos;
	dir.normalize();
	Vector3 worldUp = up;
	worldUp.normalize();
	Vector3 right = dir.crossProduct(worldUp);
	Vector3 lUp = right.crossProduct(dir);
	makeLookAt.MatrixSetBasis(right, lUp, dir, matrixPos);
	makeLookAt.SetTranslate(matrixPos);
}
void Matrix44::MatrixMakeOrthogonal(Matrix44& mat, const Vector3& n, const Vector3& f)
{
	//double check that this one actually is in correct basis.
	float sx = 1.0f / (f.x - n.x);
	float sy = 1.0f / (f.y - n.y);
	float sz = 1.0f / (f.z - n.z);

// 	const float values[] = {
// 		2.0f * sx,					0.0f,				0.0f,							0.f,
// 		0.0f,						0.0f,				2.0f * sy,						0.f,
// 		0.0f,						2.0f * sz,			0.0f,							0.f,
// 		-(f.x + n.x) * sx,           -(f.y + n.y) * sz,   -(f.z + n.z) * sy,	       1.0f
// 	};
	float const values[] = {
		2.0f * sx,			0.0f,						0.0f,							0.f,
		0.0f,				2.0f * sy,					0.0f,							0.f,
		0.0f,				0.0f,						2.0f * sz,						0.f,
		-(f.x + n.x) * sx,    -(f.z + n.z) * sy,         -(f.y + n.y) * sz, 	       1.0f
	};
	memcpy(mat.data, values, sizeof(values));
}
void Matrix44::MatrixMakeOrthogonal(Matrix44& mat, const Vector2& widthHeight, const float& nearZ, const float& farZ)
{
	const float sz = 1.f / (farZ - nearZ);
	float const values[] = {
		2.0f / widthHeight.x,			0.0f,						0.0f,							0.f,
		0.0f,				2.0f / widthHeight.y,					0.0f,							0.f,
		0.0f,				0.0f,									2.0f * sz,						0.f,
		0.f,				 0.f,									-(farZ + nearZ) * sz, 	       1.0f
	};
	memcpy(mat.data, values, sizeof(values));
}
void Matrix44::MakeEulerRotation(Matrix44& mat, const float& roll, const float& pitch, const float& yaw, const Vector3& o)
{
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
 		cx*cy,					(-cx*sy),				sx,				0.0f,
 		-(-cz*sy + cy*sx*sz),	cy*cz + sx*sy*sz,		(cx*sz),		0.0f,
 		-sy*sz - cy*cz*sx,		-(cy*sz - cz*sx*sy),	cx*cz,			0.0f,
 		o.x,					o.y,		o.z,						1.0f
 	};

	memcpy(mat.data, values, sizeof(values));
}
void Matrix44::MakeEulerRotationYUp(Matrix44& mat, const float& roll, const float& pitch, const float& yaw, const Vector3& o)
{
	// yaw = y
	// pitch = x
	// roll = z

	float sx = sin(roll);
	float cx = cos(roll);

	float sy = sin(-pitch);
	float cy = cos(-pitch);

	float sz = sin(yaw);
	float cz = cos(yaw);

	 float const values[] = {
	 		cy*cz + sx*sy*sz,		-cx*sz,		-cz*sy + cy*sx*sz,		0.0f,
	 		cy*sz - cz*sx*sy ,		cx*cz,		-sy*sz - cy*cz*sx,		0.0f,
	 		cx*sy,					sx ,		cx*cy,					0.0f,
	 		o.x,					o.y,		o.z,					1.0f
	 };
	//onst float values[] = {
	//	cx*cy,					(-cx*sy),				sx,				0.0f,
	//	-(-cz*sy + cy*sx*sz),	cy*cz + sx*sy*sz,		(cx*sz),		0.0f,
	//	-sy*sz - cy*cz*sx,		-(cy*sz - cz*sx*sy),	cx*cz,			0.0f,
	//	o.x,					o.y,		o.z,						1.0f
	//;

	memcpy(mat.data, values, sizeof(values));
}


void Matrix44::MatrixSetRight(const Vector3& right)
{
	rows[0] = Vector4(right.x, right.y, right.z, 0.f);
}
void Matrix44::MatrixSetUp(const Vector3& up)
{
	rows[1] = Vector4(up.x, up.y, up.z, 0.f);
}
void Matrix44::MatrixSetForward(const Vector3& forward)
{
	rows[2] = Vector4(forward.x, forward.y, forward.z, 0.f);
}
const Vector4 Matrix44::TransformOnLeft(const Vector4& toMult) const
{
	Vector4 col0, col1, col2, col3;
	col0 = GetColumn(0);
	col1 = GetColumn(1);
	col2 = GetColumn(2);
	col3 = GetColumn(3);
	Vector4 ret = Vector4(col0.dotProduct(toMult), col1.dotProduct(toMult), col2.dotProduct(toMult), col3.dotProduct(toMult));

	return ret;
}

//-----------------------------------------------------------------------------
//Getters

void Matrix44::MatrixGetBasis(const Matrix44& mat, Vector3& r, Vector3& u, Vector3& f, Vector3& o)
{
	r = mat.rows[0].XYZ();
	u = mat.rows[1].XYZ();
	f = mat.rows[2].XYZ();
	o = mat.rows[3].XYZ();
}
const Matrix44 Matrix44::MatrixFromBasis(const Vector3& r, const Vector3& u, const Vector3& f, const Vector3& t)
{
	Matrix44 mat = Matrix44(true);

	mat.rows[0] = Vector4(r.x, r.y, r.z, 0.f);
	mat.rows[1] = Vector4(u.x, u.y, u.z, 0.f);
	mat.rows[2] = Vector4(f.x, f.y, f.z, 0.f);
	mat.rows[3] = Vector4(t.x, t.y, t.z, 1.f);
	return mat;
}

const float Matrix44::MatrixGetDeterminant() const
{
	//float inv[4];

	float det;
	float inv[16];
	const float* m = data;

	inv[0] = (m[5] * m[10] * m[15]) -
		(m[5] * m[11] * m[14]) -
		(m[9] * m[6] * m[15]) +
		(m[9] * m[7] * m[14]) +
		(m[13] * m[6] * m[11]) -
		(m[13] * m[7] * m[10]);

	inv[4] = (-m[4] * m[10] * m[15]) +
		(m[4] * m[11] * m[14]) +
		(m[8] * m[6] * m[15]) -
		(m[8] * m[7] * m[14]) -
		(m[12] * m[6] * m[11]) +
		(m[12] * m[7] * m[10]);

	inv[8] = (m[4] * m[9] * m[15]) -
		(m[4] * m[11] * m[13]) -
		(m[8] * m[5] * m[15]) +
		(m[8] * m[7] * m[13]) +
		(m[12] * m[5] * m[11]) -
		(m[12] * m[7] * m[9]);

	inv[12] = (-m[4] * m[9] * m[14]) +
		(m[4] * m[10] * m[13]) +
		(m[8] * m[5] * m[14]) -
		(m[8] * m[6] * m[13]) -
		(m[12] * m[5] * m[10]) +
		(m[12] * m[6] * m[9]);

	inv[1] = (-m[1] * m[10] * m[15]) +
		(m[1] * m[11] * m[14]) +
		(m[9] * m[2] * m[15]) -
		(m[9] * m[3] * m[14]) -
		(m[13] * m[2] * m[11]) +
		(m[13] * m[3] * m[10]);

	inv[5] = (m[0] * m[10] * m[15]) -
		(m[0] * m[11] * m[14]) -
		(m[8] * m[2] * m[15]) +
		(m[8] * m[3] * m[14]) +
		(m[12] * m[2] * m[11]) -
		(m[12] * m[3] * m[10]);

	inv[9] = (-m[0] * m[9] * m[15]) +
		(m[0] * m[11] * m[13]) +
		(m[8] * m[1] * m[15]) -
		(m[8] * m[3] * m[13]) -
		(m[12] * m[1] * m[11]) +
		(m[12] * m[3] * m[9]);

	inv[13] = (m[0] * m[9] * m[14]) -
		(m[0] * m[10] * m[13]) -
		(m[8] * m[1] * m[14]) +
		(m[8] * m[2] * m[13]) +
		(m[12] * m[1] * m[10]) -
		(m[12] * m[2] * m[9]);

	inv[2] = (m[1] * m[6] * m[15]) -
		(m[1] * m[7] * m[14]) -
		(m[5] * m[2] * m[15]) +
		(m[5] * m[3] * m[14]) +
		(m[13] * m[2] * m[7]) -
		(m[13] * m[3] * m[6]);

	inv[6] = (-m[0] * m[6] * m[15]) +
		(m[0] * m[7] * m[14]) +
		(m[4] * m[2] * m[15]) -
		(m[4] * m[3] * m[14]) -
		(m[12] * m[2] * m[7]) +
		(m[12] * m[3] * m[6]);

	inv[10] = (m[0] * m[5] * m[15]) -
		(m[0] * m[7] * m[13]) -
		(m[4] * m[1] * m[15]) +
		(m[4] * m[3] * m[13]) +
		(m[12] * m[1] * m[7]) -
		(m[12] * m[3] * m[5]);

	inv[14] = (-m[0] * m[5] * m[14]) +
		(m[0] * m[6] * m[13]) +
		(m[4] * m[1] * m[14]) -
		(m[4] * m[2] * m[13]) -
		(m[12] * m[1] * m[6]) +
		(m[12] * m[2] * m[5]);

	inv[3] = (-m[1] * m[6] * m[11]) +
		(m[1] * m[7] * m[10]) +
		(m[5] * m[2] * m[11]) -
		(m[5] * m[3] * m[10]) -
		(m[9] * m[2] * m[7]) +
		(m[9] * m[3] * m[6]);

	inv[7] = (m[0] * m[6] * m[11]) -
		(m[0] * m[7] * m[10]) -
		(m[4] * m[2] * m[11]) +
		(m[4] * m[3] * m[10]) +
		(m[8] * m[2] * m[7]) -
		(m[8] * m[3] * m[6]);

	inv[11] = (-m[0] * m[5] * m[11]) +
		(m[0] * m[7] * m[9]) +
		(m[4] * m[1] * m[11]) -
		(m[4] * m[3] * m[9]) -
		(m[8] * m[1] * m[7]) +
		(m[8] * m[3] * m[5]);

	inv[15] = (m[0] * m[5] * m[10]) -
		(m[0] * m[6] * m[9]) -
		(m[4] * m[1] * m[10]) +
		(m[4] * m[2] * m[9]) +
		(m[8] * m[1] * m[6]) -
		(m[8] * m[2] * m[5]);

	det = (m[0] * inv[0]) + (m[1] * inv[4]) + (m[2] * inv[8]) + (m[3] * inv[12]);
	return det;
}
const bool Matrix44::MatrixIsOrthogonal() const
{
	Vector4 row0 = rows[0];
	Vector4 row1 = rows[1];
	Vector4 row2 = rows[2];

	row0.w = 0.0f;
	row1.w = 0.0f;
	row2.w = 0.0f;

	return ((row1.dotProduct(row1) == 0.f)
		&& (row2.dotProduct(row0) == 0.0f)
		&& (row2.dotProduct(row1) == 0.0f));
}
const Vector4 Matrix44::GetColumn(const unsigned int& colIndex) const
{
	Vector4 col = Vector4();
	switch (colIndex)
	{
	case 0:
		col.x = rows[0].x;
		col.y = rows[1].x;
		col.z = rows[2].x;
		col.w = rows[3].x;
		break;
	case 1:
		col.x = rows[0].y;
		col.y = rows[1].y;
		col.z = rows[2].y;
		col.w = rows[3].y;
		break;
	case 2:
		col.x = rows[0].z;
		col.y = rows[1].z;
		col.z = rows[2].z;
		col.w = rows[3].z;
		break;
	case 3:
		col.x = rows[0].w;
		col.y = rows[1].w;
		col.z = rows[2].w;
		col.w = rows[3].w;
		break;
	default:
		break;
	}
	return col;
}
const Vector4 Matrix44::MatrixGetOffset() const
{
	return rows[3];
}


//----------------------------------------------------------------------------
//Global

const Vector4 operator*(const Vector4& v, const Matrix44& mat)
{
	return mat.TransformOnLeft(v);
}

const Matrix44 operator*(const float& f, const Matrix44& mat)
{
	return mat * f;
}


const Matrix44 Matrix44::MatrixLerp(const Matrix44& a, const Matrix44& b, const float& time)
{
	Vector3 r0, u0, f0, t0;
	Vector3 r1, u1, f1, t1;
	Matrix44::MatrixGetBasis(a, r0, u0, f0, t0);
	Matrix44::MatrixGetBasis(b, r1, u1, f1, t1);

	Vector3 r, u, f, t;
	r = Vector3::Slerp(r0, r1, time);
	u = Vector3::Slerp(u0, u1, time);
	f = Vector3::Slerp(f0, f1, time);
	t = Vector3::Lerp(t0, t1, time);

	return MatrixFromBasis(r, u, f, t);
}
#include "Engine/Math/Matrix22.hpp"

//-------------------------------------------------------------------
//static variables
const Matrix22 Matrix22::mat22_identity = Matrix22(true);
const Matrix22 Matrix22::mat22_zeros = Matrix22(false);
const int Matrix22::mat22_numOfVals = 4;
const int Matrix22::mat22_numOfRows = 2;

//---------------------------------------------------------------------------------------
//Constructors
Matrix22::Matrix22(const bool& makeIdentity)
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
Matrix22::Matrix22(const float& v0, const float& v1,
	const float& v2, const float& v3)
{
	rows[0] = Vector2(v0, v1);
	rows[1] = Vector2(v2, v3);
}
Matrix22::Matrix22(const Vector2& row1, const Vector2& row2)
{
	rows[0] = row1;
	rows[1] = row2;
}
Matrix22::Matrix22(const Matrix22& mat)
{
	rows[0] = mat.rows[0];
	rows[1] = mat.rows[1];
}

//-------------------------------------------------------------------------------------------------------------
//Setters
void Matrix22::MatrixMakeIdentity()
{
	data[0] = 1.f;
	data[1] = 0.f;
	data[2] = 1.f;
	data[3] = 0.f;
}
void Matrix22::MatrixMakeZeros()
{
	data[0] = 0.f;
	data[1] = 0.f;
	data[2] = 0.f;
	data[3] = 0.f;
}
void Matrix22::SetRow(const Vector2& row, unsigned int rowIndex)
{
	if (rowIndex > 0 && rowIndex < mat22_numOfRows)
	{
		rows[rowIndex] = row;
	}
}
void Matrix22::SetCol(const Vector2& col, unsigned int colIndex)
{
	switch (colIndex)
	{
	case 0:
		rows[0].x = col.x;
		rows[1].x = col.y;
		break;
	case 1:
		rows[0].y = col.x;
		rows[1].y = col.y;
		break;
	default:
		break;
	}
}

//-------------------------------------------------------------------------------------------------------------
//Operators
void Matrix22::MakeTranspose()
{
	//need to implement
}
void Matrix22::MakeNormalized()
{
	//need to implement
}
void Matrix22::MatrixAdjoint()
{
	Vector2 r1, r2;

	r1 = Vector2(rows[0].x, rows[1].x);
	r2 = Vector2(rows[0].y, rows[1].y);

	rows[0] = r1;
	rows[1] = r2;
}
void Matrix22::MatrixInvertOrthogonal(Matrix22& mat)
{
	mat;
	//need to implement
}
void Matrix22::MatrixInvert()
{
	float determinant = MatrixGetDeterminant();
	if (determinant == 0.f)
	{
		return;
	}
	MatrixAdjoint();

	rows[0] = rows[0] / determinant;
	rows[1] = rows[1] / determinant;

}
const Vector2 Matrix22::operator*(const Vector2& vec4)
{
	Vector2 toReturn;
	toReturn.x = (rows[0].x * vec4.x) + (rows[0].y *vec4.y);
	toReturn.y = (rows[1].x * vec4.x) + (rows[1].y *vec4.y);
	return toReturn;
}
void Matrix22::MatrixMultiply(const Matrix22* left, const Matrix22* right, Matrix22* out)
{
	//data[0] = (left[0] * right[0]) + (left[1] * right[2]);
	//data[2] = (left[2] * right[0]) + (left[3] * right[2]);
	//data[1] = (left[0] * right[1]) + (left[1] * right[3]);
	//data[3] = (left[2] * right[1]) + (left[3] * right[3]);

	for (size_t row = 0; row < mat22_numOfRows; row++)
	{
		for (size_t col = 0; col < mat22_numOfRows; col++)
		{
			out->data[(col * mat22_numOfRows) + row] = (left->data[(col * mat22_numOfRows)] * right->data[row])
				+ (left->data[(col * mat22_numOfRows) + 1] * right->data[row + mat22_numOfRows]);
		}
	}
}
const Vector2 Matrix22::TransformOnLeft(const Vector2& toMult) const
{

	Vector2 col0, col1, col2;
	col0 = GetColumn(0);
	col1 = GetColumn(1);
	Vector2 ret = Vector2(col0.dotProduct(toMult), col1.dotProduct(toMult));
	return ret;
}


//-------------------------------------------------------------------------------------------------------------
//Getters
const float Matrix22::MatrixGetDeterminant() const
{
	float determinant = (data[0] * data[3]) - (data[1] * data[2]);
	return determinant;
}
const bool Matrix22::MatrixIsOrthogonal() const
{
	//need to implement
	return false;
}
const Vector2 Matrix22::GetColumn(const unsigned int& colIndex) const
{
	switch (colIndex)
	{
	case 0:
		return Vector2(rows[0].x, rows[1].y);
	case 1:
		return Vector2(rows[0].y, rows[1].y);
	default:
		return Vector2(0.f, 0.f);
	}
}


const Vector2 operator*(const Vector2& v, const Matrix22& mat)
{
	return mat.TransformOnLeft(v);
}
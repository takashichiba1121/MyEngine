/*
* 4*4の行列
*/
#pragma once
#include "Vector3.h"

struct Matrix4
{
	float m[4][4];
};

namespace Matrix4Math
{

	Matrix4 identity();

	Matrix4 scale(const Vector3& s);

	Matrix4 rotateX(float angle);
	Matrix4 rotateY(float angle);
	Matrix4 rotateZ(float angle);

	Matrix4 translate(const Vector3& t);

	Vector3 transform(const Vector3& v, const Matrix4& m);

	Matrix4 MakeInverse(const Matrix4& matrix4);
	 
	Matrix4 ViewMat(const Vector3& eye, const Vector3& target, const Vector3& up);

	Matrix4 ProjectionMat(float fovAngleY, float aspectRatio, float nearZ, float farZ);

	float ConvertToRadians(float rot);
}

	Matrix4 operator*=(const Matrix4& m1, const Matrix4& m2);

	const Matrix4 operator*(const Matrix4& m1, const Matrix4& m2);
	const Vector3 operator*(const Vector3& v, const Matrix4& m);
 #include "Matrix4.h"
#include <cmath>

Matrix4 Matrix4Math::identity()
{
	static const Matrix4 result
	{
		1.0f,0.0f,0.0f,0.0f,
		0.0f,1.0f,0.0f,0.0f,
		0.0f,0.0f,1.0f,0.0f,
		0.0f,0.0f,0.0f,1.0f
	};

	return result;
}

Matrix4 Matrix4Math::scale(const Vector3& s)
{
	Matrix4 result = identity();
	result.m[0][0] = s.x;
	result.m[1][1] = s.y;
	result.m[2][2] = s.z;

	return result;
}

Matrix4 Matrix4Math::rotateX(float angle)
{
	float sin = std::sin(angle);
	float cos = std::cos(angle);

	Matrix4 result = identity();
	result.m[1][1] = cos;
	result.m[1][2] = sin;
	result.m[2][1] = -sin;
	result.m[2][2] = cos;

	return result;
}

Matrix4 Matrix4Math::rotateY(float angle)
{
	float sin = std::sin(angle);
	float cos = std::cos(angle);

	Matrix4 result = identity();
	result.m[0][0] = cos;
	result.m[0][2] = -sin;
	result.m[2][0] = sin;
	result.m[2][2] = cos;

	return result;
}

Matrix4 Matrix4Math::rotateZ(float angle)
{
	float sin = std::sin(angle);
	float cos = std::cos(angle);

	Matrix4 result = identity();
	result.m[0][0] = cos;
	result.m[0][1] = sin;
	result.m[1][0] = -sin;
	result.m[1][1] = cos;

	return result;
}

Matrix4 Matrix4Math::translate(const Vector3& t)
{
	Matrix4 result = identity();
	result.m[3][0] = t.x;
	result.m[3][1] = t.y;
	result.m[3][2] = t.z;

	return result;
}

Vector3 Matrix4Math::transform(const Vector3& v, const Matrix4& m)
{
	Vector3 matv = { 0,0,0 };
	matv.x = v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0];
	matv.y = v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1];
	matv.z = v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2];
	return matv;
}

Matrix4 Matrix4Math::MakeInverse(const Matrix4& matrix4)
{
	Matrix4 temp;
	float mat[4][8] = { 0 };

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			mat[i][j] = matrix4.m[i][j];
		}
	}

	mat[0][4] = 1;
	mat[1][5] = 1;
	mat[2][6] = 1;
	mat[3][7] = 1;

	for (int n = 0; n < 4; n++) {
		//最大の絶対値を探索する(とりあえず対象成分を最大と仮定しておく)
		float max = abs(mat[n][n]);
		int maxIndex = n;

		for (int i = n + 1; i < 4; i++) {
			if (abs(mat[i][n]) > max) {
				max = abs(mat[i][n]);
				maxIndex = i;
			}
		}

		//最大の絶対値が0だったら逆行列は求められない
		if (abs(mat[maxIndex][n]) <= 0.000001f) {
			return temp; //とりあえず単位行列返しちゃう
		}

		//入れ替え
		if (n != maxIndex) {
			for (int i = 0; i < 8; i++) {
				float f = mat[maxIndex][i];
				mat[maxIndex][i] = mat[n][i];
				mat[n][i] = f;
			}
		}

		//掛けたら1になる値を算出
		float mul = 1 / mat[n][n];

		//掛ける
		for (int i = 0; i < 8; i++) {
			mat[n][i] *= mul;
		}

		//他全部0にする
		for (int i = 0; i < 4; i++) {
			if (n == i) {
				continue;
			}

			mul = -mat[i][n];

			for (int j = 0; j < 8; j++) {
				mat[i][j] += mat[n][j] * mul;
			}
		}
	}

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++)
		{
			temp.m[i][j] = mat[i][j + 4];
		}
	}

	Matrix4 result = temp;

	return result;
}

Matrix4 Matrix4Math::ViewMat(const Vector3& eye, const Vector3& target, const Vector3& up)
{
	Vector3 zaxis = target - eye;
	zaxis.normalize();
	Vector3 xaxis = up.cross(zaxis);
	xaxis.normalize();
	Vector3 yaxis = zaxis.cross(xaxis);
	yaxis.normalize();

	Matrix4 LookAt = {
		xaxis.x,	xaxis.y,	xaxis.z,	0,
		yaxis.x,	yaxis.y,	yaxis.z,	0,
		zaxis.x,	zaxis.y,	zaxis.z,	0,
		eye.x,		eye.y,		eye.z,		1
	};

	LookAt=MakeInverse(LookAt);

	return LookAt;
}

Matrix4 Matrix4Math::ProjectionMat(float fovAngleY, float aspectRatio, float nearZ, float farZ)
{
	float h = 1 / tanf(fovAngleY / 2);
	float w = h / aspectRatio;
	float a = farZ / (farZ - nearZ);
	float b = (-nearZ * farZ) / (farZ - nearZ);

	Matrix4 perspectiveFovLH = {
		w,		 0,		 0,		 0,
		0,		 h,		 0,		 0,
		0,		 0,		 a,		 1,
		0,		 0,		 b,		 0
	};

	return perspectiveFovLH;
}

float Matrix4Math::ConvertToRadians(float rot)
{
	const float Radians = 3.1415f / 180.0f;

	return rot * Radians;
}

Matrix4 operator*=(const Matrix4& m1, const Matrix4& m2)
{
	Matrix4 result{ 0 };

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			for (int k = 0; k < 4; k++) {
				result.m[i][j] += m1.m[i][k] * m2.m[k][j];
			}
		}
	}

	return result;
}

const Matrix4 operator*(const Matrix4& m1, const Matrix4& m2)
{
	Matrix4 result = m1;
	
	return result *= m2;
}

const Vector3 operator*(const Vector3& v, const Matrix4& m)
{
	return Matrix4Math::transform(v, m);
}
 #include"vector3.h"
#include<cmath>
//コンストラクタ
//零ベクトルとする
Vector3::Vector3()
	:x(0), y(0), z(0)
{
}
Vector3::Vector3(float x, float y, float z)
	: x(x), y(y), z(z)
{
}//x成分、ｙ成分、ｚ成分を指定しての生成 

//メンバ関数
float Vector3::length() const
{
	return sqrt(x * x + y * y + z * z);
}
Vector3& Vector3::normalize()
{
	float len = length();
	if (len != 0)
	{
		return *this /= len;
	}
	return *this;
} 
float Vector3::dot(const Vector3& v) const
{
	return v.x * x + v.y * y + v.z * z;
}
Vector3 Vector3::cross(const Vector3& v) const
{
	Vector3 c;
	c.x = y * v.z - z * v.y;
	c.y = z * v.x - x * v.z;
	c.z = x * v.y - y * v.x;
	return c;
}

//単項演算子オーバーロード
Vector3 Vector3::operator+() const
{
	return *this;
}
Vector3 Vector3::operator-() const
{
	return Vector3(-x, -y, -z);
}

//代入演算子オーバーロード
Vector3& Vector3::operator+=(const Vector3& v)
{
	x += v.x;
	y += v.y;
	z += v.z;
	return *this;
}
Vector3& Vector3::operator-=(const Vector3& v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
	return *this;
}
Vector3& Vector3::operator*=(float s)
{
	x *= s;
	y *= s;
	z *= s;
	return *this;
}
Vector3& Vector3::operator*=(Vector3 s)
{
	x *= s.x;
	y *= s.y;
	z *= s.z;
	return *this;
}
Vector3& Vector3::operator/=(float s)
{
	x /= s;
	y /= s;
	z /= s;
	return *this;
}

//二項演算子オーバーロード
//いろんな引数のパターンに対応(引数の順序)するために、以下のように準備している
const Vector3 operator+(const Vector3& v1, const Vector3& v2)
{
	Vector3 temp(v1);
	return temp += v2;
}
const Vector3 operator-(const Vector3& v1, const Vector3& v2)
{
	Vector3 temp(v1);
	return temp -= v2;
}
const Vector3 operator*(const Vector3& v, float s)
{
	Vector3 temp(v);
	return temp *= s;
}
const Vector3 operator*(const Vector3& v, const Vector3 s)
{
	Vector3 temp(v);

	return temp*=s;
}
//const Vector3 operator*(float s, const Vector3& v)
//{
//	return v * s;
//}
const Vector3 operator/(const Vector3& v, float s)
{
	Vector3 temp(v);
	return temp /= s;
}

const bool operator==(const Vector3& v, const Vector3& s)
{
	return v.x==s.x&& v.y == s.y&& v.z == s.z;
}

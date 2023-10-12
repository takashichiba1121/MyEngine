#include"Vector4.h"
#include<cmath>
//コンストラクタ
//零ベクトルとする
Vector4::Vector4()
	:x(0), y(0), z(0),w(0)
{
}
Vector4::Vector4(float x, float y, float z,float w)
	: x(x), y(y), z(z),w(w)
{
}//x成分、ｙ成分、ｚ成分を指定しての生成 

//単項演算子オーバーロード
Vector4 Vector4::operator+() const
{
	return *this;
}
Vector4 Vector4::operator-() const
{
	return Vector4(-x, -y, -z,-w);
}

//代入演算子オーバーロード
Vector4& Vector4::operator+=(const Vector4& v)
{
	x += v.x;
	y += v.y;
	z += v.z;
	w += v.z;
	return *this;
}
Vector4& Vector4::operator-=(const Vector4& v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
	w -= v.z;
	return *this;
}
Vector4& Vector4::operator*=(float s)
{
	x *= s;
	y *= s;
	z *= s;
	w *= s;
	return *this;
}
Vector4& Vector4::operator/=(float s)
{
	x /= s;
	y /= s;
	z /= s;
	w /= s;
	return *this;
}

//二項演算子オーバーロード
//いろんな引数のパターンに対応(引数の順序)するために、以下のように準備している
const Vector4 operator+(const Vector4& v1, const Vector4& v2)
{
	Vector4 temp(v1);
	return temp += v2;
}
const Vector4 operator-(const Vector4& v1, const Vector4& v2)
{
	Vector4 temp(v1);
	return temp -= v2;
}
const Vector4 operator*(const Vector4& v, float s)
{
	Vector4 temp(v);
	return temp *= s;
}
const Vector4 operator*(float s, const Vector4& v)
{
	return v * s;
}
const Vector4 operator/(const Vector4& v, float s)
{
	Vector4 temp(v);
	return temp /= s;
}
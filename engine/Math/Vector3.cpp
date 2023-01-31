#include"vector3.h"
#include<cmath>
//�R���X�g���N�^
//��x�N�g���Ƃ���
Vector3::Vector3()
	:x(0), y(0), z(0)
{
}
Vector3::Vector3(float x, float y, float z)
	: x(x), y(y), z(z)
{
}//x�����A�������A���������w�肵�Ă̐��� 

//�����o�֐�
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

//�P�����Z�q�I�[�o�[���[�h
Vector3 Vector3::operator+() const
{
	return *this;
}
Vector3 Vector3::operator-() const
{
	return Vector3(-x, -y, -z);
}

//������Z�q�I�[�o�[���[�h
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
Vector3& Vector3::operator/=(float s)
{
	x /= s;
	y /= s;
	z /= s;
	return *this;
}

//�񍀉��Z�q�I�[�o�[���[�h
//�����Ȉ����̃p�^�[���ɑΉ�(�����̏���)���邽�߂ɁA�ȉ��̂悤�ɏ������Ă���
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
const Vector3 operator*(float s, const Vector3& v)
{
	return v * s;
}
const Vector3 operator/(const Vector3& v, float s)
{
	Vector3 temp(v);
	return temp /= s;
}
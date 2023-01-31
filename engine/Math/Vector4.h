#pragma once

class Vector4
{
public:
	float x; //x����
	float y; //y����
	float z; //z����
	float w;

public:
	//�R���X�g���N�^
	Vector4(); //��x�N�g���Ƃ���
	Vector4(float x, float y, float z,float w);//x�����A�������A���������w�肵�Ă̐��� 

	//�P�����Z�q�I�[�o�[���[�h
	Vector4 operator+() const;
	Vector4 operator-() const;

	//������Z�q�I�[�o�[���[�h
	Vector4& operator+=(const Vector4& v);
	Vector4& operator-=(const Vector4& v);
	Vector4& operator*=(float s);
	Vector4& operator/=(float s);
};
//�񍀉��Z�q�I�[�o�[���[�h
	//�����Ȉ����̃p�^�[���ɑΉ�(�����̏���)���邽�߂ɁA�ȉ��̂悤�ɏ������Ă���
const Vector4 operator+(const Vector4& v1, const Vector4& v2);
const Vector4 operator-(const Vector4& v1, const Vector4& v2);
const Vector4 operator*(const Vector4& v, float s);
const Vector4 operator*(float s, const Vector4& v);
const Vector4 operator/(const Vector4& v, float s);
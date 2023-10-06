#pragma once

class Vector4
{
public:
	float x; //x成分
	float y; //y成分
	float z; //z成分
	float w;

public:
	//コンストラクタ
	Vector4(); //零ベクトルとする
	Vector4(float x, float y, float z,float w);//x成分、ｙ成分、ｚ成分を指定しての生成 

	//単項演算子オーバーロード
	Vector4 operator+() const;
	Vector4 operator-() const;

	//代入演算子オーバーロード
	Vector4& operator+=(const Vector4& v);
	Vector4& operator-=(const Vector4& v);
	Vector4& operator*=(float s);
	Vector4& operator/=(float s); 
};
//二項演算子オーバーロード
	//いろんな引数のパターンに対応(引数の順序)するために、以下のように準備している
const Vector4 operator+(const Vector4& v1, const Vector4& v2);
const Vector4 operator-(const Vector4& v1, const Vector4& v2);
const Vector4 operator*(const Vector4& v, float s);
const Vector4 operator*(float s, const Vector4& v);
const Vector4 operator/(const Vector4& v, float s);
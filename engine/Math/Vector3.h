#pragma once

/*
* 三次元ベクトル
*/
class Vector3
{
public:
	float x; //x成分
	float y; //y成分
	float z; //z成分

public:
	//コンストラクタ
	Vector3(); //零ベクトルとする
	Vector3(float x, float y, float z);//x成分、ｙ成分、ｚ成分を指定しての生成 

	//メンバ関数
	//ノルム（長さ）を求める
	float length() const;
	//正規化
	Vector3& normalize();
	//内積を求める
	float dot(const Vector3& v) const;
	//外積を求める
	Vector3 cross(const Vector3& v) const;

	//単項演算子オーバーロード
	Vector3 operator+() const;
	Vector3 operator-() const;

	//代入演算子オーバーロード
	Vector3& operator+=(const Vector3& v); 
	Vector3& operator-=(const Vector3& v);
	Vector3& operator*=(float s);
	Vector3& operator*=(Vector3 s);
	Vector3& operator/=(float s);
};
//二項演算子オーバーロード
	//いろんな引数のパターンに対応(引数の順序)するために、以下のように準備している
const Vector3 operator+(const Vector3& v1, const Vector3& v2);
const Vector3 operator-(const Vector3& v1, const Vector3& v2);
const Vector3 operator*(const Vector3& v, float s);
const Vector3 operator*(const Vector3& v,const Vector3 s);
const Vector3 operator*(float s, const Vector3& v);
const Vector3 operator/(const Vector3& v, float s);
const bool operator==(const Vector3& v,const Vector3& s);
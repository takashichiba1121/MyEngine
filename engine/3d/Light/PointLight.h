/*
* 点ライト
*/
#pragma once
#include"Vector3.h"
#include"DirectXCommon.h"
class PointLight
{
private: // エイリアス
public:
	struct  ConstBufferData
	{
		Vector3 lightPos;
		float pad1;
		Vector3 lightcolor;
		float pad2;
		Vector3 lightatten;
		uint16_t active;
	};
private:

	Vector3 lightPos_ = { 1,0,0 };

	Vector3 lightColor_ = { 1,1,1 };

	Vector3 lightAtten_ = { 1.0f,1.0f,1.0f };

	//有効フラグ
	bool active_ = false;
public:

	void SetLightPos(const Vector3& lightPos) { lightPos_=lightPos; }

	void SetLightColor(const Vector3& lightColor) { lightColor_ = lightColor; }

	void SetLightAtten(const Vector3& lightAtten) {lightAtten_ = lightAtten; }

	Vector3 GetLightAtten() { return lightAtten_; }

	Vector3 GetLightPos() { return lightPos_; }

	Vector3 GetLightColor() { return lightColor_; }

	/// <summary>
	/// 有効フラグをセット
	/// </summary>
	/// <param name="active">有効フラグ</param>
	inline void SetActive(bool active) { this->active_ = active; }

	/// <summary>
	/// 有効チェック
	/// </summary>
	/// <returns>有効フラグ</returns>
	inline bool IsActive() { return active_; }

};

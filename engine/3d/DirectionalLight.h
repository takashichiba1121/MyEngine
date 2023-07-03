#pragma once
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>
#include"Vector3.h"
#include"DirectXCommon.h"

class DirectionalLight
{
private: // エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
public:
	struct  ConstBufferData
	{
		Vector3 lightv;
		float shininess;
		Vector3 lightcolor;
		uint16_t active;
	};
private:

	Vector3 lightDir_ = {1,0,0};

	Vector3 lightColor_ = { 1,1,1 };

	float shininess_ = 4;

	//有効フラグ
	bool active_ = false;
public:

	void SetLightDir(const Vector3& lightDir);

	void SetLightColor(const Vector3& lightColor);

	Vector3 GetLightDir() { return lightDir_; }

	Vector3 GetLightColor() { return lightColor_; }

	void SetShininess(const float shininess);

	float GetShininess() {return shininess_ ; }

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

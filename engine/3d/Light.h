#pragma once
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>
#include"Vector3.h"
#include"DirectXCommon.h"

class Light
{
private: // エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
public:
	struct  ConstBufferData
	{
		Vector3 lightv;
		float a;
		Vector3 lightcolor;
	};
private:
	static ID3D12Device* sDevice;
public:
	static void StaticInitialize(ID3D12Device* device);

private:
	ComPtr<ID3D12Resource> constBuff_;

	Vector3 lightDir_ = {1,0,0};

	Vector3 lightColor_ = { 1,1,1 };

	bool dirty_ = false;
public:
	void Initialize();

	void TransferConstBuffer();

	void SetLightDir(const Vector3& lightDir);

	void SetLightColor(const Vector3& lightColor);

	void Update();

	void Draw(ID3D12GraphicsCommandList* cmdList,UINT rootParameterIndex);
};

#pragma once
#include<d3dcompiler.h>
#include <tchar.h>
#include<iostream>
#include<cassert>
#include<string>
#include<DirectXMath.h>
#include<DirectXTex.h>
#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#include"DirectXCommon.h"
#include"Matrix4.h"

class SpriteCommon
{
public:
	//èâä˙âª
	static void Initialize(DirectXCommon* DXCommon);

	static DirectXCommon* GetDxCommon() {return sDxCommon ; }

	static ID3D12PipelineState* GetPipelineState() { return sPipelineState.Get(); }
	static ID3D12RootSignature* GetRootSignature() { return sRootSignature.Get(); }
	static Matrix4 GetMatProjection() { return sMatProjection; }

	static void PreDraw();

	static void PostDraw();

	static void Finalize();
private:
	static DirectXCommon* sDxCommon;

	static Microsoft::WRL::ComPtr<ID3D12PipelineState> sPipelineState;
	static Microsoft::WRL::ComPtr<ID3D12RootSignature> sRootSignature;

	static Matrix4 sMatProjection;
};


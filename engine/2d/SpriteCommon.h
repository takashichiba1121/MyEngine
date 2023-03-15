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

	static DirectXCommon* GetDxCommon() {return dxCommon ; }

	static ID3D12PipelineState* GetPipelineState() { return pipelineState.Get(); }
	static ID3D12RootSignature* GetRootSignature() { return rootSignature.Get(); }
	static Matrix4 GetMatProjection() { return matProjection; }

	static void PreDraw();

	static void PostDraw();
private:
	static DirectXCommon* dxCommon;

	static Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState;
	static Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature;

	static Matrix4 matProjection;
};


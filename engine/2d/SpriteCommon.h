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
	void Initialize(DirectXCommon* DXCommon);

	DirectXCommon* GetDxCommon() {return dxCommon ; }

	ID3D12PipelineState* GetPipelineState() { return pipelineState.Get(); }
	ID3D12RootSignature* GetRootSignature() { return rootSignature.Get(); }
	Matrix4 GetMatProjection() { return matProjection; }

	void PreDraw();

	void PostDrow();
private:
	DirectXCommon* dxCommon;

	Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState;
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature;

	Matrix4 matProjection;
};


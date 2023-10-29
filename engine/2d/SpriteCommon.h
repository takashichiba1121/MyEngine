/*
* スプライトの共通部分
*/
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
#pragma region Singleton
private:
	SpriteCommon() {};

	~SpriteCommon() {};
public:
	SpriteCommon(const SpriteCommon& carManager) = delete;

	SpriteCommon& operator=(const SpriteCommon& carManager) = delete;

	static SpriteCommon* Instance();
#pragma endregion
public:
	//初期化
	void Initialize(DirectXCommon* DXCommon);

	/// <summary>
	/// グラフィックパイプライン生成
	/// </summary>
	void InitializeGraphicsPipeline();

	/// <summary>
	/// ディゾルブグラフィックパイプライン生成
	/// </summary>
	void InitializeDissolveGraphicsPipeline();

	DirectXCommon* GetDxCommon() { return dxCommon; }

	Matrix4 GetMatProjection() { return matProjection; }

	void PreDraw();

	void PostDraw();

	void DissolvePreDraw();

	void DissolvePostDraw();

	void Finalize();
private:
	DirectXCommon* dxCommon=nullptr;

	Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState;
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature;
	Microsoft::WRL::ComPtr<ID3D12PipelineState> dissolvePipelineState;
	Microsoft::WRL::ComPtr<ID3D12RootSignature> dissolveRootSignature;

	Matrix4 matProjection=Matrix4Math::identity();
};


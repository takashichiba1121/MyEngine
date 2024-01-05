#pragma once
#include <d3d12.h>
#include <d3dx12.h>
#include <dxgi1_6.h>
#include<DirectXTex.h>
#include<wrl.h>
#include<array>
#include<string>
#include <Windows.h>
#include <cstdlib>
#include"DirectXCommon.h"
#include<string>
#include "WinApp.h"
/*
* テクスチャの管理
*/
class TextureManager
{
#pragma region Singleton
private:
	TextureManager() {};

	~TextureManager() {};
public:
	TextureManager(const TextureManager& carManager) = delete;

	TextureManager& operator=(const TextureManager& carManager) = delete;

	static TextureManager* Instance();
#pragma endregion
public:
	/// <summary>
	/// テクスチャの読み込み
	/// </summary>
	/// <param name="fileName">テクスチャのパス</param>
	/// <returns>テクスチャの識別番号</returns>
	uint32_t LoadTexture(const std::string fileName = "NULL");

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(DirectXCommon* DxCommon);

	/// <summary>
	/// SRVヒープの作成
	/// </summary>
	void CreateSRV(ID3D12Resource* texBuff);

	/// <summary>
	/// GPUにテクスチャを送る
	/// </summary>
	void ExcuteComandList();

	/// <summary>
	/// テクスチャをGPUに送る準備
	/// </summary>
	[[nodiscard]]
	ID3D12Resource* UploadTextureData(ID3D12Resource* texture, const DirectX::ScratchImage& mipImages, ID3D12Device* device,
		ID3D12GraphicsCommandList* commandList);

	/// <summary>
	/// 終了処理
	/// </summary>
	void Finalize();

public:
	const size_t spriteSRVCount = 2056;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descHeap;			//デスクリプタヒープ
	std::array< Microsoft::WRL::ComPtr<ID3D12Resource>,2056 >texBuffuers;	//テクスチャバッファ
	std::string textureName[ 2056 ];
	D3D12_RESOURCE_DESC textureResourceDesc = {};
	DirectXCommon* dxCommon_ = nullptr;
	uint32_t srvIncrementIndex = 0;
};


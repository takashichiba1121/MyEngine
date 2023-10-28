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
	const size_t spriteSRVCount = 2056;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descHeap;			//デスクリプタヒープ
	std::array< Microsoft::WRL::ComPtr<ID3D12Resource>,2056 >texBuffuers;	//テクスチャバッファ
	std::string textureName[2056];
	D3D12_RESOURCE_DESC textureResourceDesc = {};
	DirectXCommon* dxCommon_=nullptr;
	uint32_t srvIncrementIndex=0;
public:
	uint32_t LoadTexture(const std::string fileName = "NULL");
	void Initialize(DirectXCommon* DxCommon);
	void CreateSRV(ID3D12Resource* texBuff);

	void ExcuteComandList();

	[[nodiscard]]
	ID3D12Resource* UploadTextureData(ID3D12Resource* texture, const DirectX::ScratchImage& mipImages, ID3D12Device* device,
		ID3D12GraphicsCommandList* commandList);

	void Finalize();
};


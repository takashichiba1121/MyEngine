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

#include "WinApp.h"
class Texture
{
public:
	static const size_t sSpriteSRVCount = 2056;
	static Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> sDescHeap;			//デスクリプタヒープ
	static std::array< Microsoft::WRL::ComPtr<ID3D12Resource>, sSpriteSRVCount >sTexBuffuers;	//テクスチャバッファ
	static D3D12_RESOURCE_DESC sTextureResourceDesc;
	static DirectXCommon* sDxCommon_;
	static uint32_t sSrvIncrementIndex;
public:
	static uint32_t LoadTexture(const wchar_t* fileName = L"NULL");
	static void Initialize(DirectXCommon* DxCommon);
	static void CreateSRV(ID3D12Resource* texBuff);

	static void ExcuteComandList();

	[[nodiscard]]
	static ID3D12Resource* UploadTextureData(ID3D12Resource* texture, const DirectX::ScratchImage& mipImages, ID3D12Device* device,
		ID3D12GraphicsCommandList* commandList);

	static void fin();
};


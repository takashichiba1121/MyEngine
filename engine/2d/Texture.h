#pragma once
#include<DirectXTex.h>
#include<wrl.h>
#include<array>
#include<string>
#include <Windows.h>
#include <cstdlib>
#include <d3d12.h>
#include <d3dx12.h>
#include <dxgi1_6.h>

#include "WinApp.h"
class Texture
{
public:
	static const size_t spriteSRVCount = 2056;
	static Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descHeap;			//�f�X�N���v�^�q�[�v
	static std::array< Microsoft::WRL::ComPtr<ID3D12Resource>, spriteSRVCount >texBuffuers;	//�e�N�X�`���o�b�t�@
	static D3D12_RESOURCE_DESC textureResourceDesc;
	static Microsoft::WRL::ComPtr<ID3D12Device> dev;


public:
	static uint32_t LoadTexture(const wchar_t* fileName = L"NULL");
	static void Initialize(ID3D12Device* device);
	static void CreateSRV(ID3D12Resource* texBuff, D3D12_CPU_DESCRIPTOR_HANDLE& srvHandle);
};


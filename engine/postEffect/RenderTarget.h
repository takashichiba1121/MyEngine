#pragma once
#include"DirectXCommon.h"
#include"Matrix4.h"
class RenderTarget
{
public:

	static void StaticInitialze(ID3D12Device* dev);

	void Initialize();

	void PreDraw(ID3D12GraphicsCommandList* commandList);

	void PostDraw(ID3D12GraphicsCommandList* commandList);

	uint32_t GettexHandle() {return textureHandle;}

private:

		//デバイス
	static ID3D12Device* sDevice;


	Microsoft::WRL::ComPtr<ID3D12Resource> texBuff;
	//深度バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> depthBuff;
	//RTV用のデスクリプタヒープ
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descHeapRTV;
	//DSV用のデスクリプタヒープ
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descHeapDSV;

	uint32_t textureHandle;
};
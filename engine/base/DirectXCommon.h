#pragma once
#include<d3d12.h>
#include<dxgi1_6.h>
#include<wrl.h>
#include<vector>
#include"WinApp.h"
#include<chrono>

class DirectXCommon
{
public://メンバ関数
	//初期化
	void Initialize(WinApp* WinApp);

	void InitializeDevice();

	void InitializeCommand();

	void InitializeSwapchain();

	void InitializeRenderTargetView();
	 
	void InitializeDepthBuffer();
	 
	void IntializeFence();

	void PreDraw();

	void PostDrow();

	//デバイス取得
	ID3D12Device* GetDevice() const { return device_.Get(); }

	//コマンドリスト取得
	ID3D12GraphicsCommandList* GetCommandList() const { return commandList_.Get(); }

	WinApp* getWinApp() const { return winApp_; }

	//バックバッファの数を取得
	size_t GetBackBufferCount() const {return backBuffers_.size(); }

	ID3D12CommandQueue* GetCommandQueue() const { return commandQueue_.Get(); }

	void PreIncrimentFenceValue() {++fenceVal_ ; }

	ID3D12Fence* GetFence() const { return fence_.Get(); }
	
	uint32_t GetFenceValue() { return fenceVal_; }

	ID3D12CommandAllocator* GetCommandAllocator() {return commandAllocator_.Get(); }
private://メンバ関数
	//FPS固定初期化
	void InitializeFixFPS();

	//FPS固定更新
	void UpdateFixFPS();

private:

	HRESULT result_;
	Microsoft::WRL::ComPtr<ID3D12Device> device_;
	Microsoft::WRL::ComPtr<IDXGIFactory7> dxgiFactory_;
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> commandAllocator_ = nullptr;
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList_ = nullptr;
	Microsoft::WRL::ComPtr<ID3D12CommandQueue> commandQueue_ = nullptr;
	Microsoft::WRL::ComPtr<IDXGISwapChain4> swapChain_;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> rtvHeap_ = nullptr;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> dsvHeap_ = nullptr;
	std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> backBuffers_;
	Microsoft::WRL::ComPtr<ID3D12Fence> fence_ = nullptr;
	Microsoft::WRL::ComPtr<ID3D12Resource> depthBuff_ = nullptr;

	uint32_t fenceVal_ = 0;
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc_{};
	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc_{};
	D3D12_RESOURCE_BARRIER barrierDesc_{};

	//記録時間(FPS固定用)
	std::chrono::steady_clock::time_point reference_;

	//WindowsAPI
	WinApp* winApp_ = nullptr;
};


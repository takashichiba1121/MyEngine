#pragma once
#include<d3d12.h>
#include<dxgi1_6.h>
#include<wrl.h>
#include<vector>
#include"WinApp.h"
#include<chrono>

class DirectXCommon
{
public://�����o�֐�
	//������
	void Initialize(WinApp* WinApp);

	void InitializeDevice();

	void InitializeCommand();

	void InitializeSwapchain();

	void InitializeRenderTargetView();

	void InitializeDepthBuffer();

	void IntializeFence();

	void PreDraw();

	void PostDrow();

	//�f�o�C�X�擾
	ID3D12Device* GetDevice() const { return device.Get(); }

	//�R�}���h���X�g�擾
	ID3D12GraphicsCommandList* GetCommandList() const { return commandList.Get(); }

	WinApp* getWinApp() const { return winApp_; }

	//�o�b�N�o�b�t�@�̐����擾
	size_t GetBackBufferCount() const {return backBuffers.size(); }
private://�����o�֐�
	//FPS�Œ菉����
	void InitializeFixFPS();

	//FPS�Œ�X�V
	void UpdateFixFPS();

private:

	HRESULT result;
	Microsoft::WRL::ComPtr<ID3D12Device> device;
	Microsoft::WRL::ComPtr<IDXGIFactory7> dxgiFactory;
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> comdAllocator = nullptr;
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList = nullptr;
	Microsoft::WRL::ComPtr<ID3D12CommandQueue> commandQueue = nullptr;
	Microsoft::WRL::ComPtr<IDXGISwapChain4> swapChain;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> rtvHeap = nullptr;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> dsvHeap = nullptr;
	std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> backBuffers;
	Microsoft::WRL::ComPtr<ID3D12Fence> fence = nullptr;
	Microsoft::WRL::ComPtr<ID3D12Resource> depthBuff = nullptr;

	UINT64 fenceVal = 0;
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};
	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc{};
	D3D12_RESOURCE_BARRIER barrierDesc{};

	//�L�^����(FPS�Œ�p)
	std::chrono::steady_clock::time_point reference_;

	//WindowsAPI
	WinApp* winApp_ = nullptr;
};


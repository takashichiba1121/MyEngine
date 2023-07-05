#include "DirectXCommon.h"
#include<cassert>
#include<thread>

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")

using namespace Microsoft::WRL;

void DirectXCommon::Initialize(WinApp* WinApp)
{
	//NULL���o
	assert(WinApp);
	//�����o�ϐ��ɋL�^
	winApp_ = WinApp;

	//FPS�Œ菈��������
	InitializeFixFPS();

	//�f�o�C�X�̐���
	InitializeDevice();
	//�R�}���h�֘A�̏�����
	InitializeCommand();
	//�X���b�v�`�F�[���̏�����
	InitializeSwapchain();
	//�����_�[�^�[�Q�b�g�r���[�̏�����
	InitializeRenderTargetView();
	//�[�x�o�b�t�@�̏�����
	InitializeDepthBuffer();
	//�t�F���X�̏�����
	IntializeFence();
}

void DirectXCommon::InitializeDevice()
{
#ifdef _DEBUG
	//�f�o�b�O���C���[���I����
	ID3D12Debug1* debugController;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)))) {
		debugController->EnableDebugLayer();
		debugController->SetEnableGPUBasedValidation(TRUE);
	}
#endif

	//�Ή����x���̔z��
	D3D_FEATURE_LEVEL levels[] = {
		D3D_FEATURE_LEVEL_12_1,
		D3D_FEATURE_LEVEL_12_0,
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
	};
	//DXGI�t�@�N�g���[�̐���
	result_ = CreateDXGIFactory(IID_PPV_ARGS(&dxgiFactory_));
	assert(SUCCEEDED(result_));

	//�A�_�v�^�[�̗񋓗p
	std::vector<ComPtr<IDXGIAdapter4>>adapters;
	//�����ɓ���̖��O�����A�_�v�^�[�I�u�W�F�N�g������
	ComPtr<IDXGIAdapter4> tmpAdapter = nullptr;

	//�p�t�H�[�}���X���������̂��珇�ɁA���ׂẴA�_�v�^�[��񋓂���
	for (uint32_t i = 0;
		dxgiFactory_->EnumAdapterByGpuPreference(i,
			DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE,
			IID_PPV_ARGS(&tmpAdapter)) != DXGI_ERROR_NOT_FOUND;
		i++) {
		//���I�z��ɒǉ�����
		adapters.push_back(tmpAdapter);
	}
	//�Ó��ȃA�_�v�^��I�ʂ���
	for (size_t i = 0; i < adapters.size(); i++) {
		DXGI_ADAPTER_DESC3 adapterDesc;
		//�A�_�v�^�[�̏����擾����
		adapters[i]->GetDesc3(&adapterDesc);

		//�\�t�g�E�F�A�f�o�C�X�����
		if (!(adapterDesc.Flags & DXGI_ADAPTER_FLAG3_SOFTWARE)) {
			//�f�o�C�X���̗p���ă��[�v�𔲂���
			tmpAdapter = adapters[i].Get();
			break;
		}
	}
	D3D_FEATURE_LEVEL featureLevel;

	for (size_t i = 0; i < _countof(levels); i++) {
		//�̗p�����A�_�v�^�[�Ńf�o�C�X�𐶐�
		result_ = D3D12CreateDevice(tmpAdapter.Get(), levels[i],
			IID_PPV_ARGS(&device_));
		if (result_ == S_OK) {
			//�f�o�C�X�𐶐��ł������_�Ń��[�v�𔲂���
			featureLevel = levels[i];
		}
	}

#ifdef _DEBUG

	ID3D12InfoQueue* infoQueue;
	if (SUCCEEDED(device_->QueryInterface(IID_PPV_ARGS(&infoQueue)))) {
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, true);//��΂��G���[�̎��Ɏ~�܂�
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, true);//�G���[���Ɏ~�܂�
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, true);//�G���[���Ɏ~�܂�
		infoQueue->Release();
	}

	//�}������G���[
	D3D12_MESSAGE_ID denyIds[] = {

		/*
		*Windows11�ł�DXGI�f�o�b�N���C���[��DX12�f�o�b�N���C���[�̑��ݍ�p�o�O�ɂ��G���[���b�Z�[�W
		* https://stackoverflow.com/questions/69805245/directx-12-application-is-crashing-in-windows-11
		*/
		D3D12_MESSAGE_ID_RESOURCE_BARRIER_MISMATCHING_COMMAND_LIST_TYPE
	};

	//�}������\�����x��
	D3D12_MESSAGE_SEVERITY severities[] = { D3D12_MESSAGE_SEVERITY_INFO };
	D3D12_INFO_QUEUE_FILTER filter{};
	filter.DenyList.NumIDs = _countof(denyIds);
	filter.DenyList.pIDList = denyIds;
	filter.DenyList.NumSeverities = _countof(severities);
	filter.DenyList.pSeverityList = severities;
	//�w�肵���G���[�̕\����}������
	infoQueue->PushStorageFilter(&filter);
#endif
}

void DirectXCommon::InitializeCommand()
{
	//�R�}���h�A���P�[�^�𐶐�
	result_ = device_->CreateCommandAllocator(
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		IID_PPV_ARGS(&commandAllocator_));
	assert(SUCCEEDED(result_));
	//�R�}���h���X�g�̐���
	result_ = device_->CreateCommandList(0,
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		commandAllocator_.Get(), nullptr,
		IID_PPV_ARGS(&commandList_));
	//�R�}���h�L���[�̐ݒ�
	D3D12_COMMAND_QUEUE_DESC commandQueueDesc{};
	//�R�}���h�L���[�𐶐�
	result_ = device_->CreateCommandQueue(&commandQueueDesc, IID_PPV_ARGS(&commandQueue_));
	assert(SUCCEEDED(result_));
}

void DirectXCommon::InitializeSwapchain()
{
	//�X���b�v�`�F�[���̐ݒ�
	swapChainDesc_.Width = 1280;
	swapChainDesc_.Height = 720;
	swapChainDesc_.Format = DXGI_FORMAT_R8G8B8A8_UNORM;//�F���̏���
	swapChainDesc_.SampleDesc.Count = 1;//�}���`�T���v�����Ȃ�
	swapChainDesc_.BufferUsage = DXGI_USAGE_BACK_BUFFER;//�o�b�N�o�b�t�@�p
	swapChainDesc_.BufferCount = 2;
	swapChainDesc_.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;//�t���b�v��͔j��
	swapChainDesc_.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	//IDXGISwapChain1��Ptr��p��
	ComPtr<IDXGISwapChain1> swapchain1;

	//�X���b�v�`�F�[���̐���
	result_ = dxgiFactory_->CreateSwapChainForHwnd(
		commandQueue_.Get(), winApp_->GetHwnd(), &swapChainDesc_, nullptr, nullptr,
		&swapchain1);
	assert(SUCCEEDED(result_));

	//��������IDXGISwapChain1�̃I�u�W�F�N�g��IDXGISwapChain4�ɕϊ�����
	swapchain1.As(&swapChain_);
}

void DirectXCommon::InitializeRenderTargetView()
{
	//�f�X�N���v�^�q�[�v�̐ݒ�
	rtvHeapDesc_.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;//�����_�[�^�[�Q�b�g�r���[
	rtvHeapDesc_.NumDescriptors = swapChainDesc_.BufferCount;//���\�̓��

	//�f�X�N���v�^�q�[�v�̐���
	device_->CreateDescriptorHeap(&rtvHeapDesc_, IID_PPV_ARGS(&rtvHeap_));

	//�o�b�N�o�b�t�@
	backBuffers_.resize(swapChainDesc_.BufferCount);

	//�X���b�v�`�F�[���̂��ׂẴo�b�t�@�ɂ��ď�������
	for (size_t i = 0; i < backBuffers_.size(); i++) {
		//�X���b�v�`�F�[������o�b�t�@���擾
		swapChain_->GetBuffer((uint32_t)i, IID_PPV_ARGS(&backBuffers_[i]));
		//�f�X�N���v�^�q�[�v�̃n���h�����擾
		D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = rtvHeap_->GetCPUDescriptorHandleForHeapStart();
		//�����\���ŃA�h���X�������
		rtvHandle.ptr += i * device_->GetDescriptorHandleIncrementSize(rtvHeapDesc_.Type);
		//�����_�[�^�[�Q�b�g�r���[���ݒ�
		D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};
		//�V�F�[�_�[�̌v�Z���ʂ�SRGB�ɕϊ����ď�������
		rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
		rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
		//�����_�[�^�[�Q�b�g�r���[�̐���
		device_->CreateRenderTargetView(backBuffers_[i].Get(), &rtvDesc, rtvHandle);
	}
}

void DirectXCommon::InitializeDepthBuffer()
{
	//���\�[�X�ݒ�
	D3D12_RESOURCE_DESC depthResourceDesc{};
	depthResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	depthResourceDesc.Width = WinApp::window_width;//�����_�[�^�[�Q�b�g�ɍ��킹��
	depthResourceDesc.Height = WinApp::window_height;//�����_�[�^�[�Q�b�g�ɍ��킹��
	depthResourceDesc.DepthOrArraySize = 1;
	depthResourceDesc.Format = DXGI_FORMAT_D32_FLOAT;//�[�x�t�H�[�}�b�g
	depthResourceDesc.SampleDesc.Count = 1;
	depthResourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;//�f�v�X�X�e���V��

	//�[�x�l�p�q�[�v�v���p�e�B
	D3D12_HEAP_PROPERTIES depthHeapProp{};
	depthHeapProp.Type = D3D12_HEAP_TYPE_DEFAULT;
	//�[�w�l�̃N���A�ݒ�
	D3D12_CLEAR_VALUE depthClearValue{};
	depthClearValue.DepthStencil.Depth = 1.0f;//�[�x1.0f(�ő�l)�N���A
	depthClearValue.Format = DXGI_FORMAT_D32_FLOAT;//�[�x�l�t�H�[�}�b�g

	//���\�[�X
	result_ = device_->CreateCommittedResource(
		&depthHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&depthResourceDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,//�[�x�l�������݂Ɏg�p
		&depthClearValue,
		IID_PPV_ARGS(&depthBuff_));

	//�[�x�r���[�p�f�X�N���v�^�q�[�v�쐬
	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc{};
	dsvHeapDesc.NumDescriptors = 1;//�[�x�r���[��1��
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;//�f�X�X�e���V���r���[
	result_ = device_->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&dsvHeap_));

	//�[�x�r���[�쐬
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;//�[�x�l�t�H�[�}�b�g
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	device_->CreateDepthStencilView(
		depthBuff_.Get(),
		&dsvDesc,
		dsvHeap_->GetCPUDescriptorHandleForHeapStart());
}

void DirectXCommon::IntializeFence()
{
	//�t�F���X�̐���

	result_ = device_->CreateFence(fenceVal_, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence_));
}

void DirectXCommon::PreDraw()
{
	//�o�b�N�o�b�t�@�̔ԍ����擾�i�Q�Ȃ̂łO�Ԃ��P�ԁj
	uint32_t bbIndex = swapChain_->GetCurrentBackBufferIndex();

	//1.���\�[�X�o���A�ŏ������݉\�ɕύX
	barrierDesc_.Transition.pResource = backBuffers_[bbIndex].Get();//�o�b�N�o�b�t�@���w��
	barrierDesc_.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;//�\����Ԃ���
	barrierDesc_.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
	commandList_->ResourceBarrier(1, &barrierDesc_);
	//2.�`���̕ύX
	// �����_�[�^�[�Q�b�g�r���[�̃n���h�����擾
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = rtvHeap_->GetCPUDescriptorHandleForHeapStart();
	rtvHandle.ptr += bbIndex * device_->GetDescriptorHandleIncrementSize(rtvHeapDesc_.Type);
	//�[�x�X�e���V���r���[�p�f�X�N���v�^�q�[�v�̃n���h�����擾
	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = dsvHeap_->GetCPUDescriptorHandleForHeapStart();
	commandList_->OMSetRenderTargets(1, &rtvHandle, false, &dsvHandle);

	//3.��ʃN���A
	FLOAT clearColor[] = { 0.1f,0.25f,0.5f,0.0f };//���ۂ��F
	commandList_->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
	commandList_->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

	//�r���[�|�[�g�ݒ�R�}���h
	D3D12_VIEWPORT viewport{};
	viewport.Width = WinApp::window_width;
	viewport.Height = WinApp::window_height;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	//�r���[�|�[�g�ݒ�R�}���h���A�R�}���h���X�g�ɐς�
	commandList_->RSSetViewports(1, &viewport);

	//�V�U�[�Z�a
	D3D12_RECT scissorRect{};
	scissorRect.left = 0;
	scissorRect.right = scissorRect.left + WinApp::window_width;
	scissorRect.top = 0;
	scissorRect.bottom = scissorRect.top + WinApp::window_height;

	//�V�U�\�Z�a�ݒ�R�}���h���A�R�}���h���X�g�ɐς�
	commandList_->RSSetScissorRects(1, &scissorRect);
}

void DirectXCommon::PostDrow()
{
	//�o�b�N�o�b�t�@�̔ԍ����擾�i�Q�Ȃ̂łO�Ԃ��P�ԁj
	uint32_t bbIndex = swapChain_->GetCurrentBackBufferIndex();

	// 5.���\�[�X�o���A��߂�
	barrierDesc_.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;//�`���Ԃ���
	barrierDesc_.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
	commandList_->ResourceBarrier(1, &barrierDesc_);

	//���߃N���[�Y
	result_ = commandList_->Close();
	assert(SUCCEEDED(result_));
	//�R�}���h���X�g�̎��s
	ID3D12CommandList* commandLists[] = { commandList_.Get() };
	commandQueue_->ExecuteCommandLists(1, commandLists);

	//��ʂɕ\������o�b�t�@���t���b�v�i���\�̓���ւ��j
	result_ = swapChain_->Present(1, 0);
	assert(SUCCEEDED(result_));
	//�R�}���h�̎��s������҂�
	commandQueue_->Signal(fence_.Get(), ++fenceVal_);
	if (fence_->GetCompletedValue() != fenceVal_) {
		HANDLE event = CreateEvent(nullptr, false, false, nullptr);
		fence_->SetEventOnCompletion(fenceVal_, event);
		WaitForSingleObject(event, INFINITE);
		CloseHandle(event);
	}

	//FPS�Œ�
	UpdateFixFPS();
	
	//�L���[�̃N���A
	result_ = commandAllocator_->Reset();
	assert(SUCCEEDED(result_));
	//�ĂуR�}���h���X�g�����߂鏀��
	result_ = commandList_->Reset(commandAllocator_.Get(), nullptr);
	assert(SUCCEEDED(result_));
}

void DirectXCommon::InitializeFixFPS()
{
	reference_ = std::chrono::steady_clock::now();
}

void DirectXCommon::UpdateFixFPS()
{
	// 1/60�b�҂�����̎���
	const std::chrono::microseconds kMinTime(uint64_t(1000000.0f / 60.0f));
	//1/60�b���킸���ɒZ������
	const std::chrono::microseconds kMinCheckTime(uint64_t(1000000.0f / 65.0f));

	//���ݎ��Ԃ��擾����
	std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
	//�O��L�^����̌o�ߎ��ԂɎ擾����
	std::chrono::microseconds elapsed =
		std::chrono::duration_cast<std::chrono::microseconds>(now - reference_);

	// 1/60�b(���킸���ɒZ������)�o���Ă��Ȃ��ꍇ
	if (elapsed<kMinCheckTime)
	{
		//�@1/60�b�o�߂���܂Ŕ����ȃX���[�v���J��Ԃ�
		while (std::chrono::steady_clock::now()-reference_<kMinTime)
		{
			//1�}�C�N���b�X���[�v
			std::this_thread::sleep_for(std::chrono::microseconds(1));
		}
	}
	//���݂̎��Ԃ��L�^����
	reference_ = std::chrono::steady_clock::now();
}

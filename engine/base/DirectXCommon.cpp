#include "DirectXCommon.h"
#include<cassert>
#include<thread>

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")

using namespace Microsoft::WRL;

void DirectXCommon::Initialize(WinApp* WinApp)
{
	//NULL検出
	assert(WinApp);
	//メンバ変数に記録
	winApp_ = WinApp;

	//FPS固定処理初期化
	InitializeFixFPS();

	//デバイスの生成
	InitializeDevice();
	//コマンド関連の初期化
	InitializeCommand();
	//スワップチェーンの初期化
	InitializeSwapchain();
	//レンダーターゲットビューの初期化
	InitializeRenderTargetView();
	//深度バッファの初期化
	InitializeDepthBuffer();
	//フェンスの初期化
	IntializeFence();
}

void DirectXCommon::InitializeDevice()
{
#ifdef _DEBUG
	//デバッグレイヤーをオンに
	ID3D12Debug1* debugController;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)))) {
		debugController->EnableDebugLayer();
		debugController->SetEnableGPUBasedValidation(TRUE);
	}
#endif

	//対応レベルの配列
	D3D_FEATURE_LEVEL levels[] = {
		D3D_FEATURE_LEVEL_12_1,
		D3D_FEATURE_LEVEL_12_0,
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
	};
	//DXGIファクトリーの生成
	result = CreateDXGIFactory(IID_PPV_ARGS(&dxgiFactory));
	assert(SUCCEEDED(result));

	//アダプターの列挙用
	std::vector<ComPtr<IDXGIAdapter4>>adapters;
	//ここに特定の名前を持つアダプターオブジェクトが入る
	ComPtr<IDXGIAdapter4> tmpAdapter = nullptr;

	//パフォーマンスが高いものから順に、すべてのアダプターを列挙する
	for (UINT i = 0;
		dxgiFactory->EnumAdapterByGpuPreference(i,
			DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE,
			IID_PPV_ARGS(&tmpAdapter)) != DXGI_ERROR_NOT_FOUND;
		i++) {
		//動的配列に追加する
		adapters.push_back(tmpAdapter);
	}
	//妥当なアダプタを選別する
	for (size_t i = 0; i < adapters.size(); i++) {
		DXGI_ADAPTER_DESC3 adapterDesc;
		//アダプターの情報を取得する
		adapters[i]->GetDesc3(&adapterDesc);

		//ソフトウェアデバイスを回避
		if (!(adapterDesc.Flags & DXGI_ADAPTER_FLAG3_SOFTWARE)) {
			//デバイスを採用してループを抜ける
			tmpAdapter = adapters[i].Get();
			break;
		}
	}
	D3D_FEATURE_LEVEL featureLevel;

	for (size_t i = 0; i < _countof(levels); i++) {
		//採用したアダプターでデバイスを生成
		result = D3D12CreateDevice(tmpAdapter.Get(), levels[i],
			IID_PPV_ARGS(&device));
		if (result == S_OK) {
			//デバイスを生成できた時点でループを抜ける
			featureLevel = levels[i];
		}
	}

#ifdef _DEBUG

	ID3D12InfoQueue* infoQueue;
	if (SUCCEEDED(device->QueryInterface(IID_PPV_ARGS(&infoQueue)))) {
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, true);//やばいエラーの時に止まる
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, true);//エラー時に止まる
		infoQueue->Release();
	}

	//抑制するエラー
	D3D12_MESSAGE_ID denyIds[] = {

		/*
		*Windows11でのDXGIデバックレイヤーとDX12デバックレイヤーの相互作用バグによるエラーメッセージ
		* https://stackoverflow.com/questions/69805245/directx-12-application-is-crashing-in-windows-11
		*/
		D3D12_MESSAGE_ID_RESOURCE_BARRIER_MISMATCHING_COMMAND_LIST_TYPE
	};

	//抑制する表示レベル
	D3D12_MESSAGE_SEVERITY severities[] = { D3D12_MESSAGE_SEVERITY_INFO };
	D3D12_INFO_QUEUE_FILTER filter{};
	filter.DenyList.NumIDs = _countof(denyIds);
	filter.DenyList.pIDList = denyIds;
	filter.DenyList.NumSeverities = _countof(severities);
	filter.DenyList.pSeverityList = severities;
	//指定したエラーの表示を抑制する
	infoQueue->PushStorageFilter(&filter);
#endif
}

void DirectXCommon::InitializeCommand()
{
	//コマンドアロケータを生成
	result = device->CreateCommandAllocator(
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		IID_PPV_ARGS(&comdAllocator));
	assert(SUCCEEDED(result));
	//コマンドリストの生成
	result = device->CreateCommandList(0,
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		comdAllocator.Get(), nullptr,
		IID_PPV_ARGS(&commandList));
	//コマンドキューの設定
	D3D12_COMMAND_QUEUE_DESC commandQueueDesc{};
	//コマンドキューを生成
	result = device->CreateCommandQueue(&commandQueueDesc, IID_PPV_ARGS(&commandQueue));
	assert(SUCCEEDED(result));
}

void DirectXCommon::InitializeSwapchain()
{
	//スワップチェーンの設定
	swapChainDesc.Width = 1280;
	swapChainDesc.Height = 720;
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;//色情報の書式
	swapChainDesc.SampleDesc.Count = 1;//マルチサンプルしない
	swapChainDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER;//バックバッファ用
	swapChainDesc.BufferCount = 2;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;//フリップ後は破棄
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	//IDXGISwapChain1のPtrを用意
	ComPtr<IDXGISwapChain1> swapchain1;

	//スワップチェーンの生成
	result = dxgiFactory->CreateSwapChainForHwnd(
		commandQueue.Get(), winApp_->GetHwnd(), &swapChainDesc, nullptr, nullptr,
		&swapchain1);
	assert(SUCCEEDED(result));

	//生成したIDXGISwapChain1のオブジェクトをIDXGISwapChain4に変換する
	swapchain1.As(&swapChain);
}

void DirectXCommon::InitializeRenderTargetView()
{
	//デスクリプタヒープの設定
	rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;//レンダーターゲットビュー
	rtvHeapDesc.NumDescriptors = swapChainDesc.BufferCount;//裏表の二つ

	//デスクリプタヒープの生成
	device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&rtvHeap));

	//バックバッファ
	backBuffers.resize(swapChainDesc.BufferCount);

	//スワップチェーンのすべてのバッファについて処理する
	for (size_t i = 0; i < backBuffers.size(); i++) {
		//スワップチェーンからバッファを取得
		swapChain->GetBuffer((UINT)i, IID_PPV_ARGS(&backBuffers[i]));
		//デスクリプタヒープのハンドルを取得
		D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = rtvHeap->GetCPUDescriptorHandleForHeapStart();
		//裏か表かでアドレスがずれる
		rtvHandle.ptr += i * device->GetDescriptorHandleIncrementSize(rtvHeapDesc.Type);
		//レンダーターゲットビューも設定
		D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};
		//シェーダーの計算結果をSRGBに変換して書き込む
		rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
		rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
		//レンダーターゲットビューの生成
		device->CreateRenderTargetView(backBuffers[i].Get(), &rtvDesc, rtvHandle);
	}
}

void DirectXCommon::InitializeDepthBuffer()
{
	//リソース設定
	D3D12_RESOURCE_DESC depthResourceDesc{};
	depthResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	depthResourceDesc.Width = WinApp::window_width;//レンダーターゲットに合わせる
	depthResourceDesc.Height = WinApp::window_heigit;//レンダーターゲットに合わせる
	depthResourceDesc.DepthOrArraySize = 1;
	depthResourceDesc.Format = DXGI_FORMAT_D32_FLOAT;//深度フォーマット
	depthResourceDesc.SampleDesc.Count = 1;
	depthResourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;//デプスステンシル

	//深度値用ヒーププロパティ
	D3D12_HEAP_PROPERTIES depthHeapProp{};
	depthHeapProp.Type = D3D12_HEAP_TYPE_DEFAULT;
	//深層値のクリア設定
	D3D12_CLEAR_VALUE depthClearValue{};
	depthClearValue.DepthStencil.Depth = 1.0f;//深度1.0f(最大値)クリア
	depthClearValue.Format = DXGI_FORMAT_D32_FLOAT;//深度値フォーマット

	//リソース
	result = device->CreateCommittedResource(
		&depthHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&depthResourceDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,//深度値書き込みに使用
		&depthClearValue,
		IID_PPV_ARGS(&depthBuff));

	//深度ビュー用デスクリプタヒープ作成
	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc{};
	dsvHeapDesc.NumDescriptors = 1;//深度ビューは1つ
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;//デスステンシルビュー
	result = device->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&dsvHeap));

	//深度ビュー作成
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;//深度値フォーマット
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	device->CreateDepthStencilView(
		depthBuff.Get(),
		&dsvDesc,
		dsvHeap->GetCPUDescriptorHandleForHeapStart());
}

void DirectXCommon::IntializeFence()
{
	//フェンスの生成

	result = device->CreateFence(fenceVal, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));
}

void DirectXCommon::PreDraw()
{
	//バックバッファの番号を取得（２つなので０番か１番）
	UINT bbIndex = swapChain->GetCurrentBackBufferIndex();

	//1.リソースバリアで書き込み可能に変更
	barrierDesc.Transition.pResource = backBuffers[bbIndex].Get();//バックバッファを指定
	barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;//表示状態から
	barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
	commandList->ResourceBarrier(1, &barrierDesc);
	//2.描画先の変更
	// レンダーターゲットビューのハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = rtvHeap->GetCPUDescriptorHandleForHeapStart();
	rtvHandle.ptr += bbIndex * device->GetDescriptorHandleIncrementSize(rtvHeapDesc.Type);
	//深度ステンシルビュー用デスクリプタヒープのハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = dsvHeap->GetCPUDescriptorHandleForHeapStart();
	commandList->OMSetRenderTargets(1, &rtvHandle, false, &dsvHandle);

	//3.画面クリア
	FLOAT clearColor[] = { 0.1f,0.25f,0.5f,0.0f };//青っぽい色
	commandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
	commandList->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

	//ビューポート設定コマンド
	D3D12_VIEWPORT viewport{};
	viewport.Width = WinApp::window_width;
	viewport.Height = WinApp::window_heigit;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	//ビューポート設定コマンドを、コマンドリストに積む
	commandList->RSSetViewports(1, &viewport);

	//シザー短径
	D3D12_RECT scissorRect{};
	scissorRect.left = 0;
	scissorRect.right = scissorRect.left + WinApp::window_width;
	scissorRect.top = 0;
	scissorRect.bottom = scissorRect.top + WinApp::window_heigit;

	//シザ―短径設定コマンドを、コマンドリストに積む
	commandList->RSSetScissorRects(1, &scissorRect);
}

void DirectXCommon::PostDrow()
{
	//バックバッファの番号を取得（２つなので０番か１番）
	UINT bbIndex = swapChain->GetCurrentBackBufferIndex();

	// 5.リソースバリアを戻す
	barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;//描画状態から
	barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
	commandList->ResourceBarrier(1, &barrierDesc);

	//命令クローズ
	result = commandList->Close();
	assert(SUCCEEDED(result));
	//コマンドリストの実行
	ID3D12CommandList* commandLists[] = { commandList.Get() };
	commandQueue->ExecuteCommandLists(1, commandLists);

	//画面に表示するバッファをフリップ（裏表の入れ替え）
	result = swapChain->Present(1, 0);
	assert(SUCCEEDED(result));
	//コマンドの実行完了を待つ
	commandQueue->Signal(fence.Get(), ++fenceVal);
	if (fence->GetCompletedValue() != fenceVal) {
		HANDLE event = CreateEvent(nullptr, false, false, nullptr);
		fence->SetEventOnCompletion(fenceVal, event);
		WaitForSingleObject(event, INFINITE);
		CloseHandle(event);
	}

	//FPS固定
	UpdateFixFPS();
	
	//キューのクリア
	result = comdAllocator->Reset();
	assert(SUCCEEDED(result));
	//再びコマンドリストをためる準備
	result = commandList->Reset(comdAllocator.Get(), nullptr);
	assert(SUCCEEDED(result));
}

void DirectXCommon::InitializeFixFPS()
{
	reference_ = std::chrono::steady_clock::now();
}

void DirectXCommon::UpdateFixFPS()
{
	// 1/60秒ぴったりの時間
	const std::chrono::microseconds kMinTime(uint64_t(1000000.0f / 60.0f));
	//1/60秒よりわずかに短い時間
	const std::chrono::microseconds kMinCheckTime(uint64_t(1000000.0f / 65.0f));

	//現在時間を取得する
	std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
	//前回記録からの経過時間に取得する
	std::chrono::microseconds elapsed =
		std::chrono::duration_cast<std::chrono::microseconds>(now - reference_);

	// 1/60秒(よりわずかに短い時間)経っていない場合
	if (elapsed<kMinCheckTime)
	{
		//　1/60秒経過するまで微小なスリープを繰り返す
		while (std::chrono::steady_clock::now()-reference_<kMinTime)
		{
			//1マイクロ秒スリープ
			std::this_thread::sleep_for(std::chrono::microseconds(1));
		}
	}
	//現在の時間を記録する
	reference_ = std::chrono::steady_clock::now();
}

#include<d3dx12.h>
#include"WinApp.h"
#include "RenderTarget.h"
#include<cassert>
#include"PostEffectCommon.h"

//静的メンバ変数の実体
ID3D12Device* RenderTarget::sDevice = nullptr;

void RenderTarget::StaticInitialze(ID3D12Device* dev)
{
	sDevice = dev;
}
void RenderTarget::Initialize()
{
	const float clearColor[ 4 ] = { 0.0f,0.0f,0.0f,0.0f };

	HRESULT result;

	D3D12_RESOURCE_DESC texresDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,
		WinApp::window_width,
		( UINT ) WinApp::window_height,
		1,0,1,0,D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET);
	{
		//テクスチャバッファの生成
		//ヒープ設定
		CD3DX12_HEAP_PROPERTIES heapProp(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK,
			D3D12_MEMORY_POOL_L0);

		CD3DX12_CLEAR_VALUE clearValue(DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,clearColor);


		result = sDevice->CreateCommittedResource(
			&heapProp,
			D3D12_HEAP_FLAG_NONE,
			&texresDesc,
			D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
			&clearValue,
			IID_PPV_ARGS(&texBuff));
		assert(SUCCEEDED(result));
		//texBuff->SetName(L"Luminance");

		//{//テクスチャを赤クリア
		//	//画素数(1280*720=921600ピクセル)
		//	const UINT pixelCount = WinApp::window_width * WinApp::window_height;
		//	//画像1行分のデータサイズ
		//	const UINT rowPitch = sizeof(UINT) * WinApp::window_width;
		//	//画像全体のデータサイズ
		//	const UINT depthPitch = rowPitch * WinApp::window_height;
		//	//画像イメージ
		//	UINT* img = new UINT[pixelCount];
		//	for (int i = 0; i < pixelCount; i++) { img[i] = 0xff0000ff; }


		//	result = texBuff->WriteToSubresource(0, nullptr,
		//		img, rowPitch, depthPitch);
		//	assert(SUCCEEDED(result));
		//	delete[] img;
		//}
	}
	textureHandle = PostEffectCommon::Instance()->CreateDescHeapSRV(texBuff.Get());

	//RTV用のデスクリプタヒープ設定
	D3D12_DESCRIPTOR_HEAP_DESC rtvDescHeapDesc{};

	rtvDescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvDescHeapDesc.NumDescriptors = 1;

	//RTV用のデスクリプタヒープを生成
	result = sDevice->CreateDescriptorHeap(&rtvDescHeapDesc,IID_PPV_ARGS(&descHeapRTV));
	assert(SUCCEEDED(result));
	//レンダーターゲットビューも設定
	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};
	//シェーダーの計算結果をSRGBに変換して書き込む
	rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
	//デスクリプタヒープにRTVを作成
	sDevice->CreateRenderTargetView(texBuff.Get(),&rtvDesc,descHeapRTV->GetCPUDescriptorHandleForHeapStart());

	//深度バッファ
	CD3DX12_RESOURCE_DESC depthResDesc =
		CD3DX12_RESOURCE_DESC::Tex2D(
			DXGI_FORMAT_D32_FLOAT,
			WinApp::window_width,
			WinApp::window_height,
			1,0,
			1,0,
			D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL);

	CD3DX12_HEAP_PROPERTIES HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

	CD3DX12_CLEAR_VALUE CLEAR_VALUE(DXGI_FORMAT_D32_FLOAT,1.0f,0);

	result = sDevice->CreateCommittedResource(
		&HEAP_PROPERTIES,
		D3D12_HEAP_FLAG_NONE,
		&depthResDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,
		&CLEAR_VALUE,
		IID_PPV_ARGS(&depthBuff));
	assert(SUCCEEDED(result));

	//DSV用のデスクリプタヒープ設定
	D3D12_DESCRIPTOR_HEAP_DESC DescHeapDesc{};
	DescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	DescHeapDesc.NumDescriptors = 1;
	//DSV用デスクリプタヒープの作成
	result = sDevice->CreateDescriptorHeap(&DescHeapDesc,IID_PPV_ARGS(&descHeapDSV));
	assert(SUCCEEDED(result));

	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	sDevice->CreateDepthStencilView(depthBuff.Get(),
		&dsvDesc,
		descHeapDSV->GetCPUDescriptorHandleForHeapStart());
}

void RenderTarget::PreDraw(ID3D12GraphicsCommandList* commandList)
{
	CD3DX12_RESOURCE_BARRIER resouceBar = CD3DX12_RESOURCE_BARRIER::Transition(texBuff.Get(),
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
		D3D12_RESOURCE_STATE_RENDER_TARGET);

	//リソースバリアを変更(シェーダーリソース→描画可能)
	commandList->ResourceBarrier(1,
		&resouceBar);

	//レンダーターゲットビュー用のディスクリプタヒープのハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE rtvH = descHeapRTV->GetCPUDescriptorHandleForHeapStart();
	//深度ステンシルビュー用デスクリプタヒープのハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE dsvH = descHeapDSV->GetCPUDescriptorHandleForHeapStart();
	//レンダーターゲットをセット
	commandList->OMSetRenderTargets(1,&rtvH,false,&dsvH);

	CD3DX12_VIEWPORT viewPort(0.0f,0.0f,WinApp::window_width,WinApp::window_height);

	//ビューポートの設定
	commandList->RSSetViewports(1,&viewPort);

	CD3DX12_RECT rect(0,0,WinApp::window_width,WinApp::window_height);

	//シザリング矩形の設定
	commandList->RSSetScissorRects(1,&rect);

	FLOAT clearColor[ ] = { 0.0f,0.0f,0.0f,0.0f };//青っぽい色
	//全画面のクリア
	commandList->ClearRenderTargetView(rtvH,clearColor,0,nullptr);
	//深度バッファのクリア
	commandList->ClearDepthStencilView(dsvH,D3D12_CLEAR_FLAG_DEPTH,1.0f,0,0,nullptr);
}

void RenderTarget::PostDraw(ID3D12GraphicsCommandList* commandList)
{
	CD3DX12_RESOURCE_BARRIER RESOURCE_BARRIER = CD3DX12_RESOURCE_BARRIER::Transition(texBuff.Get(),
D3D12_RESOURCE_STATE_RENDER_TARGET,D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);

	commandList->ResourceBarrier(1,&RESOURCE_BARRIER);
}

#include "PostEffect.h"
#include<d3dx12.h>
#include"WinApp.h"
#include"Texture.h"
#include"SpriteCommon.h"

using namespace DirectX;

const float PostEffect::clearColor[4] = {0.25f,0.5f,0.1f,0.0f};

PostEffect::PostEffect() :Sprite()
{

}

void PostEffect::Initialize()
{
	HRESULT result;

	ID3D12Device* device = dxCommon->GetDevice();

	D3D12_RESOURCE_DESC texresDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,
		WinApp::window_width,
		(UINT)WinApp::window_height,
		1,0,1,0,D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET);

	//テクスチャバッファの生成
	//ヒープ設定
	CD3DX12_HEAP_PROPERTIES heapProp(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK,
		D3D12_MEMORY_POOL_L0);

	CD3DX12_CLEAR_VALUE clearValue(DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, clearColor);


	result = device->CreateCommittedResource(
	&heapProp,
	D3D12_HEAP_FLAG_NONE,
		&texresDesc,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
		&clearValue,
		IID_PPV_ARGS(&texBuff));
	assert(SUCCEEDED(result));

	{//テクスチャを赤クリア
		//画素数(1280*720=921600ピクセル)
		const UINT pixelCount = WinApp::window_width * WinApp::window_height;
		//画像1行分のデータサイズ
		const UINT rowPitch = sizeof(UINT) * WinApp::window_width;
		//画像全体のデータサイズ
		const UINT depthPitch = rowPitch * WinApp::window_height;
		//画像イメージ
		UINT* img = new UINT[pixelCount];
		for (int i = 0; i < pixelCount; i++) { img[i] = 0xff0000ff; }


		result = texBuff->WriteToSubresource(0,nullptr,
			img,rowPitch,depthPitch);
		assert(SUCCEEDED(result));
		delete[] img;
	}
	//SRV用のデスクリプタヒープ設定
	D3D12_DESCRIPTOR_HEAP_DESC srvDescHeapDesc = {};
	srvDescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvDescHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	srvDescHeapDesc.NumDescriptors = 1;
	//SRVデスクリプタヒープ設定
	result = device->CreateDescriptorHeap(&srvDescHeapDesc,IID_PPV_ARGS(&descHeapSRV));
	assert(SUCCEEDED(result));
	//SRV設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;

	//デスクリプタヒープにSRVを作成
	device->CreateShaderResourceView(texBuff.Get(),
		&srvDesc,
		descHeapSRV->GetCPUDescriptorHandleForHeapStart()
	);

	//RTV用のデスクリプタヒープ設定
	D3D12_DESCRIPTOR_HEAP_DESC rtvDescHeapDesc{};

	rtvDescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvDescHeapDesc.NumDescriptors = 1;
	
	//RTV用のデスクリプタヒープを生成
	result = device->CreateDescriptorHeap(&rtvDescHeapDesc, IID_PPV_ARGS(&descHeapRTV));
	assert(SUCCEEDED(result));
	//デスクリプタヒープにRTVを作成
	device->CreateRenderTargetView(texBuff.Get(), nullptr, descHeapRTV->GetCPUDescriptorHandleForHeapStart());

	//深度バッファ
	CD3DX12_RESOURCE_DESC depthResDesc =
		CD3DX12_RESOURCE_DESC::Tex2D(
			DXGI_FORMAT_D32_FLOAT,
			WinApp::window_width,
			WinApp::window_height,
			1, 0,
			1, 0,
			D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL);

	CD3DX12_HEAP_PROPERTIES HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

	CD3DX12_CLEAR_VALUE CLEAR_VALUE(DXGI_FORMAT_D32_FLOAT, 1.0f, 0);

	result = device->CreateCommittedResource(
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
	result = device->CreateDescriptorHeap(&DescHeapDesc, IID_PPV_ARGS(&descHeapDSV));
	assert(SUCCEEDED(result));

	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	device->CreateDepthStencilView(depthBuff.Get(),
		&dsvDesc,
		descHeapDSV->GetCPUDescriptorHandleForHeapStart());


	Sprite::Initialize();
}

void PostEffect::PreDrawScene(ID3D12GraphicsCommandList* cmdList)
{
	CD3DX12_RESOURCE_BARRIER resouceBar = CD3DX12_RESOURCE_BARRIER::Transition(texBuff.Get(),
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
		D3D12_RESOURCE_STATE_RENDER_TARGET);

	//リソースバリアを変更(シェーダーリソース→描画可能)
	cmdList->ResourceBarrier(1,
		&resouceBar);

	//レンダーターゲットビュー用のディスクリプタヒープのハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE rtvH = descHeapRTV->GetCPUDescriptorHandleForHeapStart();
	//深度ステンシルビュー用デスクリプタヒープのハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE dsvH = descHeapDSV->GetCPUDescriptorHandleForHeapStart();
	//レンダーターゲットをセット
	cmdList->OMSetRenderTargets(1, &rtvH, false, &dsvH);

	CD3DX12_VIEWPORT viewPort(0.0f, 0.0f, WinApp::window_width, WinApp::window_height);

	//ビューポートの設定
	cmdList->RSSetViewports(1, &viewPort);

	CD3DX12_RECT rect(0, 0, WinApp::window_width, WinApp::window_height);

	//シザリング矩形の設定
	cmdList->RSSetScissorRects(1, &rect);

	//全画面のクリア
	cmdList->ClearRenderTargetView(rtvH, clearColor, 0, nullptr);
	//深度バッファのクリア
	cmdList->ClearDepthStencilView(dsvH, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
}

void PostEffect::Draw()
{
	//パイプラインステートとルートシグネチャの設定コマンド
	dxCommon->GetCommandList()->SetPipelineState(SpriteCommon::GetPipelineState());
	dxCommon->GetCommandList()->SetGraphicsRootSignature(SpriteCommon::GetRootSignature());

	//SRVヒープの設定コマンド
	ID3D12DescriptorHeap* ppHeaps[] = { descHeapSRV.Get()};
	dxCommon->GetCommandList()->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	//プリミティブ形状の設定コマンド
	dxCommon->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	//頂点バッファビューの設定コマンド
	dxCommon->GetCommandList()->IASetVertexBuffers(0, 1, &vbView);
	//定数バッファビュー（CBV）の設定コマンド
	dxCommon->GetCommandList()->SetGraphicsRootConstantBufferView(0, constBuffMaterial->GetGPUVirtualAddress());
	//定数バッファビュー（CBV）の設定コマンド
	dxCommon->GetCommandList()->SetGraphicsRootConstantBufferView(2, constBuffTransform->GetGPUVirtualAddress());

	//画像描画
	//SRVヒープの先頭ハンドルを取得（SRVを指しているはず）
	dxCommon->GetCommandList()->SetGraphicsRootDescriptorTable(1, descHeapSRV->GetGPUDescriptorHandleForHeapStart());

	//描画コマンド
	dxCommon->GetCommandList()->DrawInstanced(_countof(vertices), 1, 0, 0);//すべての頂点を使って描画
}

void PostEffect::PostDrawScene(ID3D12GraphicsCommandList* cmdList)
{
	CD3DX12_RESOURCE_BARRIER RESOURCE_BARRIER= CD3DX12_RESOURCE_BARRIER::Transition(texBuff.Get(),
		D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);

	cmdList->ResourceBarrier(1, &RESOURCE_BARRIER);
}

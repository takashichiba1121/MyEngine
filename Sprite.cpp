#include "Sprite.h"


using namespace DirectX;
using namespace std;

void Sprite::Initialize(SpriteCommon* SpriteCommon)
{
	this->spriteCommon = SpriteCommon;

	this->dxCommon = spriteCommon->GetDxCommon();

	HRESULT result;

	Vertex vertices_[4] = {
		{{-0.4f,-0.7f,0.0f},{0.0f,1.0f}},
		{{-0.4f,+0.7f,0.0f},{0.0f,0.0f}},
		{{+0.4f,-0.7f,0.0f},{1.0f,1.0f}},
		{{+0.4f,+0.7f,0.0f},{1.0f,0.0f}},
	};
	for (int i = 0; i < 4; i++)
	{
		vertices[i] = vertices_[i];
	}

	//頂点データ全体のサイズ=頂点データ一つ分のサイズ*頂点データの要素数
	UINT sizeVB = static_cast<UINT>(sizeof(vertices_[0]) * _countof(vertices_));
	//頂点バッファの設定
	D3D12_HEAP_PROPERTIES heapProp{};  //ヒープ設定
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD; //GPUへの転送用
	//リソース設定
	D3D12_RESOURCE_DESC resDesc{};
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeVB; //頂点データ全体のサイズ
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	//頂点バッファの生成
	result = dxCommon->GetDevice()->CreateCommittedResource(
		&heapProp, //ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&resDesc, //リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff));
	assert(SUCCEEDED(result));

	//GPU上のバッファに対応した仮想メモリ（メインメモリ上）を取得
	Vertex* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result));
	//前頂点に対して
	for (int i = 0; i < _countof(vertices_); i++) {
		vertMap[i] = vertices_[i]; //座標コピー
	}
	//繋がり解除
	vertBuff->Unmap(0, nullptr);

	//GPU仮想アドレス
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	//頂点バッファのサイズ
	vbView.SizeInBytes = sizeVB;
	//頂点一つ分のデータサイズ
	vbView.StrideInBytes = sizeof(vertices_[0]);

	//ヒープ設定
	D3D12_HEAP_PROPERTIES cbHeapProp{};
	cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;//GPUへの転送用
	//リソース設定
	D3D12_RESOURCE_DESC cbResourceDesc{};
	cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbResourceDesc.Width = (sizeof(ConstBufferDataMaterial) + 0xff) & ~0xff;//256バイトアラインメント
	cbResourceDesc.Height = 1;
	cbResourceDesc.DepthOrArraySize = 1;
	cbResourceDesc.MipLevels = 1;
	cbResourceDesc.SampleDesc.Count = 1;
	cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	//定数バッファの生成
	result = dxCommon->GetDevice()->CreateCommittedResource(
		&cbHeapProp,//ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&cbResourceDesc,//リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffMaterial));
	assert(SUCCEEDED(result));
	//定数バッファのマッピング
	ConstBufferDataMaterial* constMapMaterial = nullptr;
	result = constBuffMaterial->Map(0, nullptr, (void**)&constMapMaterial);//マッピング
	//値を書き込むと自動的に転送される
	constMapMaterial->color = XMFLOAT4(1, 0, 0, 0.5f);//RGBAで半透明の赤
	assert(SUCCEEDED(result));
}

void Sprite::Update()
{
}

void Sprite::Draw()
{
	//パイプラインステートとルートシグネチャの設定コマンド
	dxCommon->GetCommandList()->SetPipelineState(spriteCommon->GetPipelineState());
	dxCommon->GetCommandList()->SetGraphicsRootSignature(spriteCommon->GetRootSignature());
	//プリミティブ形状の設定コマンド
	dxCommon->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	//頂点バッファビューの設定コマンド
	dxCommon->GetCommandList()->IASetVertexBuffers(0, 1, &vbView);
	//定数バッファビュー（CBV）の設定コマンド
	dxCommon->GetCommandList()->SetGraphicsRootConstantBufferView(0, constBuffMaterial->GetGPUVirtualAddress());
	//描画コマンド
	dxCommon->GetCommandList()->DrawInstanced(_countof(vertices), 1, 0, 0);//すべての頂点を使って描画
}

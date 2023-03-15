#include "Sprite.h"
#include<DirectXTex.h>
#include"Texture.h"


using namespace DirectX;
using namespace std;

DirectXCommon* Sprite::dxCommon;

void Sprite::StaticInitialize()
{

	dxCommon = SpriteCommon::GetDxCommon();
}

void Sprite::Initialize(uint32_t textureIndex)
{

	HRESULT result;

	Vertex vertices_[4] = {
		{{  0.0f,100.0f,0.0f},{0.0f,1.0f}},
		{{  0.0f,  0.0f,0.0f},{0.0f,0.0f}},
		{{100.0f,100.0f,0.0f},{1.0f,1.0f}},
		{{100.0f,  0.0f,0.0f},{1.0f,0.0f}},
	};
	for (int i = 0; i < 4; i++)
	{
		vertices[i] = vertices_[i];
	}

	//頂点データ全体のサイズ=頂点データ一つ分のサイズ*頂点データの要素数
	UINT sizeVB = static_cast<UINT>(sizeof(vertices[0]) * _countof(vertices));
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
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result));
	//前頂点に対して
	for (int i = 0; i < _countof(vertices); i++) {
		vertMap[i] = vertices[i]; //座標コピー
	}

	//GPU仮想アドレス
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	//頂点バッファのサイズ
	vbView.SizeInBytes = sizeVB;
	//頂点一つ分のデータサイズ
	vbView.StrideInBytes = sizeof(vertices[0]);
	{
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
	}
	//定数バッファのマッピング
	result = constBuffMaterial->Map(0, nullptr, (void**)&constMapMaterial);//マッピング
	//値を書き込むと自動的に転送される
	constMapMaterial->color = { 1.0f, 1.0f, 1.0f, 1.0f };
	assert(SUCCEEDED(result));

	{
		//ヒープ設定
		D3D12_HEAP_PROPERTIES cbHeapProp{};
		cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;//GPUへの転送用
		//リソース設定
		D3D12_RESOURCE_DESC cbResourceDesc{};
		cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		cbResourceDesc.Width = (sizeof(ConstBufferDateTransform) + 0xff) & ~0xff;//256バイトアラインメント
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
			IID_PPV_ARGS(&constBuffTransform));
		assert(SUCCEEDED(result));
	}
	//定数バッファのマッピング
	result = constBuffTransform->Map(0, nullptr, (void**)&constMapTransform);//マッピング
	//値を書き込むと自動的に転送される
	constMapTransform->mat = Matrix4Math::identity();

	constMapTransform->mat = SpriteCommon::GetMatProjection();

	assert(SUCCEEDED(result));

	if (textureIndex != UINT_MAX)
	{
		this->textureIndex = textureIndex;
		AdjustTextureSize();
		scale_ = textureSize_;
	}


}

void Sprite::AdjustTextureSize()
{

	ID3D12Resource* textureBuffer = Texture::texBuffuers[textureIndex].Get();
	//指定番号の画像が読み込み済みなら
	assert(textureBuffer);

	//テクスチャ情報取得
	D3D12_RESOURCE_DESC resDesc = textureBuffer->GetDesc();

	textureSize_.x = static_cast<float>(resDesc.Width);
	textureSize_.y = static_cast<float>(resDesc.Height);
}

void Sprite::Update()
{
	ID3D12Resource* textureBuffer = Texture::texBuffuers[textureIndex].Get();
	//指定番号の画像が読み込み済みなら
	if (textureBuffer)
	{
		//テクスチャ情報取得
		D3D12_RESOURCE_DESC resDesc = textureBuffer->GetDesc();

		float tex_left = textureLeftTop_.x / resDesc.Width;
		float tex_right = (textureLeftTop_.x + textureSize_.x) / resDesc.Width;
		float tex_top = textureLeftTop_.y / resDesc.Height;
		float tex_bottom = (textureLeftTop_.y + textureSize_.y) / resDesc.Height;
		//頂点のUVに反映する
		vertices[LB].uv = { tex_left,tex_bottom };//左下
		vertices[LT].uv = { tex_left,tex_top };//左下
		vertices[RB].uv = { tex_right,tex_bottom };//左下
		vertices[RT].uv = { tex_right,tex_top };//左下
	}


	float left = (0.0f - anchorPoint_.x) * scale_.x;
	float right = (1.0f - anchorPoint_.x) * scale_.x;
	float top = (0.0f - anchorPoint_.y) * scale_.y;
	float bottom = (1.0f - anchorPoint_.y) * scale_.y;

	//左右反転
	if (isFlipX_) {
		left = -left;
		right = -right;
	}

	//上下反転
	if (isFlipY_) {
		top = -top;
		bottom = -bottom;
	}

	vertices[LB].pos = { left,bottom,0.0f };
	vertices[LT].pos = { left,top,0.0f };
	vertices[RB].pos = { right,bottom,0.0f };
	vertices[RT].pos = { right,top ,0.0f };

	//前頂点に対して
	for (int i = 0; i < _countof(vertices); i++) {
		vertMap[i] = vertices[i]; //座標コピー
	}

	//ワールド変換行列
	Matrix4 matWorld = Matrix4Math::identity();

	Matrix4 matRot;//回転行列
	matRot = Matrix4Math::rotateZ(XMConvertToRadians(rotation_));//Z軸回りに0度回転

	Matrix4 matTrans; //平行移動行列
	matTrans = Matrix4Math::translate({ position_.x, position_.y, 0.0f });//(-50,0,0)平行移動

	matWorld = matRot* matTrans;//ワールド行列に回転を反映
	//行列の計算
	constMapTransform->mat = matWorld * SpriteCommon::GetMatProjection();
}

void Sprite::Draw()
{
	if (isInvisible_)
	{
		return;
	}
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
	D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = Texture::descHeap->GetGPUDescriptorHandleForHeapStart();
	UINT incrementSize = dxCommon->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	srvGpuHandle.ptr += incrementSize * textureIndex;
	dxCommon->GetCommandList()->SetGraphicsRootDescriptorTable(1, srvGpuHandle);

	//描画コマンド
	dxCommon->GetCommandList()->DrawInstanced(_countof(vertices), 1, 0, 0);//すべての頂点を使って描画
}

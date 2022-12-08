#pragma once
#include"SpriteCommon.h"

struct Vertex
	{
	DirectX::XMFLOAT3 pos;    //xyz座標
	DirectX::XMFLOAT2 uv;     //uv座標
	};

//定数バッファ用データ構造体（マテリアル）
struct ConstBufferDataMaterial {
	DirectX::XMFLOAT4 color;//色（RGBA）
};
class Sprite
{
public:
	//初期化
	void Initialize(SpriteCommon* SpriteCommon);

	void Update();

	void Draw();
private:
	Vertex vertices[4] = {};

	Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff = nullptr;	//頂点バッファ

	SpriteCommon* spriteCommon;

	DirectXCommon* dxCommon;

	//頂点バッファビューの作成
	D3D12_VERTEX_BUFFER_VIEW vbView{};

	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffMaterial = nullptr;

};


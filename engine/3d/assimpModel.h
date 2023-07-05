#pragma once
#include <cassert>
#include <string>
#include <Windows.h>
#include <fstream>
#include <sstream>
#include <DirectXMath.h>
#include <vector>
#include <d3d12.h>
#include <d3dx12.h>
#include <DirectXTex.h>
#include"Vector2.h"
#include"Vector3.h"
#include"Vector4.h"
#include"Matrix4.h"

class AssimpModel
{
private:
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
public:

	// 定数バッファ用データ構造体B1
	struct ConstBufferDataB1
	{
		Vector3 ambient;	//アンビエント係数
		float pad1;			//パディング
		Vector3 diffuse;	//ディフューズ係数
		float pad2;			//パディング
		Vector3 specular;	//スペキュラー係数
		float alpha;		//アルファ
	};
	// 頂点データ構造体
	struct VertexPosNormalUv
	{
		Vector3 pos; // xyz座標
		Vector3 normal; // 法線ベクトル
		Vector2 uv;  // uv座標
	};
	//マテリアル
	struct Material {
		std::string name;	//マテリアル名
		Vector3 ambient;	//アンビエント影響度
		Vector3 diffuse;	//ディフューズ影響度
		Vector3 specular;	//スペキュラー影響度
		float alpha;		//アルファ
		uint32_t  texHandle;	//テクスチャファイル名
		//コンストラクタ
		Material() {
			ambient = { 0.3f,0.3f,0.3f };
			diffuse = { 0.0f,0.0f,0.0f };
			specular = { 0.0f,0.0f,0.0f };
			alpha = 1.0f;
			texHandle = 0;
		}
	};

	struct Mesh {
		std::vector<VertexPosNormalUv> vertexs;
		std::vector<unsigned short> indices;
		Material materials;

		// 頂点バッファ
		ComPtr<ID3D12Resource> vertBuff;
		// 頂点バッファビュー
		D3D12_VERTEX_BUFFER_VIEW vbView;
		// インデックスバッファ
		ComPtr<ID3D12Resource> indexBuff;
		// インデックスバッファビュー
		D3D12_INDEX_BUFFER_VIEW ibView;
		// 定数バッファ(マテリアル)
		ComPtr<ID3D12Resource> constBuffB1;
	};
public:

	~AssimpModel();
	void CreateBuffers();

	//OBJファイルから3Dモデルを読み込む
	static AssimpModel* LoadFormFBX(const std::string& modelname);

	//setter
	static void SetDevice(ID3D12Device* device) { sDevice = device; }

	void Draw(ID3D12GraphicsCommandList* cmdList, uint32_t rootParamIndexMaterial);

	void SetMesh(std::unique_ptr<AssimpModel::Mesh> Meshs) { meshs_.push_back(std::move(Meshs)); }

private:
	std::vector<std::unique_ptr<AssimpModel::Mesh>> meshs_;

	//デバイス
	static ID3D12Device* sDevice;
	// シェーダリソースビューのハンドル(CPU)
	CD3DX12_CPU_DESCRIPTOR_HANDLE cpuDescHandleSRV_;
	// シェーダリソースビューのハンドル(CPU)
	CD3DX12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV_;
	// 定数バッファ(マテリアル)
	ComPtr<ID3D12Resource> constBuffB1_;
};
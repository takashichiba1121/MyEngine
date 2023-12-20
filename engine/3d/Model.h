/*
* objモデルクラス
*/
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
#include<map>
//3Dモデル
class Model 
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
		std::string textureFilename;	//テクスチャファイル名
		//コンストラクタ
		Material() {
			ambient = { 1,1,1 };
			diffuse = { 1,1,1 };
			specular = { 1,1,1 };
			alpha = 1.0f;
		}
	};
public://メンバ関数
	//OBJファイルから3Dモデルを読み込む
	static Model* LoadFormOBJ(const std::string& modelname, bool smoothing);

		//OBJファイルから3Dモデルを読み込む
	static Model* CreatePlaneModel(uint32_t texIndex);

	//setter
	static void SetDevice(ID3D12Device* device) { Model::sDevice = device; }

	//各種バッファの生成
	void CreateBuffers();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="cmdlist">描画コマンドリスト</param>
	/// <param name="rootParamIndexMaterial">マテリアル用ルートパラメータ番号</param>
	void Draw(ID3D12GraphicsCommandList* cmdList, uint32_t rootParamIndexMaterial);

	/// <summary>
/// 画像指定描画
/// </summary>
/// <param name="cmdlist">描画コマンドリスト</param>
/// <param name="rootParamIndexMaterial">マテリアル用ルートパラメータ番号</param>
	void Draw(ID3D12GraphicsCommandList* cmdList, uint32_t rootParamIndexMaterial,uint32_t TextureHandle);

	std::vector<VertexPosNormalUv> GetVertices() {return vertices_; }


private://非公開のメンバ関数
	//OBJファイルから3Dモデルを読み込む(非公開)
	void LoadFromOBJInternal(const std::string& modelname, bool smoothing);

	void CreatePlaneModelJInternal(uint32_t texIndex);
	
	void LoadTexture(const std::string& directoryPath, const std::string& filename);

	/// <summary>
/// マテリアルの読み込み
/// </summary>
	void LoadMaterial(const std::string& directoryPath, const std::string& filename);

private:
	//デバイス
	static ID3D12Device* sDevice;
		// 頂点データ配列
	std::vector<VertexPosNormalUv> vertices_;
	// インデックスバッファ
	ComPtr<ID3D12Resource> indexBuff_;
	// インデックスバッファビュー
	D3D12_INDEX_BUFFER_VIEW ibView_ = {};
	// 頂点インデックス配列
	std::vector<unsigned short>indices_;
	//マテリアル
	Material material_;
	// 定数バッファ(マテリアル)
	ComPtr<ID3D12Resource> constBuffB1_;

	uint32_t textureIndex_=0;
};
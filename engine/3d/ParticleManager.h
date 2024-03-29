#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4.h"
#include"object3d.h"
/*
* 3Dパーティクルクラス(CPUパーティクル)
*/
class ParticleManager
{
private: // エイリアス

public: // サブクラス
	//// 頂点データ構造体
	//struct VertexPosNormalUv
	//{
	//	Vector3 pos; // xyz座標
	//	Vector3 normal; // 法線ベクトル
	//	vextor2 uv;  // uv座標
	//};
	 
	// 頂点データ構造体
	struct VertexPos
	{
		Vector3 pos; // xyz座標
		float scale;
		Vector4 color;
	};

	// 定数バッファ用データ構造体
	struct ConstBufferData
	{
		//Vector4 color;	// 色 (RGBA)
		Matrix4 mat;	// 3D変換行列
		Matrix4 matBillboard;//ビルボード行列
	};

	//パーティクル一粒
	struct OutParticle
	{
		//座標
		Vector3 position = {};
		//方向
		Vector3 velocity = {};
		//加速度
		Vector3 accel = {};
		// 現在フレーム
		uint32_t frame = 0;
		//終了フレーム
		uint32_t numFrame = 0;

		float scale = 1.0f;

		float startScale = 1.0f;

		float endScale = 0.0f;

		Vector4 color = {};

		Vector4 startColor = {};

		Vector4 endColor = {};

	};


public: // 静的メンバ関数
	/// <summary>
	/// 静的初期化
	/// </summary>
	/// <param name="device">デバイス</param>
	/// <param name="window_width">画面幅</param>
	/// <param name="window_height">画面高さ</param>
	static void StaticInitialize(ID3D12Device* device);

	/// <summary>
	/// 描画前処理
	/// </summary>
	/// <param name="cmdList">描画コマンドリスト</param>
	static void PreDraw(ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	/// 描画後処理
	/// </summary>
	static void PostDraw();

	/// <summary>
	/// 終了処理
	/// </summary>
	static void Finalize();

private: // 静的メンバ変数
	// デバイス
	static ID3D12Device* sDevice;
	// コマンドリスト
	static ID3D12GraphicsCommandList* sCmdList;
	// ルートシグネチャ
	static Microsoft::WRL::ComPtr<ID3D12RootSignature> sRootsignature;
	// パイプラインステートオブジェクト
	static Microsoft::WRL::ComPtr<ID3D12PipelineState> sPipelinestate;
	//ビルボード行列
	static Matrix4 sMatBillboard;

private:// 静的メンバ関数

	/// <summary>
	/// グラフィックパイプライン生成
	/// </summary>
	/// <returns>成否</returns>
	static void InitializeGraphicsPipeline();

	/// <summary>
	/// 頂点バッファの初期化
	/// </summary>
	void InitializeVerticeBuff();

public: // メンバ関数
	void Initialize();
	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	//テクスチャの指定
	void SetTextureHandle(uint32_t textureHandle) {textureHandle_=textureHandle ; }

	size_t GetParticlesListSize() {return Particles_.size() ; }

	void Clear() { Particles_.clear() ; }

	/// <summary>
	/// イージングパーティクル（out)
	/// </summary>
	/// <param name="life">寿命(イージングなんでスピードにも関係する)</param>
	/// <param name="startPosition">初期座標</param>
	/// <param name="velocity">速度</param>
	/// <param name="startScale">初期サイズ</param>
	/// <param name="endScale">最終サイズ</param>
	/// <param name="startColor">初期色</param>
	/// <param name="endColor">最終色</param>
	void Add(uint32_t life,const Vector3& startPosition,const Vector3& velocity,const Vector3& accel, float startScale, float endScale,const Vector4& startColor,const Vector4& endColor);

private: // メンバ変数
		// 頂点バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> VertBuff;
	// 頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW VbView;

	Microsoft::WRL::ComPtr<ID3D12Resource> constBuff; // 定数バッファ

	//パーティクル配列
	std::list<OutParticle>Particles_;

	uint32_t textureHandle_ = 0;
};

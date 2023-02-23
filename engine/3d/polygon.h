#pragma once

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>
#include "Model.h"
#include"object3d.h"
#include<list>
class PolygonMana
{
	private: // エイリアス
		// Microsoft::WRL::を省略
		template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	public: // サブクラス\

		struct VertexPos
		{
			Vector3 pos; // xyz座標
			Vector4 color;
		};

		// 定数バッファ用データ構造体B0
		struct ConstBufferDataB0
		{
			//Vector4 color;	// 色 (RGBA)
			Matrix4 mat;	// ３Ｄ変換行列
		};

		//パーティクル一粒
		struct polygon
		{
			//座標
			Vector3 position[3] = {};
			// 速度
			Vector3 velocity = {};
			// 加速度
			Vector3 accel = {};
			// 現在フレーム
			int frame = 0;
			//終了フレーム
			int num_frame = 0;

			Vector4 color = {};

			Vector4 s_color = {};

			Vector4 e_color = {};
		};


	public: // 静的メンバ関数
		/// <summary>
		/// 静的初期化
		/// </summary>
		/// <param name="device">デバイス</param>
		/// <param name="window_width">画面幅</param>
		/// <param name="window_height">画面高さ</param>
		static void StaticInitialize(ID3D12Device* device, int window_width, int window_height);

	private: // 静的メンバ変数
		// デバイス
		static ID3D12Device* device;
		// コマンドリスト
		static ID3D12GraphicsCommandList* cmdList;
		// ルートシグネチャ
		static ComPtr<ID3D12RootSignature> rootsignature;
		// パイプラインステートオブジェクト
		static ComPtr<ID3D12PipelineState> pipelinestate;
		// ビュー行列
		static Matrix4 matViewPro;

	private:// 静的メンバ関数

		/// <summary>
		/// グラフィックパイプライン生成
		/// </summary>
		/// <returns>成否</returns>
		static void InitializeGraphicsPipeline();

		/// <summary>
		/// ビュー行列を更新
		/// </summary>
		static void UpdateViewMatrix();

	public: // メンバ関数
		bool Initialize();
		/// <summary>
		/// 毎フレーム処理
		/// </summary>
		void Update();

		/// <summary>
		/// 描画
		/// </summary>
		void Draw();

		///<summary>
	///パーティクルの追加
	///</summary>
	///<param name="life">生存時間</param>
	///<param name="position">初期座標</param>
	/// <param name="velocity">	速度</param>
	/// <param name="accel">加速度</param>
		void Add(int life, Vector3 position, Vector3 position2, Vector3 position3, Vector3 velocity, Vector3 accel, Vector4 startColor, Vector4 endColor,Matrix4 matworld);

		void Addobj(Object3d* obj,Vector4 startColor,Vector4 endColor);

	private: // メンバ変数
		ComPtr<ID3D12Resource> constBuffB0; // 定数バッファ
		
		Matrix4 mat;

		//パーティクル配列
		std::list<polygon>Polygons;

		Matrix4 matWorld;

		// 頂点バッファ
		ComPtr<ID3D12Resource> vertBuff;
		// 頂点バッファビュー
		D3D12_VERTEX_BUFFER_VIEW vbView;
};


#pragma once

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>
#include "Model.h"
#include"LightGroup.h"


/// <summary>
/// 3Dオブジェクト
/// </summary>
class Object3d
{
private: // エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public: // サブクラス
	// 頂点データ構造体
	struct VertexPosNormalUv
	{
		Vector3 pos; // xyz座標
		Vector3 normal; // 法線ベクトル
		Vector2 uv;  // uv座標
	};

	// 定数バッファ用データ構造体B0
	struct ConstBufferDataB0
	{
		//XMFLOAT4 color;	// 色 (RGBA)
		Matrix4 viewproj;	// ３Ｄ変換行列
		Matrix4 world;
		Vector3 cameraPos;
		float shininess;//光沢度
	};
	// 定数バッファ用データ構造体
	struct ConstBufferPolygonExplosion
	{
		float _Destruction = 0.0f;
		float _ScaleFactor = 1.0f;
		float _RotationFactor = 0.0f;
		float _PositionFactor = 0.0f;
	};

public: // 静的メンバ関数
	/// <summary>
	/// 静的初期化
	/// </summary>
	/// <param name="device">デバイス</param>
	/// <param name="window_width">画面幅</param>
	/// <param name="window_height">画面高さ</param>
	static void StaticInitialize(ID3D12Device* device, uint32_t window_width, uint32_t window_height);

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
	/// 視点座標の取得
	/// </summary>
	/// <returns>座標</returns>
	static const Vector3& GetEye() { return sEye; }

	/// <summary>
	/// 視点座標の設定
	/// </summary>
	/// <param name="position">座標</param>
	static void SetEye(const Vector3& eye);

	/// <summary>
	/// 注視点座標の取得
	/// </summary>
	/// <returns>座標</returns>
	static const Vector3& GetTarget() { return sTarget; }

	/// <summary>
	/// 注視点座標の設定
	/// </summary>
	/// <param name="position">座標</param>
	static void SetTarget(const Vector3& target);

	static Matrix4 GetMatViewPro();

	/// <summary>
	/// カメラ初期化
	/// </summary>
	/// <param name="window_width">画面横幅</param>
	/// <param name="window_height">画面縦幅</param>
	static void InitializeCamera(uint32_t window_width, uint32_t window_height);

	/// <summary>
	/// グラフィックパイプライン生成
	/// </summary>
	/// <returns>成否</returns>
	static void InitializeGraphicsPipeline();

	/// <summary>
	/// ビュー行列を更新
	/// </summary>
	static void UpdateViewMatrix();

	static void SetLight(LightGroup* light) {Object3d::sLightGroup =light ; }


private: // 静的メンバ変数
	// デバイス
	static ID3D12Device* sDevice;
	// コマンドリスト
	static ID3D12GraphicsCommandList* sCmdList;
	// ルートシグネチャ
	static ComPtr<ID3D12RootSignature> sRootsignature;
	// パイプラインステートオブジェクト
	static ComPtr<ID3D12PipelineState> sPipelinestate;
	// ビュー行列
	static Matrix4 sMatView;
	// 射影行列
	static Matrix4 sMatProjection;
	// 視点座標
	static Vector3 sEye;
	// 注視点座標
	static Vector3 sTarget;
	// 上方向ベクトル
	static Vector3 sUp;
	//ライト
	static LightGroup* sLightGroup;

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

	/// <summary>
/// 画像指定描画
/// </summary>
	void Draw(uint32_t texHandle);

	/// <summary>
	/// 座標の取得
	/// </summary>
	/// <returns>座標</returns>
	const Vector3& GetPosition() const { return position_; }

	/// <summary>
	/// 座標の設定
	/// </summary>
	/// <param name="position">座標</param>
	void SetPosition(const Vector3& position) { this->position_ = position; }

	/// <summary>
	/// 座標の取得
	/// </summary>
	/// <returns>座標</returns>
	const Vector3& GetScale() const { return scale_; }

	/// <summary>
	/// 座標の設定
	/// </summary>
	/// <param name="position">座標</param>
	void SetScale(const Vector3& scale) { this->scale_ = scale; }

	/// <summary>
	/// 座標の取得
	/// </summary>
	/// <returns>座標</returns>
	const Vector3& GetRot() const { return rotation_; }

	/// <summary>
	/// 座標の設定
	/// </summary>
	/// <param name="position">座標</param>
	void SetRot(const Vector3& rot) { this->rotation_ = rot; }

	//setter
	void SetModel(Model* model) { this->model_ = model; }

	const ConstBufferPolygonExplosion GetPolygonExplosion() { return *ConstMapPolygon_; }

	void SetPolygonExplosion(const ConstBufferPolygonExplosion& polygonExplosion) { *ConstMapPolygon_ = polygonExplosion; }

	void SetShininess(float shininess) {shininess_= shininess; }

private: // メンバ変数
	ComPtr<ID3D12Resource> constBuffB0_; // 定数バッファ
	ComPtr<ID3D12Resource> constBuffB1_; // 定数バッファ
	ComPtr<ID3D12Resource> constBuffB2_; // 定数バッファ
	//// 色
	//Vector4 color = { 1,1,1,1 };
	// ローカルスケール
	Vector3 scale_ = { 1,1,1 };
	// X,Y,Z軸回りのローカル回転角
	Vector3 rotation_ = { 0,0,0 };
	// ローカル座標
	Vector3 position_ = { 0,0,0 };
	// ローカルワールド変換行列
	Matrix4 matWorld_ = Matrix4Math::identity();
	// 親オブジェクト
	Object3d* parent_ = nullptr;

	ConstBufferPolygonExplosion* ConstMapPolygon_ ;

	//モデル
	Model* model_ = nullptr;

	//光沢度
	float shininess_=4;
};

float ToRadian(float angle);


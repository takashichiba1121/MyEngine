/*
* ライトクラス
*/
#pragma once
#include"DirectionalLight.h"
#include"PointLight.h"
class LightGroup
{
private: // エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
public://定数
	static const uint32_t cDirLightNum = 3;

	static const uint32_t cPointLightNum = 3;

public://サブクラス
	//定数バッファ用データ構造体
	struct ConstBufferData
	{
		//環境光の色
		Vector3 ambientColor;
		float pad1;
		//平行光源用
		DirectionalLight::ConstBufferData dirLights[cDirLightNum];

		PointLight::ConstBufferData pointLight[ cPointLightNum ];
	};

private:
	static ID3D12Device* sDevice;
public:
	static void StaticInitialize(ID3D12Device* device);
private:
	ComPtr<ID3D12Resource> constBuff_;

	Vector3 ambientColor_ = { 1,1,1 };

	DirectionalLight dirLights_[cDirLightNum];

	PointLight pointLight_[cDirLightNum];

	bool dirty_ = false;
public:
	/// <summary>
	/// インスタンス生成
	/// </summary>
	/// <returns></returns>
	static LightGroup* Create();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(ID3D12GraphicsCommandList* cmdList, UINT rootParameterIndex);

	/// <summary>
	/// 定数バッファにデータを送る
	/// </summary>
	void TransferConstBuffer();

	/// <summary>
	/// 環境光のライト色をセット
	/// </summary>
	/// <param name="color">ライト色</param>
	void SetAmbientColor(const Vector3& color);

	/// <summary>
	/// 平行光源の有効フラグをセット
	/// </summary>
	/// <param name="index">ライト番号</param>
	/// <param name="active">有効フラグ</param>
	void SetDirLightActive(uint32_t index, bool active);

	/// <summary>
	/// 平行光源のライトの方向設定
	/// </summary>
	/// <param name="index">ライト番号</param>
	/// <param name="lightdir">ライト方向</param>
	void SetDirLightDir(uint32_t index, const Vector3& lightdir);

	/// <summary>
	/// 平行光源のライト色をセット
	/// </summary>
	/// <param name="index">ライト番号</param>
	/// <param name="lightcolor">ライト色</param>
	void SetDirLightColor(uint32_t index, const Vector3& lightcolor);

	/// <summary>
/// 点光源の有効フラグをセット
/// </summary>
/// <param name="index">ライト番号</param>
/// <param name="active">有効フラグ</param>
	void SetPointActive(uint32_t index, bool active);

		/// <summary>
/// 点光源の有効フラグをセット
/// </summary>
/// <param name="index">ライト番号</param>
/// <param name="active">有効フラグ</param>
	bool GetPointActive(uint32_t index);

	/// <summary>
	/// 点光源のライトの方向設定
	/// </summary>
	/// <param name="index">ライト番号</param>
	/// <param name="lightdir">ライト方向</param>
	void SetPointPos(uint32_t index, const Vector3& lightPos);

	/// <summary>
	/// 点光源のライト色をセット
	/// </summary>
	/// <param name="index">ライト番号</param>
	/// <param name="lightcolor">ライト色</param>
	void SetPointColor(uint32_t index, const Vector3& lightcolor);

	/// <summary>
/// 点光源のライト色をセット
/// </summary>
/// <param name="index">ライト番号</param>
/// <param name="lightcolor">ライト色</param>
	void SetPointAtten(uint32_t index, const Vector3& lightAtten);

	/// <summary>
	/// 標準のライト設定
	/// </summary>
	void DefaultLightSetting();
};
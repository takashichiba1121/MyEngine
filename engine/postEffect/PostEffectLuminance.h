#pragma once
#include "Sprite.h"
#include"PostEffectCommon.h"
#include"RenderTarget.h"
/*
* 輝度抽出のポストエフェクト
*/
class PostEffectLuminance
{
#pragma region Singleton
private:
	PostEffectLuminance() {};

	~PostEffectLuminance() {};
public:
	PostEffectLuminance(const PostEffectLuminance& carManager) = delete;

	PostEffectLuminance& operator=(const PostEffectLuminance& carManager) = delete;

	static PostEffectLuminance* Instance();
#pragma endregion
public:

		// 定数バッファ用データ構造体B0
	struct ConstBufferDataB0
	{
		Vector3 color;
		float pad1;
		float smoothstepMax;
		float smoothstepMin;
	};
	//初期化
    void Initialize();

    /// <summary>
    /// パイプライン生成
    /// </summary>
    void CreatGraphicsPipelineState();

	//更新
	void Update(Vector3 LuminanceColor,float smoothstepMax,float smoothstepMin);

	//描画
    void Draw(ID3D12GraphicsCommandList* cmdList,uint32_t textureHandle);
	//終了
	void Fin();

private://静的メンバ変数
    static const float clearColor[4];

private:

    ID3D12GraphicsCommandList* commandList = nullptr;

//ポストエフェクト

    Vertex vertices[4] = {};

    Vertex* vertMap = nullptr;

    Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff = nullptr;	//頂点バッファ

    //頂点バッファビューの作成
    D3D12_VERTEX_BUFFER_VIEW vbView{};

	Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState;
	Microsoft::WRL::ComPtr<ID3D12PipelineState> normalPipelineState;
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature;

	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffB0_; // 定数バッファ
};


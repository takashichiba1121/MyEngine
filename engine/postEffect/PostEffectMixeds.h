#pragma once
#include "Sprite.h"
#include"PostEffectCommon.h"
#include"RenderTarget.h"
class PostEffectMixeds
{
#pragma region Singleton
private:
	PostEffectMixeds() {};

	~PostEffectMixeds() {};
public:
	PostEffectMixeds(const PostEffectMixeds& carManager) = delete;

	PostEffectMixeds& operator=(const PostEffectMixeds& carManager) = delete;

	static PostEffectMixeds* Instance();
#pragma endregion
public:

    void Initialize();

    /// <summary>
    /// パイプライン生成
    /// </summary>
    void CreatGraphicsPipelineState(); 

    void Draw(ID3D12GraphicsCommandList* cmdList,uint32_t textureHandle1,uint32_t textureHandle2);

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
};


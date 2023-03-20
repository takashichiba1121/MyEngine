#pragma once
#include "Sprite.h"
class PostEffect
{
public:

    void Initialize(DirectXCommon* dxCommon);
    
    /// <summary>
    /// パイプライン生成
    /// </summary>
    void CreatGraphicsPipelineState();

    /// <summary>
    /// シーン描画前処理
    /// </summary>
    /// <param name="cmdList">コマンドリスト</param>
    void PreDrawScene(ID3D12GraphicsCommandList* cmdList);

    void Draw();

    /// <summary>
    /// シーン描画後処理
    /// </summary>
    /// <param name="cmdList">コマンド処理</param>
    void PostDrawScene();

private://静的メンバ変数
    static const float clearColor[4];

private://メンバ変数

    ID3D12Device* device=nullptr;

    ID3D12GraphicsCommandList* commandList=nullptr;

    Vertex vertices[4] = {};

    Vertex* vertMap = nullptr;

    Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff = nullptr;	//頂点バッファ

    //頂点バッファビューの作成
    D3D12_VERTEX_BUFFER_VIEW vbView{};
    Microsoft::WRL::ComPtr<ID3D12Resource> texBuff;

    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descHeapSRV;
    //深度バッファ
    Microsoft::WRL::ComPtr<ID3D12Resource> depthBuff;
    //RTV用のデスクリプタヒープ
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descHeapRTV;
    //DSV用のデスクリプタヒープ
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descHeapDSV;

    Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState;
    Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature;

    Microsoft::WRL::ComPtr<ID3D12Resource> constBuffTransform = nullptr;

    Microsoft::WRL::ComPtr<ID3D12Resource> constBuffMaterial = nullptr;

    ConstBufferDataMaterial* constMapMaterial = nullptr;

    ConstBufferDateTransform* constMapTransform = nullptr;
};


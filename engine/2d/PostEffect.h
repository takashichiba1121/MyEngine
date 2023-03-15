#pragma once
#include "Sprite.h"
class PostEffect :
    public Sprite
{
public:
    /// <summary>
    /// コンストラクタ
    /// </summary>
    PostEffect();

    void Initialize();

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
    void PostDrawScene(ID3D12GraphicsCommandList* cmdList);

private://静的メンバ変数
    static const float clearColor[4];

private://メンバ変数
    Microsoft::WRL::ComPtr<ID3D12Resource> texBuff;

    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descHeapSRV;
    //深度バッファ
    Microsoft::WRL::ComPtr<ID3D12Resource> depthBuff;
    //RTV用のデスクリプタヒープ
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descHeapRTV;
    //DSV用のデスクリプタヒープ
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descHeapDSV;

};


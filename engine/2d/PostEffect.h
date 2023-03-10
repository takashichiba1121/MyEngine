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

    void Draw();

private:
    Microsoft::WRL::ComPtr<ID3D12Resource> texBuff;

    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descHeapSRV;

};


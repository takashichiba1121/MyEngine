#pragma once
#include "Sprite.h"
class PostEffectCommon
{
public:

    void StaticInitialize(DirectXCommon* dxCommon);

    uint32_t CreateDescHeapSRV(ID3D12Resource* texBuff); 

    ID3D12Device* device;

    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descHeapSRV;

    uint32_t texIndex;
};


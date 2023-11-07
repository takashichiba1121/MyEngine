#pragma once
#include "Sprite.h"
class PostEffectCommon
{
#pragma region Singleton
private:
	PostEffectCommon() {};

	~PostEffectCommon() {};
public:
	PostEffectCommon(const PostEffectCommon& carManager) = delete;

	PostEffectCommon& operator=(const PostEffectCommon& carManager) = delete;

	static PostEffectCommon* Instance();
#pragma endregion

public:

    void Initialize(DirectXCommon* dxCommon);

    uint32_t CreateDescHeapSRV(ID3D12Resource* texBuff);

	void Fin();

    ID3D12Device* device;

    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descHeapSRV;

    uint32_t texIndex;
};


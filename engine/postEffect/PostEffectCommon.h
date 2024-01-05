#pragma once
#include "Sprite.h"
/*
* ポストエフェクトの管理
*/
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
	//初期化
    void Initialize(DirectXCommon* dxCommon);

	//SRVの作成
    uint32_t CreateDescHeapSRV(ID3D12Resource* texBuff);

	void Fin();

    ID3D12Device* device;

    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descHeapSRV;

    uint32_t texIndex;
};


#include "PostEffectCommon.h"
#include<d3dx12.h>
#include"WinApp.h"
#include"Texture.h"
#include"SpriteCommon.h"

using namespace DirectX;

void PostEffectCommon::StaticInitialize(DirectXCommon* dxCommon)
{
	HRESULT result;

	device = dxCommon->GetDevice();

	//SRV�p�̃f�X�N���v�^�q�[�v�ݒ�
	D3D12_DESCRIPTOR_HEAP_DESC srvDescHeapDesc = {};
	srvDescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvDescHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	srvDescHeapDesc.NumDescriptors = 7;
	//SRV�f�X�N���v�^�q�[�v�ݒ�
	result = device->CreateDescriptorHeap(&srvDescHeapDesc, IID_PPV_ARGS(&descHeapSRV));
	assert(SUCCEEDED(result));
}

uint32_t PostEffectCommon::CreateDescHeapSRV(ID3D12Resource* texBuff)
{
	uint32_t textureHandle = texIndex;
	//SRV�ݒ�
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;

	//SRV�q�[�v�̐擪�n���h�����擾�iSRV���w���Ă���͂��j
	D3D12_CPU_DESCRIPTOR_HANDLE srvGpuHandle = descHeapSRV->GetCPUDescriptorHandleForHeapStart();
	UINT incrementSize =device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	srvGpuHandle.ptr += incrementSize * texIndex;

	//�f�X�N���v�^�q�[�v��SRV���쐬
	device->CreateShaderResourceView(texBuff,
		&srvDesc,
		srvGpuHandle
	);
	texIndex++;
	return textureHandle;
}

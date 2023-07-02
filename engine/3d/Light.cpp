#include"Light.h"
#include<assert.h>
#include<d3d12.h>

ID3D12Device* Light::sDevice = nullptr;

void Light::StaticInitialize(ID3D12Device* device)
{
	assert(!sDevice);

	assert(device);

	sDevice = device;
}

void Light::Initialize()
{
	HRESULT result;
	// �q�[�v�v���p�e�B
	CD3DX12_HEAP_PROPERTIES heapProps = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	// ���\�[�X�ݒ�
	CD3DX12_RESOURCE_DESC resourceDesc =
		CD3DX12_RESOURCE_DESC::Buffer((sizeof(constBuff_) + 0xff) & ~0xff);

	// �萔�o�b�t�@�̐���
	result = sDevice->CreateCommittedResource(
		&heapProps, // �A�b�v���[�h�\
		D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
		IID_PPV_ARGS(&constBuff_));
	assert(SUCCEEDED(result));

	TransferConstBuffer();
}

void Light::TransferConstBuffer()
{
	HRESULT result;

	// �萔�o�b�t�@�փf�[�^�]��
	ConstBufferData* constMap = nullptr;
	result = constBuff_->Map(0, nullptr, (void**)&constMap);
	/*constMap->color = color;*/
	constMap->lightv = -lightDir_;
	constMap->shininess = shininess_;
	constMap->lightcolor = lightColor_;
	constBuff_->Unmap(0, nullptr);
}

void Light::SetLightDir(const Vector3& lightDir)
{
	lightDir_ = lightDir;
	lightDir_.normalize();
	dirty_ = true;
}

void Light::SetLightColor(const Vector3& lightColor)
{
	lightColor_ = lightColor;
	dirty_ = true;
}

void Light::SetShininess(const float shininess)
{
	shininess_= shininess;
	dirty_ = true;
}

void Light::Update()
{
	if (dirty_)
	{
		TransferConstBuffer();
		dirty_ = false;
	}
}

void Light::Draw(ID3D12GraphicsCommandList* cmdList, uint32_t rootParameterIndex)
{
	cmdList->SetGraphicsRootConstantBufferView(rootParameterIndex,
		constBuff_->GetGPUVirtualAddress());
}

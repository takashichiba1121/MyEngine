#include"LightGroup.h"

ID3D12Device* LightGroup::sDevice = nullptr;

void LightGroup::StaticInitialize(ID3D12Device* device)
{
	//�ď������`�F�b�N
	assert(!LightGroup::sDevice);

	//nullptr�`�F�b�N
	assert(device);

	LightGroup::sDevice = device;
}

LightGroup* LightGroup::Create()
{
	//���C�g�̃I�u�W�F�N�g�̃C���X�^���X�𐶐�
	LightGroup* instance = new LightGroup();

	//������
	instance->Initialize();

	return instance;
}

void LightGroup::Initialize()
{
	//���C�g�̕W���ݒ�
	DefaultLightSetting();

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

	//�萔�o�b�t�@�փf�[�^�]��
	TransferConstBuffer();
}

void LightGroup::Update()
{
	//�l�̍X�V���������������萔�o�b�t�@�ɓ]������
	if (dirty_)
	{
		TransferConstBuffer();
		dirty_ = false;
	}
}

void LightGroup::Draw(ID3D12GraphicsCommandList* cmdList, UINT rootParameterIndex)
{
	//�萔�o�b�t�@�r���[���Z�b�g
	cmdList->SetGraphicsRootConstantBufferView(rootParameterIndex,
		constBuff_->GetGPUVirtualAddress());
}

void  LightGroup::TransferConstBuffer()
{
	HRESULT result;

	// �萔�o�b�t�@�փf�[�^�]��
	ConstBufferData* constMap = nullptr;
	result = constBuff_->Map(0, nullptr, (void**)&constMap);
	for (uint32_t i=0;i<cDirLightNum;i++)
	{
		if (dirLights[i].IsActive())
		{
			constMap->dirLights[i].active = 1;
			constMap->dirLights[i].lightv = -dirLights[i].GetLightDir();
			constMap->dirLights[i].lightcolor = dirLights[i].GetLightColor();
			constMap->dirLights[i].shininess = dirLights[i].GetShininess();
		}
		else
		{
			constMap->dirLights[i].active = 0;
		}
	}
	constBuff_->Unmap(0, nullptr);
}

void LightGroup::SetAmbientColor(const Vector3& color)
{
	ambientColor_ = color;
	dirty_ = true;
}

void LightGroup::SetDirLightActive(uint32_t index, bool active)
{
	assert(0 <= index && index < cDirLightNum);
	dirLights[index].SetActive(active);
}

void LightGroup::SetDirLightColor(uint32_t index, const Vector3& lightcolor)
{
	assert(0 <= index && index < cDirLightNum);
	dirLights[index].SetLightColor(lightcolor);
	dirty_ = true;
}

void LightGroup::SetDirLightShininess(uint32_t index, float lightShininess)
{
	assert(0 <= index && index < cDirLightNum);
	dirLights[index].SetShininess(lightShininess);
	dirty_ = true;
}

void LightGroup::DefaultLightSetting()
{
	dirLights[0].SetActive(true);
	dirLights[0].SetLightColor({1.0f,1.0f,1.0f});
	dirLights[0].SetLightDir({0.0f,1.0f,0.0f});
	dirLights[0].SetShininess(10);

	dirLights[1].SetActive(true);
	dirLights[1].SetLightColor({ 1.0f,1.0f,1.0f });
	dirLights[1].SetLightDir({ 0.0f,1.0f,0.0f });
	dirLights[1].SetShininess(10);

	dirLights[2].SetActive(true);
	dirLights[2].SetLightColor({ 1.0f,1.0f,1.0f });
	dirLights[2].SetLightDir({ 0.0f,1.0f,0.0f });
	dirLights[2].SetShininess(10);
}

void LightGroup::SetDirLightDir(uint32_t index, const Vector3& lightdir)
{
	assert(0 <= index && index < cDirLightNum);
	dirLights[index].SetLightDir(lightdir);
	dirty_ = true;
}

#include "Sprite.h"
#include<DirectXTex.h>
#include"Texture.h"


using namespace DirectX;
using namespace std;

DirectXCommon* Sprite::sDxCommon;

void Sprite::StaticInitialize()
{

	sDxCommon = SpriteCommon::GetDxCommon();
}

void Sprite::Initialize(uint32_t textureIndex)
{

	HRESULT result;

	Vertex vertices_[4] = {
		{{  0.0f,100.0f,0.0f},{0.0f,1.0f}},
		{{  0.0f,  0.0f,0.0f},{0.0f,0.0f}},
		{{100.0f,100.0f,0.0f},{1.0f,1.0f}},
		{{100.0f,  0.0f,0.0f},{1.0f,0.0f}},
	};
	for (int i = 0; i < 4; i++)
	{
		vertices_[i] = vertices_[i];
	}

	//���_�f�[�^�S�̂̃T�C�Y=���_�f�[�^����̃T�C�Y*���_�f�[�^�̗v�f��
	UINT sizeVB = static_cast<UINT>(sizeof(vertices_[0]) * _countof(vertices_));
	//���_�o�b�t�@�̐ݒ�
	D3D12_HEAP_PROPERTIES heapProp{};  //�q�[�v�ݒ�
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD; //GPU�ւ̓]���p
	//���\�[�X�ݒ�
	D3D12_RESOURCE_DESC resDesc{};
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeVB; //���_�f�[�^�S�̂̃T�C�Y
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	//���_�o�b�t�@�̐���
	result = sDxCommon->GetDevice()->CreateCommittedResource(
		&heapProp, //�q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&resDesc, //���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff_));
	assert(SUCCEEDED(result));

	//GPU��̃o�b�t�@�ɑΉ��������z�������i���C����������j���擾
	result = vertBuff_->Map(0, nullptr, (void**)&vertMap_);
	assert(SUCCEEDED(result));
	//�O���_�ɑ΂���
	for (int i = 0; i < _countof(vertices_); i++) {
		vertMap_[i] = vertices_[i]; //���W�R�s�[
	}

	//GPU���z�A�h���X
	vbView_.BufferLocation = vertBuff_->GetGPUVirtualAddress();
	//���_�o�b�t�@�̃T�C�Y
	vbView_.SizeInBytes = sizeVB;
	//���_����̃f�[�^�T�C�Y
	vbView_.StrideInBytes = sizeof(vertices_[0]);
	{
		//�q�[�v�ݒ�
		D3D12_HEAP_PROPERTIES cbHeapProp{};
		cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;//GPU�ւ̓]���p
		//���\�[�X�ݒ�
		D3D12_RESOURCE_DESC cbResourceDesc{};
		cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		cbResourceDesc.Width = (sizeof(ConstBufferDataMaterial) + 0xff) & ~0xff;//256�o�C�g�A���C�������g
		cbResourceDesc.Height = 1;
		cbResourceDesc.DepthOrArraySize = 1;
		cbResourceDesc.MipLevels = 1;
		cbResourceDesc.SampleDesc.Count = 1;
		cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
		//�萔�o�b�t�@�̐���
		result = sDxCommon->GetDevice()->CreateCommittedResource(
			&cbHeapProp,//�q�[�v�ݒ�
			D3D12_HEAP_FLAG_NONE,
			&cbResourceDesc,//���\�[�X�ݒ�
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&constBuffMaterial_));
		assert(SUCCEEDED(result));
	}
	//�萔�o�b�t�@�̃}�b�s���O
	result = constBuffMaterial_->Map(0, nullptr, (void**)&constMapMaterial_);//�}�b�s���O
	//�l���������ނƎ����I�ɓ]�������
	constMapMaterial_->color = { 1.0f, 1.0f, 1.0f, 1.0f };
	assert(SUCCEEDED(result));

	{
		//�q�[�v�ݒ�
		D3D12_HEAP_PROPERTIES cbHeapProp{};
		cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;//GPU�ւ̓]���p
		//���\�[�X�ݒ�
		D3D12_RESOURCE_DESC cbResourceDesc{};
		cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		cbResourceDesc.Width = (sizeof(ConstBufferDateTransform) + 0xff) & ~0xff;//256�o�C�g�A���C�������g
		cbResourceDesc.Height = 1;
		cbResourceDesc.DepthOrArraySize = 1;
		cbResourceDesc.MipLevels = 1;
		cbResourceDesc.SampleDesc.Count = 1;
		cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
		//�萔�o�b�t�@�̐���
		result = sDxCommon->GetDevice()->CreateCommittedResource(
			&cbHeapProp,//�q�[�v�ݒ�
			D3D12_HEAP_FLAG_NONE,
			&cbResourceDesc,//���\�[�X�ݒ�
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&constBuffTransform_));
		assert(SUCCEEDED(result));
	}
	//�萔�o�b�t�@�̃}�b�s���O
	result = constBuffTransform_->Map(0, nullptr, (void**)&constMapTransform_);//�}�b�s���O
	//�l���������ނƎ����I�ɓ]�������
	constMapTransform_->mat = Matrix4Math::identity();

	constMapTransform_->mat = SpriteCommon::GetMatProjection();

	assert(SUCCEEDED(result));

	if (textureIndex != UINT_MAX)
	{
		this->textureIndex_ = textureIndex;
		AdjustTextureSize();
		scale_ = textureSize_;
	}


}

void Sprite::AdjustTextureSize()
{

	ID3D12Resource* textureBuffer = Texture::texBuffuers[textureIndex_].Get();
	//�w��ԍ��̉摜���ǂݍ��ݍς݂Ȃ�
	assert(textureBuffer);

	//�e�N�X�`�����擾
	D3D12_RESOURCE_DESC resDesc = textureBuffer->GetDesc();

	textureSize_.x = static_cast<float>(resDesc.Width);
	textureSize_.y = static_cast<float>(resDesc.Height);
}

void Sprite::Update()
{
	ID3D12Resource* textureBuffer = Texture::texBuffuers[textureIndex_].Get();
	//�w��ԍ��̉摜���ǂݍ��ݍς݂Ȃ�
	if (textureBuffer)
	{
		//�e�N�X�`�����擾
		D3D12_RESOURCE_DESC resDesc = textureBuffer->GetDesc();

		float tex_left = textureLeftTop_.x / resDesc.Width;
		float tex_right = (textureLeftTop_.x + textureSize_.x) / resDesc.Width;
		float tex_top = textureLeftTop_.y / resDesc.Height;
		float tex_bottom = (textureLeftTop_.y + textureSize_.y) / resDesc.Height;
		//���_��UV�ɔ��f����
		vertices_[LB].uv = { tex_left,tex_bottom };//����
		vertices_[LT].uv = { tex_left,tex_top };//����
		vertices_[RB].uv = { tex_right,tex_bottom };//����
		vertices_[RT].uv = { tex_right,tex_top };//����
	}


	float left = (0.0f - anchorPoint_.x) * scale_.x;
	float right = (1.0f - anchorPoint_.x) * scale_.x;
	float top = (0.0f - anchorPoint_.y) * scale_.y;
	float bottom = (1.0f - anchorPoint_.y) * scale_.y;

	//���E���]
	if (isFlipX_) {
		left = -left;
		right = -right;
	}

	//�㉺���]
	if (isFlipY_) {
		top = -top;
		bottom = -bottom;
	}

	vertices_[LB].pos = { left,bottom,0.0f };
	vertices_[LT].pos = { left,top,0.0f };
	vertices_[RB].pos = { right,bottom,0.0f };
	vertices_[RT].pos = { right,top ,0.0f };

	//�O���_�ɑ΂���
	for (int i = 0; i < _countof(vertices_); i++) {
		vertMap_[i] = vertices_[i]; //���W�R�s�[
	}

	//���[���h�ϊ��s��
	Matrix4 matWorld = Matrix4Math::identity();

	Matrix4 matRot;//��]�s��
	matRot = Matrix4Math::rotateZ(XMConvertToRadians(rotation_));//Z������0�x��]

	Matrix4 matTrans; //���s�ړ��s��
	matTrans = Matrix4Math::translate({ position_.x, position_.y, 0.0f });//(-50,0,0)���s�ړ�

	matWorld = matRot* matTrans;//���[���h�s��ɉ�]�𔽉f
	//�s��̌v�Z
	constMapTransform_->mat = matWorld * SpriteCommon::GetMatProjection();
}

void Sprite::Draw()
{
	if (isInvisible_)
	{
		return;
	}
	//�v���~�e�B�u�`��̐ݒ�R�}���h
	sDxCommon->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	//���_�o�b�t�@�r���[�̐ݒ�R�}���h
	sDxCommon->GetCommandList()->IASetVertexBuffers(0, 1, &vbView_);
	//�萔�o�b�t�@�r���[�iCBV�j�̐ݒ�R�}���h
	sDxCommon->GetCommandList()->SetGraphicsRootConstantBufferView(0, constBuffMaterial_->GetGPUVirtualAddress());
	//�萔�o�b�t�@�r���[�iCBV�j�̐ݒ�R�}���h
	sDxCommon->GetCommandList()->SetGraphicsRootConstantBufferView(2, constBuffTransform_->GetGPUVirtualAddress());

	//�摜�`��
	//SRV�q�[�v�̐擪�n���h�����擾�iSRV���w���Ă���͂��j
	D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = Texture::sDescHeap->GetGPUDescriptorHandleForHeapStart();
	UINT incrementSize = sDxCommon->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	srvGpuHandle.ptr += incrementSize * textureIndex_;
	sDxCommon->GetCommandList()->SetGraphicsRootDescriptorTable(1, srvGpuHandle);

	//�`��R�}���h
	sDxCommon->GetCommandList()->DrawInstanced(_countof(vertices_), 1, 0, 0);//���ׂĂ̒��_���g���ĕ`��
}

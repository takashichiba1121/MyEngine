#include "assimpObject3d.h"
#include <d3dcompiler.h>
#include <DirectXTex.h>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include"object3d.h"
using namespace std;


#pragma comment(lib, "d3dcompiler.lib")

using namespace DirectX;
using namespace Microsoft::WRL;

//assimpObject3d::Material assimpObject3d::material;

/// <summary>
/// �ÓI�����o�ϐ��̎���
/// </summary>
ID3D12Device* assimpObject3d::sDevice = nullptr;
//UINT assimpObject3d::descriptorHandleIncrementSize = 0;
ID3D12GraphicsCommandList* assimpObject3d::sCmdList = nullptr;
ComPtr<ID3D12RootSignature> assimpObject3d::sRootsignature;
ComPtr<ID3D12PipelineState> assimpObject3d::sPipelinestate;
//ComPtr<ID3D12DescriptorHeap> assimpObject3d::descHeap;
//ComPtr<ID3D12Resource> assimpObject3d::texbuff;
//CD3DX12_CPU_DESCRIPTOR_HANDLE assimpObject3d::cpuDescHandleSRV;
//CD3DX12_GPU_DESCRIPTOR_HANDLE assimpObject3d::gpuDescHandleSRV;
Matrix4 assimpObject3d::sMatView{};
Matrix4 assimpObject3d::sMatProjection{};
Vector3 assimpObject3d::sEye = { 0, 0, 0.0f };
Vector3 assimpObject3d::sTarget = { 0, 0, 0 };
Vector3 assimpObject3d::sUp = { 0, 1, 0 };
//assimpObject3d::VertexPosNormalUv assimpObject3d::vertices[vertexCount];
//unsigned short assimpObject3d::indices[planeCount * 3];

void assimpObject3d::StaticInitialize(ID3D12Device* device, uint32_t window_width, uint32_t window_height)
{
	// nullptr�`�F�b�N
	assert(device);

	assimpObject3d::sDevice = device;

	Model::SetDevice(device);

	// �J����������
	InitializeCamera(window_width, window_height);

	// �p�C�v���C��������
	InitializeGraphicsPipeline();

}

void assimpObject3d::PreDraw(ID3D12GraphicsCommandList* cmdList)
{
	// PreDraw��PostDraw���y�A�ŌĂ΂�Ă��Ȃ���΃G���[
	assert(assimpObject3d::sCmdList == nullptr);

	// �R�}���h���X�g���Z�b�g
	assimpObject3d::sCmdList = cmdList;

	// �p�C�v���C���X�e�[�g�̐ݒ�
	cmdList->SetPipelineState(sPipelinestate.Get());
	// ���[�g�V�O�l�`���̐ݒ�
	cmdList->SetGraphicsRootSignature(sRootsignature.Get());
	// �v���~�e�B�u�`���ݒ�
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void assimpObject3d::PostDraw()
{
	// �R�}���h���X�g������
	assimpObject3d::sCmdList = nullptr;
}

void assimpObject3d::SetEye(Vector3 eye)
{
	assimpObject3d::sEye = eye;

	UpdateViewMatrix();
}

void assimpObject3d::SetTarget(Vector3 target)
{
	assimpObject3d::sTarget = target;

	UpdateViewMatrix();
}

void assimpObject3d::InitializeCamera(uint32_t window_width, uint32_t window_height)
{
	// �r���[�s��̐���
	sMatView = Matrix4Math::ViewMat(
		sEye,
		sTarget,
		sUp);

	// �������e�ɂ��ˉe�s��̐���
	sMatProjection = Matrix4Math::ProjectionMat(
		ToRadian(45.0f),
		(float)window_width / window_height,
		0.1f, 1000.0f
	);
}

void assimpObject3d::InitializeGraphicsPipeline()
{
	HRESULT result = S_FALSE;
	ComPtr<ID3DBlob> vsBlob; // ���_�V�F�[�_�I�u�W�F�N�g
	ComPtr<ID3DBlob> gsBlob;	// �s�N�Z���V�F�[�_�I�u�W�F�N�g
	ComPtr<ID3DBlob> psBlob;	// �s�N�Z���V�F�[�_�I�u�W�F�N�g
	ComPtr<ID3DBlob> errorBlob; // �G���[�I�u�W�F�N�g

	// ���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		L"Resources/shaders/FbxVS.hlsl",	// �V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ���
		"main", "vs_5_0",	// �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
		0,
		&vsBlob, &errorBlob);
	if (FAILED(result)) {
		// errorBlob����G���[���e��string�^�ɃR�s�[
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		// �G���[���e���o�̓E�B���h�E�ɕ\��
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	// �s�N�Z���V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		L"Resources/shaders/FbxGS.hlsl",	// �V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ���
		"main", "gs_5_0",	// �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
		0,
		&gsBlob, &errorBlob);
	if (FAILED(result)) {
		// errorBlob����G���[���e��string�^�ɃR�s�[
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		// �G���[���e���o�̓E�B���h�E�ɕ\��
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	// �s�N�Z���V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		L"Resources/shaders/FbxPS.hlsl",	// �V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ���
		"main", "ps_5_0",	// �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
		0,
		&psBlob, &errorBlob);
	if (FAILED(result)) {
		// errorBlob����G���[���e��string�^�ɃR�s�[
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		// �G���[���e���o�̓E�B���h�E�ɕ\��
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	// ���_���C�A�E�g
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{ // xy���W(1�s�ŏ������ق������₷��)
			"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{ // �@���x�N�g��(1�s�ŏ������ق������₷��)
			"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{ // uv���W(1�s�ŏ������ق������₷��)
			"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
	};

	// �O���t�B�b�N�X�p�C�v���C���̗����ݒ�
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{};
	gpipeline.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get());
	gpipeline.GS = CD3DX12_SHADER_BYTECODE(gsBlob.Get());
	gpipeline.PS = CD3DX12_SHADER_BYTECODE(psBlob.Get());

	// �T���v���}�X�N
	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // �W���ݒ�
	// ���X�^���C�U�X�e�[�g
	//gpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	gpipeline.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;//�|���S�����h��Ԃ�
	gpipeline.RasterizerState.DepthClipEnable = true;//�[�x�O���b�s���O��L����
	// �f�v�X�X�e���V���X�e�[�g
	gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);

	// �����_�[�^�[�Q�b�g�̃u�����h�ݒ�
	D3D12_RENDER_TARGET_BLEND_DESC blenddesc{};
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;	// RBGA�S�Ẵ`�����l����`��
	blenddesc.BlendEnable = true;
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;

	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;

	// �u�����h�X�e�[�g�̐ݒ�
	gpipeline.BlendState.RenderTarget[0] = blenddesc;

	// �[�x�o�b�t�@�̃t�H�[�}�b�g
	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;

	// ���_���C�A�E�g�̐ݒ�
	gpipeline.InputLayout.pInputElementDescs = inputLayout;
	gpipeline.InputLayout.NumElements = _countof(inputLayout);

	// �}�`�̌`��ݒ�i�O�p�`�j
	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	gpipeline.NumRenderTargets = 1;	// �`��Ώۂ�1��
	gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0�`255�w���RGBA
	gpipeline.SampleDesc.Count = 1; // 1�s�N�Z���ɂ�1��T���v�����O

	// �f�X�N���v�^�����W
	CD3DX12_DESCRIPTOR_RANGE descRangeSRV;
	descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0); // t0 ���W�X�^

	// ���[�g�p�����[�^
	/*CD3DX12_ROOT_PARAMETER rootparams[2];
	rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);
	rootparams[1].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);*/

	CD3DX12_ROOT_PARAMETER rootparams[4];
	rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);
	rootparams[1].InitAsConstantBufferView(1, 0, D3D12_SHADER_VISIBILITY_ALL);
	rootparams[2].InitAsConstantBufferView(2, 0, D3D12_SHADER_VISIBILITY_ALL);
	rootparams[3].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);

	// �X�^�e�B�b�N�T���v���[
	CD3DX12_STATIC_SAMPLER_DESC samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0);

	// ���[�g�V�O�l�`���̐ݒ�
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
	rootSignatureDesc.Init_1_0(_countof(rootparams), rootparams, 1, &samplerDesc, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	ComPtr<ID3DBlob> rootSigBlob;
	// �o�[�W������������̃V���A���C�Y
	result = D3DX12SerializeVersionedRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);
	// ���[�g�V�O�l�`���̐���
	result = sDevice->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&sRootsignature));
	assert(SUCCEEDED(result));

	gpipeline.pRootSignature = sRootsignature.Get();

	// �O���t�B�b�N�X�p�C�v���C���̐���
	result = sDevice->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&sPipelinestate));
	assert(SUCCEEDED(result));

}
void assimpObject3d::UpdateViewMatrix()
{
	// �r���[�s��̐���
	sMatView = Matrix4Math::ViewMat(
		sEye,
		sTarget,
		sUp);
}

bool assimpObject3d::Initialize()
{
	// nullptr�`�F�b�N
	assert(sDevice);

	HRESULT result;

	// �q�[�v�v���p�e�B
	CD3DX12_HEAP_PROPERTIES heapProps = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	{
		// ���\�[�X�ݒ�
		CD3DX12_RESOURCE_DESC resourceDesc =
			CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferDataB0) + 0xff) & ~0xff);

		// �萔�o�b�t�@�̐���
		result = sDevice->CreateCommittedResource(
			&heapProps, // �A�b�v���[�h�\
			D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
			IID_PPV_ARGS(&constBuffB0_));
		//// �萔�o�b�t�@�̐���
		//result = device->CreateCommittedResource(
		//	&heapPropsB1,
		//	D3D12_HEAP_FLAG_NONE,
		//	&resourceDescB1,
		//	D3D12_RESOURCE_STATE_GENERIC_READ,
		//	nullptr,
		//	IID_PPV_ARGS(&constBuffB1));
		assert(SUCCEEDED(result));
	}

	{
		// ���\�[�X�ݒ�
		CD3DX12_RESOURCE_DESC resourceDesc =
			CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferPolygonExplosion) + 0xff) & ~0xff);

		// �萔�o�b�t�@�̐���
		result = sDevice->CreateCommittedResource(
			&heapProps, // �A�b�v���[�h�\
			D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
			IID_PPV_ARGS(&constBuffB2_));
		//// �萔�o�b�t�@�̐���
		//result = device->CreateCommittedResource(
		//	&heapPropsB1,
		//	D3D12_HEAP_FLAG_NONE,
		//	&resourceDescB1,
		//	D3D12_RESOURCE_STATE_GENERIC_READ,
		//	nullptr,
		//	IID_PPV_ARGS(&constBuffB1));
		assert(SUCCEEDED(result));
	}
	result = constBuffB2_->Map(0, nullptr, (void**)&ConstMapPolygon_);
	return true;
}

void assimpObject3d::Update()
{
	HRESULT result;
	Matrix4 matScale, matRot, matTrans;

	// �X�P�[���A��]�A���s�ړ��s��̌v�Z
	matScale = Matrix4Math::scale(scale_);
	matRot = Matrix4Math::identity();
	matRot = matRot * Matrix4Math::rotateZ(ToRadian(rotation_.z));
	matRot = matRot * Matrix4Math::rotateX(ToRadian(rotation_.x));
	matRot = matRot * Matrix4Math::rotateY(ToRadian(rotation_.y));
	matTrans = Matrix4Math::translate(position_);

	// ���[���h�s��̍���
	matWorld_ = Matrix4Math::identity(); // �ό`�����Z�b�g
	matWorld_ = matWorld_ * matScale; // ���[���h�s��ɃX�P�[�����O�𔽉f
	matWorld_ = matWorld_ * matRot; // ���[���h�s��ɉ�]�𔽉f
	matWorld_ = matWorld_ * matTrans; // ���[���h�s��ɕ��s�ړ��𔽉f

	// �e�I�u�W�F�N�g�������
	if (parent_ != nullptr) {
		// �e�I�u�W�F�N�g�̃��[���h�s����|����
		matWorld_ *= parent_->matWorld_;
	}

	// �萔�o�b�t�@�փf�[�^�]��
	ConstBufferDataB0* constMap = nullptr;
	result = constBuffB0_->Map(0, nullptr, (void**)&constMap);
	/*constMap->color = color;*/
	constMap->mat = matWorld_ * sMatView * sMatProjection;	// �s��̍���
	constBuffB0_->Unmap(0, nullptr);

}

void assimpObject3d::Draw()
{
	// nullptr�`�F�b�N
	assert(sDevice);
	assert(assimpObject3d::sCmdList);

	//���f�����Z�b�g����Ă��Ȃ���Ε`����X�L�b�v
	if (model_ == nullptr) {
		return;
	}

	// �萔�o�b�t�@�r���[���Z�b�g
	sCmdList->SetGraphicsRootConstantBufferView(0, constBuffB0_->GetGPUVirtualAddress());

	// �萔�o�b�t�@�r���[���Z�b�g
	sCmdList->SetGraphicsRootConstantBufferView(2, constBuffB2_->GetGPUVirtualAddress());

	//���f����`��
	model_->Draw(sCmdList, 1);
}

Matrix4 assimpObject3d::GetMatViewPro()
{

	return sMatView * sMatProjection;
}

//void assimpObject3d::LoadMaterial(const std::string& directoryPath, const std::string& filename)
//{
//	//�t�@�C���X�g���[��
//	std::ifstream file;
//	//�}�e���A���t�@�C�����J��
//	file.open(directoryPath + filename);
//	//�t�@�C���I�[�v�����s���`�F�b�N
//	if (file.fail()) {
//		assert(0);
//	}
//
//	//1�s���ǂݍ���
//	string line;
//	while (getline(file, line)) {
//
//		//1�s���̕�������X�g���[���ɕϊ�
//		std::istringstream line_stream(line);
//		//���p�X�y�[�X��؂�ōs�̐擪��������擾
//		string key;
//		getline(line_stream, key, ' ');
//
//		//�擪�̃^�u�����͖�������
//		if (key[0] == '\t') {
//			key.erase(key.begin());	//�擪�̕������폜
//		}
//		//�擪������newmtl�Ȃ�}�e���A����
//		if (key == "newmtl") {
//			//�}�e���A�����ǂݍ���
//			line_stream >> material.name;
//		}
//		//�擪������Ka�Ȃ�A���r�G���g�F
//		if (key == "Ka") {
//			line_stream >> material.ambient.x;
//			line_stream >> material.ambient.y;
//			line_stream >> material.ambient.z;
//		}
//		//�擪������Kd�Ȃ�f�B�t���[�Y�F
//		if (key == "Kd") {
//			line_stream >> material.diffuse.x;
//			line_stream >> material.diffuse.y;
//			line_stream >> material.diffuse.z;
//		}
//		//�擪������Ks�Ȃ�X�y�L�����[�F
//		if (key == "Ks") {
//			line_stream >> material.specular.x;
//			line_stream >> material.specular.y;
//			line_stream >> material.specular.z;
//		}
//		//�擪������map_Kd�Ȃ�e�N�X�`���t�@�C����
//		if (key == "map_Kd") {
//			//�e�N�X�`���̃t�@�C�����ǂݍ���
//			line_stream >> material.textureFilename;
//			//�e�N�X�`���ǂݍ���
//			LoadTexture(directoryPath, material.textureFilename);
//		}
//	}
//	//�t�@�C�������
//	file.close();
//}



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
ID3D12Device* assimpObject3d::device = nullptr;
//UINT assimpObject3d::descriptorHandleIncrementSize = 0;
ID3D12GraphicsCommandList* assimpObject3d::cmdList = nullptr;
ComPtr<ID3D12RootSignature> assimpObject3d::rootsignature;
ComPtr<ID3D12PipelineState> assimpObject3d::pipelinestate;
//ComPtr<ID3D12DescriptorHeap> assimpObject3d::descHeap;
//ComPtr<ID3D12Resource> assimpObject3d::texbuff;
//CD3DX12_CPU_DESCRIPTOR_HANDLE assimpObject3d::cpuDescHandleSRV;
//CD3DX12_GPU_DESCRIPTOR_HANDLE assimpObject3d::gpuDescHandleSRV;
Matrix4 assimpObject3d::matView{};
Matrix4 assimpObject3d::matProjection{};
Vector3 assimpObject3d::eye = { 0, 0, 0.0f };
Vector3 assimpObject3d::target = { 0, 0, 0 };
Vector3 assimpObject3d::up = { 0, 1, 0 };
//assimpObject3d::VertexPosNormalUv assimpObject3d::vertices[vertexCount];
//unsigned short assimpObject3d::indices[planeCount * 3];

void assimpObject3d::StaticInitialize(ID3D12Device* device, int window_width, int window_height)
{
	// nullptr�`�F�b�N
	assert(device);

	assimpObject3d::device = device;

	Model::SetDevice(device);

	// �J����������
	InitializeCamera(window_width, window_height);

	// �p�C�v���C��������
	InitializeGraphicsPipeline();

}

void assimpObject3d::PreDraw(ID3D12GraphicsCommandList* cmdList)
{
	// PreDraw��PostDraw���y�A�ŌĂ΂�Ă��Ȃ���΃G���[
	assert(assimpObject3d::cmdList == nullptr);

	// �R�}���h���X�g���Z�b�g
	assimpObject3d::cmdList = cmdList;

	// �p�C�v���C���X�e�[�g�̐ݒ�
	cmdList->SetPipelineState(pipelinestate.Get());
	// ���[�g�V�O�l�`���̐ݒ�
	cmdList->SetGraphicsRootSignature(rootsignature.Get());
	// �v���~�e�B�u�`���ݒ�
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void assimpObject3d::PostDraw()
{
	// �R�}���h���X�g������
	assimpObject3d::cmdList = nullptr;
}

void assimpObject3d::SetEye(Vector3 eye)
{
	assimpObject3d::eye = eye;

	UpdateViewMatrix();
}

void assimpObject3d::SetTarget(Vector3 target)
{
	assimpObject3d::target = target;

	UpdateViewMatrix();
}

void assimpObject3d::InitializeCamera(int window_width, int window_height)
{
	// �r���[�s��̐���
	matView = Matrix4Math::ViewMat(
		eye,
		target,
		up);

	// �������e�ɂ��ˉe�s��̐���
	matProjection = Matrix4Math::ProjectionMat(
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
	result = device->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&rootsignature));
	assert(SUCCEEDED(result));

	gpipeline.pRootSignature = rootsignature.Get();

	// �O���t�B�b�N�X�p�C�v���C���̐���
	result = device->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&pipelinestate));
	assert(SUCCEEDED(result));

}

//bool assimpObject3d::LoadTexture(const std::string& directoryPath, const std::string& filename)
//{
//	HRESULT result = S_FALSE;
//
//	TexMetadata metadata{};
//	ScratchImage scratchImg{};
//
//	//�t�@�C���p�X������
//	string filepath = directoryPath + filename;
//
//	//���j�R�[�h������ɕϊ�����
//	wchar_t wfilepath[128];
//	int iBufferSize = MultiByteToWideChar(CP_ACP, 0, filepath.c_str(), -1, wfilepath, _countof(wfilepath));
//
//
//	//// WIC�e�N�X�`���̃��[�h
//	//result = LoadFromWICFile(L"Resources/tex1.png", WIC_FLAGS_NONE, &metadata, scratchImg);
//	//assert(SUCCEEDED(result));
//	// WIC�e�N�X�`���̃��[�h
//	result = LoadFromWICFile(wfilepath, WIC_FLAGS_NONE, &metadata, scratchImg);
//	assert(SUCCEEDED(result));
//
//	ScratchImage mipChain{};
//	// �~�b�v�}�b�v����
//	result = GenerateMipMaps(
//		scratchImg.GetImages(), scratchImg.GetImageCount(), scratchImg.GetMetadata(),
//		TEX_FILTER_DEFAULT, 0, mipChain);
//	if (SUCCEEDED(result)) {
//		scratchImg = std::move(mipChain);
//		metadata = scratchImg.GetMetadata();
//	}
//
//	// �ǂݍ��񂾃f�B�t���[�Y�e�N�X�`����SRGB�Ƃ��Ĉ���
//	metadata.format = MakeSRGB(metadata.format);
//
//	// ���\�[�X�ݒ�
//	CD3DX12_RESOURCE_DESC texresDesc = CD3DX12_RESOURCE_DESC::Tex2D(
//		metadata.format, metadata.width, (UINT)metadata.height, (UINT16)metadata.arraySize,
//		(UINT16)metadata.mipLevels);
//
//	// �q�[�v�v���p�e�B
//	CD3DX12_HEAP_PROPERTIES heapProps =
//		CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0);
//
//	// �e�N�X�`���p�o�b�t�@�̐���
//	result = device->CreateCommittedResource(
//		&heapProps, D3D12_HEAP_FLAG_NONE, &texresDesc,
//		D3D12_RESOURCE_STATE_GENERIC_READ, // �e�N�X�`���p�w��
//		nullptr, IID_PPV_ARGS(&texbuff));
//	assert(SUCCEEDED(result));
//
//	// �e�N�X�`���o�b�t�@�Ƀf�[�^�]��
//	for (size_t i = 0; i < metadata.mipLevels; i++) {
//		const Image* img = scratchImg.GetImage(i, 0, 0); // ���f�[�^���o
//		result = texbuff->WriteToSubresource(
//			(UINT)i,
//			nullptr,              // �S�̈�փR�s�[
//			img->pixels,          // ���f�[�^�A�h���X
//			(UINT)img->rowPitch,  // 1���C���T�C�Y
//			(UINT)img->slicePitch // 1���T�C�Y
//		);
//		assert(SUCCEEDED(result));
//	}
//
//	// �V�F�[�_���\�[�X�r���[�쐬
//	cpuDescHandleSRV = CD3DX12_CPU_DESCRIPTOR_HANDLE(descHeap->GetCPUDescriptorHandleForHeapStart(), 0, descriptorHandleIncrementSize);
//	gpuDescHandleSRV = CD3DX12_GPU_DESCRIPTOR_HANDLE(descHeap->GetGPUDescriptorHandleForHeapStart(), 0, descriptorHandleIncrementSize);
//
//	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{}; // �ݒ�\����
//	D3D12_RESOURCE_DESC resDesc = texbuff->GetDesc();
//
//	srvDesc.Format = resDesc.Format;
//	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
//	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2D�e�N�X�`��
//	srvDesc.Texture2D.MipLevels = 1;
//
//	device->CreateShaderResourceView(texbuff.Get(), //�r���[�Ɗ֘A�t����o�b�t�@
//		&srvDesc, //�e�N�X�`���ݒ���
//		cpuDescHandleSRV
//	);
//	if (result == NULL) {
//		return false;
//	}
//
//	return true;
//}

void assimpObject3d::UpdateViewMatrix()
{
	// �r���[�s��̐���
	matView = Matrix4Math::ViewMat(
		eye,
		target,
		up);
}

bool assimpObject3d::Initialize()
{
	// nullptr�`�F�b�N
	assert(device);

	HRESULT result;

	// �q�[�v�v���p�e�B
	CD3DX12_HEAP_PROPERTIES heapProps = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	{
		// ���\�[�X�ݒ�
		CD3DX12_RESOURCE_DESC resourceDesc =
			CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferDataB0) + 0xff) & ~0xff);

		// �萔�o�b�t�@�̐���
		result = device->CreateCommittedResource(
			&heapProps, // �A�b�v���[�h�\
			D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
			IID_PPV_ARGS(&constBuffB0));
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
		result = device->CreateCommittedResource(
			&heapProps, // �A�b�v���[�h�\
			D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
			IID_PPV_ARGS(&constBuffB2));
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
	result = constBuffB2->Map(0, nullptr, (void**)&ConstMapPolygon);
	return true;
}

void assimpObject3d::Update()
{
	HRESULT result;
	Matrix4 matScale, matRot, matTrans;

	// �X�P�[���A��]�A���s�ړ��s��̌v�Z
	matScale = Matrix4Math::scale(scale);
	matRot = Matrix4Math::identity();
	matRot = matRot * Matrix4Math::rotateZ(ToRadian(rotation.z));
	matRot = matRot * Matrix4Math::rotateX(ToRadian(rotation.x));
	matRot = matRot * Matrix4Math::rotateY(ToRadian(rotation.y));
	matTrans = Matrix4Math::translate(position);

	// ���[���h�s��̍���
	matWorld = Matrix4Math::identity(); // �ό`�����Z�b�g
	matWorld = matWorld * matScale; // ���[���h�s��ɃX�P�[�����O�𔽉f
	matWorld = matWorld * matRot; // ���[���h�s��ɉ�]�𔽉f
	matWorld = matWorld * matTrans; // ���[���h�s��ɕ��s�ړ��𔽉f

	// �e�I�u�W�F�N�g�������
	if (parent != nullptr) {
		// �e�I�u�W�F�N�g�̃��[���h�s����|����
		matWorld *= parent->matWorld;
	}

	// �萔�o�b�t�@�փf�[�^�]��
	ConstBufferDataB0* constMap = nullptr;
	result = constBuffB0->Map(0, nullptr, (void**)&constMap);
	/*constMap->color = color;*/
	constMap->mat = matWorld * matView * matProjection;	// �s��̍���
	constBuffB0->Unmap(0, nullptr);

}

void assimpObject3d::Draw()
{
	// nullptr�`�F�b�N
	assert(device);
	assert(assimpObject3d::cmdList);

	//���f�����Z�b�g����Ă��Ȃ���Ε`����X�L�b�v
	if (model == nullptr) {
		return;
	}

	// �萔�o�b�t�@�r���[���Z�b�g
	cmdList->SetGraphicsRootConstantBufferView(0, constBuffB0->GetGPUVirtualAddress());

	// �萔�o�b�t�@�r���[���Z�b�g
	cmdList->SetGraphicsRootConstantBufferView(2, constBuffB2->GetGPUVirtualAddress());

	//���f����`��
	model->Draw(cmdList, 1);
}

Matrix4 assimpObject3d::GetMatViewPro()
{

	return matView * matProjection;
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



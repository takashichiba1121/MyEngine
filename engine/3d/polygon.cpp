#include"polygon.h"
#include <d3dcompiler.h>
#include <DirectXTex.h>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;


#pragma comment(lib, "d3dcompiler.lib")

using namespace DirectX;
using namespace Microsoft::WRL;

//polygonMana::Material polygonMana::material;

/// <summary>
/// �ÓI�����o�ϐ��̎���
/// </summary>
ID3D12Device* PolygonMana::device = nullptr;
//UINT polygonMana::descriptorHandleIncrementSize = 0;
ID3D12GraphicsCommandList* PolygonMana::cmdList = nullptr;
ComPtr<ID3D12RootSignature> PolygonMana::rootsignature;
ComPtr<ID3D12PipelineState> PolygonMana::pipelinestate;
//ComPtr<ID3D12DescriptorHeap> polygonMana::descHeap;
//ComPtr<ID3D12Resource> polygonMana::texbuff;
//CD3DX12_CPU_DESCRIPTOR_HANDLE polygonMana::cpuDescHandleSRV;
//CD3DX12_GPU_DESCRIPTOR_HANDLE polygonMana::gpuDescHandleSRV;
Matrix4 PolygonMana::matViewPro;
//polygonMana::VertexPosNormalUv polygonMana::vertices[vertexCount];
//unsigned short polygonMana::indices[planeCount * 3];

void PolygonMana::StaticInitialize(ID3D12Device* device, int window_width, int window_height)
{
	// nullptr�`�F�b�N
	assert(device);

	PolygonMana::device = device;

	Model::SetDevice(device);

	// �p�C�v���C��������
	InitializeGraphicsPipeline();

}

void PolygonMana::InitializeGraphicsPipeline()
{
	HRESULT result;

	Microsoft::WRL::ComPtr<ID3DBlob> vsBlob = nullptr;//���_�V�F�[�_�[�I�u�W�F�N�g
	Microsoft::WRL::ComPtr<ID3DBlob> psBlob = nullptr;//�s�N�Z���V�F�[�_�[�I�u�W�F�N�g

	ID3DBlob* errorBlob = nullptr;//�G���[�I�u�W�F�N�g

	//���_�V�F�[�_�[�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		L"Resources/shaders/polygonVS.hlsl", //�V�F�[�_�[�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,//�C���N���[�h�\�ɂ���
		"main", "vs_5_0",//�G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, //�f�o�b�O�p�ݒ�
		0,
		&vsBlob, &errorBlob);

	//�G���[�Ȃ�
	if (FAILED(result)) {
		//errorBlob����G���[���e��string�^�ɃR�s�[
		std::string error;
		error.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			error.begin());
		error += "\n";
		//�G���[���e���o�̓E�C���h�E�ɕ\��
		OutputDebugStringA(error.c_str());
		assert(0);
	}
	//�s�N�Z���V�F�[�_�[�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		L"Resources/shaders/polygonPS.hlsl",
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"main", "ps_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		&psBlob, &errorBlob);
	//�G���[�Ȃ�
	if (FAILED(result)) {
		//errorBlob������e��string�^�ɃR�s�[
		std::string error;
		error.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			error.begin());
		error += "\n";
		//�G���[���e���o�̓E�C���h�E�ɕ\��
		OutputDebugStringA(error.c_str());
		assert(0);
	}
	//���_���C�A�E�g
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{
		"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,
		D3D12_APPEND_ALIGNED_ELEMENT,
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0,
		},//vyz���W(��s�ŏ������ق������₷��)
		{
		"COLOR",0,DXGI_FORMAT_R32G32_FLOAT,0,
		D3D12_APPEND_ALIGNED_ELEMENT,
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0,
		},//uv���W(��s�ŏ������ق������₷��)
	};

	//�O���t�B�b�N�X�p�C�v���C���ݒ�
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc{};
	//�V�F�[�_�[�̐ݒ�
	pipelineDesc.VS.pShaderBytecode = vsBlob->GetBufferPointer();
	pipelineDesc.VS.BytecodeLength = vsBlob->GetBufferSize();
	pipelineDesc.PS.pShaderBytecode = psBlob->GetBufferPointer();
	pipelineDesc.PS.BytecodeLength = psBlob->GetBufferSize();

	//�T���v���}�X�N�̐ݒ�
	pipelineDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
	//���X�^���C�U�̐ݒ�
	pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;//�J�����O���Ȃ�
	pipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;//�|���S�����h��Ԃ�
	pipelineDesc.RasterizerState.DepthClipEnable = true;//�[�x�O���b�s���O��L����

	//�����_�[�^�[�Q�b�g�̃u�����h�ݒ�
	D3D12_RENDER_TARGET_BLEND_DESC& blenddesc = pipelineDesc.BlendState.RenderTarget[0];
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;//RGBA�S�Ẵ`�����l����`��
	blenddesc.BlendEnable = true;//�u�����h��L���ɂ���
	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;//���Z
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;//�\�[�X�̒l��100%�g��
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;//�f�X�g�̒l��0%�g��
	////���Z����
	//blenddesc.BlendOp = D3D12_BLEND_OP_ADD;//���Z
	//blenddesc.SrcBlend = D3D12_BLEND_ONE;//�\�[�X�̒l��100%�g��
	//blenddesc.DestBlend = D3D12_BLEND_ONE;//�f�X�g�̒l��100%�g��
	////���Z����
	//blenddesc.BlendOp = D3D12_BLEND_OP_REV_SUBTRACT;//�f�X�g����\�[�X�����Z
	//blenddesc.SrcBlend = D3D12_BLEND_ONE;//�\�[�X�̒l��100%�g��
	//blenddesc.DestBlend = D3D12_BLEND_ONE;//�f�X�g�̒l��100%�g��
	////�F���]
	//blenddesc.BlendOp = D3D12_BLEND_OP_ADD;//���Z
	//blenddesc.SrcBlend = D3D12_BLEND_INV_DEST_COLOR;//1.0f-�f�X�g�J���[�̒l
	//blenddesc.DestBlend = D3D12_BLEND_ZERO;//�g��Ȃ�
	//����������
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;//���Z
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;//�\�[�X�̃A���t�@�l
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;//1.0f-�\�[�X�̃A���t�@�l
	////�u�����h�X�e�[�g
	//pipelineDesc.BlendState.RenderTarget[0].RenderTargetWriteMask
	//	= D3D12_COLOR_WRITE_ENABLE_ALL;//RBGA���ׂẴ`�����l����`��
	//���_���C�A�E�g�̐ݒ�
	pipelineDesc.InputLayout.pInputElementDescs = inputLayout;
	pipelineDesc.InputLayout.NumElements = _countof(inputLayout);
	//�}�`�̌`��ݒ�
	pipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	//���̑��̐ݒ�
	pipelineDesc.NumRenderTargets = 1;//�`��Ώۂ͈��
	pipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;//0�`255�w���RGBA
	pipelineDesc.SampleDesc.Count = 1; //1�s�N�Z���ɂ����T���v�����O


	//���[�g�p�����[�^�[�̐ݒ�
	D3D12_ROOT_PARAMETER rootParams[1] = {};
	//�e�N�X�`�����W�X�^0��
	rootParams[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;//�萔�o�b�t�@
	rootParams[0].Descriptor.ShaderRegister = 0;//�萔�o�b�t�@�ԍ�
	rootParams[0].Descriptor.RegisterSpace = 0;//�f�t�H���g�l
	rootParams[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;//�S�ẴV�F�[�_�[���猩����

	//�e�N�X�`���T���v���[�ݒ�
	D3D12_STATIC_SAMPLER_DESC samplerDesc{};
	samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;//���J��Ԃ��i�^�C�����O�j
	samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;//�c�J��Ԃ��i�^�C�����O�j
	samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;//���s�J��Ԃ��i�^�C�����O�j
	samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;//�{�[�_�[�̎��͍�
	samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;//�S�ă��j�A�⊮
	samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;//�~�b�v�}�b�v�ő�l
	samplerDesc.MinLOD = 0.0f;//�~�b�v�}�b�v�ŏ��l
	samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;//�s�N�Z���V�F�[�_�[����̂ݎg�p�\

	//���[�g�V�O�l�`��
	//���[�g�V�O�l�`���̐ݒ�
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	rootSignatureDesc.pParameters = rootParams;//���[�g�p�����[�^�[�̐擪�A�h���X
	rootSignatureDesc.NumParameters = _countof(rootParams);//���[�g�p�����[�^��
	rootSignatureDesc.pStaticSamplers = &samplerDesc;
	rootSignatureDesc.NumStaticSamplers = 1;
	//���[�g�V�O�l�`���̃V���A���C�Y
	Microsoft::WRL::ComPtr<ID3DBlob> rootSigBlob = nullptr;
	result = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0,
		&rootSigBlob, &errorBlob);
	assert(SUCCEEDED(result));
	result = device->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(&rootsignature));
	assert(SUCCEEDED(result));
	//rootSigBlob->Release();
	//�p�C�v���C���Ƀ��[�g�V�O�l�`�����Z�b�g
	pipelineDesc.pRootSignature = rootsignature.Get();

	//�p�C�v�����X�X�e�[�g�̐���
	result = device->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&pipelinestate));
	assert(SUCCEEDED(result));

}

//bool polygonMana::LoadTexture(const std::string& directoryPath, const std::string& filename)
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

void PolygonMana::UpdateViewMatrix()
{
	matViewPro = Object3d::GetMatViewPro();
}

bool PolygonMana::Initialize()
{
	// nullptr�`�F�b�N
	assert(device);

	// �q�[�v�v���p�e�B
	CD3DX12_HEAP_PROPERTIES heapProps = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	// ���\�[�X�ݒ�
	CD3DX12_RESOURCE_DESC resourceDesc =
		CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferDataB0) + 0xff) & ~0xff);


	HRESULT result;

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

	return true;
}

void PolygonMana::Update()
{
	HRESULT result;

	//�������s�����p�[�e�B�N����S�폜
	Polygons.remove_if([](polygon& x) {
		return x.frame >= x.num_frame;
		});
	//�S�p�[�e�B�N��
	for (std::list<polygon>::iterator it = Polygons.begin(); it !=Polygons .end(); it++)
	{
		//�o�߃t���[�������J�E���g
		it->frame++;
		//���x�ɉ����x�����Z
		it->velocity = it->velocity + it->accel;
		//���x�ɂ��ړ�
		it->position[0] = it->position[0] + it->velocity;

		it->position[1] = it->position[1] + it->velocity;

		it->position[2] = it->position[2] + it->velocity;


		float f = (float)it->frame / it->num_frame;

		//�Ԃ̐��`���
		it->color.x = (it->e_color.x - it->s_color.x) * f;
		it->color.x += it->s_color.x;
		//�̐��`���
		it->color.y = (it->e_color.y - it->s_color.y) * f;
		it->color.y += it->s_color.y;
		//�΂̐��`���
		it->color.z = (it->e_color.z - it->s_color.z) * f;
		it->color.z += it->s_color.z;

		//�΂̐��`���
		it->color.w = (it->e_color.w - it->s_color.w) * f;
		it->color.w += it->s_color.w;
	}
	//���_�o�b�t�@�փf�[�^�]��
	VertexPos* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	if (SUCCEEDED(result))
	{
		//�p�[�e�B�N���̏���1�����f
		for (std::list<polygon>::iterator it = Polygons.begin(); it != Polygons.end(); it++)
		{
			//���W
			vertMap->pos = it->position[0];
			vertMap->color = it->color;
			//���̒��_��
			vertMap++;
			//���W
			vertMap->pos = it->position[1];
			vertMap->color = it->color;
			//���̒��_��
			vertMap++;
			//���W
			vertMap->pos = it->position[2];
			vertMap->color = it->color;
			//���̒��_��
			vertMap++;
		}
		vertBuff->Unmap(0, nullptr);
	}

	UpdateViewMatrix();
	// �萔�o�b�t�@�փf�[�^�]��
	ConstBufferDataB0* constMap = nullptr;
	result = constBuffB0->Map(0, nullptr, (void**)&constMap);
	constMap->mat = matWorld * matViewPro;	// �s��̍���
	constBuffB0->Unmap(0, nullptr);
}

void PolygonMana::Draw()
{
	// nullptr�`�F�b�N
	assert(device);
	assert(cmdList);

	// ���_�o�b�t�@�̐ݒ�
	cmdList->IASetVertexBuffers(0, 1, &vbView);

	// �萔�o�b�t�@�r���[���Z�b�g
	cmdList->SetGraphicsRootConstantBufferView(0, constBuffB0->GetGPUVirtualAddress());

	for (std::list<polygon>::iterator it = Polygons.begin(); it != Polygons.end(); it++)
	{

		cmdList->DrawInstanced(_countof(it->position), 1, 0, 0);
	}
}

void PolygonMana::Add(int life, Object3d::VertexPosNormalUv position, Object3d::VertexPosNormalUv position2, Object3d::VertexPosNormalUv position3, Vector3 velocity, Vector3 accel, Vector4 start_color, Vector4 end_color, Matrix4 matworld)
{
	//���X�g�ɗv�f��ǉ�
	Polygons.emplace_front();
	//�ǉ������v�f�̎Q��
	polygon& p = Polygons.front();
	//�l�̃Z�b�g
	p.position[0] = position.pos;
	p.position[1] = position2.pos;
	p.position[2] = position3.pos;
	p.velocity = velocity;
	p.accel = accel;
	p.num_frame = life;
	p.s_color = start_color;
	p.e_color = end_color;
}


#include "SpriteCommon.h"
#include"Texture.h"

void SpriteCommon::Initialize(DirectXCommon* DxCommon)
{
	HRESULT result;

	Microsoft::WRL::ComPtr<ID3DBlob> vsBlob = nullptr;//���_�V�F�[�_�[�I�u�W�F�N�g
	Microsoft::WRL::ComPtr<ID3DBlob> psBlob = nullptr;//�s�N�Z���V�F�[�_�[�I�u�W�F�N�g

	dxCommon = DxCommon;
	ID3DBlob* errorBlob = nullptr;//�G���[�I�u�W�F�N�g

	//���_�V�F�[�_�[�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		L"Resources/shaders/SpriteVS.hlsl", //�V�F�[�_�[�t�@�C����
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
		L"Resources/shaders/SpritePS.hlsl",
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
		"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,
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

	//�f�X�N���v�^�����W�̐ݒ�
	D3D12_DESCRIPTOR_RANGE descriptorRange{};
	descriptorRange.NumDescriptors = 1;//��x�̕`��Ɏg���e�N�X�`�����ꖇ�Ȃ̂łP
	descriptorRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRange.BaseShaderRegister = 0;//�e�N�X�`�����W�X�^0��
	descriptorRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;


	//���[�g�p�����[�^�[�̐ݒ�
	D3D12_ROOT_PARAMETER rootParams[3] = {};
	//�e�N�X�`�����W�X�^0��
	rootParams[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;//�萔�o�b�t�@
	rootParams[0].Descriptor.ShaderRegister = 0;//�萔�o�b�t�@�ԍ�
	rootParams[0].Descriptor.RegisterSpace = 0;//�f�t�H���g�l
	rootParams[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;//�S�ẴV�F�[�_�[���猩����
	//�e�N�X�`�����W�X�^1��
	rootParams[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;//���
	rootParams[1].DescriptorTable.pDescriptorRanges = &descriptorRange;//�f�X�N���v�^�����W
	rootParams[1].DescriptorTable.NumDescriptorRanges = 1;//�f�X�N���v�^�����W��
	rootParams[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;//�S�ẴV�F�[�_�[���猩����
	//�e�N�X�`�����W�X�^2��
	rootParams[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;//�萔�o�b�t�@
	rootParams[2].Descriptor.ShaderRegister = 1;//�萔�o�b�t�@�ԍ�
	rootParams[2].Descriptor.RegisterSpace = 0;//�f�t�H���g�l
	rootParams[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;//�S�ẴV�F�[�_�[���猩����

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
	result = dxCommon->GetDevice()->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(&rootSignature));
	assert(SUCCEEDED(result));
	//rootSigBlob->Release();
	//�p�C�v���C���Ƀ��[�g�V�O�l�`�����Z�b�g
	pipelineDesc.pRootSignature = rootSignature.Get();

	//�p�C�v�����X�X�e�[�g�̐���
	result = dxCommon->GetDevice()->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&pipelineState));
	assert(SUCCEEDED(result));

	//�l���������ނƎ����I�ɓ]�������
	matProjection= Matrix4Math::identity();

	matProjection.m[0][0] = 2.0f / dxCommon->getWinApp()->window_width;
	matProjection.m[1][1] = -2.0f / dxCommon->getWinApp()->window_heigit;
	matProjection.m[3][0] = -1.0f;
	matProjection.m[3][1] = 1.0f;
}

void SpriteCommon::PreDraw()
{
	//�p�C�v���C���X�e�[�g�ƃ��[�g�V�O�l�`���̐ݒ�R�}���h
	dxCommon->GetCommandList()->SetPipelineState(pipelineState.Get());
	dxCommon->GetCommandList()->SetGraphicsRootSignature(rootSignature.Get());

	//SRV�q�[�v�̐ݒ�R�}���h
	ID3D12DescriptorHeap* ppHeaps[] = { Texture::descHeap.Get() };
	dxCommon->GetCommandList()->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
}

void SpriteCommon::PostDrow()
{
}

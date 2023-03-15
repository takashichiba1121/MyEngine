#include "PostEffect.h"
#include<d3dx12.h>
#include"WinApp.h"
#include"Texture.h"
#include"SpriteCommon.h"

using namespace DirectX;

const float PostEffect::clearColor[4] = {0.25f,0.5f,0.1f,0.0f};

PostEffect::PostEffect() :Sprite()
{

}

void PostEffect::Initialize()
{
	HRESULT result;

	ID3D12Device* device = dxCommon->GetDevice();

	D3D12_RESOURCE_DESC texresDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,
		WinApp::window_width,
		(UINT)WinApp::window_height,
		1,0,1,0,D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET);

	//�e�N�X�`���o�b�t�@�̐���
	//�q�[�v�ݒ�
	CD3DX12_HEAP_PROPERTIES heapProp(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK,
		D3D12_MEMORY_POOL_L0);

	CD3DX12_CLEAR_VALUE clearValue(DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, clearColor);


	result = device->CreateCommittedResource(
	&heapProp,
	D3D12_HEAP_FLAG_NONE,
		&texresDesc,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
		&clearValue,
		IID_PPV_ARGS(&texBuff));
	assert(SUCCEEDED(result));

	{//�e�N�X�`����ԃN���A
		//��f��(1280*720=921600�s�N�Z��)
		const UINT pixelCount = WinApp::window_width * WinApp::window_height;
		//�摜1�s���̃f�[�^�T�C�Y
		const UINT rowPitch = sizeof(UINT) * WinApp::window_width;
		//�摜�S�̂̃f�[�^�T�C�Y
		const UINT depthPitch = rowPitch * WinApp::window_height;
		//�摜�C���[�W
		UINT* img = new UINT[pixelCount];
		for (int i = 0; i < pixelCount; i++) { img[i] = 0xff0000ff; }


		result = texBuff->WriteToSubresource(0,nullptr,
			img,rowPitch,depthPitch);
		assert(SUCCEEDED(result));
		delete[] img;
	}
	//SRV�p�̃f�X�N���v�^�q�[�v�ݒ�
	D3D12_DESCRIPTOR_HEAP_DESC srvDescHeapDesc = {};
	srvDescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvDescHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	srvDescHeapDesc.NumDescriptors = 1;
	//SRV�f�X�N���v�^�q�[�v�ݒ�
	result = device->CreateDescriptorHeap(&srvDescHeapDesc,IID_PPV_ARGS(&descHeapSRV));
	assert(SUCCEEDED(result));
	//SRV�ݒ�
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;

	//�f�X�N���v�^�q�[�v��SRV���쐬
	device->CreateShaderResourceView(texBuff.Get(),
		&srvDesc,
		descHeapSRV->GetCPUDescriptorHandleForHeapStart()
	);

	//RTV�p�̃f�X�N���v�^�q�[�v�ݒ�
	D3D12_DESCRIPTOR_HEAP_DESC rtvDescHeapDesc{};

	rtvDescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvDescHeapDesc.NumDescriptors = 1;
	
	//RTV�p�̃f�X�N���v�^�q�[�v�𐶐�
	result = device->CreateDescriptorHeap(&rtvDescHeapDesc, IID_PPV_ARGS(&descHeapRTV));
	assert(SUCCEEDED(result));
	//�f�X�N���v�^�q�[�v��RTV���쐬
	device->CreateRenderTargetView(texBuff.Get(), nullptr, descHeapRTV->GetCPUDescriptorHandleForHeapStart());

	//�[�x�o�b�t�@
	CD3DX12_RESOURCE_DESC depthResDesc =
		CD3DX12_RESOURCE_DESC::Tex2D(
			DXGI_FORMAT_D32_FLOAT,
			WinApp::window_width,
			WinApp::window_height,
			1, 0,
			1, 0,
			D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL);

	CD3DX12_HEAP_PROPERTIES HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

	CD3DX12_CLEAR_VALUE CLEAR_VALUE(DXGI_FORMAT_D32_FLOAT, 1.0f, 0);

	result = device->CreateCommittedResource(
		&HEAP_PROPERTIES,
		D3D12_HEAP_FLAG_NONE,
		&depthResDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,
		&CLEAR_VALUE,
		IID_PPV_ARGS(&depthBuff));
	assert(SUCCEEDED(result));

	//DSV�p�̃f�X�N���v�^�q�[�v�ݒ�
	D3D12_DESCRIPTOR_HEAP_DESC DescHeapDesc{};
	DescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	DescHeapDesc.NumDescriptors = 1;
	//DSV�p�f�X�N���v�^�q�[�v�̍쐬
	result = device->CreateDescriptorHeap(&DescHeapDesc, IID_PPV_ARGS(&descHeapDSV));
	assert(SUCCEEDED(result));

	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	device->CreateDepthStencilView(depthBuff.Get(),
		&dsvDesc,
		descHeapDSV->GetCPUDescriptorHandleForHeapStart());


	Sprite::Initialize();
}

void PostEffect::PreDrawScene(ID3D12GraphicsCommandList* cmdList)
{
	CD3DX12_RESOURCE_BARRIER resouceBar = CD3DX12_RESOURCE_BARRIER::Transition(texBuff.Get(),
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
		D3D12_RESOURCE_STATE_RENDER_TARGET);

	//���\�[�X�o���A��ύX(�V�F�[�_�[���\�[�X���`��\)
	cmdList->ResourceBarrier(1,
		&resouceBar);

	//�����_�[�^�[�Q�b�g�r���[�p�̃f�B�X�N���v�^�q�[�v�̃n���h�����擾
	D3D12_CPU_DESCRIPTOR_HANDLE rtvH = descHeapRTV->GetCPUDescriptorHandleForHeapStart();
	//�[�x�X�e���V���r���[�p�f�X�N���v�^�q�[�v�̃n���h�����擾
	D3D12_CPU_DESCRIPTOR_HANDLE dsvH = descHeapDSV->GetCPUDescriptorHandleForHeapStart();
	//�����_�[�^�[�Q�b�g���Z�b�g
	cmdList->OMSetRenderTargets(1, &rtvH, false, &dsvH);

	CD3DX12_VIEWPORT viewPort(0.0f, 0.0f, WinApp::window_width, WinApp::window_height);

	//�r���[�|�[�g�̐ݒ�
	cmdList->RSSetViewports(1, &viewPort);

	CD3DX12_RECT rect(0, 0, WinApp::window_width, WinApp::window_height);

	//�V�U�����O��`�̐ݒ�
	cmdList->RSSetScissorRects(1, &rect);

	//�S��ʂ̃N���A
	cmdList->ClearRenderTargetView(rtvH, clearColor, 0, nullptr);
	//�[�x�o�b�t�@�̃N���A
	cmdList->ClearDepthStencilView(dsvH, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
}

void PostEffect::Draw()
{
	//�p�C�v���C���X�e�[�g�ƃ��[�g�V�O�l�`���̐ݒ�R�}���h
	dxCommon->GetCommandList()->SetPipelineState(SpriteCommon::GetPipelineState());
	dxCommon->GetCommandList()->SetGraphicsRootSignature(SpriteCommon::GetRootSignature());

	//SRV�q�[�v�̐ݒ�R�}���h
	ID3D12DescriptorHeap* ppHeaps[] = { descHeapSRV.Get()};
	dxCommon->GetCommandList()->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	//�v���~�e�B�u�`��̐ݒ�R�}���h
	dxCommon->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	//���_�o�b�t�@�r���[�̐ݒ�R�}���h
	dxCommon->GetCommandList()->IASetVertexBuffers(0, 1, &vbView);
	//�萔�o�b�t�@�r���[�iCBV�j�̐ݒ�R�}���h
	dxCommon->GetCommandList()->SetGraphicsRootConstantBufferView(0, constBuffMaterial->GetGPUVirtualAddress());
	//�萔�o�b�t�@�r���[�iCBV�j�̐ݒ�R�}���h
	dxCommon->GetCommandList()->SetGraphicsRootConstantBufferView(2, constBuffTransform->GetGPUVirtualAddress());

	//�摜�`��
	//SRV�q�[�v�̐擪�n���h�����擾�iSRV���w���Ă���͂��j
	dxCommon->GetCommandList()->SetGraphicsRootDescriptorTable(1, descHeapSRV->GetGPUDescriptorHandleForHeapStart());

	//�`��R�}���h
	dxCommon->GetCommandList()->DrawInstanced(_countof(vertices), 1, 0, 0);//���ׂĂ̒��_���g���ĕ`��
}

void PostEffect::PostDrawScene(ID3D12GraphicsCommandList* cmdList)
{
	CD3DX12_RESOURCE_BARRIER RESOURCE_BARRIER= CD3DX12_RESOURCE_BARRIER::Transition(texBuff.Get(),
		D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);

	cmdList->ResourceBarrier(1, &RESOURCE_BARRIER);
}

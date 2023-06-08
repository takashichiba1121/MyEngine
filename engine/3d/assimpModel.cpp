#include "assimpModel.h"
#include"assimpLoader.h"
#include"Texture.h"

//�ÓI�����o�ϐ��̎���
ID3D12Device* AssimpModel::sDevice= nullptr;

void AssimpModel::CreateBuffers()
{
	HRESULT result = S_FALSE;

	for (Mesh* mesh:meshs_)
	{

		UINT sizeVB = static_cast<UINT>(sizeof(VertexPosNormalUv) * mesh->vertexs.size());

		// �q�[�v�v���p�e�B
		CD3DX12_HEAP_PROPERTIES heapProps = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
		// ���\�[�X�ݒ�
		CD3DX12_RESOURCE_DESC resourceDesc = CD3DX12_RESOURCE_DESC::Buffer(sizeVB);

		// ���_�o�b�t�@����
		result = sDevice->CreateCommittedResource(
			&heapProps, D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
			IID_PPV_ARGS(&mesh->vertBuff));
		assert(SUCCEEDED(result));

		// ���_�o�b�t�@�ւ̃f�[�^�]��
		VertexPosNormalUv* vertMap = nullptr;
		result = mesh->vertBuff->Map(0, nullptr, (void**)&vertMap);
		if (SUCCEEDED(result)) {
			/*memcpy(vertMap, vertices, sizeof(vertices));*/
			std::copy(mesh->vertexs.begin(), mesh->vertexs.end(), vertMap);
			mesh->vertBuff->Unmap(0, nullptr);
		}

		// ���_�o�b�t�@�r���[�̍쐬
		mesh->vbView.BufferLocation = mesh->vertBuff->GetGPUVirtualAddress();
		/*vbView.SizeInBytes = sizeof(vertices);*/
		mesh->vbView.SizeInBytes = sizeVB;
		mesh->vbView.StrideInBytes = sizeof(mesh->vertexs[0]);

		/*UINT sizeIB = static_cast<UINT>(sizeof(indices));*/
		UINT sizeIB = static_cast<UINT>(sizeof(unsigned short) * mesh->indices.size());
		// ���\�[�X�ݒ�
		resourceDesc.Width = sizeIB;

		// �C���f�b�N�X�o�b�t�@����
		result = sDevice->CreateCommittedResource(
			&heapProps, D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
			IID_PPV_ARGS(&mesh->indexBuff));

		// �C���f�b�N�X�o�b�t�@�ւ̃f�[�^�]��
		unsigned short* indexMap = nullptr;
		result = mesh->indexBuff->Map(0, nullptr, (void**)&indexMap);
		if (SUCCEEDED(result)) {

			// �S�C���f�b�N�X�ɑ΂���
			//for (int i = 0; i < _countof(indices); i++)
			//{
			//	indexMap[i] = indices[i];	// �C���f�b�N�X���R�s�[
			//}
			std::copy(mesh->indices.begin(), mesh->indices.end(), indexMap);

			mesh->indexBuff->Unmap(0, nullptr);
		}

		// �C���f�b�N�X�o�b�t�@�r���[�̍쐬
		mesh->ibView.BufferLocation = mesh->indexBuff->GetGPUVirtualAddress();
		mesh->ibView.Format = DXGI_FORMAT_R16_UINT;
		/*ibView.SizeInBytes = sizeof(indices);*/
		mesh->ibView.SizeInBytes = sizeIB;

		// �q�[�v�v���p�e�B
		CD3DX12_HEAP_PROPERTIES heapPropsB1 = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
		// ���\�[�X�ݒ�
		CD3DX12_RESOURCE_DESC resourceDescB1 =
			CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferDataB1) + 0xff) & ~0xff);
		// �萔�o�b�t�@�̐���
		result = sDevice->CreateCommittedResource(
			&heapPropsB1,
			D3D12_HEAP_FLAG_NONE,
			&resourceDescB1,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&constBuffB1));
		// �萔�o�b�t�@�փf�[�^�]��
		ConstBufferDataB1* constMap1 = nullptr;
		result = constBuffB1->Map(0, nullptr, (void**)&constMap1);
		if (SUCCEEDED(result)) {
			constMap1->ambient = mesh->materials.ambient;
			constMap1->diffuse = mesh->materials.diffuse;
			constMap1->specular = mesh->materials.specular;
			constMap1->alpha = mesh->materials.alpha;
			constBuffB1->Unmap(0, nullptr);
		}
	}
}

AssimpModel* AssimpModel::LoadFormFBX(const std::string& modelname)
{
	AssimpModel* assimpModel = new AssimpModel;

	assimpModel = AssimpLoader::Load(modelname);

	assimpModel->CreateBuffers();

	return assimpModel;
}

void AssimpModel::Draw(ID3D12GraphicsCommandList* cmdList, UINT rootParamIndexMaterial)
{
	for (Mesh* mesh : meshs_)
	{
		// ���_�o�b�t�@�̐ݒ�
		cmdList->IASetVertexBuffers(0, 1, &mesh->vbView);
		// �C���f�b�N�X�o�b�t�@�̐ݒ�
		cmdList->IASetIndexBuffer(&mesh->ibView);

		// �f�X�N���v�^�q�[�v�̔z��
		ID3D12DescriptorHeap* ppHeaps[] = { Texture::descHeap.Get() };
		cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

		// �萔�o�b�t�@�r���[���Z�b�g
		cmdList->SetGraphicsRootConstantBufferView(rootParamIndexMaterial, constBuffB1->GetGPUVirtualAddress());
		//SRV�q�[�v�̐擪�n���h�����擾�iSRV���w���Ă���͂��j
		D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = Texture::descHeap->GetGPUDescriptorHandleForHeapStart();
		UINT incrementSize = sDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
		srvGpuHandle.ptr += incrementSize * mesh->materials.texHandle;
		cmdList->SetGraphicsRootDescriptorTable(3, srvGpuHandle);
		// �`��R�}���h
		cmdList->DrawIndexedInstanced((UINT)mesh->indices.size(), 1, 0, 0, 0);
	}
}

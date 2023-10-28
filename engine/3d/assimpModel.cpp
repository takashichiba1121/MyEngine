 #include "assimpModel.h"
#include"assimpLoader.h"
#include"TextureManager.h"

//静的メンバ変数の実体
ID3D12Device* AssimpModel::sDevice= nullptr;

AssimpModel::~AssimpModel()
{
}

void AssimpModel::CreateBuffers()
{
	HRESULT result = S_FALSE;

	for (std::unique_ptr<AssimpModel::Mesh>&  mesh:meshs_)
	{

		uint32_t sizeVB = static_cast<uint32_t>(sizeof(VertexPosNormalUv) * mesh->vertexs.size());

		// ヒーププロパティ
		CD3DX12_HEAP_PROPERTIES heapProps = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
		// リソース設定
		CD3DX12_RESOURCE_DESC resourceDesc = CD3DX12_RESOURCE_DESC::Buffer(sizeVB);

		// 頂点バッファ生成
		result = sDevice->CreateCommittedResource(
			&heapProps, D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
			IID_PPV_ARGS(&mesh->vertBuff));
		assert(SUCCEEDED(result));

		// 頂点バッファへのデータ転送
		VertexPosNormalUv* vertMap = nullptr;
		result = mesh->vertBuff->Map(0, nullptr, (void**)&vertMap);
		if (SUCCEEDED(result)) {
			/*memcpy(vertMap, vertices, sizeof(vertices));*/
			std::copy(mesh->vertexs.begin(), mesh->vertexs.end(), vertMap);
			mesh->vertBuff->Unmap(0, nullptr);
		}

		// 頂点バッファビューの作成
		mesh->vbView.BufferLocation = mesh->vertBuff->GetGPUVirtualAddress();
		/*vbView.SizeInBytes = sizeof(vertices);*/
		mesh->vbView.SizeInBytes = sizeVB;
		mesh->vbView.StrideInBytes = sizeof(mesh->vertexs[0]);

		/*UINT sizeIB = static_cast<UINT>(sizeof(indices));*/
		uint32_t sizeIB = static_cast<uint32_t>(sizeof(unsigned short) * mesh->indices.size());
		// リソース設定
		resourceDesc.Width = sizeIB;

		// インデックスバッファ生成
		result = sDevice->CreateCommittedResource(
			&heapProps, D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
			IID_PPV_ARGS(&mesh->indexBuff));

		// インデックスバッファへのデータ転送
		size_t* indexMap = nullptr;
		result = mesh->indexBuff->Map(0, nullptr, (void**)&indexMap);
		if (SUCCEEDED(result)) {

			// 全インデックスに対して
			//for (int i = 0; i < _countof(indices); i++)
			//{
			//	indexMap[i] = indices[i];	// インデックスをコピー
			//}
			std::copy(mesh->indices.begin(), mesh->indices.end(), indexMap);

			mesh->indexBuff->Unmap(0, nullptr);
		}

		// インデックスバッファビューの作成
		mesh->ibView.BufferLocation = mesh->indexBuff->GetGPUVirtualAddress();
		mesh->ibView.Format = DXGI_FORMAT_R16_UINT;
		/*ibView.SizeInBytes = sizeof(indices);*/
		mesh->ibView.SizeInBytes = sizeIB;

		// ヒーププロパティ
		CD3DX12_HEAP_PROPERTIES heapPropsB1 = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
		// リソース設定
		CD3DX12_RESOURCE_DESC resourceDescB1 =
			CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferDataB1) + 0xff) & ~0xff);
		// 定数バッファの生成
		result = sDevice->CreateCommittedResource(
			&heapPropsB1,
			D3D12_HEAP_FLAG_NONE,
			&resourceDescB1,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&constBuffB1_));
		// 定数バッファへデータ転送
		ConstBufferDataB1* constMap1 = nullptr;
		result = constBuffB1_->Map(0, nullptr, (void**)&constMap1);
		if (SUCCEEDED(result)) {
			constMap1->ambient = mesh->materials.ambient;
			constMap1->diffuse = mesh->materials.diffuse;
			constMap1->specular = mesh->materials.specular;
			constMap1->alpha = mesh->materials.alpha;
			constBuffB1_->Unmap(0, nullptr);
		}
	}
}

AssimpModel* AssimpModel::LoadFormFBX(const std::string& modelname)
{
	AssimpModel* assimpModel;

	assimpModel = AssimpLoader::Load(modelname);

	assimpModel->CreateBuffers();

	return assimpModel;
}

void AssimpModel::Draw(ID3D12GraphicsCommandList* cmdList, uint32_t rootParamIndexMaterial)
{
	for (std::unique_ptr<AssimpModel::Mesh>& mesh: meshs_) 
	{
		// 頂点バッファの設定
		cmdList->IASetVertexBuffers(0, 1, &mesh->vbView);
		// インデックスバッファの設定
		cmdList->IASetIndexBuffer(&mesh->ibView);

		// デスクリプタヒープの配列
		ID3D12DescriptorHeap* ppHeaps[] = { TextureManager::Instance()->descHeap.Get() };
		cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

		// 定数バッファビューをセット
		cmdList->SetGraphicsRootConstantBufferView(rootParamIndexMaterial, constBuffB1_->GetGPUVirtualAddress());
		//SRVヒープの先頭ハンドルを取得（SRVを指しているはず）
		D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = TextureManager::Instance()->descHeap->GetGPUDescriptorHandleForHeapStart();
		uint32_t incrementSize = sDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
		srvGpuHandle.ptr += incrementSize * mesh->materials.texHandle;
		cmdList->SetGraphicsRootDescriptorTable(3, srvGpuHandle);
		// 描画コマンド
		cmdList->DrawIndexedInstanced((uint32_t)mesh->indices.size(), 1, 0, 0, 0);
	}
}

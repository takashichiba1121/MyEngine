#include "Model.h"
#include"Texture.h"
#include<unordered_map>

using namespace std;

//�ÓI�����o�ϐ��̎���
ID3D12Device* Model::sDevice = nullptr;

map<string, Model*> Model::models;

Model* Model::LoadFormOBJ(const std::string& modelname,bool smoothing)
{
	//�V����Model�^�̃C���X�^���X��new����
	Model* model = new Model();

	//�ǂݍ���
	model->LoadFromOBJInternal(modelname,smoothing);
	
	//�o�b�t�@����
	model->CreateBuffers();

	return model;
}

void Model::LoadMaterial(const std::string& directoryPath, const std::string& filename)
{
	//�t�@�C���X�g���[��
	std::ifstream file;
	//�}�e���A���t�@�C�����J��
	file.open(directoryPath + filename);
	//�t�@�C���I�[�v�����s���`�F�b�N
	if (file.fail()) {
		assert(0);
	}

	//1�s���ǂݍ���
	string line;
	while (getline(file, line)) {

		//1�s���̕�������X�g���[���ɕϊ�
		std::istringstream line_stream(line);
		//���p�X�y�[�X��؂�ōs�̐擪��������擾
		string key;
		getline(line_stream, key, ' ');

		//�擪�̃^�u�����͖�������
		if (key[0] == '\t') {
			key.erase(key.begin());	//�擪�̕������폜
		}
		//�擪������newmtl�Ȃ�}�e���A����
		if (key == "newmtl") {
			//�}�e���A�����ǂݍ���
			line_stream >> material.name;
		}
		//�擪������Ka�Ȃ�A���r�G���g�F
		if (key == "Ka") {
			line_stream >> material.ambient.x;
			line_stream >> material.ambient.y;
			line_stream >> material.ambient.z;
		}
		//�擪������Kd�Ȃ�f�B�t���[�Y�F
		if (key == "Kd") {
			line_stream >> material.diffuse.x;
			line_stream >> material.diffuse.y;
			line_stream >> material.diffuse.z;
		}
		//�擪������Ks�Ȃ�X�y�L�����[�F
		if (key == "Ks") {
			line_stream >> material.specular.x;
			line_stream >> material.specular.y;
			line_stream >> material.specular.z;
		}
		//�擪������map_Kd�Ȃ�e�N�X�`���t�@�C����
		if (key == "map_Kd") {
			//�e�N�X�`���̃t�@�C�����ǂݍ���
			line_stream >> material.textureFilename;
			//�e�N�X�`���ǂݍ���
			LoadTexture(directoryPath, material.textureFilename);
		}
	}
	//�t�@�C�������
	file.close();
}

void Model::LoadTexture(const std::string& directoryPath, const std::string& filename)
{
	HRESULT result = S_FALSE;

	//�t�@�C���p�X������
	string filepath = directoryPath + filename;

	//���j�R�[�h������ɕϊ�����
	wchar_t wfilepath[128];
	uint32_t iBufferSize = MultiByteToWideChar(CP_ACP, 0, filepath.c_str(), -1, wfilepath, _countof(wfilepath));

	textureIndex_=Texture::LoadTexture(wfilepath);
}
void Model::SetModel(const std::string str, Model* model)
{
	models.insert(std::make_pair(str,model));
}
Model* Model::GetModel(std::string str)
{
	return models[str];
}
void Model::CreateBuffers()
{
	HRESULT result = S_FALSE;

	uint32_t sizeVB = static_cast<uint32_t>(sizeof(VertexPosNormalUv) * vertices_.size());

	// �q�[�v�v���p�e�B
	CD3DX12_HEAP_PROPERTIES heapProps = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	// ���\�[�X�ݒ�
	CD3DX12_RESOURCE_DESC resourceDesc = CD3DX12_RESOURCE_DESC::Buffer(sizeVB);

	// ���_�o�b�t�@����
	result = sDevice->CreateCommittedResource(
		&heapProps, D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
		IID_PPV_ARGS(&vertBuff_));
	assert(SUCCEEDED(result));

	// ���_�o�b�t�@�ւ̃f�[�^�]��
	VertexPosNormalUv* vertMap = nullptr;
	result = vertBuff_->Map(0, nullptr, (void**)&vertMap);
	if (SUCCEEDED(result)) {
		/*memcpy(vertMap, vertices, sizeof(vertices));*/
		std::copy(vertices_.begin(),vertices_.end(), vertMap);
		vertBuff_->Unmap(0, nullptr);
	}

	// ���_�o�b�t�@�r���[�̍쐬
	vbView_.BufferLocation = vertBuff_->GetGPUVirtualAddress();
	/*vbView.SizeInBytes = sizeof(vertices);*/
	vbView_.SizeInBytes = sizeVB;
	vbView_.StrideInBytes = sizeof(vertices_[0]);

	/*UINT sizeIB = static_cast<UINT>(sizeof(indices));*/
	uint32_t sizeIB = static_cast<uint32_t>(sizeof(unsigned short) * indices_.size());
	// ���\�[�X�ݒ�
	resourceDesc.Width = sizeIB;

	// �C���f�b�N�X�o�b�t�@����
	result = sDevice->CreateCommittedResource(
		&heapProps, D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
		IID_PPV_ARGS(&indexBuff_));

	// �C���f�b�N�X�o�b�t�@�ւ̃f�[�^�]��
	unsigned short* indexMap = nullptr;
	result = indexBuff_->Map(0, nullptr, (void**)&indexMap);
	if (SUCCEEDED(result)) {

		// �S�C���f�b�N�X�ɑ΂���
		//for (int i = 0; i < _countof(indices); i++)
		//{
		//	indexMap[i] = indices[i];	// �C���f�b�N�X���R�s�[
		//}
		std::copy(indices_.begin(), indices_.end(), indexMap);

		indexBuff_->Unmap(0, nullptr);
	}

	// �C���f�b�N�X�o�b�t�@�r���[�̍쐬
	ibView_.BufferLocation = indexBuff_->GetGPUVirtualAddress();
	ibView_.Format = DXGI_FORMAT_R16_UINT;
	/*ibView.SizeInBytes = sizeof(indices);*/
	ibView_.SizeInBytes = sizeIB;

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
		IID_PPV_ARGS(&constBuffB1_));
	// �萔�o�b�t�@�փf�[�^�]��
	ConstBufferDataB1* constMap1 = nullptr;
	result = constBuffB1_->Map(0, nullptr, (void**)&constMap1);
	if (SUCCEEDED(result)) {
		constMap1->ambient = material.ambient;
		constMap1->diffuse = material.diffuse;
		constMap1->specular = material.specular;
		constMap1->alpha = material.alpha;
		constBuffB1_->Unmap(0, nullptr);
	}
}

void Model::Draw(ID3D12GraphicsCommandList* cmdList, uint32_t rootParamIndexMaterial)
{
	// ���_�o�b�t�@�̐ݒ�
	cmdList->IASetVertexBuffers(0, 1, &vbView_);
	// �C���f�b�N�X�o�b�t�@�̐ݒ�
	cmdList->IASetIndexBuffer(&ibView_);

	// �f�X�N���v�^�q�[�v�̔z��
	ID3D12DescriptorHeap* ppHeaps[] = { Texture::sDescHeap.Get()};
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	// �萔�o�b�t�@�r���[���Z�b�g
	cmdList->SetGraphicsRootConstantBufferView(rootParamIndexMaterial, constBuffB1_->GetGPUVirtualAddress());
	//SRV�q�[�v�̐擪�n���h�����擾�iSRV���w���Ă���͂��j
	D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = Texture::sDescHeap->GetGPUDescriptorHandleForHeapStart();
	UINT incrementSize = sDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	srvGpuHandle.ptr += (SIZE_T)(incrementSize * textureIndex_);
	cmdList->SetGraphicsRootDescriptorTable(4, srvGpuHandle);
	// �`��R�}���h
	cmdList->DrawIndexedInstanced((uint32_t)indices_.size(), 1, 0, 0, 0);
}

void Model::Draw(ID3D12GraphicsCommandList* cmdList, uint32_t rootParamIndexMaterial, uint32_t textureHandle)
{
	// ���_�o�b�t�@�̐ݒ�
	cmdList->IASetVertexBuffers(0, 1, &vbView_);
	// �C���f�b�N�X�o�b�t�@�̐ݒ�
	cmdList->IASetIndexBuffer(&ibView_);

	// �f�X�N���v�^�q�[�v�̔z��
	ID3D12DescriptorHeap* ppHeaps[] = { Texture::sDescHeap.Get() };
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	// �萔�o�b�t�@�r���[���Z�b�g
	cmdList->SetGraphicsRootConstantBufferView(rootParamIndexMaterial, constBuffB1_->GetGPUVirtualAddress());
	//SRV�q�[�v�̐擪�n���h�����擾�iSRV���w���Ă���͂��j
	D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = Texture::sDescHeap->GetGPUDescriptorHandleForHeapStart();
	UINT incrementSize = sDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	srvGpuHandle.ptr += (SIZE_T)(incrementSize * textureHandle);
	cmdList->SetGraphicsRootDescriptorTable(4, srvGpuHandle);
	// �`��R�}���h
	cmdList->DrawIndexedInstanced((uint32_t)indices_.size(), 1, 0, 0, 0);
}

void Model::LoadFromOBJInternal(const std::string& modelname,bool smoothing)
{
	//oBJ�t�@�C������f�[�^��ǂݍ���
		//�t�@�C���X�g���[��
	std::ifstream file;
	//.obj�t�@�C�����J��
	/*file.open("Resources/triangle_tex/triangle_tex.obj");*/
	//const string modelname = "cube";
	const string filename = modelname + ".obj";//"triangle_mat.obj"
	const string directoryPath = "Resources/" + modelname + "/";//"Resources/triangle_mat/triangle_mat.obj"
	file.open(directoryPath + filename);
	//�t�@�C���I�[�v�����s���`�F�b�N
	assert(!file.fail());
	vector<Vector3>positions;	//���_���W
	vector<Vector3>normals;	//�@���x�N�g��
	vector<Vector2>texcodes;	//�e�N�X�`��

	//���_�@���X���[�W���O�p�f�[�^
	std::unordered_map<unsigned short, std::vector<unsigned short>> smoothDate;
	//1�s���ǂݍ���
	string line;
	while (getline(file, line)) {

		//�P�s���̕�������X�g���[���ɕϊ����ĉ�͂��₷������
		std::istringstream line_stream(line);

		//���p�X�y�[�X��؂�ōs�̐擪��������擾
		string key;
		getline(line_stream, key, ' ');

		//�擪������mtllib�Ȃ�}�e���A��
		if (key == "mtllib")
		{
			//�}�e���A���t�@�C�����ǂݍ���
			string filename;
			line_stream >> filename;
			//�}�e���A���ǂݍ���
			LoadMaterial(directoryPath, filename);
		}

		//�擪������v�Ȃ璸�_���W
		if (key == "v") {
			//X,Y,Z���W�ǂݍ���
			Vector3 position{};
			line_stream >> position.x;
			line_stream >> position.y;
			line_stream >> position.z;
			//���W�f�[�^�ɓ���
			positions.emplace_back(position);
			//���_�f�[�^�ɒǉ�
			/*VertexPosNormalUv vertex{};
			vertex.pos = position;
			vertices.emplace_back(vertex);*/
		}
		//�擪������vt�Ȃ�e�N�X�`��
		if (key == "vt") {
			//U.V�����ǂݍ���
			Vector2 texcord{};
			line_stream >> texcord.x;
			line_stream >> texcord.y;
			//V�������]
			texcord.y = 1.0f - texcord.y;
			//�e�N�X�`�����W�f�[�^�ɒǉ�
			texcodes.emplace_back(texcord);
		}
		//�擪������vn�Ȃ�@���x�N�g��
		if (key == "vn") {
			//X,Y,Z�����ǂݍ���
			Vector3 normal{};
			line_stream >> normal.x;
			line_stream >> normal.y;
			line_stream >> normal.z;
			//�@���x�N�g���f�[�^�ɒǉ�
			normals.emplace_back(normal);
		}

		//�擪������f�Ȃ�|���S���i�O�p�`�j
		if (key == "f") {
			//���p�X�y�[�X��؂�ōs�̑�����ǂݍ���
			string index_string;
			while (getline(line_stream, index_string, ' ')) {
				//���_�C���f�b�N�X�P���̕�������X�g���[�����ϊ����ĉ�͂��₷������
				std::istringstream index_stream(index_string);
				unsigned short indexPosition, indexNormal, indexTexcord;
				index_stream >> indexPosition;
				//�ǉ�
				index_stream.seekg(1, ios_base::cur);
				index_stream >> indexTexcord;
				index_stream.seekg(1, ios_base::cur);
				index_stream >> indexNormal;
				//���_�f�[�^�̒ǉ�
				VertexPosNormalUv vertex{};
				vertex.pos = positions[indexPosition - 1];
				vertex.normal = normals[indexNormal - 1];
				vertex.uv = texcodes[indexTexcord - 1];
				vertices_.emplace_back(vertex);
				smoothDate[indexPosition].emplace_back(static_cast<unsigned short>(vertices_.size() - 1));
				//�C���f�b�N�X�f�[�^�̒ǉ�
				indices_.emplace_back((unsigned short)indices_.size());

				////���_�C���f�b�N�X�ɒǉ�
				//indices.emplace_back(indexPosition - 1);

			}
		}
	}
	//�t�@�C�������
	file.close();
	if (smoothing == true)
	{
		auto itr = smoothDate.begin();
		for (; itr != smoothDate.end(); ++itr) {
			//�e�ʗp�̋��ʒ��_�R���N�V����
			std::vector<unsigned short>& v = itr->second;
			//�S���_�̖@���𕽋ς���
			Vector3 normal = {0,0,0};
			for (unsigned short index : v) {
				normal += {vertices_[index].normal.x, vertices_[index].normal.y, vertices_[index].normal.z};
			}
			normal=(normal / (float)v.size());
			normal = normal.normalize();
			//���ʖ@�����g�p����S�Ă̒��_�f�[�^�ɏ�������
			for (unsigned short index : v) {
				vertices_[index].normal = { normal.x,normal.y,normal.z };
			}
		}
	}
}

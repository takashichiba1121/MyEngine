#include "assimpLoader.h"
#include<assimp/Importer.hpp>
#include<assimp/postprocess.h>
#include<assimp/cimport.h>
#include<d3d12.h>
#include<filesystem>
#include<assert.h>
#include"Texture.h"
#include"assimpModel.h"

AssimpModel* AssimpLoader::Load(const std::string& modelname)
{
	// ���f���Ɠ������O�̃t�H���_����ǂݍ���
	const string directoryPath = "Resources/" + modelname + "/";
	// �g���q.fbx��t��
	const string fileName = modelname + ".fbx";
	// �A�����ăt���p�X�𓾂�
	const string fullpath = directoryPath + fileName;

	Assimp::Importer importer;

	uint32_t flag = 0;
	flag =
		aiProcess_Triangulate | //�O�p�ʉ�
		aiProcess_CalcTangentSpace | //�ڐ��x�N�g������
		aiProcess_GenSmoothNormals | //�X���[�W���O�x�N�g������
		aiProcess_GenUVCoords | //��}�b�s���O��K�؂�UV���W�ɕϊ�
		aiProcess_RemoveRedundantMaterials | //�璷�ȃ}�e���A�����폜
		aiProcess_OptimizeMeshes | //���b�V�������œK��
		aiProcess_MakeLeftHanded | //�m�[�h��������W�n��
		aiProcess_GenBoundingBoxes | //AABB�𐶐�
		aiProcess_JoinIdenticalVertices |//�C���f�b�N�X�𐶐�
		aiProcess_FlipUVs|//uv��y���}�C�i�X�ɂ���(�I�[�v��GL�p�̂�DirectX12�p��)
		aiProcess_LimitBoneWeights;//�e���_���e�����󂯂�{�[����4�ɐ���

	const aiScene* scene = importer.ReadFile(fullpath.c_str(), flag);

	// �t�@�C�������w�肵��FBX�t�@�C����ǂݍ���
	if (!scene) {
		assert(0);
	}

	std::vector<std::unique_ptr<AssimpModel::Mesh>> meshs;

	AssimpModel* model = new AssimpModel;

	LodeNode(scene->mRootNode,scene, directoryPath,&meshs);

	for (std::unique_ptr<AssimpModel::Mesh>& mesh:meshs)
	{
		model->SetMesh(std::move(mesh));
	}
	
	// FBX�V�[�����
	//aiReleaseImport(scene);

	return model;
}

AssimpModel::Mesh* AssimpLoader::LoadMesh(const aiMesh* src, const aiScene* scene, const string& directoryPath)
{
	aiVector3D zero3D(0.0f,0.0f,0.0f);

	aiColor4D zeroColor(0.0f,0.0f,0.0f,0.0f);

	AssimpModel::Mesh* result=new AssimpModel::Mesh;

	result->vertexs.resize(src->mNumVertices);
	for (uint32_t i = 0; i < src->mNumVertices; ++i)
	{
		aiVector3D* position = &(src->mVertices[i]);
		aiVector3D* normal=&(src->mNormals[i]);
		aiVector3D* uv = (src->HasTextureCoords(0)) ? &(src->mTextureCoords[0][i]) : &zero3D;
		aiVector3D* tangent = (src->HasTangentsAndBitangents()) ? &(src->mTangents[i]) : &zero3D;
		aiColor4D* color = (src->HasVertexColors(0)) ? &(src->mColors[0][i]) : &zeroColor;

		AssimpModel::VertexPosNormalUv vertex = {};

		vertex.pos = Vector3(position->x, position->y, position->z);
		vertex.normal = Vector3(normal->x, normal->y, normal->z);
		vertex.uv = Vector2(uv->x,uv->y);

		result->vertexs[i] = vertex;
	 }
	result->indices.resize(src->mNumFaces*3);

	for(uint32_t i=0;i<src->mNumFaces;++i)
	{
		const aiFace face = src->mFaces[i];

		result->indices[i * 3 + 0] = face.mIndices[0];
		result->indices[i * 3 + 1] = face.mIndices[1];
		result->indices[i * 3 + 2] = face.mIndices[2];
	}

	if (src->mMaterialIndex >= 0) {
		aiMaterial* material = scene->mMaterials[src->mMaterialIndex];

		AssimpModel::Material diffuseMaps = LoadMaterial(material, directoryPath);
		result->materials = diffuseMaps;
	}


	return result;
}

uint32_t AssimpLoader::LoadTexture(const std::string filename, const string& directoryPath)
{
	HRESULT result = S_FALSE;

	//�t�@�C���p�X������
	string filepath = directoryPath + filename;

	//���j�R�[�h������ɕϊ�����
	wchar_t wfilepath[128];
	uint32_t iBufferSize = MultiByteToWideChar(CP_ACP, 0, filepath.c_str(), -1, wfilepath, _countof(wfilepath));

	return Texture::LoadTexture(wfilepath);
}

AssimpModel::Material AssimpLoader::LoadMaterial(const aiMaterial* material, const string& directoryPath)
{
	AssimpModel::Material result;

	aiColor3D ambient(0.3f, 0.3f, 0.3f);
	material->Get(AI_MATKEY_COLOR_AMBIENT, ambient);
	result.ambient = Vector3(ambient.r, ambient.g, ambient.b);

	aiColor3D diffuse(0.0f, 0.0f, 0.0f);
	material->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse);
	result.diffuse = Vector3(diffuse.r, diffuse.g, diffuse.b);

	aiColor3D specular(0.0f, 0.0f, 0.0f);
	material->Get(AI_MATKEY_COLOR_SPECULAR, specular);
	result.specular = Vector3(specular.r, specular.g, specular.b);

	aiString str;
	material->Get(AI_MATKEY_NAME, str);

	material->GetTexture(aiTextureType_DIFFUSE, 0, &str);

	const string texName = str.C_Str();

	result.texHandle=LoadTexture(texName,directoryPath);


	return result;
}

void AssimpLoader::LodeNode(aiNode* node, const aiScene* scene, const string& directoryPath, std::vector<std::unique_ptr<AssimpModel::Mesh>>* meshs)
{
	for (uint32_t i = 0; i < node->mNumMeshes; i++) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		std::unique_ptr<AssimpModel::Mesh> modelmesh;

		modelmesh.reset(LoadMesh(mesh, scene, directoryPath));
		meshs->push_back(std::move(modelmesh));
	}

	for (uint32_t i = 0; i < node->mNumChildren; i++) {
		LodeNode(node->mChildren[i], scene, directoryPath,meshs);
	}
}

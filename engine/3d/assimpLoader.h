#pragma once
#include <d3d12.h>
#include <string>
#include<vector>
#include<assimp/\material.h>
#include<assimp/mesh.h>
#include<assimp/scene.h>
#include"assimpModel.h"
/*
* FBXモデルのロード
*/
class AssimpLoader
{
#pragma region Singleton
private:
	AssimpLoader() {};

	~AssimpLoader() {};
public:
	AssimpLoader(const AssimpLoader& carManager) = delete;

	AssimpLoader& operator=(const AssimpLoader& carManager) = delete;

	static AssimpLoader* Instance();
#pragma endregion

public:
	/// <summary>
	/// assimpを用いたfbxファイルの読み込み
	/// </summary>
	/// <param name="modelname"></param>
	/// <returns></returns>
	AssimpModel* Load(const std::string& modelname);

private:
	/// <summary>
	/// メッシュの読み込み
	/// </summary>
	AssimpModel::Mesh* LoadMesh(const aiMesh* src, const aiScene* scene, const std::string& directoryPath);
		/// <summary>
	/// テクスチャの読み込み
	/// </summary>
	uint32_t LoadTexture(const std::string filename, const std::string& directoryPath);
		/// <summary>
	/// マテリアルの読み込み
	/// </summary>
	AssimpModel::Material LoadMaterial(const aiMaterial* material, const std::string& directoryPath);
		/// <summary>
	/// ノードの読み込み
	/// </summary>
	void LodeNode(aiNode* node, const aiScene* scene, const std::string& directoryPath, std::vector<std::unique_ptr<AssimpModel::Mesh>>* meshs); 
private:
};


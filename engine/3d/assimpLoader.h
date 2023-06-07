#pragma once
#include <d3d12.h>
#include <string>
#include<vector>
#include<assimp/\material.h>
#include<assimp/mesh.h>
#include<assimp/scene.h>
#include"assimpModel.h"



class AssimpLoader
{
public:
	static std::vector<AssimpModel::Mesh*> Load(const std::string& modelname);

private:

	static AssimpModel::Mesh* LoadMesh(const aiMesh* src, const aiScene* scene, const std::string directoryPath);

	static uint32_t LoadTexture(const std::string filename, const std::string directoryPath);

	static AssimpModel::Material LoadMaterial(const aiMaterial* material, const std::string directoryPath);

	static void LodeNode(aiNode* node, const aiScene* scene, const std::string directoryPath, std::vector<AssimpModel::Mesh*>* meshs);
private:
};


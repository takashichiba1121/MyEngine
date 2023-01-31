#pragma once
#include <cassert>
#include <string>
#include <Windows.h>
#include <fstream>
#include <sstream>
#include <DirectXMath.h>
#include <vector>
#include <d3d12.h>
#include <d3dx12.h>
#include <DirectXTex.h>
#include"Vector2.h"
#include"Vector3.h"
#include"Vector4.h"
#include"Matrix4.h"

using namespace std;

using namespace DirectX;

//3D���f��
class Model 
{
private:
	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
public:
	// �萔�o�b�t�@�p�f�[�^�\����B1
	struct ConstBufferDataB1
	{
		Vector3 ambient;	//�A���r�G���g�W��
		float pad1;			//�p�f�B���O
		Vector3 diffuse;	//�f�B�t���[�Y�W��
		float pad2;			//�p�f�B���O
		Vector3 specular;	//�X�y�L�����[�W��
		float alpha;		//�A���t�@
	};
	// ���_�f�[�^�\����
	struct VertexPosNormalUv
	{
		Vector3 pos; // xyz���W
		Vector3 normal; // �@���x�N�g��
		Vector2 uv;  // uv���W
	};
	//�}�e���A��
	struct Material {
		std::string name;	//�}�e���A����
		Vector3 ambient;	//�A���r�G���g�e���x
		Vector3 diffuse;	//�f�B�t���[�Y�e���x
		Vector3 specular;	//�X�y�L�����[�e���x
		float alpha;		//�A���t�@
		std::string textureFilename;	//�e�N�X�`���t�@�C����
		//�R���X�g���N�^
		Material() {
			ambient = { 0.3f,0.3f,0.3f };
			diffuse = { 0.0f,0.0f,0.0f };
			specular = { 0.0f,0.0f,0.0f };
			alpha = 1.0f;
		}
	};
public:
	//OBJ�t�@�C������3D���f����ǂݍ���
	static Model* LoadFormOBJ(const std::string& modelname);
	/// <summary>
	/// �}�e���A���̓ǂݍ���
	/// </summary>
	void LoadMaterial(const std::string& directoryPath, const std::string& filename);

	//setter
	static void SetDevice(ID3D12Device* device) { Model::device = device; }

	//�e��o�b�t�@�̐���
	void CreateBuffers();

	/// <summary>
	/// �`��
	/// </summary>
	/// <param name="cmdlist">�`��R�}���h���X�g</param>
	/// <param name="rootParamIndexMaterial">�}�e���A���p���[�g�p�����[�^�ԍ�</param>
	void Draw(ID3D12GraphicsCommandList* cmdList, UINT rootParamIndexMaterial);

private://����J�̃����o�֐�
	//OBJ�t�@�C������3D���f����ǂݍ���(����J)
	void LoadFromOBJInternal(const std::string& modelname);
	
	void LoadTexture(const std::string& directoryPath, const std::string& filename);

private:
	//�f�o�C�X
	static ID3D12Device* device;
	// ���_�o�b�t�@
	ComPtr<ID3D12Resource> vertBuff;
	// ���_�o�b�t�@�r���[
	D3D12_VERTEX_BUFFER_VIEW vbView;
	// �C���f�b�N�X�o�b�t�@
	ComPtr<ID3D12Resource> indexBuff;
	// �C���f�b�N�X�o�b�t�@�r���[
	D3D12_INDEX_BUFFER_VIEW ibView;
	// ���_�f�[�^�z��
	std::vector<VertexPosNormalUv> vertices;
	// ���_�C���f�b�N�X�z��
	std::vector<unsigned short>indices;
	//�}�e���A��
	Material material;
	// �V�F�[�_���\�[�X�r���[�̃n���h��(CPU)
	CD3DX12_CPU_DESCRIPTOR_HANDLE cpuDescHandleSRV;
	// �V�F�[�_���\�[�X�r���[�̃n���h��(CPU)
	CD3DX12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV;
	// �萔�o�b�t�@(�}�e���A��)
	ComPtr<ID3D12Resource> constBuffB1;

	uint32_t textureIndex;
};
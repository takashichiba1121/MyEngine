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

class AssimpModel
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
		uint32_t  texHandle;	//�e�N�X�`���t�@�C����
		//�R���X�g���N�^
		Material() {
			ambient = { 0.3f,0.3f,0.3f };
			diffuse = { 0.0f,0.0f,0.0f };
			specular = { 0.0f,0.0f,0.0f };
			alpha = 1.0f;
			texHandle = 0;
		}
	};

	struct Mesh {
		std::vector<VertexPosNormalUv> vertexs;
		std::vector<unsigned short> indices;
		Material materials;

		// ���_�o�b�t�@
		ComPtr<ID3D12Resource> vertBuff;
		// ���_�o�b�t�@�r���[
		D3D12_VERTEX_BUFFER_VIEW vbView;
		// �C���f�b�N�X�o�b�t�@
		ComPtr<ID3D12Resource> indexBuff;
		// �C���f�b�N�X�o�b�t�@�r���[
		D3D12_INDEX_BUFFER_VIEW ibView;
		// �萔�o�b�t�@(�}�e���A��)
		ComPtr<ID3D12Resource> constBuffB1;
	};
public:

	~AssimpModel();
	void CreateBuffers();

	//OBJ�t�@�C������3D���f����ǂݍ���
	static AssimpModel* LoadFormFBX(const std::string& modelname);

	//setter
	static void SetDevice(ID3D12Device* device) { sDevice = device; }

	void Draw(ID3D12GraphicsCommandList* cmdList, uint32_t rootParamIndexMaterial);

	void SetMesh(std::unique_ptr<AssimpModel::Mesh> Meshs) { meshs_.push_back(std::move(Meshs)); }

private:
	std::vector<std::unique_ptr<AssimpModel::Mesh>> meshs_;

	//�f�o�C�X
	static ID3D12Device* sDevice;
	// �V�F�[�_���\�[�X�r���[�̃n���h��(CPU)
	CD3DX12_CPU_DESCRIPTOR_HANDLE cpuDescHandleSRV_;
	// �V�F�[�_���\�[�X�r���[�̃n���h��(CPU)
	CD3DX12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV_;
	// �萔�o�b�t�@(�}�e���A��)
	ComPtr<ID3D12Resource> constBuffB1_;
};
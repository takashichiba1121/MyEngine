#pragma once

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>
#include "Model.h"
#include"object3d.h"
#include<list>
class PolygonMana
{
	private: // �G�C���A�X
		// Microsoft::WRL::���ȗ�
		template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	public: // �T�u�N���X\

		struct VertexPos
		{
			Vector3 pos; // xyz���W
			Vector4 color;
		};

		// �萔�o�b�t�@�p�f�[�^�\����B0
		struct ConstBufferDataB0
		{
			//Vector4 color;	// �F (RGBA)
			Matrix4 mat;	// �R�c�ϊ��s��
		};

		//�p�[�e�B�N���ꗱ
		struct polygon
		{
			//���W
			Vector3 position[3] = {};
			// ���x
			Vector3 velocity = {};
			// �����x
			Vector3 accel = {};
			// ���݃t���[��
			int frame = 0;
			//�I���t���[��
			int num_frame = 0;

			Vector4 color = {};

			Vector4 s_color = {};

			Vector4 e_color = {};
		};


	public: // �ÓI�����o�֐�
		/// <summary>
		/// �ÓI������
		/// </summary>
		/// <param name="device">�f�o�C�X</param>
		/// <param name="window_width">��ʕ�</param>
		/// <param name="window_height">��ʍ���</param>
		static void StaticInitialize(ID3D12Device* device, int window_width, int window_height);

	private: // �ÓI�����o�ϐ�
		// �f�o�C�X
		static ID3D12Device* device;
		// �R�}���h���X�g
		static ID3D12GraphicsCommandList* cmdList;
		// ���[�g�V�O�l�`��
		static ComPtr<ID3D12RootSignature> rootsignature;
		// �p�C�v���C���X�e�[�g�I�u�W�F�N�g
		static ComPtr<ID3D12PipelineState> pipelinestate;
		// �r���[�s��
		static Matrix4 matViewPro;

	private:// �ÓI�����o�֐�

		/// <summary>
		/// �O���t�B�b�N�p�C�v���C������
		/// </summary>
		/// <returns>����</returns>
		static void InitializeGraphicsPipeline();

		/// <summary>
		/// �r���[�s����X�V
		/// </summary>
		static void UpdateViewMatrix();

	public: // �����o�֐�
		bool Initialize();
		/// <summary>
		/// ���t���[������
		/// </summary>
		void Update();

		/// <summary>
		/// �`��
		/// </summary>
		void Draw();

		///<summary>
	///�p�[�e�B�N���̒ǉ�
	///</summary>
	///<param name="life">��������</param>
	///<param name="position">�������W</param>
	/// <param name="velocity">	���x</param>
	/// <param name="accel">�����x</param>
		void Add(int life, Vector3 position, Vector3 position2, Vector3 position3, Vector3 velocity, Vector3 accel, Vector4 startColor, Vector4 endColor,Matrix4 matworld);

		void Addobj(Object3d* obj,Vector4 startColor,Vector4 endColor);

	private: // �����o�ϐ�
		ComPtr<ID3D12Resource> constBuffB0; // �萔�o�b�t�@
		
		Matrix4 mat;

		//�p�[�e�B�N���z��
		std::list<polygon>Polygons;

		Matrix4 matWorld;

		// ���_�o�b�t�@
		ComPtr<ID3D12Resource> vertBuff;
		// ���_�o�b�t�@�r���[
		D3D12_VERTEX_BUFFER_VIEW vbView;
};


#pragma once

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>
#include "Model.h"
#include"assimpModel.h"


/// <summary>
/// 3D�I�u�W�F�N�g
/// </summary>
class assimpObject3d
{

	private: // �G�C���A�X
		// Microsoft::WRL::���ȗ�
		template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	public: // �T�u�N���X
		// ���_�f�[�^�\����
		struct VertexPosNormalUv
		{
			Vector3 pos; // xyz���W
			Vector3 normal; // �@���x�N�g��
			Vector2 uv;  // uv���W
		};

		// �萔�o�b�t�@�p�f�[�^�\����B0
		struct ConstBufferDataB0
		{
			//XMFLOAT4 color;	// �F (RGBA)
			Matrix4 mat;	// �R�c�ϊ��s��
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
		// �萔�o�b�t�@�p�f�[�^�\����
		struct ConstBufferPolygonExplosion
		{
			float _Destruction = 0.0f;
			float _ScaleFactor = 1.0f;
			float _RotationFactor = 0.0f;
			float _PositionFactor = 0.0f;
			float _AlphaFactor = 1.0f;
		};

	public: // �ÓI�����o�֐�
		/// <summary>
		/// �ÓI������
		/// </summary>
		/// <param name="device">�f�o�C�X</param>
		/// <param name="window_width">��ʕ�</param>
		/// <param name="window_height">��ʍ���</param>
		static void StaticInitialize(ID3D12Device* device, uint32_t window_width, uint32_t window_height);

		/// <summary>
		/// �`��O����
		/// </summary>
		/// <param name="cmdList">�`��R�}���h���X�g</param>
		static void PreDraw(ID3D12GraphicsCommandList* cmdList);

		/// <summary>
		/// �`��㏈��
		/// </summary>
		static void PostDraw();

		/// <summary>
		/// ���_���W�̎擾
		/// </summary>
		/// <returns>���W</returns>
		static const Vector3& GetEye() { return sEye; }

		/// <summary>
		/// ���_���W�̐ݒ�
		/// </summary>
		/// <param name="position">���W</param>
		static void SetEye(Vector3 eye);

		/// <summary>
		/// �����_���W�̎擾
		/// </summary>
		/// <returns>���W</returns>
		static const Vector3& GetTarget() { return sTarget; }

		/// <summary>
		/// �����_���W�̐ݒ�
		/// </summary>
		/// <param name="position">���W</param>
		static void SetTarget(Vector3 target);

		static Matrix4 GetMatViewPro();

	private: // �ÓI�����o�ϐ�
		// �f�o�C�X
		static ID3D12Device* sDevice;
		// �R�}���h���X�g
		static ID3D12GraphicsCommandList* sCmdList;
		// ���[�g�V�O�l�`��
		static ComPtr<ID3D12RootSignature> sRootsignature;
		// �p�C�v���C���X�e�[�g�I�u�W�F�N�g
		static ComPtr<ID3D12PipelineState> sPipelinestate;
		// �r���[�s��
		static Matrix4 sMatView;
		// �ˉe�s��
		static Matrix4 sMatProjection;
		// ���_���W
		static Vector3 sEye;
		// �����_���W
		static Vector3 sTarget;
		// ������x�N�g��
		static Vector3 sUp;

	private:// �ÓI�����o�֐�

		/// <summary>
		/// �J����������
		/// </summary>
		/// <param name="window_width">��ʉ���</param>
		/// <param name="window_height">��ʏc��</param>
		static void InitializeCamera(uint32_t window_width, uint32_t window_height);

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

		/// <summary>
		/// ���W�̎擾
		/// </summary>
		/// <returns>���W</returns>
		const Vector3& GetPosition() const { return position; }

		/// <summary>
		/// ���W�̐ݒ�
		/// </summary>
		/// <param name="position">���W</param>
		void SetPosition(const Vector3& position) { this->position = position; }

		/// <summary>
		/// ���W�̎擾
		/// </summary>
		/// <returns>���W</returns>
		const Vector3& GetScale() const { return scale; }

		/// <summary>
		/// ���W�̐ݒ�
		/// </summary>
		/// <param name="position">���W</param>
		void SetScale(const Vector3& scale) { this->scale = scale; }

		/// <summary>
		/// ���W�̎擾
		/// </summary>
		/// <returns>���W</returns>
		const Vector3& GetRot() const { return rotation; }

		/// <summary>
		/// ���W�̐ݒ�
		/// </summary>
		/// <param name="position">���W</param>
		void SetRot(const Vector3& rot) { this->rotation = rot; }

		//setter
		void SetModel(AssimpModel* model) { this->model = model; }

		const ConstBufferPolygonExplosion GetPolygonExplosion() { return *ConstMapPolygon; }

		const void SetPolygonExplosion(const ConstBufferPolygonExplosion& polygonExplosion) { *ConstMapPolygon = polygonExplosion; }

	private: // �����o�ϐ�
		ComPtr<ID3D12Resource> constBuffB0; // �萔�o�b�t�@
		ComPtr<ID3D12Resource> constBuffB1; // �萔�o�b�t�@
		ComPtr<ID3D12Resource> constBuffB2; // �萔�o�b�t�@
		//// �F
		//Vector4 color = { 1,1,1,1 };
		// ���[�J���X�P�[��
		Vector3 scale = { 1,1,1 };
		// X,Y,Z�����̃��[�J����]�p
		Vector3 rotation = { 0,0,0 };
		// ���[�J�����W
		Vector3 position = { 0,0,0 };
		// ���[�J�����[���h�ϊ��s��
		Matrix4 matWorld;
		// �e�I�u�W�F�N�g
		assimpObject3d* parent = nullptr;

		ConstBufferPolygonExplosion* ConstMapPolygon;

		//���f��
		AssimpModel* model = nullptr;


};


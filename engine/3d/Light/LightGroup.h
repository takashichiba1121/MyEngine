#pragma once
#include"DirectionalLight.h"
#include"PointLight.h"
class LightGroup
{
private: // �G�C���A�X
	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
public://�萔
	static const uint32_t cDirLightNum = 3;

	static const uint32_t cPointLightNum = 3;

public://�T�u�N���X
	//�萔�o�b�t�@�p�f�[�^�\����
	struct ConstBufferData
	{
		//�����̐F
		Vector3 ambientColor;
		float pad1;
		//���s�����p
		DirectionalLight::ConstBufferData dirLights[cDirLightNum];

		PointLight::ConstBufferData pointLight[cDirLightNum];
	};

private:
	static ID3D12Device* sDevice;
public:
	static void StaticInitialize(ID3D12Device* device);
private:
	ComPtr<ID3D12Resource> constBuff_;

	Vector3 ambientColor_ = { 1,1,1 };

	DirectionalLight dirLights_[cDirLightNum];

	PointLight pointLight_[cDirLightNum];

	bool dirty_ = false;
public:
	/// <summary>
	/// �C���X�^���X����
	/// </summary>
	/// <returns></returns>
	static LightGroup* Create();

	/// <summary>
	/// ������
	/// </summary>
	void Initialize();

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw(ID3D12GraphicsCommandList* cmdList, UINT rootParameterIndex);

	/// <summary>
	/// �萔�o�b�t�@�Ƀf�[�^�𑗂�
	/// </summary>
	void TransferConstBuffer();

	/// <summary>
	/// �����̃��C�g�F���Z�b�g
	/// </summary>
	/// <param name="color">���C�g�F</param>
	void SetAmbientColor(const Vector3& color);

	/// <summary>
	/// ���s�����̗L���t���O���Z�b�g
	/// </summary>
	/// <param name="index">���C�g�ԍ�</param>
	/// <param name="active">�L���t���O</param>
	void SetDirLightActive(uint32_t index, bool active);

	/// <summary>
	/// ���s�����̃��C�g�̕����ݒ�
	/// </summary>
	/// <param name="index">���C�g�ԍ�</param>
	/// <param name="lightdir">���C�g����</param>
	void SetDirLightDir(uint32_t index, const Vector3& lightdir);

	/// <summary>
	/// ���s�����̃��C�g�F���Z�b�g
	/// </summary>
	/// <param name="index">���C�g�ԍ�</param>
	/// <param name="lightcolor">���C�g�F</param>
	void SetDirLightColor(uint32_t index, const Vector3& lightcolor);

	/// <summary>
/// �_�����̗L���t���O���Z�b�g
/// </summary>
/// <param name="index">���C�g�ԍ�</param>
/// <param name="active">�L���t���O</param>
	void SetPointActive(uint32_t index, bool active);

	/// <summary>
	/// �_�����̃��C�g�̕����ݒ�
	/// </summary>
	/// <param name="index">���C�g�ԍ�</param>
	/// <param name="lightdir">���C�g����</param>
	void SetPointPos(uint32_t index, const Vector3& lightPos);

	/// <summary>
	/// �_�����̃��C�g�F���Z�b�g
	/// </summary>
	/// <param name="index">���C�g�ԍ�</param>
	/// <param name="lightcolor">���C�g�F</param>
	void SetPointColor(uint32_t index, const Vector3& lightcolor);

	/// <summary>
/// �_�����̃��C�g�F���Z�b�g
/// </summary>
/// <param name="index">���C�g�ԍ�</param>
/// <param name="lightcolor">���C�g�F</param>
	void SetPointAtten(uint32_t index, const Vector3& lightAtten);

	/// <summary>
	/// �W���̃��C�g�ݒ�
	/// </summary>
	void DefaultLightSetting();
};
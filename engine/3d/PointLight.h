#pragma once
#include"Vector3.h"
#include"DirectXCommon.h"
class DirectionalLight
{
private: // �G�C���A�X
public:
	struct  ConstBufferData
	{
		Vector3 lightv;
		float shininess;
		Vector3 lightcolor;
		uint16_t active;
	};
private:

	Vector3 lightDir_ = { 1,0,0 };

	Vector3 lightColor_ = { 1,1,1 };

	float shininess_ = 4;

	//�L���t���O
	bool active_ = false;
public:

	void SetLightDir(const Vector3& lightDir);

	void SetLightColor(const Vector3& lightColor);

	Vector3 GetLightDir() { return lightDir_; }

	Vector3 GetLightColor() { return lightColor_; }

	void SetShininess(const float shininess);

	float GetShininess() { return shininess_; }

	/// <summary>
	/// �L���t���O���Z�b�g
	/// </summary>
	/// <param name="active">�L���t���O</param>
	inline void SetActive(bool active) { this->active_ = active; }

	/// <summary>
	/// �L���`�F�b�N
	/// </summary>
	/// <returns>�L���t���O</returns>
	inline bool IsActive() { return active_; }

};

#pragma once
#include"object3d.h"

class PlayerBullet
{
public:
	
	///<summary>
	///����������
	///</summary>
	///<param name="model">���f��</param>
	///<param name="velocity">���x</param>
	///<param name="position">�����ʒu</param>
	void Initialize(Model* model,Vector2 velocity,Vector3 position);
	///<summary>
	///���t���[������
	///</summary>
	void Update();

	///<summary>
	///�`��
	///</summary>
	void Draw();
	
	///<summary>
	///�Փ˂����o������Ăяo���ꂽ��R�[���o�b�N�֐�
	///</summary>
	void OnCollision();

/// <summary>
/// ������ׂ���
/// </summary>
	bool IsDead() {return isDead_ ; }

private:

	std::unique_ptr<Object3d> obj_;

	//���x
	Vector2 velocity_;
	//����<frm>
	static const int32_t kLifeTime = 60;
	//�f�X�^�C�}�[
	int32_t deathTimer_ = kLifeTime;
	//�f�X�t���O
	bool isDead_ = false;
};
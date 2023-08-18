#pragma once
#include"object3d.h"

class EnemyBullet
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

	Vector3 GetPosition() {return obj_->GetPosition() ; }

	Vector3 GetScale() { return obj_->GetScale(); }

private:

	std::unique_ptr<Object3d> obj_;

	//���x
	Vector2 velocity_;
	//����<frm>
	static const uint32_t kLifeTime = 60;
	//�f�X�^�C�}�[
	uint32_t deathTimer_ = kLifeTime;
	//�f�X�t���O
	bool isDead_ = false;
};
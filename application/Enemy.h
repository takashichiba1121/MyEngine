#pragma once
#include"object3d.h"
#include"EnemyBullet .h"
#include"Player.h"
class Enemy
{
public:
	///<summary>
	///�������S��
	///</summary>
	///<param name="model">���f��</param>
	///<param name="bulletModel">�e�̃��f��</param>
	///<param name="position">�ʒu</param>
	void Initialize(Model* bulletModel, Vector3 position,Object3d* playerObj);
	/// <summary>
	/// ���t���[������
	/// </summary>
	void Update();

	/// <summary>
	/// �U���S��
	/// </summary>
	void Attack();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	bool IsDaed() { return isDaed_; }

	Object3d* GetEnemyObj() { return obj_.get(); }

	void OnCollision();
private:
	std::unique_ptr<Model> model;

	Model* bulletModel_;

	std::unique_ptr<Object3d> obj_;

	bool isDaed_=false;

	//�U���Ԋu
	static const uint32_t kAttackTime = 300;

	uint32_t attackTimer_ = kAttackTime;

	Object3d* playerObj_;
};

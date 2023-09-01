#pragma once
#include"object3d.h"
#include"EnemyBullet .h"
#include"Player.h"
class Enemy
{
public:
	///<summary>
	///‰Šú‰»‘S”Ê
	///</summary>
	///<param name="model">ƒ‚ƒfƒ‹</param>
	///<param name="bulletModel">’e‚Ìƒ‚ƒfƒ‹</param>
	///<param name="position">ˆÊ’u</param>
	void Initialize(Model* bulletModel, Vector3 position,Object3d* playerObj);
	/// <summary>
	/// –ˆƒtƒŒ[ƒ€ˆ—
	/// </summary>
	void Update();

	/// <summary>
	/// UŒ‚‘S”Ê
	/// </summary>
	void Attack();

	/// <summary>
	/// •`‰æ
	/// </summary>
	void Draw();

	bool IsDaed() { return isDaed_; }

	Object3d* GetObj() { return obj_.get(); }

	void OnCollision();

	void SetIsAttack(bool isAttack) { isAttack_ = isAttack; }
private:
	std::unique_ptr<Model> model;

	Model* bulletModel_;

	std::unique_ptr<Object3d> obj_;

	bool isDaed_=false;

	//UŒ‚ŠÔŠu
	static const uint32_t kAttackTime = 300;

	uint32_t attackTimer_ = kAttackTime;

	Object3d* playerObj_;

	bool isAttack_=false;
};

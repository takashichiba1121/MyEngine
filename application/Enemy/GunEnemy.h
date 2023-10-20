#pragma once
#include "Enemy.h"
class GunEnemy :
	public Enemy
{

public:


	void Update(float attackRange) override;

	void Move() override;

	/// <summary>
/// 攻撃全般
/// </summary>
	void Attack();

	void SetBulletModel();

private:
	Model* bulletModel_;

	const uint32_t kAttackTime_ = 120;
};


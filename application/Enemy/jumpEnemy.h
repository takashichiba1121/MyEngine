#pragma once
#include "Enemy.h"
class jumpEnemy :
    public Enemy
{
public:


	void Update() override;

	void Move() override;

		/// <summary>
/// 攻撃全般
/// </summary>
	void Attack();

	void AttackOff() override;

private:

	float attackRange_ = 40;

	uint32_t kAttackTimer_ = 90;

	uint32_t attackTimer_ = kAttackTimer_;

	Vector3 attackVec;

	float attackSpeed_ = 0.7f;

	float moveSpeed_ = 0.2f;

	Vector3 attackStartPoint = {0,0,0};

	Vector3 attackMiddlePoint = {0,0,0};

	Vector3 attackEndPoint = {0,0,0};

	float attackStartRot = 0;

	float attackEndRot = 0;
};


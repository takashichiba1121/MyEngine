#pragma once
#include "Enemy.h"
class GunEnemy :
	public Enemy
{

public:

	void Initialize(Model* enemyModel,Model* bulletModel,const Vector3& position,Player* player,EnemyType enemyType,uint32_t number) override;

	void Update() override;

	void Move() override;

	/// <summary>
/// 攻撃全般
/// </summary>
	void Attack();

	void AttackOff() override;



private:

	const uint32_t kAttackTime_ = 120;

	float attackRange_=50;

	uint32_t kAttackTimer_ = 30;

	uint32_t attackTimer_ = kAttackTimer_;

	Vector3 attackVec_;

	float attackSpeed_ = 0.5f;

	float moveSpeed_ = 0.25f;
};


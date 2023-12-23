#pragma once
#include "Enemy.h"
class jumpEnemy :
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

	void OnEnemyCollision(Vector3 reject) override;

private:

	float attackRange_ = 40;

	uint32_t kAttackTimer_ = 60;

	uint32_t attackTimer_ = kAttackTimer_;

	Vector3 attackVec_;

	float attackSpeed_ = 0.7f;

	float moveSpeed_ = 0.2f;

	Vector3 attackStartPoint_ = {0,0,0};

	Vector3 attackEndPoint_ = {0,0,0};

	float attackStartRot_ = 0;

	float attackEndRot_ = 0;

	float fallSpeed_=0;

	const float StartJumpSpeed_=-0.7f;

	const float gravityAcceleration_ = 0.03f;
};


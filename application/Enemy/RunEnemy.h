#pragma once
#include "Enemy.h"
/*
* 走るエネミーのクラス
*/
class RunEnemy :
	public Enemy
{

public:
	//初期化
	void Initialize(Model* enemyModel,Model* bulletModel,Player* player,uint32_t number) override;

	//更新
	void Update() override;

	//移動
	void Move() override;


	//攻撃全般
	void Attack();

	//攻撃の強制終了
	void AttackOff() override;

private:

	float attackRange_=40;

	uint32_t kAttackTimer_ = 70;

	uint32_t attackTimer_ = kAttackTimer_;

	Vector3 attackVec_;

	float attackSpeed_ = 0.7f;

	float moveSpeed_ =0.2f;

	float attackStartRot_ = 0;

	float attackEndRot_ = 0;
};


#pragma once
#include "Enemy.h"
/*
* 弾を打つエネミーのクラス
*/
class GunEnemy :
	public Enemy
{

public:
	//初期化
	void Initialize(Model* enemyModel,Model* bulletModel,const Vector3& position,Player* player,EnemyType enemyType,uint32_t number) override;

	//更新
	void Update() override;

	//移動
	void Move() override;

	
	// 攻撃全般
	void Attack();

	//攻撃の途中終了
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


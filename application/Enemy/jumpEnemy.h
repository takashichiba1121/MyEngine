#pragma once
#include "Enemy.h"
/*
* 跳ぶエネミーのクラス
*/
class jumpEnemy :
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

	//敵同士の衝突処理
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


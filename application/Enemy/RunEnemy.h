#pragma once
#include "Enemy.h"
class RunEnemy :
	public Enemy
{

public:


	void Update() override;

	void Move() override;

private:

	const uint32_t kAttackTime_ = 120;

	float attackRange_=40;
};


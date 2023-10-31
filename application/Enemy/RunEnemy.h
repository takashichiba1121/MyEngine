#pragma once
#include "Enemy.h"
class RunEnemy :
	public Enemy
{

public:


	void Update(float attackRange) override;

	void Move() override;

private:

	const uint32_t kAttackTime_ = 120;
};


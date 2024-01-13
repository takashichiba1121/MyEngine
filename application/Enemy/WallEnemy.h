#pragma once
#include "Enemy.h"
class WallEnemy :
    public Enemy
{
public:
		//初期化
	void Initialize(Model* enemyModel,Model* bulletModel,Player* player,uint32_t number) override;

	//更新
	void Update() override;
};


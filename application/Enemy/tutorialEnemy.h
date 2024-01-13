#pragma once
#include "Enemy.h"
/*
* チュートリアル用の敵
*/
class TutorialEnemy :
    public Enemy
{
public:
		//初期化
	void Initialize(Model* enemyModel,Model* bulletModel,Player* player,uint32_t number) override;

	//更新
	void Update() override;

private:
	bool isRevival_=false;
};


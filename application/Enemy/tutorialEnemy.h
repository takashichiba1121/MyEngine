#pragma once
#include "Enemy.h"
/*
* チュートリアル用の敵
*/
class TutorialEnemy :
    public Enemy
{
public:

	//更新
	void Update() override;

private:
	bool isRevival_=false;
};


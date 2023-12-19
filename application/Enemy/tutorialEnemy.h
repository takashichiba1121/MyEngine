#pragma once
#include "Enemy.h"
class TutorialEnemy :
    public Enemy
{
public:


	void Update() override;

private:
	bool isRevival_=false;
};


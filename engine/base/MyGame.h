#pragma once
#include"GameScene.h"
#include"Framework.h"

class MyGame : public Framework
{
public:
	//初期化
	void Initialize() override;

	void Finalize() override;

	void Update() override;

	void Draw() override;

private:
};

 
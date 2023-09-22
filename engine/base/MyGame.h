#pragma once
#include"GameScene.h"
#include"Framework.h"

class MyGame : public Framework
{
public:
	//‰Šú‰»
	void Initialize() override;

	void Finalize() override;

	void Update() override;

	void Draw() override;

private:

	std::unique_ptr<GameScene> gameScene;
};


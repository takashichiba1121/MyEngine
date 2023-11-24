#pragma once
#include"GameScene.h"
#include"Framework.h"
#include"RenderTarget.h"

class MyGame : public Framework
{
public:
	//初期化
	void Initialize() override;

	void Finalize() override;

	void Update() override;

	void Draw() override;

private:
	std::unique_ptr<RenderTarget> renderTarget1;

	std::unique_ptr<RenderTarget> renderTarget2;

	std::unique_ptr<RenderTarget> renderTarget3;

	Vector3 LuminanceColor;
	float smoothstepMax;
	float smoothstepMin;

	float sigma;
	float stepWidth;
};

 
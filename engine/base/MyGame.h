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
	std::unique_ptr<RenderTarget> renderTarget1_;

	std::unique_ptr<RenderTarget> renderTarget2_;

	std::unique_ptr<RenderTarget> renderTarget3_;

	Vector3 LuminanceColor_;
	float smoothstepMax_;
	float smoothstepMin_;

	float sigma_;
	float stepWidth_;

	bool isPostEffect_=true;
};

 
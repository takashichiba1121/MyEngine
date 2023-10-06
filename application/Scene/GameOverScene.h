#pragma once
#include"BaseScene.h"
#include"object3d.h"
#include"Sprite.h"
#include"Audio.h"
#include"ParticleManager.h"
#include"LightGroup.h"
#include"Player.h"
#include"EnemyManager.h"
class GameOverScene :public BaseScene
{

	void Initialize() override;

	void Finalize() override;

	void Update() override;

	void Draw(DirectXCommon* dxCommon) override;

private:

	uint32_t texHandle = 0;

#pragma region シーン遷移用関数

	bool sceneStart = true;

	bool sceneChange = false;

	const uint32_t endFrame = 120;

	uint32_t frame = endFrame;

	//シーン遷移のスプライトの開始地点
	const float startSpriteY = -720; 

	//シーン遷移のスプライトの終了地点
	const uint32_t endSpriteY = 0;

	//シーン遷移用のスプライト
	std::unique_ptr<Sprite> sceneSprite;

#pragma endregion

	//シーン遷移用のスプライト
	std::unique_ptr<Sprite> gameOverSprite;

	std::unique_ptr<Sprite> buttonSprite;

	std::unique_ptr<Sprite> titleSprite;

	uint32_t spaceTexHandle;
	uint32_t aTexHandle;

	std::unique_ptr<Model> skyModel_;

	std::unique_ptr<Object3d> skyObj_;

	std::unique_ptr<LightGroup> light;
};


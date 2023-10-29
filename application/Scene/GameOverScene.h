#pragma once
#include"BaseScene.h"
#include"object3d.h"
#include"Sprite.h"
#include"Sound.h"
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

	uint32_t texHandle_ = 0;

#pragma region シーン遷移用関数

	bool sceneStart_ = true;

	bool sceneChange_ = false;

	const uint32_t endFrame_ = 60;

	int32_t frame_ = 0;

	//シーン遷移のスプライトの開始地点
	const float startSpriteY_ = -720; 

	//シーン遷移のスプライトの終了地点
	const uint32_t endSpriteY_ = 0;

	//シーン遷移用のスプライト
	std::unique_ptr<Sprite> sceneSprite_;

#pragma endregion

	//シーン遷移用のスプライト
	std::unique_ptr<Sprite> gameOverSprite_;

	std::unique_ptr<Sprite> buttonSprite_;

	std::unique_ptr<Sprite> titleSprite_;

	uint32_t spaceTexHandle_;
	uint32_t aTexHandle_;

	std::unique_ptr<Model> skyModel_;

	std::unique_ptr<Object3d> skyObj_;

	std::unique_ptr<LightGroup> light_;

	std::unique_ptr<ParticleManager> particles_;

	uint32_t ParticleFlame_ = 0;
};


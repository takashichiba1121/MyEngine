#pragma once
#include"BaseScene.h"
#include"object3d.h"
#include"Sprite.h"
#include"Audio.h"
#include"ParticleManager.h"
#include"LightGroup.h"
#include"Player.h"
#include"EnemyManager.h"
class TitileScene :public BaseScene
{

	void Initialize() override;

	void Finalize() override;

	void Update() override;

	void Draw(DirectXCommon* dxCommon) override;

private:

	uint32_t texHandle = 0;

#pragma region �V�[���J�ڗp�ϐ��Q

	bool sceneStart = true;

	bool sceneChange = false;

	const uint32_t endFrame = 120;

	uint32_t frame = endFrame;

	//�V�[���J�ڂ̃X�v���C�g�̊J�n�n�_
	const float startSpriteY = -720;

	//�V�[���J�ڂ̃X�v���C�g�̏I���n�_
	const uint32_t endSpriteY = 0;

	//�V�[���J�ڗp�̃X�v���C�g
	std::unique_ptr<Sprite> sceneSprite;
	//�V�[���J�ڗp�̃X�v���C�g
	std::unique_ptr<Sprite> titleSprite;

	std::unique_ptr<Sprite> spaceSprite;

	std::unique_ptr<Sprite> startSprite;

	uint32_t spaceTexHandle;
	uint32_t aTexHandle;

#pragma endregion

	std::unique_ptr<Model> skyModel_;

	std::unique_ptr<Object3d> skyObj_;

	std::unique_ptr<LightGroup> light;
};


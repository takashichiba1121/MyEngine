#pragma once
#include"object3d.h"
#include"Sprite.h"
#include"Audio.h"
#include"ParticleManager.h"
#include"LightGroup.h"
#include"Player.h"
#include"EnemyManager.h"
class GameScene
{
	enum class Scene {
		Title,
		Game,
		Result,
	};

public: // �����o�֐�
	GameScene();

	~GameScene();

	void Initialize();
	/// <summary>
	/// ���t���[������
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw(DirectXCommon* dxCommon);

	/// <summary>
/// �`��
/// </summary>
	void PostEffectDraw(DirectXCommon* dxCommon);

	/// <summary>
	/// �}�b�v�f�[�^�̃��[�h
	/// </summary>
	void MapLoad();

	void Collision();
private:
	std::unique_ptr <Model> mapModel;
	std::unique_ptr <Model> goalModel;
	std::unique_ptr <Model> bulletModel;

	std::unique_ptr<LightGroup> light;

	uint32_t texHandle=0;

	std::map<std::string, Model*> models;
	std::vector<std::unique_ptr<Object3d>> objects;

	std::unique_ptr<Object3d> goalObj_;
	std::unique_ptr<Object3d> spawnObj_;

	float ambientColor[3] = {1,1,1};

	//��������������
	float lightPos[3] = { 0,0,0 };
	float lightColor[3] = { 1,1,1 };
	float lightAtten[3] = { 0.3f,0.1f,0.1f };

	std::unique_ptr<Player> player_;

	#pragma region �V�[���J�ڗp�ϐ��Q

	Scene scene_ = Scene::Title;

	bool EasingStart = false;

	uint32_t frame=0;

	const uint32_t endFrame=120;

	//�V�[���J�ڂ̃X�v���C�g�̊J�n�n�_
	const float startSpriteY = -720;

	//�V�[���J�ڂ̃X�v���C�g�̏I���n�_
	const uint32_t endSpriteY = 0;

	//�V�[���J�ڗp�̃X�v���C�g
	std::unique_ptr<Sprite> sceneSprite;
	//�V�[���J�ڗp�̃X�v���C�g
	std::unique_ptr<Sprite> TitleSprite;

	std::unique_ptr<Sprite> spaceSprite;

	#pragma endregion
};


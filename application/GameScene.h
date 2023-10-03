#pragma once
#include"object3d.h"
#include"Sprite.h"
#include"Audio.h"
#include"ParticleManager.h"
#include"LightGroup.h"
#include"Player.h"
#include"EnemyManager.h"
#include"BaseScene.h"
class GameScene:public BaseScene
{

public: // �����o�֐�
	GameScene();

	~GameScene();

	void Initialize() override;
	/// <summary>
	/// ���t���[������
	/// </summary>
	void Update() override;

	/// <summary>
	/// �`��
	/// </summary>
	void Draw(DirectXCommon* dxCommon) override;

	void Finalize() override;

	/// <summary>
	/// �}�b�v�f�[�^�̃��[�h
	/// </summary>
	void MapLoad(std::string mapfullpath);

	void Collision();
private:
	std::map<std::string, Model*> models;

	std::unique_ptr <Model> mapModel;
	std::unique_ptr <Model> goalModel;
	std::unique_ptr <Model> bulletModel;

	std::unique_ptr<LightGroup> light;

	uint32_t texHandle=0;

	std::vector<std::unique_ptr<Object3d>> objects;

	std::unique_ptr<Object3d> goalObj_;
	std::unique_ptr<Object3d> spawnObj_;

	std::unique_ptr<Object3d> stage1Obj_;
	std::unique_ptr<Object3d> stage2Obj_;
	std::unique_ptr<Object3d> stage3Obj_;

	bool isNext_=false;
	bool isStage1 = false;
	bool isStage2 = false;
	bool isStage3 = false;

	bool isGoal=false;

	float ambientColor[3] = {1,1,1};

	//��������������
	float lightPos[3] = { 0,0,0 };
	float lightColor[3] = { 1,1,1 };
	float lightAtten[3] = { 0.3f,0.1f,0.1f };

	const Vector3 cameraPos = { 0.0f,40.0f,-40.0f };

	std::unique_ptr<Player> player_;

	bool isClear_=false;

	#pragma region �V�[���J�ڗp�ϐ�

	bool sceneStart = true;

	bool sceneChange = false;

	bool IsCameraSet = true;

	Vector3 cameraStart = {0.0,0.0,0.0};

	Vector3 cameraEnd = { 0.0,0.0,0.0 };

	const uint32_t endFrame=120;

	uint32_t frame =endFrame ;

	//�V�[���J�ڂ̃X�v���C�g�̊J�n�n�_
	const float startSpriteY = -720;

	//�V�[���J�ڂ̃X�v���C�g�̏I���n�_
	const uint32_t endSpriteY = 0;

	//�V�[���J�ڗp�̃X�v���C�g
	std::unique_ptr<Sprite> sceneSprite;

	uint32_t padSousaTexHandle;
	uint32_t keySousaTexHandle;

	std::unique_ptr<Sprite> sousaSprite;

	#pragma endregion

	std::unique_ptr<Model> GroundModel_;

	std::unique_ptr<Object3d> GroundObj_;

	std::unique_ptr<Model> skyModel_;

	std::unique_ptr<Object3d> skyObj_;

	std::string mapName;
};


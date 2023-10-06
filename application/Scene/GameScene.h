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

public: // メンバ関数
	GameScene();

	~GameScene();

	void Initialize() override;
	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(DirectXCommon* dxCommon) override;

	void Finalize() override;

	/// <summary>
	/// マップデータのロード
	/// </summary>
	void MapLoad(std::string mapfullpath);
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

	//光線方向初期化
	float lightPos[3] = { 0,0,0 };
	float lightColor[3] = { 1,1,1 };
	float lightAtten[3] = { 0.3f,0.1f,0.1f };

	const Vector3 cameraPos = { 0.0f,40.0f,-40.0f };

	std::unique_ptr<Player> player_;

	bool isClear_=false;

	#pragma region シーン遷移用変数

	bool sceneStart = true;

	bool sceneChange = false;

	bool IsCameraSet = true;

	Vector3 cameraStart = {0.0,0.0,0.0};

	Vector3 cameraEnd = { 0.0,0.0,0.0 };

	const uint32_t endFrame=120;

	uint32_t frame =endFrame ;

	//シーン遷移のスプライトの開始地点
	const float startSpriteY = -720;

	//シーン遷移のスプライトの終了地点
	const uint32_t endSpriteY = 0;

	//シーン遷移用のスプライト
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


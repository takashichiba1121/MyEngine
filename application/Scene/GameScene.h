#pragma once
#include"object3d.h"
#include"Sprite.h"
#include"Sound.h"
#include"ParticleManager.h"
#include"LightGroup.h"
#include"Player.h"
#include"EnemyManager.h"
#include"BaseScene.h"
class GameScene :public BaseScene
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
	void ModelLoad();

private:
#pragma region モデル

	std::map<std::string,Model*> models_;

	std::unique_ptr <Model> mapModel_;
	std::unique_ptr <Model> stageModel_;
	std::unique_ptr <Model> goalModel_;
	std::unique_ptr <Model> bulletModel_;

	std::unique_ptr<Model> GroundModel_;

	std::unique_ptr<Model> skyModel_;

	std::unique_ptr<Model> enemyModel_;
	std::unique_ptr<Model> gunEnemyModel_;

#pragma endregion

	std::unique_ptr<LightGroup> light_;

	uint32_t texHandle_ = 0;

	std::vector<std::unique_ptr<Object3d>> objects_;

	std::unique_ptr<Object3d> goalObj_;
	std::unique_ptr<Object3d> spawnObj_;

	std::unique_ptr<Object3d> stage1Obj_;
	std::unique_ptr<Object3d> stage2Obj_;
	std::unique_ptr<Object3d> stage3Obj_;

	bool isNext_ = false;
	bool isStage1_ = false;
	bool isStage2_ = false;
	bool isStage3_ = false;

	bool isGoal_ = false;

	float ambientColor_[ 3 ] = { 1,1,1 };

	//光線方向初期化
	float lightPos_[ 3 ] = { 0,0,0 };
	float lightColor_[ 3 ] = { 1,1,1 };
	float lightAtten_[ 3 ] = { 0.3f,0.1f,0.1f };

	const Vector3 cameraPos_ = { 0.0f,40.0f,-40.0f };

	std::unique_ptr<Player> player_;

	bool isClear_ = false;

#pragma region シーン遷移用変数

	bool sceneStart_ = true;

	bool sceneChange_ = false;

	bool IsCameraSet_ = true;

	Vector3 cameraStart_ = { 0.0,0.0,0.0 };

	Vector3 cameraEnd_ = { 0.0,0.0,0.0 };

	const uint32_t endFrame_ =60;

	int32_t frame_ = 0;

	const uint32_t cameraEndFrame_ = 120;

	int32_t cameraFrame_ = 0;

	//シーン遷移のスプライトの開始地点
	const float startSpriteY_ = -720;

	//シーン遷移のスプライトの終了地点
	const uint32_t endSpriteY_ = 0;

	//シーン遷移用のスプライト
	std::unique_ptr<Sprite> sceneSprite_;

#pragma endregion

	uint32_t padSousaTexHandle_ = 0;
	uint32_t keySousaTexHandle_ = 0;

	std::unique_ptr<Sprite> sousaSprite_;

	std::unique_ptr<Object3d> skyObj_;

	std::string mapName_;

	Vector3 lightV = { -1,-1,1 };
};


#pragma once
#include"object3d.h"
#include"Sprite.h"
#include"Sound.h"
#include"LightGroup.h"
#include"Player.h"
#include"ParticleManager.h"
#include"BaseScene.h"
#include"Map.h"
/*
* ゲームシーン
*/
class GameScene :public BaseScene
{
public:
	enum class Stage
	{
		Stage1,
		Stage2,
		Stage3
	};

	enum class SceneFlow
	{
		Start,
		Play,
		End
	};
public: // メンバ関数
	GameScene();

	~GameScene();

	//初期化
	void Initialize() override;
	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update() override;

	void StartUpdate();

	void PlayUpdate();

	void EndUpdate();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(DirectXCommon* dxCommon) override;

		/// <summary>
	/// 描画
	/// </summary>
	void SpriteDraw() override;

	void Finalize() override;

private:
#pragma region モデル

	std::unique_ptr<Model> skyModel_;
#pragma endregion

	std::unique_ptr<LightGroup> light_;

	uint32_t texHandle_ = 0;

	std::unique_ptr<ParticleManager> particleManager_;

	std::unique_ptr<Map> map;

	bool isNext_ = false;
	bool isStage1_ = false;
	bool isStage2_ = false;
	bool isStage3_ = false;

	float ambientColor_[ 3 ] = { 1,1,1 };

	//光線方向初期化
	float lightPos_[ 3 ] = { 0,0,0 };
	float lightColor_[ 3 ] = { 1,1,1 };
	float lightAtten_[ 3 ] = { 0.3f,0.1f,0.1f };

	const Vector3 cCameraPos_ = { 0.0f,30.0f,-30.0f };

	std::unique_ptr<Player> player_;

#pragma region シーン遷移用変数

	bool sceneStart_ = true;

	bool sceneChange_ = false;

	bool IsCameraSet_ = true;

	Vector3 cameraStart_ = { 0.0,0.0,0.0 };

	Vector3 cameraEnd_ = { 0.0,0.0,0.0 };

	const uint32_t cEndSceneChangeFrame_ = 30;

	uint32_t SceneChangeFrame_ = cEndSceneChangeFrame_;

	const uint32_t cEndCameraFrame_ = 120;

	int32_t cameraFrame_ = 0;

	//シーン遷移のスプライトの開始地点
	const float cStartSpriteY_ = -720;

	//シーン遷移のスプライトの終了地点
	const uint32_t cEndSpriteY_ = 0;

	//シーン遷移用のスプライト
	std::unique_ptr<Sprite> sceneSprite_;

	std::unique_ptr<Sprite> pauseSprite_;
	std::unique_ptr<Sprite> pauseTutorialSprite_;

#pragma endregion

	uint32_t padSousaTexHandle_ = 0;
	uint32_t keySousaTexHandle_ = 0;

	std::unique_ptr<Sprite> retrySprite_;
	std::unique_ptr<Sprite> yesSprite_;
	std::unique_ptr<Sprite> noSprite_;
	std::unique_ptr<Sprite> yazirusiSprite_;

	std::unique_ptr<Sprite> tutorial0Sprite_;
	std::unique_ptr<Sprite> tutorial4Sprite_;

	const Vector2 cStartMenuPosition_= { 1280,720 };

	const Vector2 cEndMenuPosition_ = { 860,636 };

	const uint32_t cEndUiMovingFrame_=10;

	int32_t uiMovingFrame_=0;

	uint32_t keyTexHandle_=0;

	uint32_t padTexHandle_=0;

	std::unique_ptr<Sprite> spaceSprite_;

	std::unique_ptr<Object3d> skyObj_;

	std::string mapName_;

	Vector3 lightV_ = { 0,-1,0 };

	bool retry_ = false;

	Stage nowStage_=Stage::Stage3;

	Map::Phase pause_= Map::Phase::Before;

	Map::Phase oldPause_ = Map::Phase::Before;

	uint32_t pauseFrame_=0;

	SceneFlow sceneFlow=SceneFlow::Start;
};


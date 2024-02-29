#pragma once
#include"object3d.h"
#include"Sprite.h"
#include"Sound.h"
#include"ParticleManager.h"
#include"LightGroup.h"
#include"Player.h"
#include"EnemyManager.h"
#include"BaseScene.h"
/*
* ゲームシーン
*/
class GameScene :public BaseScene
{
public:
	enum class Phase
	{
		Before,
		Middle,
		After,
	};

	enum class Stage
	{
		Stage1,
		Stage2,
		Stage3
	};

	struct Plane
	{
		std::unique_ptr<Object3d> plane;
		uint32_t texHandle = 0;
		Vector2 UVSift = { 0,0 };
		Vector2 UVSiftSpeed = { 0,0 };
	};

	struct Tutorial
	{
		std::unique_ptr<Object3d> obj;
		uint32_t texHandle = 0;
		bool isDraw = true;
	};

	struct GoalSwitch
	{
		std::unique_ptr<Object3d> obj;
		std::unique_ptr<Object3d> light;
		std::unique_ptr<Object3d> spotLight;
		uint32_t lightFrame;
		uint32_t lightIndex = 0;
		Phase phase = Phase::Before;

		uint32_t partFrame;
	};

	struct Switch
	{
		std::unique_ptr<Object3d> obj;
		std::unique_ptr<Object3d> light;
		std::unique_ptr<Object3d> spotLight;
		uint32_t lightIndex = 0;
		uint32_t lightFrame;
		Phase phase = Phase::Before;
		uint32_t index = 0;

		uint32_t partFrame;

		// 最後のconstを忘れると"instantiated from here"というエラーが出てコンパイルできないので注意
		bool operator<(const Switch& right) const {
			return index < right.index;
		}
	};

	struct Gimmick
	{
		std::unique_ptr<Object3d> obj;
		Phase phase = Phase::Before;
		uint32_t index = 0;
		float EndPosY = 0;

		// 最後のconstを忘れると"instantiated from here"というエラーが出てコンパイルできないので注意
		bool operator<(const Gimmick& right) const {
			return index < right.index;
		}
	};

	struct Camera
	{
		Vector3 pos;
		Vector3 rot;
		Vector3 scale;
	};

	struct Middle
	{
		std::unique_ptr<Object3d> obj;
		Phase phase = Phase::Before;

		uint32_t Frame=0;
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

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(DirectXCommon* dxCommon) override;

		/// <summary>
	/// 描画
	/// </summary>
	void SpriteDraw() override;

	void Finalize() override;

	/// <summary>
	/// マップデータのロード
	/// </summary>
	void MapLoad(std::string mapfullpath,bool middleSwitchRLoad);

private:
	//モデルのロード
	void ModelLoad();

	void SwitchCollsion();

	void planeUpdate();

	void tutorialUpdate();

	void goalSwitchUpdate();

	void switchUpdate();

	void middleUpdate();

private:
#pragma region モデル

	std::map<std::string,std::unique_ptr<Model>> models_;

	std::unique_ptr<Model> skyModel_;
#pragma endregion

	std::unique_ptr<LightGroup> light_;

	uint32_t texHandle_ = 0;

	std::vector<std::unique_ptr<Object3d>> objects_;

	std::vector<std::unique_ptr<Plane>> planes_;
	uint32_t planeDrawNum_=0;

	std::vector<std::unique_ptr<Tutorial>> tutorials_;

	std::vector<std::unique_ptr<GoalSwitch>> goalSwitchs_;

	std::vector<std::unique_ptr<Switch>> switchs_;

	std::vector<std::unique_ptr<Gimmick>> gimmicks_;

	std::vector<std::unique_ptr<Camera>> cameras_;

	std::vector<std::unique_ptr<Middle>> middles_;

	std::unique_ptr<Object3d> goalObj_;
	std::unique_ptr<Object3d> spawnObj_;

	std::unique_ptr<ParticleManager> particleManager_;

	bool isNext_ = false;
	bool isStage1_ = false;
	bool isStage2_ = false;
	bool isStage3_ = false;

	bool isGoal_ = false;
	Phase goalOpen_ = Phase::Before;
	float goalOpenFlame_ = 0;
	const float cEndGoalOpenFlame_ = 60;

	float ambientColor_[ 3 ] = { 1,1,1 };

	//光線方向初期化
	float lightPos_[ 3 ] = { 0,0,0 };
	float lightColor_[ 3 ] = { 1,1,1 };
	float lightAtten_[ 3 ] = { 0.3f,0.1f,0.1f };

	const Vector3 cCameraPos_ = { 0.0f,30.0f,-30.0f };

	std::unique_ptr<Player> player_;

	bool isClear_ = false;

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

	int32_t uiMovingFrame_;

	uint32_t keyTexHandle_;

	uint32_t padTexHandle_;

	std::unique_ptr<Sprite> spaceSprite_;

	std::unique_ptr<Object3d> skyObj_;

	std::string mapName_;

	Vector3 lightV_ = { 0,-1,0 };

	bool retry_ = false;

	uint32_t spotLightTex_ = 0;

	const uint32_t cEndSpotLightFrame_ = 30;

	const uint32_t cEndLightFrame_ = 30;

	Stage nowStage_=Stage::Stage3;

	Phase pause_=Phase::Before;

	Phase oldPause_ = Phase::Before;

	uint32_t pauseFrame_=0;

	const uint32_t cEndSwitchPartFrame = 15;

	Vector3 cameraScale_ = { 80,80,80 };
};


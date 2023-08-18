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

public: // メンバ関数
	GameScene();

	~GameScene();

	void Initialize();
	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(DirectXCommon* dxCommon);

	/// <summary>
/// 描画
/// </summary>
	void PostEffectDraw(DirectXCommon* dxCommon);

	/// <summary>
	/// マップデータのロード
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

	//光線方向初期化
	float lightPos[3] = { 0,0,0 };
	float lightColor[3] = { 1,1,1 };
	float lightAtten[3] = { 0.3f,0.1f,0.1f };

	std::unique_ptr<Player> player_;

	#pragma region シーン遷移用変数群

	Scene scene_ = Scene::Title;

	bool EasingStart = false;

	uint32_t frame=0;

	const uint32_t endFrame=120;

	//シーン遷移のスプライトの開始地点
	const float startSpriteY = -720;

	//シーン遷移のスプライトの終了地点
	const uint32_t endSpriteY = 0;

	//シーン遷移用のスプライト
	std::unique_ptr<Sprite> sceneSprite;
	//シーン遷移用のスプライト
	std::unique_ptr<Sprite> TitleSprite;

	std::unique_ptr<Sprite> spaceSprite;

	#pragma endregion
};


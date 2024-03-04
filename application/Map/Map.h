#pragma once
#include"object3d.h"
#include"Player.h"
class Map
{
public:

	enum class Phase
	{
		Before,
		Middle,
		After,
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

		uint32_t Frame = 0;
	};

	struct MonsterHouse
	{
		Vector3 pos;
		Vector3 scale;
		std::string EnemyPopName;
		std::unique_ptr<Switch> clearSwitch;
		std::unique_ptr<Gimmick> clearGimmick;
		std::unique_ptr<Gimmick> startGimmick;
	};
public:
	//初期化
	void Initialize(Player* player,LightGroup* light);
	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	void ParticleDraw();

		/// <summary>
	/// マップデータのロード
	/// </summary>
	void MapLoad(std::string mapfullpath,bool middleSwitchRLoad);

	std::vector<std::unique_ptr<Gimmick>>* GetGimmick() {
		return &gimmicks_;
	}

	std::vector<std::unique_ptr<Object3d>>* GetObjects() {
		return &objects_;
	}

	std::string GetMapName() {
		return mapName_;
	}

	bool IsClear() {
		return isClear_;
	}
		
private:

//モデルのロード
	void ModelLoad();

	void SwitchCollsion();

	void planeUpdate();

	void tutorialUpdate();

	void goalSwitchUpdate();

	void switchUpdate();

	void middleUpdate();

	void MonsterHouseUpdate();

	void ObjectUpdate();
private:
#pragma region モデル

	std::map<std::string,std::unique_ptr<Model>> models_;

	std::unique_ptr<Model> skyModel_;
#pragma endregion

	LightGroup* light_;

	uint32_t texHandle_ = 0;

	std::vector<std::unique_ptr<Object3d>> objects_;

	std::vector<std::unique_ptr<Plane>> planes_;
	uint32_t planeDrawNum_ = 0;

	std::vector<std::unique_ptr<Tutorial>> tutorials_;

	std::vector<std::unique_ptr<GoalSwitch>> goalSwitchs_;

	std::vector<std::unique_ptr<Switch>> switchs_;

	std::vector<std::unique_ptr<Gimmick>> gimmicks_;

	std::vector<std::unique_ptr<Camera>> cameras_;

	std::vector<std::unique_ptr<Middle>> middles_;

	std::vector<std::unique_ptr<MonsterHouse>> monsterHouses_;

	std::unique_ptr<Object3d> goalObj_;
	std::unique_ptr<Object3d> spawnObj_;

	Map::Phase goalOpen_ = Map::Phase::Before;
	float goalOpenFlame_ = 0;
	const float cEndGoalOpenFlame_ = 60;

	Player* player_;

	std::string mapName_;

	bool isClear_ = false;

	std::unique_ptr<ParticleManager> particleManager_;

	const Vector3 cCameraScale_ = { 80,80,80 };

	const uint32_t cEndSwitchPartFrame = 15;

	const uint32_t cEndSpotLightFrame_ = 30;

	const uint32_t cEndLightFrame_ = 30;

	uint32_t spotLightTex_ = 0;
};


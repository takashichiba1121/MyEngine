#pragma once
#include"object3d.h"
#include"ParticleManager.h"
#include"PlayerBullet.h"
#include"Sound.h"
#include"Sprite.h"

/*
* プレイヤー
*/
class Player
{
public:
	enum class AttackType
	{
		Normal,
		ThreeWay,
		Division,
		Bomb,
	};

public:
	/// <summary>
	/// 初期化全般
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="bulletModel">弾のモデル</param>
	void Initialize();
	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 入力は受け付けないがアップデートはする(カメラ座標のみを動かすときなどに使う)
	/// </summary>
	void ObjectUpdate() {obj_->Update(); }

	/// <summary>
	/// 動き全般の処理
	/// </summary>
	void Move();

	/// <summary>
	/// 終了処理
	/// </summary>
	void Finalize();

	/// <summary>
	/// 攻撃全般
	/// </summary>
	void Attack();

	/// <summary>
	/// 緊急回避
	/// </summary>
	void Avoid();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// パーティクルの描画
	/// </summary>
	void ParticleDraw();

	/// <summary>
	///スプライトの描画
	/// </summary>
	void SpriteDraw();

	/// <summary>
	/// リセット
	/// </summary>
	void Reset();

	void SetMapData(std::vector<std::unique_ptr<Object3d>>* objects);

	void SetGimmickData(std::vector<Object3d*> objects);

	/// <summary>
	/// jsonから読み取った地形データとプレイヤーの当たり判定
	/// </summary>
	void MapCollision();

	/// <summary>
	/// エネミーとの当たり判定
	/// </summary>
	void EnemyCollision();

	/// <summary>
	/// スポーン位置の設定
	/// </summary>
	void SetSpawnPos(Vector3 spawnPosition);

	/// <summary>
	/// スポーン位置にプレイヤーを戻す
	/// </summary>
	void RSpawn();

	bool GetOnGround() {return onGround_;}

	Object3d* GetObj() { return obj_.get(); }

	uint32_t GetMoving() {return notMovingFrame_;}

	bool IsDaed() { return isDelete_; }

	void SetLight(LightGroup* light) {light_ = light;}

	LightGroup* GetLight() {return light_;}

	void SetCameraMove(Vector3 cameraPos);

	void SetCameraPos(Vector3 cameraPos) {
		endCameraPos_ = cameraPos;
		cameraPos_=cameraPos;
	}

	Vector3 GetEndCameraPos() {return endCameraPos_;}

	void CameraPosChange();


private:
#pragma region パラメーター

	float gravityAcceleration_ = 0.03f;

	float StartJumpSpeed_ = -0.7f;

	int32_t resetPoint_ = -10;

	float kBulletSpeed_ = 1.0f;

	int32_t bulletLife_ = 45;

	int32_t maxHp_ = 1;

	const uint32_t cNormalAttackInterval_ = 10;

	const uint32_t cThreeWayAttackInterval_ = 20;

	const uint32_t cDivisionAttackInterval_ = 30;

	const uint32_t cBombAttackInterval_ = 30;

#pragma endregion

	std::unique_ptr<Model> model_;

	std::unique_ptr<Model> bulletModel_;

	std::vector<Object3d*> objects_;

	std::unique_ptr<Object3d> obj_;

	std::unique_ptr<Sprite> normalAttack_;
	std::unique_ptr<Sprite> threeWayAttack_;
	std::unique_ptr<Sprite> divisionAttack_;
	std::unique_ptr<Sprite> bombAttack_;

	Vector3 move_;

	float fallSpeed_ = 0;

	bool onGround_;

	std::unique_ptr<ParticleManager> paMan_;

	Vector3 spawnPosition_ = { 0,0,0 };

	Vector3 goalPosition_ = { 0,0,0 };
	
	Vector3 goalScale_ = { 0,0,0 };

	//プレイヤーから見たカメラの位置
	Vector3 cameraPos_ = { 0.0f,30.0f,-30.0f };

	const float cCameraSpeed_ = 0.1f;

	const float cMoveSpeed_ = 0.25f;

	const uint32_t cEndAvoidInterval_=30;

	std::list<std::unique_ptr<PlayerBullet>> bullets_;

	bool isDaed_ = false;

	bool isDelete_ = false;

	uint32_t hp_ = maxHp_;

	float ExplosionFrame_ = 0;

	const float cEndExplosionFrame_ = 60;

	uint32_t AttackInterval_=0;

	bool isAvoid_=false;

	uint32_t avoidTime_=0;

	const uint32_t cEndAvoidTime_=20;

	Vector3 avoidVec_ = {0,0,0};

	const float cAvoidSpeed_ = 0.5f;

	Vector3 initialRot_ = {0,0,0};

	uint32_t avoidInterval_ = 0;

	AttackType attackType_=AttackType::Normal;

	LightGroup* light_;

#pragma region カメラ回転用変数
	bool isChangCamera_ = false;

	Vector3 startCameraPos_;

	Vector3 endCameraPos_;

	uint32_t cameraChangeFrame_;

	const uint32_t cEndCameraCgangeFrame_=60;

	uint32_t playerNuwCamera_=0;
#pragma endregion

	uint32_t notMovingFrame_=0;
};


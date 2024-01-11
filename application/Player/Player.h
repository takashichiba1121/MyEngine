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
	Vector3 MapCollision();

	/// <summary>
	/// エネミーとの当たり判定
	/// </summary>
	void EnemyCollision();

	/// <summary>
	/// スポーン位置の設定
	/// </summary>
	void SetSpawn(Vector3 spawnPosition);

	void SetCameraPos(const Vector3 cameraPos) {cameraPos_=cameraPos ; }

	bool GetOnGround() {return onGround_;}

	Object3d* GetObj() { return obj_.get(); }

	bool IsDaed() { return isDelete_; }

	void SetLight(LightGroup* light) {light_ = light;}

	LightGroup* GetLight() {return light_;}
private:
#pragma region パラメーター

	float gravityAcceleration_ = 0.03f;

	float StartJumpSpeed_ = -0.7f;

	int32_t resetPoint_ = 0;

	float kBulletSpeed_ = 1.0f;

	int32_t bulletLife_ = 90;

	int32_t maxHp_ = 1;

	const uint32_t normalAttackInterval_ = 10;

	const uint32_t threeWayAttackInterval_ = 20;

	const uint32_t divisionAttackInterval_ = 30;

	const uint32_t bombAttackInterval_ = 30;

#pragma endregion

	std::unique_ptr<Model> model_;

	std::unique_ptr<Model> bulletModel_;

	std::vector<Object3d*> objects_;

	std::unique_ptr<Object3d> obj_;

	std::unique_ptr<Sprite> normalAttack;
	std::unique_ptr<Sprite> threeWayAttack;
	std::unique_ptr<Sprite> divisionAttack;
	std::unique_ptr<Sprite> bombAttack;

	Vector3 move_;

	float fallSpeed_ = 0;

	bool onGround_;

	std::unique_ptr<ParticleManager> paMan_;

	Vector3 spawnPosition_ = { 0,0,0 };

	Vector3 goalPosition_ = { 0,0,0 };
	
	Vector3 goalScale_ = { 0,0,0 };

	//プレイヤーから見たカメラの位置
	Vector3 cameraPos_ = { 0.0f,40.0f,-40.0f };

	const float cameraSpeed_ = 0.1f;

	const float moveSpeed_ = 0.25f;

	const uint32_t maxAvoidInterval_=30;

	std::list<std::unique_ptr<PlayerBullet>> bullets_;

	bool isDaed_ = false;

	bool isDelete_ = false;

	uint32_t hp_ = maxHp_;

	float ExplosionFrame_ = 0;

	const float ExplosionMaxFrame_ = 60;

	uint32_t AttackInterval_=0;

	bool isAvoid_=false;

	uint32_t avoidTime_=0;

	Vector3 avoidVec_ = {0,0,0};

	const float avoidSpeed_ = 0.5f;

	Vector3 initialRot_ = {0,0,0};

	uint32_t avoidInterval_ = 0;

	AttackType attackType=AttackType::Normal;

	LightGroup* light_;
};


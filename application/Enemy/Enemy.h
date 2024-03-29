#pragma once
#include"object3d.h"
#include"EnemyBullet.h"
#include"Player.h"
/*
* エネミーの基底クラス
*/
class Enemy
{
public:
	enum class EnemyType
	{
		Gun,
		Run,
		Jump,
		Tutorial,
		Wall
	};

public:
	//初期化
	virtual void Initialize(Model* enemyModel,Model* bulletModel,Player* player,uint32_t number);

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	virtual void Update();

	/// <summary>
	/// 動き全般
	/// </summary>
	virtual void Move();

	/// <summary>
	/// 入力は受け付けないがアップデートはする(カメラ座標のみを動かすときなどに使う)
	/// </summary>
	void ObjectUpdate();

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw();

	bool IsDaed()
	{
		return isDaed_;
	}

	bool IsDelete() {
		return isDelete_;
	}

	Object3d* GetObj() {
		return obj_.get();
	}

	bool IsAttack() {
		return isAttack_;
	}

	bool IsMove() {
		return isMove_;
	}

	uint32_t GetNumber() {
		return number_;
	}

	//衝突処理
	virtual void OnCollision();

	//エネミー同士の衝突処理
	virtual void OnEnemyCollision(Vector3 reject);

	EnemyType GetType() {return enemyType_;}

	virtual void AttackOff();

	static void SetLight(LightGroup* light) {light_ = light;}
protected:
	static LightGroup* light_;

	Model* model_;

	Model* bulletModel_;

	std::unique_ptr<Object3d> obj_;

	bool isDaed_ = false;

	bool isDelete_ = false;

	Player* player_;

	float ExplosionFrame_ = 0;

	const float cEndExplosionFrame_ = 60;

	float attackRange_ = 25.0f;

			//攻撃間隔
	const uint32_t cIntervalTime_ = 120;

	uint32_t IntervalTimer_ = cIntervalTime_ / 2;

	bool isMove_ = false;

	bool isAttack_ = false;

	EnemyType enemyType_;

	uint32_t number_;
#pragma region 壁用変数群
	static const uint32_t cPartiacleNum_ = 10;

	std::array<std::unique_ptr<Object3d>,cPartiacleNum_> particle_;

	const Vector3 cVec_[ cPartiacleNum_ ] =
	{
		{  0   , 1, 0   },
		{  0.5f, 1, 0.5f},
		{  0.5f, 1,-0.5f},
		{ -0.5f, 1, 0.5f},
		{ -0.5f, 1,-0.5f},
		{  0   ,-1, 0   },
		{  0.5f,-1, 0.5f},
		{  0.5f,-1,-0.5f},
		{ -0.5f,-1, 0.5f},
		{ -0.5f,-1,-0.5f},
	};
#pragma endregion
};

#pragma once
#include"object3d.h"
#include"ParticleManager.h"
#include"PlayerBullet.h"


class Player
{
public:
	/// <summary>
	/// 初期化全般
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="bulletModel">弾のモデル</param>
	void Initialize(Model* bulletModel);
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
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// パーティクルの描画
	/// </summary>
	void ParticleDraw();

	void Reset();

	void SetMapData(std::vector<std::unique_ptr<Object3d>>* objects);

	/// <summary>
	/// jsonから読み取った地形データとプレイヤーの当たり判定
	/// </summary>
	Vector3 MapCollision();

	void EnemyCollision();

	void SetSpawn(Vector3 spawnPosition);

	void SetCameraPos(const Vector3 cameraPos) {cameraPos_=cameraPos ; }

	Object3d* GetObj() { return obj_.get(); }

	bool IsDaed() { return isDaed_; }
private:
	std::unique_ptr<Model> model_;

	Model* bulletModel_;

	std::vector<std::unique_ptr<Object3d>>* objects_;

	std::unique_ptr<Object3d> obj_;

	Vector3 move_;

	float fallSpeed_ = 0;

	const float fallAcceleration_ = 0.03f;

	const float StartJumpSpeed_ = -0.7f;

	bool onGround_;

	std::unique_ptr<ParticleManager> paMan_;

	Vector3 spawnPosition_ = { 0,0,0 };

	Vector3 goalPosition_ = { 0,0,0 };
	
	Vector3 goalScale_ = { 0,0,0 };

	//プレイヤーから見たカメラの位置
	Vector3 cameraPos_ = { 0.0f,40.0f,-40.0f };

	const float cameraSpeed_ = 0.1f;

	std::list<std::unique_ptr<PlayerBullet>> bullets_;

	bool isKnockBack_ = false;

	bool isDaed_ = false;

    const uint32_t maxHp_ = 1;

	uint32_t hp_ = maxHp_;
};


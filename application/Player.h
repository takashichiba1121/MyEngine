#pragma once
#include"object3d.h"
#include"ParticleManager.h"
#include"PlayerBullet.h"


class Player
{
public:
	void Initialize(Model* model,Model* bulletModel);
	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 動き全般の処理
	/// </summary>
	void Move();

	void Attack();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// パーティクルの描画
	/// </summary>
	void ParticleDraw();

	void SetMapData(std::vector<std::unique_ptr<Object3d>>* objects);

	/// <summary>
/// jsonから読み取った地形データとプレイヤーの当たり判定
/// </summary>
	Vector3 MapCollision();

	void SetGoal(Vector3 goalPosition, Vector3 goalScale);

	void SetSpawn(Vector3 spawnPosition);

	bool GetClear() {return isClear ; }
private:

	Model* bulletModel_;

	std::vector<std::unique_ptr<Object3d>>* objects_;

	std::unique_ptr<Object3d> obj_;

	Vector3 move_;

	float fallSpeed_ =0;

	const float fallAcceleration_ = 0.03f;

	const float StartJumpSpeed_ = -0.5;

	bool onGround_;

	std::unique_ptr<ParticleManager> paMan_;

	Vector3 spawnPosition_ = { 0,0,0 };

	Vector3 goalPosition_ = { 0,0,0 };

	Vector3 goalScale_ = { 0,0,0 };

	bool isClear = false;

	//プレイヤーから見たカメラの位置
	const Vector3 cameraPos = {0,20.0f,-10.0f};

	const float cameraSpeed=0.1f;

	std::list<std::unique_ptr<PlayerBullet>> bullets_;

	double y;

};


#pragma once
#include"object3d.h"


class Player
{
public:
	void Initialize(Model* model);
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

	void SetMapData(std::vector<std::unique_ptr<Object3d>>* objects);

	/// <summary>
/// jsonから読み取った地形データとプレイヤーの当たり判定
/// </summary>
	Vector3 MapCollision();
private:

	std::vector<std::unique_ptr<Object3d>>* objects_;

	std::unique_ptr<Object3d> obj_;

	Vector3 move;

	float fallSpeed =0;

	const float fallAcceleration = 0.03f;

	const float StartJumpSpeed = -0.5;

	bool onGround;
};


#pragma once
#include"object3d.h"
#include"EnemyBullet .h"
#include"Player.h"
class Enemy
{
public:
	///<summary>
	///初期化全般
	///</summary>
	///<param name="model">モデル</param>
	///<param name="bulletModel">弾のモデル</param>
	///<param name="position">位置</param>
	void Initialize(Model* bulletModel, Vector3 position,Object3d* playerObj);
	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 攻撃全般
	/// </summary>
	void Attack();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	bool IsDaed() { return isDaed_; }

	Object3d* GetEnemyObj() { return obj_.get(); }

	void OnCollision();
private:
	std::unique_ptr<Model> model;

	Model* bulletModel_;

	std::unique_ptr<Object3d> obj_;

	bool isDaed_=false;

	//攻撃間隔
	static const uint32_t kAttackTime = 300;

	uint32_t attackTimer_ = kAttackTime;

	Object3d* playerObj_;
};

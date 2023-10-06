#pragma once
#include"object3d.h"

class EnemyBullet
{
public:
	
	///<summary>
	///初期化処理
	///</summary>
	///<param name="model">モデル</param>
	///<param name="velocity">速度</param>
	///<param name="position">初期位置</param>
	void Initialize(Model* model,Vector2 velocity,Vector3 position);
	///<summary>
	///毎フレーム処理
	///</summary>
	void Update();

	///<summary>
	///描画
	///</summary>
	void Draw();
	
	///<summary>
	///衝突を検出したら呼び出されたるコールバック関数
	///</summary>
	void OnCollision();

/// <summary>
/// 消えるべきか
/// </summary>
	bool IsDead() {return isDead_ ; }

	Vector3 GetPosition() {return obj_->GetPosition() ; }

	Vector3 GetScale() { return obj_->GetScale(); }

private:

	std::unique_ptr<Object3d> obj_;

	//速度
	Vector2 velocity_;
	//寿命<frm>
	static const uint32_t kLifeTime = 60;
	//デスタイマー
	uint32_t deathTimer_ = kLifeTime;
	//デスフラグ
	bool isDead_ = false;
};
#pragma once
#include"object3d.h"

class PlayerBullet
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

private:

	std::unique_ptr<Object3d> obj_;

	//速度
	Vector2 velocity_;
	//寿命<frm>
	static const int32_t kLifeTime = 60;
	//デスタイマー
	int32_t deathTimer_ = kLifeTime;
	//デスフラグ
	bool isDead_ = false;
};
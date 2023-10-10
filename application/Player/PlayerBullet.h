#pragma once 
#include"object3d.h"
#include"EnemyBullet.h"

class PlayerBullet
{
public:
	
	///<summary>
	///初期化処理
	///</summary>
	///<param name="model">モデル</param>
	///<param name="velocity">速度</param>
	///<param name="position">初期位置</param>
	void Initialize(Model* model,Vector2 velocity,Vector3 position,uint32_t life);
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
	//デスタイマー
	uint32_t life_;
	//デスフラグ
	bool isDead_ = false;
};
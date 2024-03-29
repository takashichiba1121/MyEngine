#pragma once
#include"object3d.h"
/*
* エネミーの弾
*/
class EnemyBullet
{
public:
	enum class Phase
	{
		Charge,
		Attack,
	};

public:
	
	///<summary>
	///初期化処理
	///</summary>
	///<param name="model">モデル</param>
	///<param name="velocity">速度</param>
	///<param name="position">初期位置</param>
	void Initialize(Model* model,Vector3 velocity,Vector3 position);
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

	void SetPhase(Phase phase);

	Vector3 GetPosition() {return obj_->GetPosition() ; }

	Vector3 GetScale() { return obj_->GetScale(); }

	void SetLight(int32_t lightIndex);

	static void SetLight(LightGroup* light) {light_ = light;}

private:

	static LightGroup* light_;

	std::unique_ptr<Object3d> obj_;

	//速度
	Vector3 velocity_;
	//寿命<frm>
	static const uint32_t kLifeTime_ = 120;
	//デスタイマー
	uint32_t deathTimer_ = kLifeTime_;
	//デスフラグ
	bool isDead_ = false;

	//弾のチャージ時間
	uint32_t chaseTime_ = 20;

	uint32_t chaseTimer_ = 0;

	Phase phase_=Phase::Charge;

	int32_t lightIndex_;

	const Vector3 bulletColor_ = { 0.1f,1.0f,0.1f };
};
#pragma once 
#include"object3d.h"
#include"EnemyBullet.h"
/*
* プレイヤーの弾の基底クラス
*/
class PlayerBullet
{
public:
	enum class Phase
	{
		Charge,
		Attack,
		Delete,
	};

	enum class Type
	{
		Nolmal,
		Division,
		Bomb
	};
public:
	
	///<summary>
	///初期化処理
	///</summary>
	///<param name="model">モデル</param>
	///<param name="velocity">速度</param>
	///<param name="position">初期位置</param>
	virtual void Initialize(Model* model,Vector2 velocity,Vector3 position,uint32_t life);

	///<summary>
	///毎フレーム処理
	///</summary>
	virtual void Update();

	///<summary>
	///描画
	///</summary>
	void Draw();
	
	///<summary>
	///衝突を検出したら呼び出されたるコールバック関数
	///</summary>
	 virtual void OnCollision();

/// <summary>
/// 消えるべきか
/// </summary>
	bool IsDead() {return isDead_ ; }

	Vector3 GetPosition() {return obj_->GetPosition() ; }

	Vector3 GetScale() { return obj_->GetScale(); }

	void SetPhase(Phase phase);

	void SetChageTime(uint32_t Time) {chageTime_=Time;}

	virtual void SetLight(LightGroup* light,int32_t lightIndex);

	void SetRot(Vector3 rot) {
		obj_->SetRot(rot);
	}

	Type GetType() {
		return type;
	}

protected:

	std::unique_ptr<Object3d> obj_;

	Model* model_;

	//速度
	Vector2 velocity_;
	//デスタイマー
	uint32_t life_;
	//デスフラグ
	bool isDead_ = false;

		//弾のチャージ時間
	uint32_t chageTime_ = 60;

	uint32_t chageTimer_ = 0;

	Phase phase_ = Phase::Charge;

	LightGroup* light_;

	int32_t lightIndex_;

	const Vector3 bulletColor_ = { 0.88f,0.78f,0.39f };

	Type type;
};
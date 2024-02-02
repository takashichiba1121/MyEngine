#pragma once
#include "PlayerBullet.h"
/*
* プレイヤーの爆発する弾
*/
class BombPlayerBullet :
    public PlayerBullet
{
public:
		///<summary>
	///初期化処理
	///</summary>
	///<param name="model">モデル</param>
	///<param name="velocity">速度</param>
	///<param name="position">初期位置</param>
	void Initialize(Model* model,Vector2 velocity,Vector3 position,uint32_t life) override;

	///<summary>
	///毎フレーム処理
	///</summary>
	void Update() override;

	///<summary>
	///衝突を検出したら呼び出されたるコールバック関数
	///</summary>
	void OnCollision() override;

	void SetLight(LightGroup* light,int32_t lightIndex) override;

protected:
	float kBulletSpeed_ = 1.0f;

	int32_t bulletLife_ = 90;

	const Vector3 bulletColor_ = { 0,0.5f,1 };
};


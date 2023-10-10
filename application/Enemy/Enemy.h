#pragma once
#include"object3d.h"
#include"EnemyBullet.h"
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
	void Initialize(Model* bulletModel,Vector3 position,Object3d* playerObj);
	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
/// 入力は受け付けないがアップデートはする(カメラ座標のみを動かすときなどに使う)
/// </summary>
	void ObjectUpdate() {
		obj_->Update();
	}

/// <summary>
/// 攻撃全般
/// </summary>
	void Attack();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	bool IsDelete() {
		return isDelete_;
	}

	Object3d* GetObj() {
		return obj_.get();
	}

	void OnCollision();

	void SetIsAttack(bool isAttack) {
		isAttack_ = isAttack;
	}
private:
	std::unique_ptr<Model> model_;

	Model* bulletModel_;

	std::unique_ptr<Object3d> obj_;

	bool isDaed_ = false;

	bool isDelete_ = false;

	//攻撃間隔
	static const uint32_t kAttackTime_ = 300;

	uint32_t attackTimer_ = kAttackTime_;

	Object3d* playerObj_;

	bool isAttack_ = false;

	float ExplosionFrame=0;

	const float ExplosionMaxFrame = 60;
};

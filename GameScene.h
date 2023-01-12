#pragma once
#include"Model.h"
#include"object3d.h"
#include"SpriteCommon.h"
#include"Sprite.h"
#include"input.h"
#include"Texture.h"
class GameScene
{
public:
public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(GameScene* gameScene);

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void ObjectDraw();

	/// <summary>
	/// 描画
	/// </summary>
	void SpriteDraw();

private:
	Model* model;

	Object3d* obj = new Object3d;

	Sprite* sprite = nullptr;

	Sprite* sprite2 = nullptr;
};


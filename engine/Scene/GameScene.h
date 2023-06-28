#pragma once
#include"object3d.h"
#include"input.h"
#include"WinApp.h"
#include"DirectXCommon.h"
#include"SpriteCommon.h"
#include"Sprite.h"
#include"Texture.h"
#include"Audio.h"
#include"imguiManager.h"
#include<imgui.h>
#include"ParticleManager.h"
#include"assimpModel.h"
#include"assimpModel.h"
#include"assimpObject3d.h"
class GameScene
{
public: // メンバ関数
	GameScene();

	~GameScene();

	void Initialize();
	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(DirectXCommon* dxCommon);

	/// <summary>
/// 描画
/// </summary>
	void PostEffectDraw(DirectXCommon* dxCommon);
private:

	std::unique_ptr<Model> model;

	std::unique_ptr<Object3d> obj;


	std::unique_ptr<Model> skydome;

	std::unique_ptr<Object3d> skydomeObj;

	bool isSkydomeDraw=false;
};


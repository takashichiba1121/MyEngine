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
class GameScene
{
public: // ƒƒ“ƒoŠÖ”
	GameScene();

	~GameScene();

	void Initialize();
	/// <summary>
	/// –ˆƒtƒŒ[ƒ€ˆ—
	/// </summary>
	void Update();

	/// <summary>
	/// •`‰æ
	/// </summary>
	void Draw(DirectXCommon* dxCommon);

	/// <summary>
/// •`‰æ
/// </summary>
	void PostEffectDraw(DirectXCommon* dxCommon);
private:
	std::unique_ptr<Sprite> sprite;

	std::unique_ptr<Model> model;

	std::unique_ptr<Object3d> obj;

	std::unique_ptr<Model> model2;

	std::unique_ptr<Object3d> obj2;

	std::unique_ptr<imguiManager> imgui;

	float objX = 0;

	float objY = 0;

	bool operation = false;

	std::unique_ptr<Sound> sound;

	Vector2 pos = { 0,0 };

	std::unique_ptr<ParticleManager> pMan;

	float flame=0;

	bool isP=false;
};


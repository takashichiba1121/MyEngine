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
#include"LightGroup.h"
#include"Player.h"
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

	std::unique_ptr<Model> model;

	std::unique_ptr<Object3d> obj;


	std::unique_ptr<Model> skydome;

	std::unique_ptr<Object3d> skydomeObj;

	bool isSkydomeDraw=false;

	std::unique_ptr<LightGroup> light;

	uint32_t texHandle=0;

	std::map<std::string, Model*> models;
	std::vector<bool> collision;
	std::vector<std::unique_ptr<Object3d>> objects;

	std::unique_ptr<Sprite> sprite;

	float ambientColor[3] = {1,1,1};

	//Œõü•ûŒü‰Šú‰»
	float lightPos[3] = { 0,0,0 };
	float lightColor[3] = { 1,1,1 };
	float lightAtten[3] = { 0.3f,0.1f,0.1f };

	std::unique_ptr<Player> player_;
};


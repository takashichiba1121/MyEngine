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
	float lightDir0[3] = { 0,0,1 };
	float lightColor0[3] = { 1,0,0 };

	float lightDir1[3] = { 0,1,0 };
	float lightColor1[3] = { 0,1,0 };

	float lightDir2[3] = { 1,0,0 };
	float lightColor2[3] = { 0,0,1 };
};


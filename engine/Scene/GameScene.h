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
#include"PostEffect.h"
class GameScene
{
public: // �����o�֐�
	GameScene();

	~GameScene();

	void Initialize();
	/// <summary>
	/// ���t���[������
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	/// <summary>
	/// �X�v���C�g�`��
	/// </summary>
	void SpriteDraw();

	void PostEffectDraw();
private:
	std::unique_ptr<Sprite> sprite;

	std::unique_ptr<Model> model;

	std::unique_ptr<Object3d> obj;

	std::unique_ptr<imguiManager> imgui;

	float objX = 0;

	float objY = 0;

	bool operation = false;

	std::unique_ptr<Sound> sound;

	PostEffect* postEffect = nullptr;
};


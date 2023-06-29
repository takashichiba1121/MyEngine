#include "GameScene.h"
#include"DirectXCommon.h"

GameScene::GameScene()
{
}

GameScene::~GameScene()
{
}

void GameScene::Initialize()
{
	model.reset(Model::LoadFormOBJ("sphere",true));

	obj = std::make_unique<Object3d>();

	obj->SetModel(model.get());

	obj->Initialize();

	obj->SetRot({ 0,0,0 });

	obj->SetPolygonExplosion({ 0.0f,1.0f,6.28f,10.0f,1.0f });

	light = std::make_unique<Light>();

	light->Initialize();

	light->SetLightColor({1,1,1});

	Object3d::SetLight(light.get());

	Object3d::SetEye({ 0.0f,0.0f,-5.0f });

	skydome.reset(Model::LoadFormOBJ("skydome",true));

	skydomeObj = std::make_unique<Object3d>();

	skydomeObj->SetModel(skydome.get());

	skydomeObj->Initialize();

	skydomeObj->SetPosition({ 0,0,0 });

	skydomeObj->SetScale({ 50.0f,50.0f,50.0f });

	skydomeObj->SetPolygonExplosion({ 0.0f,1.0f,6.28f,10.0f,1.0f });
}

void GameScene::Update()
{
		Vector3 rot = obj->GetRot();
		rot += {0, 1, 0};
		obj->SetRot(rot);

		{
			static Vector3 lightDir = { 0,1,5 };

			if (Input::PushKey(DIK_W)) { lightDir.y += 1.0f; }
			if (Input::PushKey(DIK_S)) { lightDir.y -= 1.0f; }
			if (Input::PushKey(DIK_D)) { lightDir.x += 1.0f; }
			if (Input::PushKey(DIK_A)) { lightDir.x -= 1.0f; }

			light->SetLightDir(lightDir);
		}

	light->Update();

	obj->Update();

	skydomeObj->Update();
}

void GameScene::Draw(DirectXCommon* dxCommon)
{ 
	Object3d::PreDraw(dxCommon->GetCommandList());

	obj->Draw();
	if (isSkydomeDraw)
	{
		skydomeObj->Draw();
	}

	Object3d::PostDraw();

	assimpObject3d::PreDraw(dxCommon->GetCommandList());

	assimpObject3d::PostDraw();

	ParticleManager::PreDraw(dxCommon->GetCommandList());
	ParticleManager::PostDraw();

	SpriteCommon::PreDraw();
	SpriteCommon::PostDraw();
}

void GameScene::PostEffectDraw(DirectXCommon* dxCommon)
{

	Object3d::PreDraw(dxCommon->GetCommandList());

	obj->Draw();

	skydomeObj->Draw();

	Object3d::PostDraw();

	ParticleManager::PreDraw(dxCommon->GetCommandList());
	//pMan->Draw();
	ParticleManager::PostDraw();
}

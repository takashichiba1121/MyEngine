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
	model.reset(Model::LoadFormOBJ("Cube2"));

	obj = std::make_unique<Object3d>();

	obj->SetModel(model.get());

	obj->Initialize();

	obj->SetPosition({ 0,0,0 });

	obj->SetPolygonExplosion({ 0.0f,1.0f,6.28f,10.0f,1.0f });

	Object3d::SetEye({ 0.0f,0.0f,5.0f });

	skydome.reset(Model::LoadFormOBJ("skydome"));

	skydomeObj = std::make_unique<Object3d>();

	skydomeObj->SetModel(skydome.get());

	skydomeObj->Initialize();

	skydomeObj->SetPosition({ 0,0,0 });

	skydomeObj->SetScale({ 50.0f,50.0f,50.0f });

	skydomeObj->SetPolygonExplosion({ 0.0f,1.0f,6.28f,10.0f,1.0f });
}

void GameScene::Update()
{
	if(Input::TriggerKey(DIK_0))
	{
		if (isSkydomeDraw)
		{
			isSkydomeDraw = false;
		}
		else
		{
			isSkydomeDraw = true;
		}
	}

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

	if (isSkydomeDraw)
	{
		skydomeObj->Draw();
	}

	Object3d::PostDraw();

	ParticleManager::PreDraw(dxCommon->GetCommandList());
	//pMan->Draw();
	ParticleManager::PostDraw();
}

#include "GameScene.h"
#include"DirectXCommon.h"
#include"levelLoad.h"
#include"Collider.h"
#include"Texture.h"
#include<imgui.h>
#include"input.h"
#include<time.h>
#include"Easing.h"

GameScene::GameScene()
{
}

GameScene::~GameScene()
{
}

void GameScene::Initialize()
{
	bulletModel.reset(Model::LoadFormOBJ("playerBullet", true));

	mapModel.reset(Model::LoadFormOBJ("Map", true));

	goalModel.reset(Model::LoadFormOBJ("Goal", true));

	models.insert(std::make_pair("PlayerBullet",bulletModel.get()));

	models.insert(std::make_pair("Map",mapModel.get()));

	models.insert(std::make_pair("Goal", goalModel.get()));

	Object3d::SetEye({ 0.0f,20.0f,-20.0f });

	spaceSprite = std::make_unique<Sprite>();

	spaceSprite->Initialize(Texture::LoadTexture(L"Resources/spaceKey.png"));

	spaceSprite->SetPosition({ 640.0f,540.0f });

	spaceSprite->SetAnchorPoint({ 0.5f,0.5f });

	spaceSprite->Update();

	player_ = std::make_unique<Player>();

	player_->Initialize(models["Map"], models["PlayerBullet"]);

	goalObj_= std::make_unique<Object3d>();

	goalObj_->Initialize();

	MapLoad();

	player_->SetMapData(&objects);

	light.reset(LightGroup::Create());

	Object3d::SetLight(light.get());

	//light->SetDirLightActive(0, false);

	light->SetDirLightDir(0, { 0,-1,1 });

	light->SetDirLightColor(0, { 1,1,1 });

	light->SetDirLightActive(1, false);

	light->SetDirLightActive(2, false);

	//light->SetPointActive(0, true);

	//light->SetPointPos(0,{0.5f,1.0f,0.0f});

	sceneSprite = std::make_unique<Sprite>();

	sceneSprite->Initialize(Texture::LoadTexture(L"Resources/scene.png"));

	sceneSprite->SetAnchorPoint({0,0});

	sceneSprite->SetScale({1280,720});

	sceneSprite->SetPosition({0,float(startSpriteY)});

	sceneSprite->Update();

	TitleSprite = std::make_unique<Sprite>();

	TitleSprite->Initialize(Texture::LoadTexture(L"Resources/Title.png"));

	TitleSprite->SetPosition({640,230 });

	TitleSprite->SetAnchorPoint({ 0.5f,0.5f });

	TitleSprite->Update();
	light->Update();

	for (std::unique_ptr<Object3d>& obj : objects)
	{
		obj->Update();
	}
}

void GameScene::Update()
{
	srand((unsigned int)time(NULL));

	float shininess = 20;

 	switch (scene_)
	{
	case GameScene::Scene::Title:
		if (Input::TriggerKey(DIK_SPACE))
		{
			EasingStart = true;
		}
		if (EasingStart)
		{
			frame++;

			float f = Easing::easeOutBounce((float)frame/endFrame);

			sceneSprite->SetPosition({0,((endSpriteY-startSpriteY)*f)+startSpriteY});

			if (frame>=endFrame)
			{
				scene_ = Scene::Game;
				//frame = 0;

				MapLoad();

				player_->SetMapData(&objects);
				player_->Update();
			}
		}
		sceneSprite->Update();

		ImGui::Begin("Partcle");

		ImGui::Text("%d", frame);

		ImGui::Text("%f", sceneSprite->GetPosition().y);

		ImGui::End();
		break;
	case GameScene::Scene::Game:
		if (EasingStart)
		{
			frame--;
			float f = (float)frame / endFrame;

			sceneSprite->SetPosition({ 0,((endSpriteY- startSpriteY) * f) + startSpriteY });

			if (frame <=0)
			{
				EasingStart = false;
				frame = 0;
			}
			sceneSprite->Update();
		}
		else
		{
			light->SetAmbientColor(Vector3({ ambientColor[0],ambientColor[1],ambientColor[2] }));

			light->SetPointPos(0, Vector3({ lightPos[0],lightPos[1],lightPos[2] }));
			light->SetPointAtten(0, Vector3({ lightAtten[0],lightAtten[1],lightAtten[2] }));
			light->SetPointColor(0, Vector3({ lightColor[0],lightColor[1],lightColor[2] }));

			player_->Update();
			light->Update();

			if (Input::TriggerKey(DIK_0)) 
			{
				objects.clear();
				MapLoad();

				player_->SetMapData(&objects);
			}

			if (player_->GetClear())
			{
				scene_ = Scene::Result;
			}
		}
		break;
	case GameScene::Scene::Result:
		if (Input::TriggerKey(DIK_SPACE))
		{
			scene_ = Scene::Title;
		}

		break;
	default:
		break;
	}

	for (std::unique_ptr<Object3d>& obj : objects)
	{
		obj->SetShininess(shininess);

		obj->Update();
	}

	goalObj_->Update();
}

void GameScene::Draw(DirectXCommon* dxCommon)
{
	switch (scene_)
	{
	case GameScene::Scene::Title:
		SpriteCommon::PreDraw();
		spaceSprite->Draw();
		TitleSprite->Draw();
		sceneSprite->Draw();
		SpriteCommon::PostDraw();
		break;
	case GameScene::Scene::Game:
		Object3d::PreDraw(dxCommon->GetCommandList());
		for (uint32_t i = 0; i < objects.size(); i++)
		{
			objects[i]->Draw();
		}

		player_->Draw();

		goalObj_->Draw();

		Object3d::PostDraw();

		ParticleManager::PreDraw(dxCommon->GetCommandList());
		player_->ParticleDraw();
		ParticleManager::PostDraw();

		SpriteCommon::PreDraw();

		sceneSprite->Draw();
		SpriteCommon::PostDraw();
		break;
	case GameScene::Scene::Result:
		SpriteCommon::PreDraw();
		spaceSprite->Draw();
		SpriteCommon::PostDraw();
		break;
	default:
		break;
	}
}

void GameScene::PostEffectDraw(DirectXCommon* dxCommon)
{

	Object3d::PreDraw(dxCommon->GetCommandList());

	Object3d::PostDraw();

	ParticleManager::PreDraw(dxCommon->GetCommandList());
	//pMan->Draw();
	ParticleManager::PostDraw();
}

void GameScene::MapLoad()
{
	std::unique_ptr<LevelData> levelData;
	levelData.reset(LevelLoad::Load("Resources/level.json"));
	for (auto& objectData : levelData->objects)
	{
		if (objectData.tagName=="Map")
		{
			//モデルを指定して3Dオブジェクトを生成
			std::unique_ptr<Object3d> newObject = std::make_unique<Object3d>();
			newObject->Initialize();
			newObject->SetModel(models[objectData.fileName]);

			assert(newObject);

			// 座標
			newObject->SetPosition({ objectData.trans });

			// 回転角
			newObject->SetRot({ objectData.rot });

			// 座標
			newObject->SetScale({ objectData.scale });

			newObject->SetPolygonExplosion({ 0.0f,1.0f,6.28f,100.0f });

			// 配列に登録
			objects.push_back(std::move(newObject));
		}
		if (objectData.tagName == "Spawn")
		{
			player_->SetSpawn(objectData.trans);
		}
		if (objectData.tagName == "Goal")
		{
			player_->SetGoal(objectData.trans,objectData.scale);

			//モデルを指定して3Dオブジェクトを生成
			goalObj_->SetModel(models[objectData.fileName]);

			// 座標
			goalObj_->SetPosition({ objectData.trans });

			// 回転角
			goalObj_->SetRot({ objectData.rot });

			// 座標
			goalObj_->SetScale({ objectData.scale });

			goalObj_->SetPolygonExplosion({ 0.0f,1.0f,6.28f,100.0f });

			goalObj_->Setalpha(0.3f);

			goalObj_->Update();
		}
	}
}

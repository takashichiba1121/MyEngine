#include "GameScene.h"
#include"DirectXCommon.h"
#include"levelLoad.h"
#include"Collider.h"
#include"Texture.h"
#include<imgui.h>
#include"input.h"
#include<time.h>
#include"Easing.h"
#include"SceneManager.h"

GameScene::GameScene()
{
}

GameScene::~GameScene()
{
	EnemyManager::Fin();
}

void GameScene::Initialize()
{
	bulletModel.reset(Model::LoadFormOBJ("playerBullet", true));

	mapModel.reset(Model::LoadFormOBJ("Map", true));

	goalModel.reset(Model::LoadFormOBJ("Goal", true));

	models.insert(std::make_pair("PlayerBullet", bulletModel.get()));

	models.insert(std::make_pair("Map", mapModel.get()));

	models.insert(std::make_pair("Goal", goalModel.get()));

	Object3d::SetEye({ 0.0f,20.0f,-20.0f });

	spaceTexHandle = Texture::Instance()->LoadTexture(L"Resources/spaceKey.png");

	aTexHandle = Texture::Instance()->LoadTexture(L"Resources/Abotton.png");

	padSousaTexHandle=Texture::Instance()->LoadTexture(L"Resources/KyeSousa.png");

	keySousaTexHandle=Texture::Instance()->LoadTexture(L"Resources/PadSousa.png");

	spaceSprite = std::make_unique<Sprite>();

	player_ = std::make_unique<Player>();

	if (Input::Instance()->IsLinkGamePad())
	{
		spaceSprite->Initialize(aTexHandle);
	}
	else
	{
		spaceSprite->Initialize(spaceTexHandle);
	}

	spaceSprite->SetPosition({ 640.0f,500.0f });

	spaceSprite->SetAnchorPoint({ 0.5f,0.5f });

	spaceSprite->Update();

	goalObj_= std::make_unique<Object3d>();

	goalObj_->Initialize();

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

	sceneSprite->Initialize(Texture::Instance()->LoadTexture(L"Resources/scene.png"));

	sceneSprite->SetAnchorPoint({0,0});

	sceneSprite->SetScale({1280,720});

	sceneSprite->SetPosition({0,float(startSpriteY)});

	sceneSprite->Update();

	sousaSprite = std::make_unique<Sprite>();

	if (Input::Instance()->IsLinkGamePad())
	{
		sousaSprite->Initialize(padSousaTexHandle);
	}
	else
	{
		sousaSprite->Initialize(keySousaTexHandle);
	}

	sousaSprite->SetAnchorPoint({ 0.0f,0.5f });

	sousaSprite->SetScale({ 300,300 });

	sousaSprite->SetPosition({50,360 });

	sousaSprite->Update();

	overSprite = std::make_unique<Sprite>();

	overSprite->Initialize(Texture::Instance()->LoadTexture(L"Resources/GameOver.png"));

	overSprite->SetPosition({ 640,230 });

	overSprite->SetAnchorPoint({ 0.5f,0.5f });

	overSprite->Update();

	clearSprite = std::make_unique<Sprite>();

	clearSprite->Initialize(Texture::Instance()->LoadTexture(L"Resources/Clear.png"));

	clearSprite->SetPosition({ 640,230 });

	clearSprite->SetAnchorPoint({ 0.5f,0.5f });

	clearSprite->Update();

	titleSprite = std::make_unique<Sprite>();

	titleSprite->Initialize(Texture::Instance()->LoadTexture(L"Resources/Title.png"));

	titleSprite->SetPosition({ 640,600 });

	titleSprite->SetAnchorPoint({ 0.5f,0.5f });

	titleSprite->Update();

	light->Update();

	EnemyManager::Initialize();

	GroundModel_.reset(Model::LoadFormOBJ("Ground",true));

	GroundObj_ = std::make_unique<Object3d>();

	GroundObj_->Initialize();

	GroundObj_->SetModel(GroundModel_.get());

	GroundObj_->SetPosition({ 0,-10,0 });

	GroundObj_->SetScale({500,500,500});

	GroundObj_->SetRot({0,3.14f,0});

	GroundObj_->SetPolygonExplosion({ 0.0f,1.0f,6.28f,100.0f });

	GroundObj_->Update();

	skyModel_.reset(Model::LoadFormOBJ("skydomeTitle", true));

	skyObj_ = std::make_unique<Object3d>();

	skyObj_->Initialize();

	skyObj_->SetModel(skyModel_.get());

	skyObj_->SetPosition({ 0,0,0 });

	skyObj_->SetScale({ 200,200,200 });

	skyObj_->SetRot({ 1.57f,0,0 });

	skyObj_->SetPolygonExplosion({ 0.0f,1.0f,6.28f,100.0f });

	skyObj_->Update();

	frame = 120;

	scene_ = Scene::Game;

	sceneStart = true;

	sceneChange = false;

	player_->Initialize(models["PlayerBullet"]);

	MapLoad();

	player_->SetMapData(&objects);

	EnemyManager::SetMapData(&objects);

	EnemyManager::SetPlayer(player_.get());
	//player_->Update();
	//EnemyManager::Update();
}

void GameScene::Update()
{
	srand((unsigned int)time(NULL));

 	switch (scene_)
	{
	case Scene::Game:
		if (Input::Instance()->IsLinkGamePad())
		{
			sousaSprite->SetTexture(padSousaTexHandle);
		}
		else
		{
			sousaSprite->SetTexture(keySousaTexHandle);
		}

		if (sceneStart)
		{
			frame--;
			float f = (float)frame / endFrame;

			sceneSprite->SetPosition({ 0,((endSpriteY- startSpriteY) * f) + startSpriteY });

			if (frame <=0)
			{
				sceneStart = false;
				frame = 0;
			}
			sceneSprite->Update();
		}
		else if(player_->IsDaed()==false&&player_->IsClear()==false)
		{
			player_->Update();
			light->Update();
			EnemyManager::Update();

			if (Input::Instance()->TriggerKey(DIK_0))
			{
				objects.clear();
				MapLoad();

				EnemyManager::SetMapData(&objects);

				player_->SetMapData(&objects);
			}

			if (player_->IsClear())
			{
				sceneChange = true;
			}

			if (player_->IsDaed())
			{
				sceneChange = true;
			}
		}

		if (sceneChange)
		{
			if (frame < endFrame)
			{
				frame++;

				float f = Easing::easeOutBounce((float)frame / endFrame);

				sceneSprite->SetPosition({ 0,((endSpriteY - startSpriteY) * f) + startSpriteY });

			}
			else if (frame >= endFrame + 5)
			{
				frame = 120;

				if (player_->IsClear())
				{
					scene_ = Scene::Result;
				}
				else
				{
					scene_ = Scene::GameOver;
				}
				sceneStart = true;

				sceneChange = false;
			}
			else
			{
				frame++;
			}

			sceneSprite->Update();
		}

		for (std::unique_ptr<Object3d>& obj : objects)
		{
			obj->Update();
		}

		goalObj_->Update();

		GroundObj_->Update();
		break;
	case Scene::Result:
		if (Input::Instance()->IsLinkGamePad())
		{
			spaceSprite->SetTexture(aTexHandle);
		}
		else
		{
			spaceSprite->SetTexture(spaceTexHandle);
		}

		if (sceneStart)
		{
			frame--;
			float f = (float)frame / endFrame;

			sceneSprite->SetPosition({ 0,((endSpriteY - startSpriteY) * f) + startSpriteY });

			if (frame <= 0)
			{
				sceneStart = false;
				frame = 0;
			}
			sceneSprite->Update();
		}
		else
		{
			if (Input::Instance()->IsLinkGamePad())
			{
				if (Input::Instance()->PadTriggerKey(XINPUT_GAMEPAD_A))
				{
					sceneChange = true;
				}
			}
			else if (Input::Instance()->TriggerKey(DIK_SPACE))
			{
				sceneChange = true;
			}
		}
		if (sceneChange)
		{
			if (frame < endFrame)
			{
				frame++;

				float f = Easing::easeOutBounce((float)frame / endFrame);

				sceneSprite->SetPosition({ 0,((endSpriteY - startSpriteY) * f) + startSpriteY });

			}
			else if (frame >= endFrame + 5)
			{
				frame = 120;

				SceneManager::Instance()->ChangeScene("TITLE");

				sceneStart = true;

				sceneChange = false;

				player_->Reset();

				EnemyManager::Clear();
			}
			else
			{
				frame++;
			}

			sceneSprite->Update();
		}

		spaceSprite->Update();
		break;
	case Scene::GameOver:
		if (Input::Instance()->IsLinkGamePad())
		{
			spaceSprite->SetTexture(aTexHandle);
		}
		else
		{
			spaceSprite->SetTexture(spaceTexHandle);
		}

		if (sceneStart)
		{
			frame--;
			float f = (float)frame / endFrame;

			sceneSprite->SetPosition({ 0,((endSpriteY - startSpriteY) * f) + startSpriteY });

			if (frame <= 0)
			{
				sceneStart = false;
				frame = 0;
			}
			sceneSprite->Update();
		}
		else
		{
			if (Input::Instance()->IsLinkGamePad())
			{
				if (Input::Instance()->PadTriggerKey(XINPUT_GAMEPAD_A))
				{
					sceneChange = true;
				}
			}
			else if (Input::Instance()->TriggerKey(DIK_SPACE))
			{
				sceneChange = true;
			}
		}
		if (sceneChange)
		{
			if (frame < endFrame)
			{
				frame++;

				float f = Easing::easeOutBounce((float)frame / endFrame);

				sceneSprite->SetPosition({ 0,((endSpriteY - startSpriteY) * f) + startSpriteY });

			}
			else if (frame >= endFrame + 5)
			{
				frame = 120;

				SceneManager::Instance()->ChangeScene("TITLE");

				sceneStart = true;

				sceneChange = false;

				player_->Reset();

				EnemyManager::Clear();
			}
			else
			{
				frame++;
			}

			sceneSprite->Update();
		}
		spaceSprite->Update();
		break;
	default:
		break;
	}

	skyObj_->Update();

	ImGui::Begin("Scene");

	ImGui::Text("%d",scene_);

	ImGui::End();
}

void GameScene::Draw(DirectXCommon* dxCommon)
{
	switch (scene_)
	{
	case GameScene::Scene::Game:
		Object3d::PreDraw(dxCommon->GetCommandList());
		
		skyObj_->Draw();

		GroundObj_->Draw();

		for (uint32_t i = 0; i < objects.size(); i++)
		{
			objects[i]->Draw();
		}

		EnemyManager::Draw();

		player_->Draw();

		goalObj_->Draw();

		Object3d::PostDraw();

		ParticleManager::PreDraw(dxCommon->GetCommandList());
		player_->ParticleDraw();
		EnemyManager::ParticleDraw();
		ParticleManager::PostDraw();

		SpriteCommon::Instance()->PreDraw();
		sousaSprite->Draw();
		sceneSprite->Draw();
		SpriteCommon::Instance()->PostDraw();
		break;
	case GameScene::Scene::Result:
		Object3d::PreDraw(dxCommon->GetCommandList());
		skyObj_->Draw();
		Object3d::PostDraw();

		SpriteCommon::Instance()->PreDraw();
		spaceSprite->Draw();
		clearSprite->Draw();
		titleSprite->Draw();
		sceneSprite->Draw();
		SpriteCommon::Instance()->PostDraw();
		break;
	case GameScene::Scene::GameOver:
		Object3d::PreDraw(dxCommon->GetCommandList());
		skyObj_->Draw();
		Object3d::PostDraw();

		SpriteCommon::Instance()->PreDraw();
		spaceSprite->Draw();
		overSprite->Draw();
		titleSprite->Draw();
		sceneSprite->Draw();
		SpriteCommon::Instance()->PostDraw();
		break;
	default:
		break;
	}
}

void GameScene::Finalize()
{
}

void GameScene::MapLoad()
{
	std::unique_ptr<LevelData> levelData;
	levelData.reset(LevelLoad::Load("Resources/level.json"));

	objects.clear();

	EnemyManager::Clear();
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
		if (objectData.tagName=="Enemy")
		{
			std::unique_ptr<Enemy> enemy;

			enemy = std::make_unique<Enemy>();

			enemy->Initialize(models ["PlayerBullet"], { objectData.trans }, player_->GetObj());

			enemy->Update();

			EnemyManager::AddEnemy(std::move(enemy));
		}
	}
}
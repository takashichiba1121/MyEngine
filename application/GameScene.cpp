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
	EnemyManager::Fin();
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

	player_->Initialize(models["PlayerBullet"]);

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

	sceneSprite->Initialize(Texture::LoadTexture(L"Resources/scene.png"));

	sceneSprite->SetAnchorPoint({0,0});

	sceneSprite->SetScale({1280,720});

	sceneSprite->SetPosition({0,float(startSpriteY)});

	sceneSprite->Update();

	overSprite = std::make_unique<Sprite>();

	overSprite->Initialize(Texture::LoadTexture(L"Resources/GameOver.png"));

	overSprite->SetPosition({ 640,230 });

	overSprite->SetAnchorPoint({ 0.5f,0.5f });

	overSprite->Update();

	clearSprite = std::make_unique<Sprite>();

	clearSprite->Initialize(Texture::LoadTexture(L"Resources/Clear.png"));

	clearSprite->SetPosition({ 640,230 });

	clearSprite->SetAnchorPoint({ 0.5f,0.5f });

	clearSprite->Update();

	TitleSprite = std::make_unique<Sprite>();

	TitleSprite->Initialize(Texture::LoadTexture(L"Resources/Title.png"));

	TitleSprite->SetPosition({640,230 });

	TitleSprite->SetAnchorPoint({ 0.5f,0.5f });

	TitleSprite->Update();
	light->Update();

	EnemyManager::Initialize();

	EnemyManager::SetPlayer(player_.get());

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
}

void GameScene::Update()
{
	srand((unsigned int)time(NULL));

 	switch (scene_)
	{
	case Scene::Title:

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
			if (Input::IsLinkGamePad())
			{
				if (Input::PadTriggerKey(XINPUT_GAMEPAD_A))
				{
					sceneChange = true;
				}
			}
			else if (Input::TriggerKey(DIK_SPACE))
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
			else if (frame >= endFrame+5)
			{
				frame = 120;

				scene_ = Scene::Game;
				
				sceneStart = true;

				sceneChange = false;

				MapLoad();

				player_->SetMapData(&objects);
				EnemyManager::SetMapData(&objects);
				player_->Update();
				EnemyManager::Update();
			}
			else
			{
				frame++;
			}
		}
		sceneSprite->Update();

		ImGui::Begin("Partcle");

		ImGui::Text("%d", frame);

		ImGui::Text("%f", sceneSprite->GetPosition().y);

		ImGui::End();
		break;
	case Scene::Game:
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

			if (Input::TriggerKey(DIK_0)) 
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

				MapLoad();

				player_->SetMapData(&objects);
				EnemyManager::SetMapData(&objects);
				player_->Update();
				EnemyManager::Update();
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
			if (Input::IsLinkGamePad())
			{
				if (Input::PadTriggerKey(XINPUT_GAMEPAD_A))
				{
					sceneChange = true;
				}
			}
			else if (Input::TriggerKey(DIK_SPACE))
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

				scene_ = Scene::Title;

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
		break;
	case Scene::GameOver:
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
			if (Input::IsLinkGamePad())
			{
				if (Input::PadTriggerKey(XINPUT_GAMEPAD_A))
				{
					sceneChange = true;
				}
			}
			else if (Input::TriggerKey(DIK_SPACE))
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

				scene_ = Scene::Title;
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
	case GameScene::Scene::Title:
		Object3d::PreDraw(dxCommon->GetCommandList());
		skyObj_->Draw();
		Object3d::PostDraw();

		SpriteCommon::PreDraw();
		spaceSprite->Draw();
		TitleSprite->Draw();
		sceneSprite->Draw();
		SpriteCommon::PostDraw();
		break;
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

		SpriteCommon::PreDraw();

		sceneSprite->Draw();
		SpriteCommon::PostDraw();
		break;
	case GameScene::Scene::Result:
		Object3d::PreDraw(dxCommon->GetCommandList());
		skyObj_->Draw();
		Object3d::PostDraw();

		SpriteCommon::PreDraw();
		spaceSprite->Draw();

		clearSprite->Draw();

		sceneSprite->Draw();
		SpriteCommon::PostDraw();
		break;
	case GameScene::Scene::GameOver:
		Object3d::PreDraw(dxCommon->GetCommandList());
		skyObj_->Draw();
		Object3d::PostDraw();

		SpriteCommon::PreDraw();
		spaceSprite->Draw();

		overSprite->Draw();

		sceneSprite->Draw();
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

			enemy->Initialize(models["PlayerBullet"], { objectData.trans }, player_->GetObj());

			enemy->Update();

			EnemyManager::AddEnemy(std::move(enemy));
		}
	}
}
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

	padSousaTexHandle = Texture::Instance()->LoadTexture("Resources/KyeSousa.png");

	keySousaTexHandle = Texture::Instance()->LoadTexture("Resources/PadSousa.png");

	player_ = std::make_unique<Player>();

	goalObj_ = std::make_unique<Object3d>();

	goalObj_->Initialize();

	stage1Obj_ = std::make_unique<Object3d>();

	stage1Obj_->Initialize();

	stage2Obj_ = std::make_unique<Object3d>();

	stage2Obj_->Initialize();

	stage3Obj_ = std::make_unique<Object3d>();

	stage3Obj_->Initialize();

	light.reset(LightGroup::Create());

	Object3d::SetLight(light.get());

	light->SetDirLightDir(0, { 0,-1,0 });

	light->SetDirLightActive(1, false);

	light->SetDirLightActive(2, false);

	sceneSprite = std::make_unique<Sprite>();

	sceneSprite->Initialize(Texture::Instance()->LoadTexture("Resources/scene.png"));

	sceneSprite->SetAnchorPoint({ 0,0 });

	sceneSprite->SetScale({ 1280,720 });

	sceneSprite->SetPosition({ 0,float(startSpriteY) });

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

	sousaSprite->SetPosition({ 50,360 });

	sousaSprite->Update();

	light->Update();

	EnemyManager::Instance()->Initialize();

	GroundModel_.reset(Model::LoadFormOBJ("Ground", true));

	GroundObj_ = std::make_unique<Object3d>();

	GroundObj_->Initialize();

	GroundObj_->SetModel(GroundModel_.get());

	GroundObj_->SetPosition({ 0,-10,0 });

	GroundObj_->SetScale({ 500,500,500 });

	GroundObj_->SetRot({ 0,3.14f,0 });

	GroundObj_->Update();

	skyModel_.reset(Model::LoadFormOBJ("skydomeTitle", true));

	skyObj_ = std::make_unique<Object3d>();

	skyObj_->Initialize();

	skyObj_->SetModel(skyModel_.get());

	skyObj_->SetPosition({ 0,0,0 });

	skyObj_->SetScale({ 200,200,200 });

	skyObj_->SetRot({ 1.57f,0,0 });

	skyObj_->Update();

	frame = 120;

	sceneStart = true;

	sceneChange = false;

	player_->Initialize(models["PlayerBullet"]);

	MapLoad("Resources/Select.json");

	player_->SetMapData(&objects);

	EnemyManager::Instance()->SetMapData(&objects);

	EnemyManager::Instance()->SetPlayer(player_.get());
	player_->ObjectUpdate();
	EnemyManager::Instance()->Update();


}

void GameScene::Update()
{
	srand((unsigned int)time(NULL));

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

		sceneSprite->SetPosition({ 0,((endSpriteY - startSpriteY) * f) + startSpriteY });

		if (frame <= 0)
		{
			sceneStart = false;
			frame = 0;
		}
		sceneSprite->Update();
	}
	else if (IsCameraSet)
	{
		frame++;
		float f = (float)frame / endFrame;

		Vector3 traget = ((cameraEnd - cameraStart) * f) + cameraStart;

		Object3d::SetTarget(traget);

		Object3d::SetEye(traget + cameraPos);

		if (frame >= endFrame)
		{
			IsCameraSet = false;
			frame = 0;
		}

		player_->ObjectUpdate();
		EnemyManager::Instance()->ObjectUpdate();
	}
	else if (sceneChange==false)
	{
		player_->Update();
		light->Update();
		EnemyManager::Instance()->Update();

		Cube A, B;

		A.Pos = player_->GetObj()->GetPosition();

		A.scale = player_->GetObj()->GetScale();

		if (mapName != "Resources/Select.json")
		{
			B.Pos = goalObj_->GetPosition();

			B.scale = goalObj_->GetScale();

			if (Collider::CubeAndCube(A, B))
			{
				isClear_ = true;
				sceneChange = true;
				frame = 0;
			}
		}
		else
		{

			B.Pos = stage1Obj_->GetPosition();

			B.scale = stage1Obj_->GetScale();

			if (Collider::CubeAndCube(A, B))
			{
				isNext_ = true;
				isStage1 = true;
				sceneChange = true;
				frame = 0;
			}

			B.Pos = stage2Obj_->GetPosition();

			B.scale = stage2Obj_->GetScale();

			if (Collider::CubeAndCube(A, B))
			{
				isNext_ = true;
				isStage1 = true;
				sceneChange = true;
				frame = 0;
			}

			B.Pos = stage3Obj_->GetPosition();

			B.scale = stage3Obj_->GetScale();

			if (Collider::CubeAndCube(A, B))
			{
				isNext_ = true;
				isStage1 = true;
				sceneChange = true;
				frame = 0;
			}
		}
		if (player_->IsDaed())
		{
			sceneChange = true;
			frame = 0;
		}
	}
	else
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
			if (mapName != "Resources/Select.json")
			{
				if (isClear_)
				{
					SceneManager::Instance()->ChangeScene("RESULT");
				}
				else if (player_->IsDaed())
				{
					SceneManager::Instance()->ChangeScene("GAMEOVER");
				}
			}
			else
			{
				if (isStage1 == true)
				{
					MapLoad("Resources/Stage1.json");

					player_->SetMapData(&objects);

					EnemyManager::Instance()->SetMapData(&objects);

					EnemyManager::Instance()->SetPlayer(player_.get());
					//player_->Update();
					EnemyManager::Instance()->Update();

					IsCameraSet = true;

					sceneStart = true;
				}
				else if (isStage2 == true)
				{
					MapLoad("Resources/Stage1.json");

					player_->SetMapData(&objects);

					EnemyManager::Instance()->SetMapData(&objects);

					EnemyManager::Instance()->SetPlayer(player_.get());
					//player_->Update();
					EnemyManager::Instance()->Update();

					IsCameraSet = true;

					sceneStart = true;
				}
				else
				{
					MapLoad("Resources/Stage1.json");

					player_->SetMapData(&objects);

					EnemyManager::Instance()->SetMapData(&objects);

					EnemyManager::Instance()->SetPlayer(player_.get());
					//player_->Update();
					EnemyManager::Instance()->Update();

					IsCameraSet = true;

					sceneStart = true;
				}
			}
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

	stage1Obj_->Update();

	stage2Obj_->Update();

	stage3Obj_->Update();

	GroundObj_->Update();

	skyObj_->Update();
}

void GameScene::Draw(DirectXCommon* dxCommon)
{
	Object3d::PreDraw(dxCommon->GetCommandList());

	skyObj_->Draw();

	//GroundObj_->Draw();

	for (uint32_t i = 0; i < objects.size(); i++)
	{
		objects[i]->Draw();
	}

	EnemyManager::Instance()->Draw();

	player_->Draw();
	if (mapName == "Resources/Select.json")
	{
		stage1Obj_->Draw();
		stage2Obj_->Draw();
		stage3Obj_->Draw();
	}
	else
	{
		goalObj_->Draw();
	}

	Object3d::PostDraw();

	ParticleManager::PreDraw(dxCommon->GetCommandList());
	player_->ParticleDraw();
	EnemyManager::Instance()->ParticleDraw();
	ParticleManager::PostDraw();

	SpriteCommon::Instance()->PreDraw();
	sousaSprite->Draw();
	sceneSprite->Draw();
	SpriteCommon::Instance()->PostDraw();
}

void GameScene::Finalize()
{
	EnemyManager::Instance()->Fin();
}

void GameScene::MapLoad(std::string mapFullpath)
{
	std::unique_ptr<LevelData> levelData;
	levelData.reset(LevelLoad::Load(mapFullpath));

	objects.clear();

	EnemyManager::Instance()->Clear();

	mapName = mapFullpath;

	for (auto& objectData : levelData->objects)
	{
		if (objectData.tagName == "Map")
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

			// 配列に登録
			objects.push_back(std::move(newObject));
		}
		if (objectData.tagName == "Spawn")
		{
			player_->SetSpawn(objectData.trans);

			cameraEnd = objectData.trans;
		}
		if (objectData.tagName == "Goal")
		{
			//player_->SetGoal(objectData.trans, objectData.scale);

			//モデルを指定して3Dオブジェクトを生成
			goalObj_->SetModel(models[objectData.fileName]);

			// 座標
			goalObj_->SetPosition({ objectData.trans });

			// 回転角
			goalObj_->SetRot({ objectData.rot });

			// 座標
			goalObj_->SetScale({ objectData.scale });

			goalObj_->Setalpha(0.3f);

			goalObj_->Update();

			Object3d::SetEye(objectData.trans + cameraPos);

			Object3d::SetTarget(objectData.trans);

			isGoal = true;

			cameraStart = objectData.trans;
		}
		if (objectData.tagName == "Enemy")
		{
			std::unique_ptr<Enemy> enemy;

			enemy = std::make_unique<Enemy>();

			enemy->Initialize(models["PlayerBullet"], { objectData.trans }, player_->GetObj());

			enemy->Update();

			EnemyManager::Instance()->AddEnemy(std::move(enemy));
		}
		if (objectData.tagName == "Stage1")
		{
			//モデルを指定して3Dオブジェクトを生成
			stage1Obj_->SetModel(models[objectData.fileName]);

			// 座標
			stage1Obj_->SetPosition({ objectData.trans });

			// 回転角
			stage1Obj_->SetRot({ objectData.rot });

			// 座標
			stage1Obj_->SetScale({ objectData.scale });

			stage1Obj_->Setalpha(0.3f);

			stage1Obj_->Update();
		}
		if (objectData.tagName == "Stage2")
		{
			//モデルを指定して3Dオブジェクトを生成
			stage2Obj_->SetModel(models[objectData.fileName]);

			// 座標
			stage2Obj_->SetPosition({ objectData.trans });

			// 回転角
			stage2Obj_->SetRot({ objectData.rot });

			// 座標
			stage2Obj_->SetScale({ objectData.scale });

			stage2Obj_->Setalpha(0.3f);

			stage2Obj_->Update();
		}
		if (objectData.tagName == "Stage3")
		{
			//モデルを指定して3Dオブジェクトを生成
			stage3Obj_->SetModel(models[objectData.fileName]);

			// 座標
			stage3Obj_->SetPosition({ objectData.trans });

			// 回転角
			stage3Obj_->SetRot({ objectData.rot });

			// 座標
			stage3Obj_->SetScale({ objectData.scale });

			stage3Obj_->Setalpha(0.3f);

			stage3Obj_->Update();
		}
	}
	if (isGoal == false)
	{
		Object3d::SetEye(player_->GetObj()->GetPosition() + cameraPos);

		Object3d::SetTarget(player_->GetObj()->GetPosition());

		IsCameraSet = false;
	}
}
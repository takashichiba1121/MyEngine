#include "GameScene.h"
#include"DirectXCommon.h"
#include"levelLoad.h"
#include"Collider.h"
#include"TextureManager.h"
#include<imgui.h>
#include"input.h"
#include<time.h>
#include"Easing.h"
#include"SceneManager.h"
#include"PlayerBulletManager.h"
#include"GunEnemy.h"

GameScene::GameScene()
{
}

GameScene::~GameScene()
{
}

void GameScene::Initialize()
{
	ModelLoad();

	Object3d::SetEye({ 0.0f,20.0f,-20.0f });

	padSousaTexHandle_ = TextureManager::Instance()->LoadTexture("Resources/PadSousa.png");

	keySousaTexHandle_ = TextureManager::Instance()->LoadTexture("Resources/KeySousa.png");

	player_ = std::make_unique<Player>();

	goalObj_ = std::make_unique<Object3d>();

	goalObj_->Initialize();

	stage1Obj_ = std::make_unique<Object3d>();

	stage1Obj_->Initialize();

	stage2Obj_ = std::make_unique<Object3d>();

	stage2Obj_->Initialize();

	stage3Obj_ = std::make_unique<Object3d>();

	stage3Obj_->Initialize();

	light_.reset(LightGroup::Create());

	Object3d::SetLight(light_.get());

	light_->SetDirLightDir(0,{ 0,-1,0 });

	light_->SetDirLightActive(1,false);

	light_->SetDirLightActive(2,false);

	sceneSprite_ = std::make_unique<Sprite>();

	sceneSprite_->Initialize(TextureManager::Instance()->LoadTexture("Resources/scene.png"),TextureManager::Instance()->LoadTexture("Resources/Dissolve4.png"));

	sceneSprite_->SetAnchorPoint({ 0,0 });

	sceneSprite_->SetScale({ 1280,720 });

	sceneSprite_->Update();

	sousaSprite_ = std::make_unique<Sprite>();

	if ( Input::Instance()->IsLinkGamePad() )
	{
		sousaSprite_->Initialize(padSousaTexHandle_);
	}
	else
	{
		sousaSprite_->Initialize(keySousaTexHandle_);
	}

	sousaSprite_->SetAnchorPoint({ 0.0f,0.5f });
	;
	sousaSprite_->SetPosition({ 25,645 });

	sousaSprite_->Update();

	light_->Update();

	EnemyManager::Instance()->Initialize();

	skyModel_.reset(Model::LoadFormOBJ("skydomeTitle",true));

	skyObj_ = std::make_unique<Object3d>();

	skyObj_->Initialize();

	skyObj_->SetModel(skyModel_.get());

	skyObj_->SetPosition({ 0,0,0 });

	skyObj_->SetScale({ 300,300,300 });

	skyObj_->SetRot({ 0,0,0 });

	skyObj_->Update();

	sceneStart_ = true;

	sceneChange_ = false;

	player_->Initialize();

	MapLoad("Resources/Select.json");

	player_->SetMapData(&objects_);

	EnemyManager::Instance()->SetMapData(&objects_);

	EnemyManager::Instance()->SetPlayer(player_.get());
	player_->ObjectUpdate();
	EnemyManager::Instance()->Update();


}

void GameScene::Update()
{

	ImGui::Begin("Scene");

	ImGui::SliderFloat3("light",&lightV.x,-100,100,"%3.1f");

	ImGui::End();

	light_->SetDirLightDir(0,lightV);

	light_->Update();

	if ( Input::Instance()->TriggerKey(DIK_0) )
	{
		objects_.clear();

		MapLoad(mapName_);
	}

	srand(( unsigned int ) time(NULL));

	if ( Input::Instance()->IsLinkGamePad() )
	{
		sousaSprite_->SetTexture(padSousaTexHandle_);
	}
	else
	{
		sousaSprite_->SetTexture(keySousaTexHandle_);
	}

	if ( sceneStart_ )
	{
		frame_++;
		float f = ( float ) frame_ / endFrame_;

		sceneSprite_->SetDissolve(f);

		if ( frame_ >= (int32_t)endFrame_ )
		{
			sceneStart_ = false;

			sceneSprite_->SetMaskTexture(TextureManager::Instance()->LoadTexture("Resources/Dissolve3.png"));
		}
		sceneSprite_->Update();

	}
	else if ( IsCameraSet_ )
	{
		cameraFrame_++;
		float f = ( float ) cameraFrame_ / cameraEndFrame_;

		Vector3 traget = ( ( cameraEnd_ - cameraStart_ ) * f ) + cameraStart_;

		Object3d::SetTarget(traget);

		Object3d::SetEye(traget + cameraPos_);

		if ( cameraFrame_ >= ( int32_t ) cameraEndFrame_)
		{
			IsCameraSet_ = false;
			cameraFrame_ = 0;
		}

		player_->ObjectUpdate();
		EnemyManager::Instance()->ObjectUpdate();
	}
	else if ( sceneChange_ == false )
	{
		player_->Update();
		EnemyManager::Instance()->Update();

		Collider::Cube A,B;

		A.Pos = player_->GetObj()->GetPosition();

		A.scale = player_->GetObj()->GetScale();

		if ( mapName_ != "Resources/Select.json" )
		{
			B.Pos = goalObj_->GetPosition();

			B.scale = goalObj_->GetScale();

			if ( Collider::CubeAndCube(A,B,Collider::Collsion) )
			{
				isClear_ = true;
				sceneChange_ = true;
			}
		}
		else
		{

			B.Pos = stage1Obj_->GetPosition();

			B.scale = stage1Obj_->GetScale();

			if ( Collider::CubeAndCube(A,B,Collider::Collsion) )
			{
				isNext_ = true;
				isStage1_ = true;
				sceneChange_ = true;
			}

			B.Pos = stage2Obj_->GetPosition();

			B.scale = stage2Obj_->GetScale();

			if ( Collider::CubeAndCube(A,B,Collider::Collsion) )
			{
				isNext_ = true;
				isStage1_ = true;
				sceneChange_ = true;
			}

			B.Pos = stage3Obj_->GetPosition();

			B.scale = stage3Obj_->GetScale();

			if ( Collider::CubeAndCube(A,B,Collider::Collsion) )
			{
				isNext_ = true;
				isStage1_ = true;
				sceneChange_ = true;
			}
		}
		if ( player_->IsDaed() )
		{
			sceneChange_ = true;
		}
	}
	else
	{
		if ( frame_ > 0 )
		{
			frame_--;

			float f = ( float ) frame_ / endFrame_;

			sceneSprite_->SetDissolve(f);

		}
		else if(frame_<=-5)
		{
			if ( mapName_ != "Resources/Select.json" )
			{
				if ( isClear_ )
				{
					SceneManager::Instance()->ChangeScene("RESULT");
				}
				else if ( player_->IsDaed() )
				{
					SceneManager::Instance()->ChangeScene("GAMEOVER");
				}
			}
			else
			{
				if ( isStage1_ == true )
				{
					MapLoad("Resources/Stage2.json");

					player_->SetMapData(&objects_);

					EnemyManager::Instance()->SetMapData(&objects_);

					EnemyManager::Instance()->SetPlayer(player_.get());
					//player_->Update();
					EnemyManager::Instance()->Update();

					IsCameraSet_ = true;

					sceneStart_ = true;
				}
				else if ( isStage2_ == true )
				{
					MapLoad("Resources/Stage1.json");

					player_->SetMapData(&objects_);

					EnemyManager::Instance()->SetMapData(&objects_);

					EnemyManager::Instance()->SetPlayer(player_.get());
					//player_->Update();
					EnemyManager::Instance()->Update();

					IsCameraSet_ = true;

					sceneStart_ = true;
				}
				else
				{
					MapLoad("Resources/Stage1.json");

					player_->SetMapData(&objects_);

					EnemyManager::Instance()->SetMapData(&objects_);

					EnemyManager::Instance()->SetPlayer(player_.get());
					//player_->Update();
					EnemyManager::Instance()->Update();

					IsCameraSet_ = true;

					sceneStart_ = true;
				}
			}
			sceneSprite_->SetMaskTexture(TextureManager::Instance()->LoadTexture("Resources/Dissolve4.png"));
			sceneChange_ = false;
		}
		else
		{
			frame_--;
		}

		sceneSprite_->Update();
	}

	for ( std::unique_ptr<Object3d>& obj : objects_ )
	{
		obj->Update();

	}


	goalObj_->Update();

	stage1Obj_->Update();

	stage2Obj_->Update();

	stage3Obj_->Update();

	skyObj_->Update();
}

void GameScene::Draw(DirectXCommon* dxCommon)
{
	Object3d::PreDraw(dxCommon->GetCommandList());

	skyObj_->Draw();

	for ( uint32_t i = 0; i < objects_.size(); i++ )
	{
		objects_[ i ]->Draw();
	}

	EnemyManager::Instance()->Draw();

	player_->Draw();
	if ( mapName_ == "Resources/Select.json" )
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
	sousaSprite_->Draw();
	sceneSprite_->DissolveDraw();
	SpriteCommon::Instance()->PostDraw();
}

void GameScene::Finalize()
{
	EnemyManager::Instance()->Fin();

	PlayerBulletManager::Instance()->Fin();
}

void GameScene::MapLoad(std::string mapFullpath)
{
	std::unique_ptr<LevelData> levelData;
	levelData.reset(LevelLoad::Load(mapFullpath));

	objects_.clear();

	EnemyManager::Instance()->Clear();

	mapName_ = mapFullpath;

	for ( auto& objectData : levelData->objects )
	{
		if ( objectData.tagName == "Map" )
		{
			//モデルを指定して3Dオブジェクトを生成
			std::unique_ptr<Object3d> newObject = std::make_unique<Object3d>();
			newObject->Initialize();
			newObject->SetModel(models_[ objectData.fileName ]);

			assert(newObject);

			// 座標
			newObject->SetPosition({ objectData.trans });

			// 回転角
			newObject->SetRot({ objectData.rot });

			// 座標
			newObject->SetScale({ objectData.scale });

			// 配列に登録
			objects_.push_back(std::move(newObject));
		}
		if ( objectData.tagName == "Spawn" )
		{
			player_->SetSpawn(objectData.trans);

			cameraEnd_ = objectData.trans;
		}
		if ( objectData.tagName == "Goal" )
		{
			//player_->SetGoal(objectData.trans, objectData.scale);

			//モデルを指定して3Dオブジェクトを生成
			goalObj_->SetModel(models_[ objectData.fileName ]);

			// 座標
			goalObj_->SetPosition({ objectData.trans });

			// 回転角
			goalObj_->SetRot({ objectData.rot });

			// 座標
			goalObj_->SetScale({ objectData.scale });

			goalObj_->Setalpha(0.3f);

			goalObj_->Update();

			Object3d::SetEye(objectData.trans + cameraPos_);

			Object3d::SetTarget(objectData.trans);

			isGoal_ = true;

			cameraStart_ = objectData.trans;
		}
		if ( objectData.tagName == "GunEnemy" )
		{
			std::unique_ptr<Enemy> enemy;

			enemy = std::make_unique<GunEnemy>();

			enemy->Initialize(models_[ "Enemy" ],models_[ "enemyBullet" ],{ objectData.trans },player_->GetObj());

			enemy->Update(25);

			EnemyManager::Instance()->AddEnemy(std::move(enemy));
		}
		if ( objectData.tagName == "Enemy" )
		{
			std::unique_ptr<Enemy> enemy;

			enemy = std::make_unique<Enemy>();

			enemy->Initialize(models_[ "Enemy" ],models_[ "enemyBullet" ],{ objectData.trans },player_->GetObj());

			enemy->Update(25);

			EnemyManager::Instance()->AddEnemy(std::move(enemy));
		}
		if ( objectData.tagName == "Stage1" )
		{
			//モデルを指定して3Dオブジェクトを生成
			stage1Obj_->SetModel(models_[ objectData.fileName ]);

			// 座標
			stage1Obj_->SetPosition({ objectData.trans });

			// 回転角
			stage1Obj_->SetRot({ objectData.rot });

			// 座標
			stage1Obj_->SetScale({ objectData.scale });

			stage1Obj_->Setalpha(0.3f);

			stage1Obj_->Update();
		}
		if ( objectData.tagName == "Stage2" )
		{
			//モデルを指定して3Dオブジェクトを生成
			stage2Obj_->SetModel(models_[ objectData.fileName ]);

			// 座標
			stage2Obj_->SetPosition({ objectData.trans });

			// 回転角
			stage2Obj_->SetRot({ objectData.rot });

			// 座標
			stage2Obj_->SetScale({ objectData.scale });

			stage2Obj_->Setalpha(0.3f);

			stage2Obj_->Update();
		}
		if ( objectData.tagName == "Stage3" )
		{
			//モデルを指定して3Dオブジェクトを生成
			stage3Obj_->SetModel(models_[ objectData.fileName ]);

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
	if ( isGoal_ == false )
	{
		Object3d::SetEye(player_->GetObj()->GetPosition() + cameraPos_);

		Object3d::SetTarget(player_->GetObj()->GetPosition());

		IsCameraSet_ = false;
	}
}

void GameScene::ModelLoad()
{
	bulletModel_.reset(Model::LoadFormOBJ("enemyBullet",true));
	models_.insert(std::make_pair("enemyBullet",bulletModel_.get()));

	mapModel_.reset(Model::LoadFormOBJ("Map",true));
	models_.insert(std::make_pair("Map",mapModel_.get()));

	goalModel_.reset(Model::LoadFormOBJ("Goal",true));
	models_.insert(std::make_pair("Goal",goalModel_.get()));

	enemyModel_.reset(Model::LoadFormOBJ("enemy",true));
	models_.insert(std::make_pair("Enemy",enemyModel_.get()));
}

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
#include"RunEnemy.h"
#include"jumpEnemy.h"
#include"tutorialEnemy.h"
#include"Enemy.h"
#include"WallEnemy.h"
#include<iostream>

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

	padTexHandle_ = TextureManager::Instance()->LoadTexture("Resources/Abotton.png");

	spaceSprite_ = std::make_unique<Sprite>();

	spaceSprite_->Initialize(padTexHandle_);

	spaceSprite_->SetPosition({ 640.0f,515.0f });

	spaceSprite_->SetAnchorPoint({ 0.5f,0.5f });

	spaceSprite_->Update();

	padSousaTexHandle_ = TextureManager::Instance()->LoadTexture("Resources/PadSousa.png");

	player_ = std::make_unique<Player>();

	goalObj_ = std::make_unique<Object3d>();

	goalObj_->Initialize();

	stage1Obj_ = std::make_unique<Object3d>();

	stage1Obj_->Initialize();

	stage1Plane_.reset(Model::CreatePlaneModel(TextureManager::Instance()->LoadTexture("Resources/1-1.png")));

	stage1BillBoard_ = std::make_unique<Object3d>();

	stage1BillBoard_->Initialize();

	stage1BillBoard_->SetModel(stage1Plane_.get());

	stage1BillBoard_->SetScale({ 2,4,1 });

	stage1BillBoard_->SetRot({ 45,0,0 });

	stage1BillBoard_->SetBillBoard(true);

	stage2Plane_.reset(Model::CreatePlaneModel(TextureManager::Instance()->LoadTexture("Resources/1-2.png")));

	stage2BillBoard_ = std::make_unique<Object3d>();

	stage2BillBoard_->Initialize();

	stage2BillBoard_->SetModel(stage2Plane_.get());

	stage2BillBoard_->SetScale({ 2,4,1 });

	stage2BillBoard_->SetRot({ 45,0,0 });

	stage2BillBoard_->SetBillBoard(true);

	stage2Obj_ = std::make_unique<Object3d>();

	stage2Obj_->Initialize();

	stage3Obj_ = std::make_unique<Object3d>();

	stage3Obj_->Initialize();

	light_.reset(LightGroup::Create());

	Object3d::SetLight(light_.get());

	sceneSprite_ = std::make_unique<Sprite>();

	sceneSprite_->Initialize(TextureManager::Instance()->LoadTexture("Resources/scene.png"),TextureManager::Instance()->LoadTexture("Resources/Dissolve4.png"));

	sceneSprite_->SetAnchorPoint({ 0,0 });

	sceneSprite_->SetScale({ 1280,720 });

	sceneSprite_->Update();

	sousaSprite_ = std::make_unique<Sprite>();

	sousaSprite_->Initialize(padSousaTexHandle_);

	sousaSprite_->SetAnchorPoint({ 0.0f,0.5f });
	;
	sousaSprite_->SetPosition({ 25,645 });

	sousaSprite_->Update();

	retrySprite_ = std::make_unique<Sprite>();

	retrySprite_->Initialize(TextureManager::Instance()->LoadTexture("Resources/retry.png"),TextureManager::Instance()->LoadTexture("Resources/Dissolve4.png"));

	retrySprite_->SetAnchorPoint({ 0.5f,0.5f });

	retrySprite_->SetPosition({ 612,300 });

	retrySprite_->Update();

	yesSprite_ = std::make_unique<Sprite>();

	yesSprite_->Initialize(TextureManager::Instance()->LoadTexture("Resources/Yes.png"),TextureManager::Instance()->LoadTexture("Resources/Dissolve4.png"));

	yesSprite_->SetAnchorPoint({ 0.5f,0.5f });
	;
	yesSprite_->SetPosition({ 320,400 });

	yesSprite_->Update();

	noSprite_ = std::make_unique<Sprite>();

	noSprite_->Initialize(TextureManager::Instance()->LoadTexture("Resources/No.png"),TextureManager::Instance()->LoadTexture("Resources/Dissolve4.png"));

	noSprite_->SetAnchorPoint({ 0.5f,0.5f });
	;
	noSprite_->SetPosition({ 960,400 });

	noSprite_->Update();

	yazirusiSprite_ = std::make_unique<Sprite>();

	yazirusiSprite_->Initialize(TextureManager::Instance()->LoadTexture("Resources/yazirusi.png"),TextureManager::Instance()->LoadTexture("Resources/Dissolve4.png"));

	yazirusiSprite_->SetAnchorPoint({ 0.5f,0.5f });
	;
	yazirusiSprite_->SetPosition({ 240,400 });

	yazirusiSprite_->Update();

	light_->Update();

	EnemyManager::Instance()->Initialize();

	skyModel_.reset(Model::LoadFormOBJ("skydomeTitle",true));

	skyObj_ = std::make_unique<Object3d>();

	skyObj_->Initialize();

	skyObj_->SetModel(skyModel_.get());

	skyObj_->SetPosition({ 100,0,200 });

	skyObj_->SetScale({ 300,300,300 });

	skyObj_->SetRot({ 0,0,0 });

	skyObj_->Update();

	sceneStart_ = true;

	sceneChange_ = false;

	player_->Initialize();

	MapLoad("Resources/Stage1.json",false);

	nowStage = Stage::Stage1;

	player_->SetLight(light_.get());

	player_->SetMapData(&objects_);

	std::vector<Object3d*> a;

	for ( uint32_t i = 0; i < gimmicks_.size(); i++ )
	{
		a.push_back(gimmicks_[ i ]->obj.get());
	}

	player_->SetGimmickData(a);

	EnemyManager::Instance()->SetMapData(&objects_);

	EnemyManager::Instance()->SetPlayer(player_.get());
	player_->ObjectUpdate();
	EnemyManager::Instance()->Update();

	Enemy::SetLight(light_.get());

	EnemyBullet::SetLight(light_.get());

	particleManager_ = std::make_unique<ParticleManager>();

	particleManager_->Initialize();

	particleManager_->SetTextureHandle(TextureManager::Instance()->LoadTexture("Resources/effect4.png"));

	spotLightTex = TextureManager::Instance()->LoadTexture("Resources/lightplane.png");
}

void GameScene::Update()
{
#ifdef _DEBUG
	ImGui::Begin("Scene");

	ImGui::SliderFloat3("light",&lightV_.x,-1,1,"%3.1f");

	ImGui::End();

	if ( Input::Instance()->TriggerKey(DIK_0) )
	{
		EnemyManager::Instance()->Clear();

		MapLoad(mapName_,false);

		player_->SetMapData(&objects_);
	}
#endif

	srand(( unsigned int ) time(NULL));

	sousaSprite_->SetTexture(padSousaTexHandle_);

	if ( sceneStart_ )
	{
		frame_++;
		float f = ( float ) frame_ / endFrame_;

		sceneSprite_->SetDissolve(f);

		if ( frame_ >= ( int32_t ) endFrame_ )
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

		if ( cameraFrame_ >= ( int32_t ) cameraEndFrame_ )
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

		for ( std::unique_ptr<GoalSwitch>& goalSwitch : goalSwitchs_ )
		{
			if ( goalSwitch->phase == Phase::After )
			{
				goalOpen = Phase::Middle;
			}
			else
			{
				goalOpen = Phase::Before;
				break;
			}

		}
		if ( goalOpen == Phase::Middle )
		{
			goalOpenflame++;

			float f = goalOpenflame / goalOpenMaxFlame;

			goalObj_->SetColor({ f,f,f });

			if ( goalOpenflame >= goalOpenMaxFlame )
			{
				goalOpen = Phase::After;
			}
		}
		if ( goalOpen == Phase::After )
		{
			B.Pos = goalObj_->GetPosition();

			B.scale = goalObj_->GetScale();

			if ( Collider::CubeAndCube(A,B,Collider::Collsion) )
			{
				isClear_ = true;
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

			retry_ = true;
		}
		else if ( frame_ <= 0 )
		{
			if ( isClear_ )
			{
				std::vector<Object3d*> a;

				switch ( nowStage )
				{
				case GameScene::Stage::Stage1:
					MapLoad("Resources/Stage2.json",false);

					player_->SetMapData(&objects_);

					for ( uint32_t i = 0; i < gimmicks_.size(); i++ )
					{
						a.push_back(gimmicks_[ i ]->obj.get());
					}

					player_->SetGimmickData(a);

					EnemyManager::Instance()->SetMapData(&objects_);

					EnemyManager::Instance()->SetPlayer(player_.get());
					//player_->Update();
					EnemyManager::Instance()->Update();

					IsCameraSet_ = true;

					sceneStart_ = true;

					sceneChange_ = false;

					nowStage = Stage::Stage2;
					break;
				case GameScene::Stage::Stage2:
					MapLoad("Resources/Stage3.json",false);

					player_->SetMapData(&objects_);

					for ( uint32_t i = 0; i < gimmicks_.size(); i++ )
					{
						a.push_back(gimmicks_[ i ]->obj.get());
					}

					player_->SetGimmickData(a);

					EnemyManager::Instance()->SetMapData(&objects_);

					EnemyManager::Instance()->SetPlayer(player_.get());
					//player_->Update();
					EnemyManager::Instance()->Update();

					IsCameraSet_ = true;

					sceneStart_ = true;

					sceneChange_ = false;

					nowStage = Stage::Stage3;
					break;
				case GameScene::Stage::Stage3:
					SceneManager::Instance()->ChangeScene("RESULT");
					break;
				default:
					break;
				};
			}
			else if ( player_->IsDaed() )
			{
				if ( Input::Instance()->GetPadStick(PadStick::LX) <= -0.5 || Input::Instance()->TriggerKey(DIK_A) )
				{
					retry_ = true;
					yazirusiSprite_->SetPosition({ 240,400 });
					yazirusiSprite_->Update();
				}
				else if ( Input::Instance()->GetPadStick(PadStick::LX) >= 0.5 || Input::Instance()->TriggerKey(DIK_D) )
				{
					retry_ = false;
					yazirusiSprite_->SetPosition({ 848,400 });
					yazirusiSprite_->Update();
				}
				spaceSprite_->SetTexture(padTexHandle_);
				spaceSprite_->Update();
				if ( Input::Instance()->PadTriggerKey(XINPUT_GAMEPAD_A) || Input::Instance()->TriggerKey(DIK_SPACE) )
				{
					if ( retry_ == false )
					{
						SceneManager::Instance()->ChangeScene("TITLE");
					}
					else
					{
						MapLoad(mapName_,true);

						player_->Reset();

						player_->SetMapData(&objects_);

						std::vector<Object3d*> a;

						for ( uint32_t i = 0; i < gimmicks_.size(); i++ )
						{
							a.push_back(gimmicks_[ i ]->obj.get());
						}

						player_->SetGimmickData(a);

						EnemyManager::Instance()->SetMapData(&objects_);

						EnemyManager::Instance()->SetPlayer(player_.get());
						//player_->Update();
						EnemyManager::Instance()->Update();

						sceneStart_ = true;

						sceneChange_ = false;

						retry_ = false;
					}
				}
			}
		}

		sceneSprite_->Update();
	}

	for ( std::unique_ptr<Object3d>& obj : objects_ )
	{
		obj->Update();

	}

	for ( uint32_t i = 0; i < planes_.size(); i++ )
	{
		planes_[ i ]->UVSift += planes_[ i ]->UVSiftSpeed;

		if ( planes_[ i ]->UVSift.x >= 1 )
		{
			planes_[ i ]->UVSift.x = 0;
		}

		planes_[ i ]->plane->SetUVShift(planes_[ i ]->UVSift);

		planes_[ i ]->plane->Update();
	}
	for ( uint32_t i = 0; i < tutorials_.size(); i++ )
	{
		Vector3 vec = player_->GetObj()->GetPosition() - tutorials_[ i ]->obj->GetPosition();
		vec.x = abs(vec.x);
		vec.z = abs(vec.z);
		float A = ( vec.x + vec.z ) / 2;
		float B = ( tutorials_[ i ]->obj->GetScale().x + tutorials_[ i ]->obj->GetScale().z ) / 2;
		if ( A < B )
		{
			tutorials_[ i ]->obj->Setalpha(1.0f);
		}
		else
		{
			float X = A - B;
			X = abs(X);
			X = 1.0f / ( 0.7f + 0.7f * X + 0.7f * X * X );
			tutorials_[ i ]->obj->Setalpha(X);
			if ( X < 0 )
			{
				tutorials_[ i ]->isDraw = false;
			}
			else
			{
				tutorials_[ i ]->isDraw = true;
			}
		}

		tutorials_[ i ]->obj->Update();
	}

	SwitchCollsion();

	for ( std::unique_ptr<GoalSwitch>& goalSwitch : goalSwitchs_ )
	{
		goalSwitch->obj->Update();
		if ( goalSwitch->phase == Phase::Middle )
		{
			goalSwitch->lightFrame++;
			float a = goalSwitch->spotLight->GetScale().z;

			float f = ( float ) goalSwitch->lightFrame / lightMaxFrame;

			goalSwitch->spotLight->SetScale({ f * ( a / 5 ),1,a });

			f *= 1.5f;

			goalSwitch->light->SetScale({ f,f,f });

			if ( goalSwitch->lightFrame >= lightMaxFrame )
			{
				goalSwitch->phase = Phase::After;
			}

			light_->SetPointActive(goalSwitch->lightIndex,true);
			goalSwitch->light->Update();
			goalSwitch->spotLight->Update();
		}
		else if ( goalSwitch->phase == Phase::After )
		{
			light_->SetPointActive(goalSwitch->lightIndex,true);
			goalSwitch->light->Update();
			goalSwitch->spotLight->Update();
		}
	}

	for ( uint32_t i = 0; i < switchs_.size(); i++ )
	{
		switchs_[ i ]->obj->Update();
		if ( switchs_[ i ]->phase != Phase::Before )
		{
			switchs_[ i ]->light->Update();
			light_->SetPointActive(switchs_[ i ]->lightIndex,true);
			if ( switchs_[ i ]->phase == Phase::Middle )
			{
				switchs_[ i ]->lightFrame++;

				float f = ( float ) switchs_[ i ]->lightFrame / spotLightMaxFrame;

				f *= 1.5f;

				switchs_[ i ]->light->SetScale({ f,f,f });

				if ( switchs_[ i ]->lightFrame >= lightMaxFrame )
				{
					switchs_[ i ]->phase = Phase::After;
				}
			}
		}
		for ( uint32_t j = 0; j < gimmicks_.size(); j++ )
		{
			if ( switchs_[ i ]->index == gimmicks_[ j ]->index && switchs_[ i ]->phase != Phase::Before )
			{
				if ( gimmicks_[ j ]->phase == Phase::Before )
				{
					gimmicks_[ j ]->phase = Phase::Middle;
					gimmicks_[ j ]->EndPosY = gimmicks_[ j ]->obj->GetPosition().y - gimmicks_[ j ]->obj->GetScale().y * 2;
				}
				if ( gimmicks_[ j ]->phase == Phase::Middle )
				{
					Vector3 pos = gimmicks_[ j ]->obj->GetPosition();

					pos.y -= 0.1f;

					if ( pos.y <= gimmicks_[ j ]->EndPosY )
					{
						pos.y = gimmicks_[ j ]->EndPosY;
						gimmicks_[ j ]->phase = Phase::After;
					}
					gimmicks_[ j ]->obj->SetPosition(pos);
				}
			}
			gimmicks_[ j ]->obj->Update();
		}
	}

	for ( uint32_t i = 0; i < middles_.size(); i++ )
	{
		if ( middles_[ i ]->phase == Phase::Middle )
		{
			middles_[ i ]->Frame++;

			float f = middles_[ i ]->Frame / goalOpenMaxFlame;

			middles_[ i ]->obj->SetColor({ f,f,f });

			if ( middles_[ i ]->Frame >= goalOpenMaxFlame )
			{
				middles_[ i ]->phase = Phase::After;
			}
		}

		middles_[ i ]->obj->Update();
	}

	light_->Update();

	goalObj_->Update();

	stage1Obj_->Update();

	stage1BillBoard_->Update();

	stage2BillBoard_->Update();

	stage2Obj_->Update();

	stage3Obj_->Update();

	skyObj_->Update();

	particleManager_->Update();
}

void GameScene::Draw(DirectXCommon* dxCommon)
{
	Object3d::PreDraw(dxCommon->GetCommandList());

	skyObj_->Draw();

	for ( uint32_t i = 0; i < objects_.size(); i++ )
	{
		objects_[ i ]->Draw();
	}

	for ( uint32_t i = 0; i < planes_.size(); i++ )
	{
		planes_[ i ]->plane->Draw();
	}
	for ( uint32_t i = 0; i < tutorials_.size(); i++ )
	{
		if ( tutorials_[ i ]->isDraw )
		{
			tutorials_[ i ]->obj->Draw(tutorials_[ i ]->texHandle);
		}
	}
	for ( std::unique_ptr<Switch>& Switch : switchs_ )
	{

		if ( Switch->phase != Phase::Before )
		{
			Switch->light->Draw();
		}
		Switch->obj->Draw();
	}

	for ( std::unique_ptr<Gimmick>& gimmick : gimmicks_ )
	{
		gimmick->obj->Draw();
	}

	for ( uint32_t i = 0; i < middles_.size(); i++ )
	{
		middles_[ i ]->obj->Draw();
	}

	EnemyManager::Instance()->Draw();

	player_->Draw();
	if ( mapName_ == "Resources/Select.json" )
	{
		stage1Obj_->Draw();
		stage1BillBoard_->Draw();
		stage2Obj_->Draw();
		stage2BillBoard_->Draw();
		stage3Obj_->Draw();
	}
	else
	{
		goalObj_->Draw();
	}
	for ( std::unique_ptr<GoalSwitch>& goalSwitch : goalSwitchs_ )
	{
		if ( goalSwitch->phase != Phase::Before )
		{
			goalSwitch->light->Draw();
			goalSwitch->spotLight->Draw(spotLightTex);
		}
		goalSwitch->obj->Draw();
	}

	Object3d::PostDraw();

	ParticleManager::PreDraw(dxCommon->GetCommandList());
	player_->ParticleDraw();
	EnemyManager::Instance()->ParticleDraw();
	particleManager_->Draw();
	ParticleManager::PostDraw();
}

void GameScene::SpriteDraw()
{
	SpriteCommon::Instance()->PreDraw();
	sousaSprite_->Draw();
	player_->SpriteDraw();
	sceneSprite_->DissolveDraw();
	if ( frame_ <= 0 && player_->IsDaed() )
	{
		retrySprite_->Draw();
		yesSprite_->Draw();
		noSprite_->Draw();
		yazirusiSprite_->Draw();
		spaceSprite_->Draw();
	}
	SpriteCommon::Instance()->PostDraw();
}

void GameScene::Finalize()
{
	EnemyManager::Instance()->Fin();

	PlayerBulletManager::Instance()->Fin();
}

void GameScene::MapLoad(std::string mapFullpath,bool middleSwitchRLoad)
{
	std::unique_ptr<LevelData> levelData;
	levelData.reset(LevelLoad::Load(mapFullpath));

	for ( int i = 0; i < LightGroup::cPointLightNum; i++ )
	{
		light_->SetPointActive(i,false);
	}

	objects_.clear();

	planes_.clear();

	goalSwitchs_.clear();

	switchs_.clear();

	gimmicks_.clear();

	tutorials_.clear();

	cameras_.clear();

	if ( !middleSwitchRLoad )
	{
		middles_.clear();
	}

	EnemyManager::Instance()->Clear();

	mapName_ = mapFullpath;

	uint32_t EnemyNumber = 0;

	player_->Reset();

	for ( auto& objectData : levelData->objects )
	{
		std::string tagName;

		std::stringstream tag{ objectData.tagName };

		std::getline(tag,tagName,' ');

		if ( tagName == "Map" )
		{
			//モデルを指定して3Dオブジェクトを生成
			std::unique_ptr<Object3d> newObject = std::make_unique<Object3d>();
			newObject->Initialize();
			newObject->SetModel(models_[ objectData.fileName ].get());

			assert(newObject);

			// 座標
			newObject->SetPosition({ objectData.trans });

			// 回転角
			newObject->SetRot({ objectData.rot });

			// 座標
			newObject->SetScale({ objectData.scale });

			std::vector<Model::VertexPosNormalUv> vertices = newObject->GetVertices();

			for ( int i = 0; i < vertices.size(); i += 3 )
			{
				Model::VertexPosNormalUv Vertex0,Vertex1,Vertex2;

				Vertex0 = vertices[ i ];
				Vertex1 = vertices[ i + 1 ];
				Vertex2 = vertices[ i + 2 ];

				if ( Vertex0.pos.x == Vertex1.pos.x && Vertex0.pos.x == Vertex2.pos.x )
				{
					Vertex0.uv.x *= objectData.scale.z;
					Vertex0.uv.y *= objectData.scale.y;

					Vertex1.uv.x *= objectData.scale.z;
					Vertex1.uv.y *= objectData.scale.y;

					Vertex2.uv.x *= objectData.scale.z;
					Vertex2.uv.y *= objectData.scale.y;

					Vertex0.uv /= 2;
					Vertex1.uv /= 2;
					Vertex2.uv /= 2;
				}
				else if ( Vertex0.pos.y == Vertex1.pos.y && Vertex0.pos.y == Vertex2.pos.y )
				{
					Vertex0.uv.x *= objectData.scale.z;
					Vertex0.uv.y *= objectData.scale.x;

					Vertex1.uv.x *= objectData.scale.z;
					Vertex1.uv.y *= objectData.scale.x;

					Vertex2.uv.x *= objectData.scale.z;
					Vertex2.uv.y *= objectData.scale.x;

					Vertex0.uv /= 2;
					Vertex1.uv /= 2;
					Vertex2.uv /= 2;
				}
				else if ( Vertex0.pos.z == Vertex1.pos.z && Vertex0.pos.z == Vertex2.pos.z )
				{
					Vertex0.uv.x *= objectData.scale.y;
					Vertex0.uv.y *= objectData.scale.x;

					Vertex1.uv.x *= objectData.scale.y;
					Vertex1.uv.y *= objectData.scale.x;

					Vertex2.uv.x *= objectData.scale.y;
					Vertex2.uv.y *= objectData.scale.x;

					Vertex0.uv /= 2;
					Vertex1.uv /= 2;
					Vertex2.uv /= 2;
				}

				vertices[ i ] = Vertex0;
				vertices[ i + 1 ] = Vertex1;
				vertices[ i + 2 ] = Vertex2;
			}

			newObject->SetVertices(vertices);

			// 配列に登録
			objects_.push_back(std::move(newObject));
		}
		if ( tagName == "MapPlane" )
		{
			//モデルを指定して3Dオブジェクトを生成
			std::unique_ptr<Plane> newObject = std::make_unique<Plane>();
			newObject->plane = std::make_unique<Object3d>();
			newObject->plane->Initialize();
			newObject->plane->SetModel(models_[ "plane" ].get());

			// 座標
			newObject->plane->SetPosition({ objectData.trans });

			// 回転角
			newObject->plane->SetRot({ objectData.rot });

			// 座標
			newObject->plane->SetScale({ objectData.scale });

			newObject->UVSift = { ( float ) ( rand() % 1000 ) / 1000,0 };

			newObject->UVSiftSpeed = { 0.02f ,0 };
		// 配列に登録
			planes_.push_back(std::move(newObject));
		}
		if ( tagName == "MapTutorial" )
		{
			//モデルを指定して3Dオブジェクトを生成
			std::unique_ptr<Tutorial> newObject = std::make_unique<Tutorial>();
			newObject->obj = std::make_unique<Object3d>();
			newObject->obj->Initialize();
			newObject->obj->SetModel(models_[ "plane" ].get());

			newObject->texHandle = TextureManager::Instance()->LoadTexture("Resources/" + objectData.fileName + ".png");

			assert(newObject);

			// 座標
			newObject->obj->SetPosition({ objectData.trans });

			// 回転角
			newObject->obj->SetRot({ objectData.rot });

			// 座標
			newObject->obj->SetScale({ objectData.scale });
			// 配列に登録
			tutorials_.push_back(std::move(newObject));
		}
		if ( tagName == "goalSwitch" )
		{
			//モデルを指定して3Dオブジェクトを生成
			std::unique_ptr<GoalSwitch> newObject = std::make_unique<GoalSwitch>();
			newObject->obj = std::make_unique<Object3d>();
			newObject->obj->Initialize();
			newObject->obj->SetModel(models_[ objectData.fileName ].get());

			assert(newObject);

			// 座標
			newObject->obj->SetPosition({ objectData.trans });

			// 回転角
			newObject->obj->SetRot({ objectData.rot });

			// 座標
			newObject->obj->SetScale({ objectData.scale });

			newObject->light = std::make_unique<Object3d>();

			newObject->light->Initialize();

			newObject->light->SetModel(models_[ "enemyBullet" ].get());

			newObject->spotLight = std::make_unique<Object3d>();

			newObject->spotLight->Initialize();

			newObject->spotLight->SetModel(models_[ "plane" ].get());

			Vector3 pos = objectData.trans;

			pos.y += 2;

			newObject->light->SetPosition(pos);

			for ( int i = 0; i < LightGroup::cPointLightNum; i++ )
			{
				if ( light_->GetPointActive(i) == false )
				{
					newObject->lightIndex = i;

					break;
				}
			}
			newObject->obj->Update();

			newObject->light->Update();

			light_->SetPointActive(newObject->lightIndex,true);
		// 配列に登録
			goalSwitchs_.push_back(std::move(newObject));
		}
		if ( tagName == "Switch" )
		{
			//モデルを指定して3Dオブジェクトを生成
			std::unique_ptr<Switch> newObject = std::make_unique<Switch>();
			newObject->obj = std::make_unique<Object3d>();
			newObject->obj->Initialize();
			newObject->obj->SetModel(models_[ objectData.fileName ].get());
			// 座標
			newObject->obj->SetPosition({ objectData.trans });
			// 回転角
			newObject->obj->SetRot({ objectData.rot });
			// 座標
			newObject->obj->SetScale({ objectData.scale });

			newObject->light = std::make_unique<Object3d>();
			newObject->light->Initialize();
			newObject->light->SetModel(models_[ "enemyBullet" ].get());

			Vector3 pos = objectData.trans;
			//ライトの位置を中心から少し上に調整
			pos.y += 2;

			newObject->light->SetPosition(pos);

			for ( int i = 0; i < LightGroup::cPointLightNum; i++ )
			{
				if ( light_->GetPointActive(i) == false )
				{
					newObject->lightIndex = i;

					break;
				}
			}
			newObject->obj->Update();

			newObject->light->Update();

			light_->SetPointActive(newObject->lightIndex,true);

			std::getline(tag,tagName,' ');

			newObject->index = std::atoi(tagName.c_str());
			// 配列に登録
			switchs_.push_back(std::move(newObject));
		}
		if ( tagName == "Gimmick" )
		{
			//モデルを指定して3Dオブジェクトを生成
			std::unique_ptr<Gimmick> newObject = std::make_unique<Gimmick>();
			newObject->obj = std::make_unique<Object3d>();
			newObject->obj->Initialize();
			newObject->obj->SetModel(models_[ objectData.fileName ].get());
			// 座標
			newObject->obj->SetPosition({ objectData.trans });
			// 回転角
			newObject->obj->SetRot({ objectData.rot });
			// 座標
			newObject->obj->SetScale({ objectData.scale });

			newObject->obj->Update();

			std::getline(tag,tagName,' ');

			newObject->index = std::atoi(tagName.c_str());
			// 配列に登録
			gimmicks_.push_back(std::move(newObject));
		}
		if ( tagName == "camera" )
		{
			std::unique_ptr<Camera> camera;

			camera = std::make_unique<Camera>();

			camera->pos = objectData.trans;

			camera->rot = objectData.size;

			camera->scale = objectData.scale;

			cameras_.push_back(std::move(camera));
		}
		if ( !middleSwitchRLoad )
		{
			if ( tagName == "Middle" )
			{
				std::unique_ptr<Middle> newObject = std::make_unique<Middle>();
				newObject->obj = std::make_unique<Object3d>();

				newObject->obj->Initialize();
				newObject->obj->SetModel(models_[ objectData.fileName ].get());
				// 座標
				newObject->obj->SetPosition({ objectData.trans });
				// 回転角
				newObject->obj->SetRot({ objectData.rot });
				// 座標
				newObject->obj->SetScale({ objectData.scale });

				newObject->obj->SetColor({ 0,0,0 });

				middles_.push_back(std::move(newObject));

			}
		}
		if ( tagName == "Spawn" )
		{
			if ( !middleSwitchRLoad )
			{
				player_->SetSpawnPos(objectData.trans);

				player_->RSpawn();
			}
			else
			{
				player_->RSpawn();
			}
			cameraEnd_ = objectData.trans;
		}
		if ( tagName == "Goal" )
		{
			//player_->SetGoal(objectData.trans, objectData.scale);

			//モデルを指定して3Dオブジェクトを生成
			goalObj_->SetModel(models_[ objectData.fileName ].get());

			// 座標
			goalObj_->SetPosition({ objectData.trans });

			// 回転角
			goalObj_->SetRot({ objectData.rot });

			// 座標
			goalObj_->SetScale({ objectData.scale });

			goalObj_->SetColor({ 0,0,0 });

			goalObj_->Update();

			Object3d::SetEye(objectData.trans + cameraPos_);

			Object3d::SetTarget(objectData.trans);

			isGoal_ = true;

			cameraStart_ = objectData.trans;
		}
		if ( tagName == "GunEnemy" )
		{
			std::unique_ptr<Enemy> enemy;

			enemy = std::make_unique<GunEnemy>();

			enemy->Initialize(models_[ objectData.fileName ].get(),models_[ "enemyBullet" ].get(),player_.get(),EnemyNumber++);

			enemy->GetObj()->SetPosition(objectData.trans);

			enemy->GetObj()->SetRot(objectData.rot);

			enemy->Update();

			EnemyManager::Instance()->AddEnemy(std::move(enemy));
		}
		if ( tagName == "RunEnemy" )
		{
			std::unique_ptr<Enemy> enemy;

			enemy = std::make_unique<RunEnemy>();

			enemy->Initialize(models_[ objectData.fileName ].get(),models_[ "enemyBullet" ].get(),player_.get(),EnemyNumber++);

			enemy->GetObj()->SetPosition(objectData.trans);

			enemy->GetObj()->SetRot(objectData.rot);

			enemy->Update();

			EnemyManager::Instance()->AddEnemy(std::move(enemy));
		}
		if ( tagName == "jumpEnemy" )
		{
			std::unique_ptr<Enemy> enemy;

			enemy = std::make_unique<jumpEnemy>();

			enemy->Initialize(models_[ objectData.fileName ].get(),models_[ "enemyBullet" ].get(),player_.get(),EnemyNumber++);

			enemy->GetObj()->SetPosition(objectData.trans);

			enemy->GetObj()->SetRot(objectData.rot);

			enemy->Update();

			EnemyManager::Instance()->AddEnemy(std::move(enemy));
		}
		if ( tagName == "tutorialEnemy" )
		{
			std::unique_ptr<Enemy> enemy;

			enemy = std::make_unique<TutorialEnemy>();

			enemy->Initialize(models_[ objectData.fileName ].get(),models_[ "enemyBullet" ].get(),player_.get(),EnemyNumber++);

			enemy->GetObj()->SetPosition(objectData.trans);

			enemy->GetObj()->SetRot(objectData.rot);

			enemy->Update();

			EnemyManager::Instance()->AddEnemy(std::move(enemy));
		}
		if ( tagName == "WallEnemy" )
		{
			std::unique_ptr<Enemy> enemy;

			enemy = std::make_unique<WallEnemy>();

			enemy->Initialize(models_[ objectData.fileName ].get(),models_[ "enemyBullet" ].get(),player_.get(),EnemyNumber++);

			enemy->GetObj()->SetPosition(objectData.trans);

			enemy->GetObj()->SetRot(objectData.rot);

			enemy->GetObj()->SetScale(objectData.scale);

			std::vector<Model::VertexPosNormalUv> vertices = enemy->GetObj()->GetVertices();

			for ( int i = 0; i < vertices.size(); i += 3 )
			{
				Model::VertexPosNormalUv Vertex0,Vertex1,Vertex2;

				Vertex0 = vertices[ i ];
				Vertex1 = vertices[ i + 1 ];
				Vertex2 = vertices[ i + 2 ];

				if ( Vertex0.pos.x == Vertex1.pos.x && Vertex0.pos.x == Vertex2.pos.x )
				{
					Vertex0.uv.x *= enemy->GetObj()->GetScale().z;
					Vertex0.uv.y *= enemy->GetObj()->GetScale().y;

					Vertex1.uv.x *= enemy->GetObj()->GetScale().z;
					Vertex1.uv.y *= enemy->GetObj()->GetScale().y;

					Vertex2.uv.x *= enemy->GetObj()->GetScale().z;
					Vertex2.uv.y *= enemy->GetObj()->GetScale().y;

					Vertex0.uv /= 2;
					Vertex1.uv /= 2;
					Vertex2.uv /= 2;
				}
				else if ( Vertex0.pos.y == Vertex1.pos.y && Vertex0.pos.y == Vertex2.pos.y )
				{
					Vertex0.uv.x *= enemy->GetObj()->GetScale().z;
					Vertex0.uv.y *= enemy->GetObj()->GetScale().x;

					Vertex1.uv.x *= enemy->GetObj()->GetScale().z;
					Vertex1.uv.y *= enemy->GetObj()->GetScale().x;

					Vertex2.uv.x *= enemy->GetObj()->GetScale().z;
					Vertex2.uv.y *= enemy->GetObj()->GetScale().x;

					Vertex0.uv /= 2;
					Vertex1.uv /= 2;
					Vertex2.uv /= 2;
				}
				else if ( Vertex0.pos.z == Vertex1.pos.z && Vertex0.pos.z == Vertex2.pos.z )
				{
					Vertex0.uv.x *= enemy->GetObj()->GetScale().y;
					Vertex0.uv.y *= enemy->GetObj()->GetScale().x;

					Vertex1.uv.x *= enemy->GetObj()->GetScale().y;
					Vertex1.uv.y *= enemy->GetObj()->GetScale().x;

					Vertex2.uv.x *= enemy->GetObj()->GetScale().y;
					Vertex2.uv.y *= enemy->GetObj()->GetScale().x;

					Vertex0.uv /= 2;
					Vertex1.uv /= 2;
					Vertex2.uv /= 2;
				}

				vertices[ i ] = Vertex0;
				vertices[ i + 1 ] = Vertex1;
				vertices[ i + 2 ] = Vertex2;
			}

			enemy->GetObj()->SetVertices(vertices);

			enemy->Update();

			EnemyManager::Instance()->AddEnemy(std::move(enemy));
		}
		if ( tagName == "Stage1" )
		{
			//モデルを指定して3Dオブジェクトを生成
			stage1Obj_->SetModel(models_[ objectData.fileName ].get());

			// 座標
			stage1Obj_->SetPosition({ objectData.trans });

			stage1BillBoard_->SetPosition({ objectData.trans.x,objectData.trans.y + 5.0f,objectData.trans.z });

			// 回転角
			stage1Obj_->SetRot({ objectData.rot });

			// 座標
			stage1Obj_->SetScale({ objectData.scale });

			stage1Obj_->Setalpha(0.3f);

			stage1Obj_->Update();
		}
		if ( tagName == "Stage2" )
		{
			//モデルを指定して3Dオブジェクトを生成
			stage2Obj_->SetModel(models_[ objectData.fileName ].get());

			// 座標
			stage2Obj_->SetPosition({ objectData.trans });

			stage2BillBoard_->SetPosition({ objectData.trans.x,objectData.trans.y + 5.0f,objectData.trans.z });

			// 回転角
			stage2Obj_->SetRot({ objectData.rot });

			// 座標
			stage2Obj_->SetScale({ objectData.scale });

			stage2Obj_->Setalpha(0.3f);

			stage2Obj_->Update();
		}
		if ( tagName == "Stage3" )
		{
			//モデルを指定して3Dオブジェクトを生成
			stage3Obj_->SetModel(models_[ objectData.fileName ].get());

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

	isGoal_ = false;

	std::sort(switchs_.rbegin(),switchs_.rend());

	std::sort(gimmicks_.rbegin(),gimmicks_.rend());

	for ( std::unique_ptr<GoalSwitch>& goalSwitch : goalSwitchs_ )
	{
		light_->SetPointActive(goalSwitch->lightIndex,false);

	}

	for ( std::unique_ptr<Switch>& Switch : switchs_ )
	{
		light_->SetPointActive(Switch->lightIndex,false);

	}
}

void GameScene::ModelLoad()
{
	std::unique_ptr<Model> bulletModel_;
	bulletModel_.reset(Model::LoadFormOBJ("Bullet",true));
	models_.insert(std::make_pair("enemyBullet",std::move(bulletModel_)));

	std::unique_ptr<Model> mapModel_;
	mapModel_.reset(Model::LoadFormOBJ("Map",true));
	models_.insert(std::make_pair("Map",std::move(mapModel_)));

	std::unique_ptr<Model> goalModel_;
	goalModel_.reset(Model::LoadFormOBJ("Goal",true));
	models_.insert(std::make_pair("Goal",std::move(goalModel_)));

	std::unique_ptr<Model> enemyModel_;
	enemyModel_.reset(Model::LoadFormOBJ("RunEnemy",true));
	models_.insert(std::make_pair("RunEnemy",std::move(enemyModel_)));

	std::unique_ptr<Model> gunEnemyModel_;
	gunEnemyModel_.reset(Model::LoadFormOBJ("GunEnemy",true));
	models_.insert(std::make_pair("GunEnemy",std::move(gunEnemyModel_)));

	std::unique_ptr<Model> jumpEnemyModel_;
	jumpEnemyModel_.reset(Model::LoadFormOBJ("jumpEnemy",true));
	models_.insert(std::make_pair("jumpEnemy",std::move(jumpEnemyModel_)));

	std::unique_ptr<Model> tutorialEnemyModel_;
	tutorialEnemyModel_.reset(Model::LoadFormOBJ("tutorialEnemy",true));
	models_.insert(std::make_pair("tutorialEnemy",std::move(tutorialEnemyModel_)));

	std::unique_ptr<Model> stageModel_;
	stageModel_.reset(Model::LoadFormOBJ("stage",true));
	models_.insert(std::make_pair("stage",std::move(stageModel_)));

	std::unique_ptr<Model> planeModel_;
	planeModel_.reset(Model::CreatePlaneModel(TextureManager::Instance()->LoadTexture("Resources/plane.png")));
	models_.insert(std::make_pair("plane",std::move(planeModel_)));

	std::unique_ptr<Model> goalSwitchModel_;
	goalSwitchModel_.reset(Model::LoadFormOBJ("GoalSwitch",true));
	models_.insert(std::make_pair("GoalSwitch",std::move(goalSwitchModel_)));

	std::unique_ptr<Model> fenceModel_;
	fenceModel_.reset(Model::LoadFormOBJ("fence",true));
	models_.insert(std::make_pair("fence",std::move(fenceModel_)));

	std::unique_ptr<Model> blockModel_;
	blockModel_.reset(Model::LoadFormOBJ("block",true));
	models_.insert(std::make_pair("block",std::move(blockModel_)));
}

void GameScene::SwitchCollsion()
{
	for ( std::unique_ptr<PlayerBullet>& bullet : PlayerBulletManager::Instance()->GetBullets() )
	{
		Collider::Cube bulletCube;
		bulletCube.Pos = bullet->GetPosition();
		bulletCube.scale = bullet->GetScale();
		for ( std::unique_ptr<GoalSwitch>& goalSwitch : goalSwitchs_ )
		{
			if ( goalSwitch->phase == Phase::Before )
			{
				Collider::Cube mapCube;
				mapCube.Pos = goalSwitch->obj->GetPosition();
				mapCube.scale = goalSwitch->obj->GetScale();
				if ( Collider::CubeAndCube(mapCube,bulletCube,Collider::Collsion) == true )
				{
					bullet->OnCollision();

					goalSwitch->phase = Phase::Middle;

					light_->SetPointActive(goalSwitch->lightIndex,true);

					light_->SetPointPos(goalSwitch->lightIndex,goalSwitch->light->GetPosition());

					light_->SetPointColor(goalSwitch->lightIndex,{ 1,0.88f,0.59f });

					light_->SetPointAtten(goalSwitch->lightIndex,{ 0.03f,0.01f,0.01f });

					Vector3 pos = goalObj_->GetPosition() - goalSwitch->obj->GetPosition();

					Vector3 cameForward = { 0,0,-1 };

					Vector3  cameRight = { 1,0,0 };

					Vector3 frontVec = { 0,0,0 };

					if ( pos.x != 0.0f || pos.z != 0.0f )
					{
						frontVec = cameForward * pos.z + cameRight * pos.x;
					}

					if ( frontVec.x != 0.0f || frontVec.z != 0.0f )
					{
						goalSwitch->spotLight->SetRot({ 0, atan2f(frontVec.x, -frontVec.z) - 3.14f,3.14f });
					}

					float a = pos.length() / 2.5f;

					goalSwitch->spotLight->SetScale({ a / 5,1,a });

					pos = pos / 2 + goalSwitch->obj->GetPosition();

					pos.y = goalSwitch->obj->GetPosition().y + 0.1f;

					goalSwitch->spotLight->SetPosition(pos);

					goalSwitch->spotLight->Setalpha(0.3f);

					break;
				}
			}
		}
		for ( std::unique_ptr<Switch>& Switch : switchs_ )
		{
			if ( Switch->phase == Phase::Before )
			{
				Collider::Cube mapCube;
				mapCube.Pos = Switch->obj->GetPosition();
				mapCube.scale = Switch->obj->GetScale();
				if ( Collider::CubeAndCube(mapCube,bulletCube,Collider::Collsion) == true )
				{
					bullet->OnCollision();

					Switch->phase = Phase::Middle;

					Switch->light->SetScale({ 0,0,0 });

					light_->SetPointActive(Switch->lightIndex,true);

					light_->SetPointPos(Switch->lightIndex,Switch->light->GetPosition());

					light_->SetPointColor(Switch->lightIndex,{ 1,0.88f,0.59f });

					light_->SetPointAtten(Switch->lightIndex,{ 0.03f,0.01f,0.01f });
					break;
				}
			}
		}
	}
	Collider::Cube playerCube;
	playerCube.Pos = player_->GetObj()->GetPosition();
	playerCube.scale = player_->GetObj()->GetScale();
	for ( uint32_t i = 0; i < cameras_.size(); i++ )
	{
		Vector3 pos = cameras_[ i ]->rot;

		pos *= 40;

		if ( player_->GetEndCameraPos() != pos )
		{
			Collider::Cube cameraCube;
			cameraCube.Pos = cameras_[ i ]->pos;
			cameraCube.scale = cameras_[ i ]->scale;
			if ( Collider::CubeAndCube(playerCube,cameraCube,Collider::Collsion) == true )
			{
				player_->SetCameraPos(pos);
			}
		}
	}

	for ( uint32_t i = 0; i < middles_.size(); i++ )
	{
		if ( middles_[ i ]->phase == Phase::Before )
		{
			Collider::Cube middleCube;
			middleCube.Pos = middles_[ i ]->obj->GetPosition();
			middleCube.scale = middles_[ i ]->obj->GetScale();
			if ( Collider::CubeAndCube(playerCube,middleCube,Collider::Collsion) == true )
			{
				middles_[ i ]->phase = Phase::Middle;

				player_->SetSpawnPos(middles_[ i ]->obj->GetPosition());
			}
		}
	}
}
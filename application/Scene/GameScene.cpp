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

	stage1Plane_.reset(Model::CreatePlaneModel(TextureManager::Instance()->LoadTexture("Resources/1-1.png")));

	stage1BillBoard_ = std::make_unique<Object3d>();

	stage1BillBoard_->Initialize();

	stage1BillBoard_->SetModel(stage1Plane_.get());

	stage1BillBoard_->SetScale({2,4,1});

	stage1BillBoard_->SetRot({45,0,0});

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

	player_->SetLight(light_.get());

	player_->SetMapData(&objects_);

	EnemyManager::Instance()->SetMapData(&objects_);

	EnemyManager::Instance()->SetPlayer(player_.get());
	player_->ObjectUpdate();
	EnemyManager::Instance()->Update();

	Enemy::SetLight(light_.get());

	EnemyBullet::SetLight(light_.get());
}

void GameScene::Update()
{
#ifdef _DEBUG
	ImGui::Begin("Scene");

	ImGui::SliderFloat3("light",&lightV.x,-1,1,"%3.1f");

	ImGui::End();
#endif

	if ( Input::Instance()->TriggerKey(DIK_0) )
	{
		EnemyManager::Instance()->Clear();

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
				isStage2_ = true;
				sceneChange_ = true;
			}

			B.Pos = stage3Obj_->GetPosition();

			B.scale = stage3Obj_->GetScale();

			if ( Collider::CubeAndCube(A,B,Collider::Collsion) )
			{
				isNext_ = true;
				isStage3_ = true;
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
		else if ( frame_ <= -5 )
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
					MapLoad("Resources/Stage1.json");

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
					MapLoad("Resources/Stage2.json");

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
					MapLoad("Resources/Stage3.json");

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

	for (uint32_t i=0;i< planes_.size();i++)
	{
		UVSift_[i].x +=0.02f;

		if ( UVSift_[i].x >= 1 )
		{
			UVSift_[ i ].x = 0;
		}

		planes_[ i ]->SetUVShift(UVSift_[ i ]);

		planes_[ i ]->Update();
	}
	//light_->SetDirLightDir(0,lightV);

	light_->Update();

	goalObj_->Update();

	stage1Obj_->Update();

	stage1BillBoard_->Update();

	stage2BillBoard_->Update();

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

	for ( uint32_t i = 0; i < planes_.size(); i++ )
	{
		planes_[ i ]->Draw();
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

	Object3d::PostDraw();

	ParticleManager::PreDraw(dxCommon->GetCommandList());
	player_->ParticleDraw();
	EnemyManager::Instance()->ParticleDraw();
	ParticleManager::PostDraw();
}

void GameScene::SpriteDraw()
{
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

	planes_.clear();

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
		if ( objectData.tagName == "MapPlane" )
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

			//std::vector<Model::VertexPosNormalUv> vertices = newObject->GetVertices();

			//vertices[ 0 ].uv.x *= objectData.scale.x;
			//vertices[ 1 ].uv.x *= objectData.scale.x;
			//vertices[ 2 ].uv.x *= objectData.scale.x;
			//vertices[ 3 ].uv.x *= objectData.scale.x;

			//float UVScale = ( float ) ( rand() % 5 + 5 );

			//vertices[ 0 ].uv.x /= UVScale;
			//vertices[ 1 ].uv.x /= UVScale;
			//vertices[ 2 ].uv.x /= UVScale;
			//vertices[ 3 ].uv.x /= UVScale;

			//newObject->SetVertices(vertices);

			Vector2 UVSift = { ( float ) ( rand() % 1000 ) / 1000,0 };

			UVSift_.push_back(UVSift);

			Vector2 UVSiftSpeed = { ( float ) ( rand() % 100)/3000 ,0};

			UVSiftSpeed_.push_back(UVSiftSpeed);

			// 配列に登録
			planes_.push_back(std::move(newObject));
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

			enemy->Initialize(models_[ objectData.fileName ],models_[ "enemyBullet" ],{ objectData.trans },player_.get());

			enemy->Update();

			EnemyManager::Instance()->AddEnemy(std::move(enemy));
		}
		if ( objectData.tagName == "RunEnemy" )
		{
			std::unique_ptr<Enemy> enemy;

			enemy = std::make_unique<RunEnemy>();

			enemy->Initialize(models_[ objectData.fileName ],models_[ "enemyBullet" ],{ objectData.trans },player_.get());

			enemy->Update();

			EnemyManager::Instance()->AddEnemy(std::move(enemy));
		}
		if ( objectData.tagName == "jumpEnemy" )
		{
			std::unique_ptr<Enemy> enemy;

			enemy = std::make_unique<jumpEnemy>();

			enemy->Initialize(models_[ objectData.fileName ],models_[ "enemyBullet" ],{ objectData.trans },player_.get());

			enemy->Update();

			EnemyManager::Instance()->AddEnemy(std::move(enemy));
		}
		if ( objectData.tagName == "Stage1" )
		{
			//モデルを指定して3Dオブジェクトを生成
			stage1Obj_->SetModel(models_[ objectData.fileName ]);

			// 座標
			stage1Obj_->SetPosition({ objectData.trans });

			stage1BillBoard_->SetPosition({ objectData.trans.x,objectData.trans.y+5.0f,objectData.trans.z });

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

			stage2BillBoard_->SetPosition({ objectData.trans.x,objectData.trans.y + 5.0f,objectData.trans.z });

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

	enemyModel_.reset(Model::LoadFormOBJ("RunEnemy",true));
	models_.insert(std::make_pair("RunEnemy",enemyModel_.get()));

	gunEnemyModel_.reset(Model::LoadFormOBJ("GunEnemy",true));
	models_.insert(std::make_pair("GunEnemy",gunEnemyModel_.get()));

	jumpEnemyModel_.reset(Model::LoadFormOBJ("jumpEnemy",true));
	models_.insert(std::make_pair("jumpEnemy",jumpEnemyModel_.get()));

	stageModel_.reset(Model::LoadFormOBJ("stage",true));
	models_.insert(std::make_pair("stage",stageModel_.get()));

	planeModel_.reset(Model::CreatePlaneModel(TextureManager::Instance()->LoadTexture("Resources/MapTex.png")));
	models_.insert(std::make_pair("plane",planeModel_.get()));
}

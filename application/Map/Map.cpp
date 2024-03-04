#include "Map.h"
#include"TextureManager.h"
#include"Collider.h"
#include"PlayerBulletManager.h"
#include"GunEnemy.h"
#include"RunEnemy.h"
#include"jumpEnemy.h"
#include"tutorialEnemy.h"
#include"Enemy.h"
#include"WallEnemy.h"
#include<iostream>
#include"EnemyManager.h"
#include"levelLoad.h"
void Map::Initialize(Player* player,LightGroup* light)
{
	player_ = player;

	light_ = light;

	ModelLoad();

	goalObj_ = std::make_unique<Object3d>();

	goalObj_->Initialize();

	particleManager_ = std::make_unique<ParticleManager>();

	particleManager_->Initialize();

	particleManager_->SetTextureHandle(TextureManager::Instance()->LoadTexture("Resources/Texture/effect4.png"));

	spotLightTex_ = TextureManager::Instance()->LoadTexture("Resources/Texture/lightplane.png");
}
void Map::Update()
{
	for ( std::unique_ptr<GoalSwitch>& goalSwitch : goalSwitchs_ )
	{
		if ( goalSwitch->phase == Phase::After )
		{
			goalOpen_ = Phase::Middle;
		}
		else
		{
			goalOpen_ = Phase::Before;
			break;
		}

	}
	if ( goalOpen_ == Phase::Middle )
	{
		goalOpenFlame_++;

		float f = goalOpenFlame_ / cEndGoalOpenFlame_;

		goalObj_->SetColor({ f,f,f });

		if ( goalOpenFlame_ >= cEndGoalOpenFlame_ )
		{
			goalOpen_ = Phase::After;
		}
	}
	Collider::Cube A,B;

	A.Pos = player_->GetObj()->GetPosition();

	A.scale = player_->GetObj()->GetScale();
	if ( goalOpen_ == Phase::After )
	{
		B.Pos = goalObj_->GetPosition();

		B.scale = goalObj_->GetScale();

		if ( Collider::CubeAndCube(A,B,Collider::Collsion) )
		{
			isClear_ = true;
		}
	}
	ObjectUpdate();
}
void Map::Draw()
{
	for ( uint32_t i = 0; i < objects_.size(); i++ )
	{
		objects_[ i ]->Draw();
	}

	for ( uint32_t i = 0; i < planes_.size(); i++ )
	{
		planes_[ i ]->plane->Draw(planes_[ i ]->texHandle);
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

		if ( Switch->phase != Map::Phase::Before )
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
	goalObj_->Draw();
	for ( std::unique_ptr<GoalSwitch>& goalSwitch : goalSwitchs_ )
	{
		if ( goalSwitch->phase != Map::Phase::Before )
		{
			goalSwitch->light->Draw();
			goalSwitch->spotLight->Draw(spotLightTex_);
		}
		goalSwitch->obj->Draw();
	}
}

void Map::ParticleDraw()
{
	particleManager_->Draw();
}

void Map::MonsterHouseUpdate()
{

}

void Map::ObjectUpdate()
{
	for ( std::unique_ptr<Object3d>& obj : objects_ )
	{
		obj->Update();

	}

	planeUpdate();

	tutorialUpdate();

	SwitchCollsion();

	goalSwitchUpdate();

	switchUpdate();

	middleUpdate();

	light_->Update();

	goalObj_->Update();
}

void Map::ModelLoad()
{
	std::unique_ptr<Model> bulletModel_;
	bulletModel_.reset(Model::LoadFormOBJ("Bullet",true));
	models_.insert(std::make_pair("enemyBullet",std::move(bulletModel_)));

	std::unique_ptr<Model> mapModel_;
	mapModel_.reset(Model::LoadFormOBJ("MapBlock",true));
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
	planeModel_.reset(Model::CreatePlaneModel(TextureManager::Instance()->LoadTexture("Resources/Texture/plane.png")));
	models_.insert(std::make_pair("plane",std::move(planeModel_)));

	std::unique_ptr<Model> goalSwitchModel_;
	goalSwitchModel_.reset(Model::LoadFormOBJ("GoalSwitch",true));
	models_.insert(std::make_pair("GoalSwitch",std::move(goalSwitchModel_)));

	std::unique_ptr<Model> fenceModel_;
	fenceModel_.reset(Model::LoadFormOBJ("fence",true));
	models_.insert(std::make_pair("fence",std::move(fenceModel_)));

	std::unique_ptr<Model> fence2Model_;
	fenceModel_.reset(Model::LoadFormOBJ("fence2",true));
	models_.insert(std::make_pair("fence2",std::move(fenceModel_)));

	std::unique_ptr<Model> blockModel_;
	blockModel_.reset(Model::LoadFormOBJ("block",true));
	models_.insert(std::make_pair("block",std::move(blockModel_)));
}

void Map::MapLoad(std::string mapFullpath,bool middleSwitchRLoad)
{
	std::unique_ptr<LevelData> levelData;
	levelData.reset(LevelLoad::Load(mapFullpath));

	objects_.clear();

	planes_.clear();

	goalSwitchs_.clear();

	switchs_.clear();

	gimmicks_.clear();

	tutorials_.clear();

	cameras_.clear();

	isClear_ = false;

	if ( !middleSwitchRLoad )
	{
		middles_.clear();
	}

	EnemyManager::Instance()->Clear();

	mapName_ = mapFullpath;

	uint32_t EnemyNumber = 0;

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

			if ( objectData.scale.z <= 1.0f )
			{
				newObject->UVSift = { ( float ) ( rand() % 1000 ) / 1000,0 };

				newObject->UVSiftSpeed = { 0.02f ,0 };

				newObject->texHandle = TextureManager::Instance()->LoadTexture("Resources/Texture/plane.png");
			}
			else
			{
				newObject->UVSift = { 0,( float ) ( rand() % 1000 ) / 1000 };

				newObject->UVSiftSpeed = { 0,0.02f };

				newObject->texHandle = TextureManager::Instance()->LoadTexture("Resources/Texture/plane2.png");
			}
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

			newObject->texHandle = TextureManager::Instance()->LoadTexture("Resources/Texture/" + objectData.fileName + ".png");

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
		}
		if ( tagName == "GunEnemy" )
		{
			std::unique_ptr<Enemy> enemy;

			enemy = std::make_unique<GunEnemy>();

			enemy->Initialize(models_[ objectData.fileName ].get(),models_[ "enemyBullet" ].get(),player_,EnemyNumber++);

			enemy->GetObj()->SetPosition(objectData.trans);

			enemy->GetObj()->SetRot(objectData.rot);

			enemy->Update();

			EnemyManager::Instance()->AddEnemy(std::move(enemy));
		}
		if ( tagName == "RunEnemy" )
		{
			std::unique_ptr<Enemy> enemy;

			enemy = std::make_unique<RunEnemy>();

			enemy->Initialize(models_[ objectData.fileName ].get(),models_[ "enemyBullet" ].get(),player_,EnemyNumber++);

			enemy->GetObj()->SetPosition(objectData.trans);

			enemy->GetObj()->SetRot(objectData.rot);

			enemy->Update();

			EnemyManager::Instance()->AddEnemy(std::move(enemy));
		}
		if ( tagName == "jumpEnemy" )
		{
			std::unique_ptr<Enemy> enemy;

			enemy = std::make_unique<jumpEnemy>();

			enemy->Initialize(models_[ objectData.fileName ].get(),models_[ "enemyBullet" ].get(),player_,EnemyNumber++);

			enemy->GetObj()->SetPosition(objectData.trans);

			enemy->GetObj()->SetRot(objectData.rot);

			enemy->Update();

			EnemyManager::Instance()->AddEnemy(std::move(enemy));
		}
		if ( tagName == "tutorialEnemy" )
		{
			std::unique_ptr<Enemy> enemy;

			enemy = std::make_unique<TutorialEnemy>();

			enemy->Initialize(models_[ objectData.fileName ].get(),models_[ "enemyBullet" ].get(),player_,EnemyNumber++);

			enemy->GetObj()->SetPosition(objectData.trans);

			enemy->GetObj()->SetRot(objectData.rot);

			enemy->Update();

			EnemyManager::Instance()->AddEnemy(std::move(enemy));
		}
		if ( tagName == "WallEnemy" )
		{
			std::unique_ptr<Enemy> enemy;

			enemy = std::make_unique<WallEnemy>();

			enemy->Initialize(models_[ objectData.fileName ].get(),models_[ "enemyBullet" ].get(),player_,EnemyNumber++);

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
		if ( tagName == "MonsterHouse" )
		{
			std::unique_ptr<MonsterHouse> monsterHouse=std::make_unique<MonsterHouse>();

			monsterHouse->pos = objectData.trans;

			monsterHouse->scale = objectData.scale;

			monsterHouse->EnemyPopName = objectData.fileName;

			monsterHouses_.push_back(std::move(monsterHouse));
		}
	}



	std::sort(switchs_.rbegin(),switchs_.rend());

	std::sort(gimmicks_.rbegin(),gimmicks_.rend());
}

void Map::SwitchCollsion()
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

					for ( int i = 0; i < LightGroup::cPointLightNum; i++ )
					{
						if ( light_->GetPointActive(i) == false )
						{
							goalSwitch->lightIndex = i;

							break;
						}
					}

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

					for ( int i = 0; i < LightGroup::cPointLightNum; i++ )
					{
						if ( light_->GetPointActive(i) == false )
						{
							Switch->lightIndex = i;

							break;
						}
					}

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

		pos *= 30;

		if ( player_->GetEndCameraPos() != pos )
		{
			Collider::Cube cameraCube;
			cameraCube.Pos = cameras_[ i ]->pos;
			cameraCube.scale = cameras_[ i ]->scale;
			if ( Collider::CubeAndCube(playerCube,cameraCube,Collider::Collsion) == true )
			{
				player_->SetCameraMove(pos);
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

void Map::planeUpdate()
{
	planeDrawNum_ = 0;
	for ( uint32_t i = 0; i < planes_.size(); i++ )
	{
		planes_[ i ]->UVSift += planes_[ i ]->UVSiftSpeed;

		if ( planes_[ i ]->UVSift.x >= 1 )
		{
			planes_[ i ]->UVSift.x = 0;
		}

		planes_[ i ]->plane->SetUVShift(planes_[ i ]->UVSift);

		planes_[ i ]->plane->Update();

		Vector3 planePos = planes_[ i ]->plane->GetPosition();
		Vector3 cameraPos = Object3d::GetEye();
		if ( planePos.y <= cameraPos.y )
		{
			Collider::Cube planeCube = { {planePos},{planePos},{planes_[ i ]->plane->GetScale()} };
			Collider::Cube cameraCube = { {cameraPos},{cameraPos},{cCameraScale_} };
			if ( Collider::QuadAndQuad(planeCube,cameraCube,Collider::Type::Collsion) )
			{
				planes_[ i ]->plane->SetIsDraw(true);
				planeDrawNum_++;
			}
			else
			{
				planes_[ i ]->plane->SetIsDraw(false);
			}
		}
		else
		{
			planes_[ i ]->plane->SetIsDraw(false);
		}
	}
}

void Map::tutorialUpdate()
{
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
}

void Map::goalSwitchUpdate()
{
	for ( std::unique_ptr<GoalSwitch>& goalSwitch : goalSwitchs_ )
	{
		goalSwitch->obj->Update();
		if ( goalSwitch->phase == Phase::Middle )
		{
			goalSwitch->lightFrame++;
			float a = goalSwitch->spotLight->GetScale().z;

			float f = ( float ) goalSwitch->lightFrame / cEndLightFrame_;

			goalSwitch->spotLight->SetScale({ f * ( a / 5 ),1,a });

			f *= 1.5f;

			goalSwitch->light->SetScale({ f,f,f });

			if ( goalSwitch->lightFrame >= cEndLightFrame_ )
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
			goalSwitch->partFrame++;
			if ( goalSwitch->partFrame >= cEndSwitchPartFrame )
			{
				particleManager_->Add(90,goalSwitch->obj->GetPosition(),{ 0,0.5f,0 },{ 0,0,0 },1,1,{ 1,1,1,1 },{ 1,1,1,1 });
				goalSwitch->partFrame = 0;
			}
		}

		Vector3 planePos = goalSwitch->obj->GetPosition();
		Vector3 cameraPos = Object3d::GetEye();
		if ( planePos.y <= cameraPos.y )
		{
			Collider::Cube planeCube = { {planePos},{planePos},{goalSwitch->obj->GetScale()} };
			Collider::Cube cameraCube = { {cameraPos},{cameraPos},{cCameraScale_} };
			if ( Collider::QuadAndQuad(planeCube,cameraCube,Collider::Type::Collsion) )
			{
				goalSwitch->obj->SetIsDraw(true);
				goalSwitch->light->SetIsDraw(true);
				goalSwitch->spotLight->SetIsDraw(true);
				planeDrawNum_++;
			}
			else
			{
				goalSwitch->obj->SetIsDraw(false);
				goalSwitch->light->SetIsDraw(false);
				goalSwitch->spotLight->SetIsDraw(false);
			}
		}
		else
		{
			goalSwitch->obj->SetIsDraw(false);
			goalSwitch->light->SetIsDraw(false);
			goalSwitch->spotLight->SetIsDraw(false);
		}
	}
}

void Map::switchUpdate()
{
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

				float f = ( float ) switchs_[ i ]->lightFrame / cEndSpotLightFrame_;

				f *= 1.5f;

				switchs_[ i ]->light->SetScale({ f,f,f });

				if ( switchs_[ i ]->lightFrame >= cEndLightFrame_ )
				{
					switchs_[ i ]->phase = Phase::After;
				}
			}
			else
			{
				switchs_[ i ]->partFrame++;
				if ( switchs_[ i ]->partFrame >= cEndSwitchPartFrame )
				{
					particleManager_->Add(90,switchs_[ i ]->obj->GetPosition(),{ 0,0.5f,0 },{ 0,0,0 },1,1,{ 1,1,1,1 },{ 1,1,1,1 });
					switchs_[ i ]->partFrame = 0;
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
						gimmicks_[ j ]->obj->SetIsDraw(false);
					}
					gimmicks_[ j ]->obj->SetPosition(pos);
				}
			}
			gimmicks_[ j ]->obj->Update();
		}
		Vector3 planePos = switchs_[ i ]->obj->GetPosition();
		Vector3 cameraPos = Object3d::GetEye();
		if ( planePos.y <= cameraPos.y )
		{
			Collider::Cube planeCube = { {planePos},{planePos},{switchs_[ i ]->obj->GetScale()} };
			Collider::Cube cameraCube = { {cameraPos},{cameraPos},{cCameraScale_} };
			if ( Collider::QuadAndQuad(planeCube,cameraCube,Collider::Type::Collsion) )
			{
				switchs_[ i ]->obj->SetIsDraw(true);
				switchs_[ i ]->light->SetIsDraw(true);
				planeDrawNum_++;
			}
			else
			{
				switchs_[ i ]->obj->SetIsDraw(false);
				switchs_[ i ]->light->SetIsDraw(true);
			}
		}
		else
		{
			switchs_[ i ]->obj->SetIsDraw(false);
			switchs_[ i ]->light->SetIsDraw(true);
		}
	}
}

void Map::middleUpdate()
{
	for ( uint32_t i = 0; i < middles_.size(); i++ )
	{
		if ( middles_[ i ]->phase == Phase::Middle )
		{
			middles_[ i ]->Frame++;

			float f = middles_[ i ]->Frame / cEndGoalOpenFlame_;

			middles_[ i ]->obj->SetColor({ f,f,f });

			if ( middles_[ i ]->Frame >= cEndGoalOpenFlame_ )
			{
				middles_[ i ]->phase = Phase::After;
			}
		}

		middles_[ i ]->obj->Update();

		Vector3 planePos = middles_[ i ]->obj->GetPosition();
		Vector3 cameraPos = Object3d::GetEye();
		if ( planePos.y <= cameraPos.y )
		{
			Collider::Cube planeCube = { {planePos},{planePos},{middles_[ i ]->obj->GetScale()} };
			Collider::Cube cameraCube = { {cameraPos},{cameraPos},{cCameraScale_} };
			if ( Collider::QuadAndQuad(planeCube,cameraCube,Collider::Type::Collsion) )
			{
				middles_[ i ]->obj->SetIsDraw(true);
				planeDrawNum_++;
			}
			else
			{
				middles_[ i ]->obj->SetIsDraw(false);
			}
		}
		else
		{
			middles_[ i ]->obj->SetIsDraw(false);
		}
	}
}
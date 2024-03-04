#include "GameScene.h"
#include"DirectXCommon.h"
#include"Collider.h"
#include"TextureManager.h"
#include<imgui.h>
#include"input.h"
#include<time.h>
#include"Easing.h"
#include"SceneManager.h"
#include"PlayerBulletManager.h"
#include<iostream>
#include"EnemyManager.h"

GameScene::GameScene()
{
}

GameScene::~GameScene()
{

}

void GameScene::Initialize()
{
	Object3d::SetEye({ 0.0f,20.0f,-20.0f });

	padTexHandle_ = TextureManager::Instance()->LoadTexture("Resources/Texture/Abotton.png");

	spaceSprite_ = std::make_unique<Sprite>();

	spaceSprite_->Initialize(padTexHandle_);

	spaceSprite_->SetPosition({ 640.0f,515.0f });

	spaceSprite_->SetAnchorPoint({ 0.5f,0.5f });

	spaceSprite_->Update();

	player_ = std::make_unique<Player>();

	map = std::make_unique<Map>();

	light_.reset(LightGroup::Create());

	map->Initialize(player_.get(),light_.get());

	Object3d::SetLight(light_.get());

	sceneSprite_ = std::make_unique<Sprite>();

	sceneSprite_->Initialize(TextureManager::Instance()->LoadTexture("Resources/Texture/scene2.png"));

	sceneSprite_->SetAnchorPoint({ 0,0 });

	sceneSprite_->SetScale({ 1280,720 });

	sceneSprite_->Update();
#pragma region リトライ用スプライトの設定
	retrySprite_ = std::make_unique<Sprite>();

	retrySprite_->Initialize(TextureManager::Instance()->LoadTexture("Resources/Texture/retry.png"));

	retrySprite_->SetAnchorPoint({ 0.5f,0.5f });

	retrySprite_->SetPosition({ 612,300 });

	retrySprite_->Update();

	yesSprite_ = std::make_unique<Sprite>();

	yesSprite_->Initialize(TextureManager::Instance()->LoadTexture("Resources/Texture/Yes.png"));

	yesSprite_->SetAnchorPoint({ 0.5f,0.5f });
	;
	yesSprite_->SetPosition({ 320,400 });

	yesSprite_->Update();

	noSprite_ = std::make_unique<Sprite>();

	noSprite_->Initialize(TextureManager::Instance()->LoadTexture("Resources/Texture/No.png"));

	noSprite_->SetAnchorPoint({ 0.5f,0.5f });
	;
	noSprite_->SetPosition({ 960,400 });

	noSprite_->Update();

	yazirusiSprite_ = std::make_unique<Sprite>();

	yazirusiSprite_->Initialize(TextureManager::Instance()->LoadTexture("Resources/Texture/yazirusi.png"));

	yazirusiSprite_->SetAnchorPoint({ 0.5f,0.5f });
	;
	yazirusiSprite_->SetPosition({ 240,400 });

	yazirusiSprite_->Update();
#pragma endregion

#pragma region ポーズ中のチュートリアル用スプライトの設定

	pauseSprite_ = std::make_unique<Sprite>();
	pauseSprite_->Initialize(TextureManager::Instance()->LoadTexture("Resources/Texture/scene2.png"));
	pauseSprite_->SetAnchorPoint({ 0.5f,0.5f });
	pauseSprite_->SetScale({ 0,0 });
	pauseSprite_->SetPosition({ 640,360 });
	pauseSprite_->SetColor({ 1,1,1,0 });
	pauseSprite_->Update();

	pauseTutorialSprite_ = std::make_unique<Sprite>();
	pauseTutorialSprite_->Initialize(TextureManager::Instance()->LoadTexture("Resources/Texture/Tutorial.png"));
	pauseTutorialSprite_->SetAnchorPoint({ 0.5f,0.5f });
	pauseTutorialSprite_->SetPosition({ 400,360 });
	pauseTutorialSprite_->Update();

	tutorial0Sprite_ = std::make_unique<Sprite>();
	tutorial4Sprite_ = std::make_unique<Sprite>();

	tutorial0Sprite_->Initialize(TextureManager::Instance()->LoadTexture("Resources/Texture/Tutorial2.png"));
	tutorial4Sprite_->Initialize(TextureManager::Instance()->LoadTexture("Resources/Texture/Tutorial1.png"));

	tutorial0Sprite_->SetPosition({ 860,636 });
	tutorial0Sprite_->SetAnchorPoint({ 0.0f,0.0f });
	tutorial0Sprite_->Update();

	tutorial4Sprite_->SetPosition({ 1260,700 });
	tutorial4Sprite_->SetAnchorPoint({ 1.0f,1.0f });
	tutorial4Sprite_->Update();

#pragma endregion

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

	map->MapLoad("Resources/Map/Stage1.json",false);

	nowStage_ = Stage::Stage3;

	player_->SetLight(light_.get());

	player_->SetMapData(map->GetObjects());

	std::vector<Object3d*> a;

	for ( std::unique_ptr<Map::Gimmick>& gimmick: *map->GetGimmick())
	{
		a.push_back(gimmick->obj.get());
	}

	player_->SetGimmickData(a);

	EnemyManager::Instance()->SetMapData(map->GetObjects());
	player_->ObjectUpdate();
	EnemyManager::Instance()->Update();

	Enemy::SetLight(light_.get());

	EnemyBullet::SetLight(light_.get());
}

void GameScene::Update()
{
#ifdef _DEBUG

	if ( Input::Instance()->TriggerKey(DIK_0) )
	{
		EnemyManager::Instance()->Clear();

		map->MapLoad(map->GetMapName(),false);

		player_->SetMapData(map->GetObjects());
	}
#endif

	srand(( unsigned int ) time(NULL));

	switch ( sceneFlow )
	{
	case GameScene::SceneFlow::Start:

		StartUpdate();

		break;
	case GameScene::SceneFlow::Play:
		PlayUpdate();
		break;
	case SceneFlow::End:
		EndUpdate();
		break;
	default:
		break;
	}

	map->Update();
}

void GameScene::StartUpdate()
{
	SceneChangeFrame_--;
	float f = ( float ) SceneChangeFrame_ / cEndSceneChangeFrame_;

	sceneSprite_->SetColor({ 1,1,1,f });

	if ( SceneChangeFrame_ <= 0 )
	{
		sceneFlow = SceneFlow::Play;
	}
	sceneSprite_->Update();
}

void GameScene::PlayUpdate()
{
	float f;
	if ( pause_ == Map::Phase::After )
	{
		if ( Input::Instance()->PadTriggerKey(XINPUT_GAMEPAD_START) || Input::Instance()->TriggerKey(DIK_2) )
		{
			pause_ = Map::Phase::Middle;
			oldPause_ = Map::Phase::After;
		}
		if ( Input::Instance()->PadTriggerKey(XINPUT_GAMEPAD_BACK) || Input::Instance()->TriggerKey(DIK_3) )
		{
			sceneFlow = SceneFlow::End;
		}

	}
	else if ( pause_ == Map::Phase::Before )
	{
		player_->Update();
		EnemyManager::Instance()->Update();
		if ( Input::Instance()->PadTriggerKey(XINPUT_GAMEPAD_START) || Input::Instance()->TriggerKey(DIK_2) )
		{
			pause_ = Map::Phase::Middle;
			oldPause_ = Map::Phase::Before;
		}
		uint32_t playerMoving = player_->GetMoving();

		if ( playerMoving <= 30 )
		{
			uiMovingFrame_--;

			if ( uiMovingFrame_ <= 0 )
			{
				uiMovingFrame_ = 0;
			}
		}
		else
		{
			uiMovingFrame_++;

			if ( uiMovingFrame_ >= ( int32_t ) cEndUiMovingFrame_ )
			{
				uiMovingFrame_ = cEndUiMovingFrame_;
			}
		}
		{
			f = static_cast< float >( uiMovingFrame_ ) / static_cast< float >( cEndUiMovingFrame_ );

			Vector2 MenuPosition = ( cEndMenuPosition_ - cStartMenuPosition_ ) * f;
			MenuPosition += cStartMenuPosition_;

			tutorial0Sprite_->SetPosition(MenuPosition);

			tutorial0Sprite_->Update();
		}

		Collider::Cube A,B;

		A.Pos = player_->GetObj()->GetPosition();

		A.scale = player_->GetObj()->GetScale();

		if ( player_->IsDaed() )
		{
			sceneFlow = SceneFlow::End;
		}

		if (map->IsClear() )
		{
			sceneFlow = SceneFlow::End;
		}
	}
	else
	{
		if ( oldPause_ == Map::Phase::After )
		{
			pauseFrame_--;
			f = ( float ) pauseFrame_ / 10;
			pauseSprite_->SetScale({ 1280 * f,720 * f });
			pauseSprite_->SetColor({ 1,1,1, 0.5f * f });
			if ( pauseFrame_ <= 0 )
			{
				pause_ = Map::Phase::Before;
			}
		}
		if ( oldPause_ == Map::Phase::Before )
		{
			pauseFrame_++;
			f = ( float ) pauseFrame_ / 10;
			pauseSprite_->SetScale({ 1280 * f,720 * f });
			pauseSprite_->SetColor({ 1,1,1, 0.5f * f });
			if ( pauseFrame_ >= 10 )
			{
				pause_ = Map::Phase::After;
			}
		}
		pauseSprite_->Update();
	}
}

void GameScene::EndUpdate()
{
	float f;
	if ( SceneChangeFrame_ < cEndSceneChangeFrame_ + 5 )
	{
		SceneChangeFrame_++;

		f = ( float ) SceneChangeFrame_ / cEndSceneChangeFrame_;

		sceneSprite_->SetColor({ 1,1,1,f });

		retry_ = true;
	}
	else
	{
		if ( map->IsClear() )
		{
			std::vector<Object3d*> a;

			switch ( nowStage_ )
			{
			case GameScene::Stage::Stage1:
				map->MapLoad("Resources/Map/Stage2.json",false);

				player_->SetMapData(map->GetObjects());

				for ( std::unique_ptr<Map::Gimmick>& gimmick : *map->GetGimmick() )
				{
					a.push_back(gimmick->obj.get());
				}

				player_->SetGimmickData(a);

				player_->SetCameraPos(cCameraPos_);

				EnemyManager::Instance()->SetMapData(map->GetObjects());

				//player_->Update();
				EnemyManager::Instance()->Update();

				IsCameraSet_ = true;

				sceneStart_ = true;

				sceneChange_ = false;

				nowStage_ = Stage::Stage2;
				break;
			case GameScene::Stage::Stage2:
				map->MapLoad("Resources/Map/Stage3.json",false);

				player_->SetMapData(map->GetObjects());

				for ( std::unique_ptr<Map::Gimmick>& gimmick : *map->GetGimmick() )
				{
					a.push_back(gimmick->obj.get());
				}

				player_->SetGimmickData(a);

				player_->SetCameraPos(cCameraPos_);

				EnemyManager::Instance()->SetMapData(map->GetObjects());
				//player_->Update();
				EnemyManager::Instance()->Update();

				IsCameraSet_ = true;

				sceneStart_ = true;

				sceneChange_ = false;

				nowStage_ = Stage::Stage3;


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
					map->MapLoad(map->GetMapName(),true);

					player_->Reset();

					player_->SetMapData(map->GetObjects());

					player_->SetCameraPos(cCameraPos_);

					std::vector<Object3d*> a;

					for ( std::unique_ptr<Map::Gimmick>& gimmick : *map->GetGimmick() )
					{
						a.push_back(gimmick->obj.get());
					}

					player_->SetGimmickData(a);

					EnemyManager::Instance()->SetMapData(map->GetObjects());
					//player_->Update();
					EnemyManager::Instance()->Update();

					sceneFlow = SceneFlow::Start;

					retry_ = false;
				}
			}
		}
		else
		{
			SceneManager::Instance()->ChangeScene("TITLE");
		}
	}

	sceneSprite_->Update();
}

void GameScene::Draw(DirectXCommon* dxCommon)
{
	Object3d::PreDraw(dxCommon->GetCommandList());

	skyObj_->Draw();

	map->Draw();

	EnemyManager::Instance()->Draw();

	player_->Draw();

	Object3d::PostDraw();

	ParticleManager::PreDraw(dxCommon->GetCommandList());
	player_->ParticleDraw();
	EnemyManager::Instance()->ParticleDraw();
	ParticleManager::PostDraw();
}

void GameScene::SpriteDraw()
{
	SpriteCommon::Instance()->PreDraw();
	player_->SpriteDraw();
	if ( pause_ == Map::Phase::After || pause_ == Map::Phase::Middle )
	{
		pauseSprite_->Draw();
	}
	if ( pause_ == Map::Phase::After )
	{
		tutorial4Sprite_->Draw();
		pauseTutorialSprite_->Draw();
	}
	if ( pause_ == Map::Phase::Before )
	{
		tutorial0Sprite_->Draw();
	}
	sceneSprite_->Draw();
	if ( SceneChangeFrame_ >= cEndSceneChangeFrame_ && player_->IsDaed() )
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
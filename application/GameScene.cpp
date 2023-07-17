#include "GameScene.h"
#include"DirectXCommon.h"
#include"levelLoad.h"
#include"Collider.h"

GameScene::GameScene()
{
}

GameScene::~GameScene()
{
}

void GameScene::Initialize()
{
	texHandle = Texture::LoadTexture(L"Resources/RedTexture.png");

	model.reset(Model::LoadFormOBJ("cube", true));

	obj = std::make_unique<Object3d>();

	obj->Initialize();

	obj->SetModel(model.get());

	obj->SetPolygonExplosion({ 0.0f,1.0f,6.28f,100.0f });

	models.insert(std::make_pair("cube", model.get()));

	Object3d::SetEye({ 0.0f,0.0f,-10.0f });

	std::unique_ptr<LevelData> levelData;
	levelData.reset(LevelLoad::Load());

	sprite = std::make_unique<Sprite>();

	sprite->Initialize(Texture::LoadTexture(L"Resources/mario.jpg"));

	sprite->SetPosition({ 0.0f,0.0f });

	sprite->SetAnchorPoint({ 0.0f,0.0f });

	for (auto& objectData : levelData->objects)
	{
		//モデルを指定して3Dオブジェクトを生成
		std::unique_ptr<Object3d> newObject = std::make_unique<Object3d>();
		newObject->Initialize();
		newObject->SetModel(models[objectData.fileName]);

		assert(newObject);

		// 座標
		DirectX::XMFLOAT3 pos;
		DirectX::XMStoreFloat3(&pos, objectData.trans);
		newObject->SetPosition({ pos.x,pos.y,pos.z });

		// 回転角
		DirectX::XMFLOAT3 rot;
		DirectX::XMStoreFloat3(&rot, objectData.rot);
		newObject->SetRot({ rot.x,rot.y,rot.z });

		// 座標
		DirectX::XMFLOAT3 scale;
		DirectX::XMStoreFloat3(&scale, objectData.scale);
		newObject->SetScale({ scale.x,scale.y,scale.z });

		newObject->SetPolygonExplosion({ 0.0f,1.0f,6.28f,100.0f });

		// 配列に登録
		objects.push_back(std::move(newObject));

		collision.emplace_back();

	}

	light.reset(LightGroup::Create());

	Object3d::SetLight(light.get());

	//light->SetDirLightActive(0, false);

	light->SetDirLightDir(0, { 0,-1,1 });

	light->SetDirLightColor(0, { 1,1,1 });

	light->SetDirLightActive(1, false);

	light->SetDirLightActive(2, false);

	//light->SetPointActive(0, true);

	//light->SetPointPos(0,{0.5f,1.0f,0.0f});
}

void GameScene::Update()
{
	static Vector3 pos = { 5.0f,0.0f,0.0f };

	static Vector3 rot = { 0.0f,0.0f,0.0f };

	Vector3 move;
	if (Input::IsLinkGamePad())
	{

		move += {Input::GetPadStick(PadStick::LX), 0, Input::GetPadStick(PadStick::LY)};

		rot += {Input::GetPadStick(PadStick::RY), Input::GetPadStick(PadStick::RX), 0};

	}
	else
	{
		if (Input::PushKey(DIK_W))
		{
			move += {0, 0.1f,0};
		}
		if (Input::PushKey(DIK_A))
		{
			move += {-0.1f, 0, 0};
		}
		if (Input::PushKey(DIK_S))
		{
			move += {0, -0.1f,0};
		}
		if (Input::PushKey(DIK_D))
		{
			move += {0.1f, 0, 0};
		}
	}

	obj->SetRot(rot);

	float shininess = 20;

	ImGui::Begin("light");

	ImGui::ColorEdit3("ambientColor", ambientColor, ImGuiColorEditFlags_Float);
	ImGui::InputFloat3("lightPos", lightPos);
	ImGui::ColorEdit3("lightColor", lightColor, ImGuiColorEditFlags_Float);
	ImGui::InputFloat3("lightAtten", lightAtten);
	ImGui::InputFloat("shininess", &shininess);

	ImGui::End();

	light->SetAmbientColor(Vector3({ ambientColor[0],ambientColor[1],ambientColor[2] }));

	light->SetPointPos(0, Vector3({ lightPos[0],lightPos[1],lightPos[2] }));
	light->SetPointAtten(0, Vector3({ lightAtten[0],lightAtten[1],lightAtten[2] }));
	light->SetPointColor(0, Vector3({ lightColor[0],lightColor[1],lightColor[2] }));

	for (uint32_t i=0;i<objects.size();i++)
	{
		Cube mapCube, objCube;
		mapCube.Pos = objects[i]->GetPosition();
		mapCube.scale = objects[i]->GetScale();
		objCube.Pos = obj->GetPosition()+move;
		objCube.oldPos = obj->GetPosition();
		objCube.scale = obj->GetScale();
		if (Collider::CubeAndCube(mapCube, objCube)==true)
		{
			collision[i] = true;
			if (mapCube.Pos.y-mapCube.scale.y>=objCube.oldPos.y+objCube.scale.y)
			{
				pos.y = mapCube.Pos.y - (mapCube.scale.y + objCube.scale.y);

				move.y = 0;
			}
			else if(mapCube.Pos.y + mapCube.scale.y <= objCube.oldPos.y - objCube.scale.y)
			{
				pos.y = mapCube.Pos.y + mapCube.scale.y + objCube.scale.y;

				move.y = 0;
			}
			else
			{

				if (mapCube.Pos.x + mapCube.scale.x <= objCube.oldPos.x - objCube.scale.x)
				{

					pos.x= mapCube.Pos.x+mapCube.scale.x+objCube.scale.x;

					move.x = 0;
				}
				else if(mapCube.Pos.x - mapCube.scale.x >= objCube.oldPos.x + objCube.scale.x)
				{
					pos.x = mapCube.Pos.x-(mapCube.scale.x + objCube.scale.x);

					move.x = 0;
				}

			}
		}
		else
		{
			collision[i] = false;
		}
	}
	pos += move;

	obj->SetPosition(pos);

	light->Update();

	obj->Update();

	for (std::unique_ptr<Object3d>& obj : objects)
	{
		obj->SetShininess(shininess);

		obj->Update();
	}

	sprite->Update();
}

void GameScene::Draw(DirectXCommon* dxCommon)
{
	Object3d::PreDraw(dxCommon->GetCommandList());

	for (uint32_t i = 0; i < objects.size(); i++)
	{
		if (collision[i])
		{
			objects[i]->Draw(texHandle);
		}
		else
		{
			objects[i]->Draw();
		}
	}

	obj->Draw();

	Object3d::PostDraw();

	assimpObject3d::PreDraw(dxCommon->GetCommandList());

	assimpObject3d::PostDraw();

	ParticleManager::PreDraw(dxCommon->GetCommandList());
	ParticleManager::PostDraw();

	SpriteCommon::PreDraw();

	//sprite->Draw();
	SpriteCommon::PostDraw();
}

void GameScene::PostEffectDraw(DirectXCommon* dxCommon)
{

	Object3d::PreDraw(dxCommon->GetCommandList());

	obj->Draw();

	skydomeObj->Draw();

	Object3d::PostDraw();

	ParticleManager::PreDraw(dxCommon->GetCommandList());
	//pMan->Draw();
	ParticleManager::PostDraw();
}

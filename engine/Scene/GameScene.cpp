#include "GameScene.h"
#include"DirectXCommon.h"
#include"levelLoad.h"

GameScene::GameScene()
{
}

GameScene::~GameScene()
{
}

void GameScene::Initialize()
{
	texHandle = Texture::LoadTexture(L"Resources/RedTexture.png");

	model.reset(Model::LoadFormOBJ("playerbullet", true));

	obj = std::make_unique<Object3d>();

	obj->Initialize();

	obj->SetModel(model.get());

	obj->SetPolygonExplosion({ 0.0f,1.0f,6.28f,100.0f,0.0f });

	models.insert(std::make_pair("cube", model.get()));

	Object3d::SetEye({ 0.0f,10.0f,-10.0f });

	std::unique_ptr<LevelData> levelData;
	levelData.reset(LevelLoad::Load());

	sprite = std::make_unique<Sprite>();

	sprite->Initialize(Texture::LoadTexture(L"Resources/mario.jpg"));

	sprite->SetPosition({0.0f,0.0f});

	sprite->SetAnchorPoint({0.0f,0.0f});

	for (auto& objectData : levelData->objects)
	{
		//ファイル名から登録済みモデルを検索
		//Model* model = nullptr;
		//decltype(models)::iterator it = models.find(objectData.fileName);
		//if (it != models.end()) { model = it->second; }
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

		newObject->SetPolygonExplosion({ 0.0f,1.0f,6.28f,100.0f,0.0f });

		// 配列に登録
		objects.push_back(std::move(newObject));

		collision.emplace_back();

	}

	light.reset(LightGroup::Create());

	Object3d::SetLight(light.get());
}

void GameScene::Update()
{
	static Vector3 pos = {0.0f,0.0f,0.0f};

	static Vector3 rot = {0.0f,0.0f,0.0f};

	pos += {Input::GetPadStick(PadStick::LX), Input::GetPadStick(PadStick::LY),0};

	rot += {Input::GetPadStick(PadStick::RY), Input::GetPadStick(PadStick::RX), 0};

	obj->SetPosition(pos);

	obj->SetRot(rot);

	ImGui::Begin("light");

	ImGui::ColorEdit3("ambientColor", ambientColor, ImGuiColorEditFlags_Float);
	ImGui::InputFloat3("lightDir0", lightDir0);
	ImGui::ColorEdit3("lightColor0", lightColor0, ImGuiColorEditFlags_Float);
	ImGui::InputFloat3("lightDir1", lightDir1);
	ImGui::ColorEdit3("lightColor1", lightColor1, ImGuiColorEditFlags_Float);
	ImGui::InputFloat3("lightDir2", lightDir2);
	ImGui::ColorEdit3("lightColor2", lightColor2, ImGuiColorEditFlags_Float);

	ImGui::End();

	light->SetAmbientColor(Vector3({ ambientColor[0],ambientColor[1],ambientColor[2] }));

	light->SetDirLightDir(0, Vector3({ lightDir0[0],lightDir0[1],lightDir0[2] }));
	light->SetDirLightColor(0, Vector3({ lightColor0[0],lightColor0[1],lightColor0[2] }));
	light->SetDirLightDir(1, Vector3({ lightDir1[0],lightDir1[1],lightDir1[2] }));
	light->SetDirLightColor(1, Vector3({ lightColor1[0],lightColor1[1],lightColor1[2] }));
	light->SetDirLightDir(2, Vector3({ lightDir2[0],lightDir2[1],lightDir2[2] }));
	light->SetDirLightColor(2, Vector3({ lightColor2[0],lightColor2[1],lightColor2[2] }));

	for (uint32_t i=0;i<objects.size();i++)
	{
		Vector3 AScale=objects[i]->GetScale();

		float ASize = (AScale.x + AScale.y + AScale.z) / 3;

		Vector3 APos= objects[i]->GetPosition();

		Vector3 BScale = obj->GetScale();

		float BSize = (BScale.x + BScale.y + BScale.z) / 3;

		Vector3 BPos = obj->GetPosition();
		if (pow(APos.x- BPos.x,2)+pow(APos.y - BPos.y, 2)+pow(APos.z - BPos.z, 2)<=pow(ASize+BSize,2))
		{
			collision[i] = true;
		}
		else
		{
			collision[i] = false;
		}
	}



	light->Update();

	obj->Update();

	for (std::unique_ptr<Object3d>& obj : objects)
	{
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
			//objects[i]->Draw(texHandle);
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

	sprite->Draw();
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

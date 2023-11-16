#include "SceneManager.h"
#include<cassert>

SceneManager* SceneManager::Instance()
{
	static SceneManager instance;

	return &instance;
}

void SceneManager::ChangeScene(const std::string& sceneName)
{
	assert(sceneFactory_);
	assert(nextScene_ == nullptr);

	//次のシーンを生成
	nextScene_.reset(sceneFactory_->CreateScene(sceneName));
}

void SceneManager::Update()
{
	if ( nextScene_ )
	{
		if ( scene_ )
		{
			scene_->Finalize();
			scene_ = nullptr;
		}

		//シーンの切り替え

		scene_ = std::move(nextScene_);

		nextScene_ = nullptr;

		scene_->Initialize();
	}
	scene_->Update();
}

void SceneManager::Draw(DirectXCommon* dxCommon)
{
	scene_->Draw(dxCommon);
}

void SceneManager::SpriteDraw()
{
	scene_->SpriteDraw();
}

void SceneManager::Finalize()
{
	scene_->Finalize();

	scene_ = nullptr;
}

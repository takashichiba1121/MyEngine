#pragma once
#include"BaseScene.h"
#include"DirectXCommon.h"
#include"AbstractSceneFactory.h"
#include<memory>

class SceneManager
{
#pragma region Singleton
private:
	SceneManager() {};

	~SceneManager() {};
public:
	SceneManager(const SceneManager& carManager) = delete;

	SceneManager& operator=(const SceneManager& carManager) = delete;

	static SceneManager* Instance();
#pragma endregion
public:

	/// <summary>
	/// 次のシーン予約
	/// </summary>
	/// <param name="sceneName">シーン名</param>
	void ChangeScene(const std::string& sceneName);

	void Update();

	void Draw(DirectXCommon* dxCommon);

	void Finalize();

	void SetSceneFactory(AbstractSceneFactory* sceneFactory) {sceneFactory_=sceneFactory ; }
private:

	std::unique_ptr<BaseScene> scene_ = nullptr;

	std::unique_ptr<BaseScene> nextScene_ = nullptr;

	AbstractSceneFactory* sceneFactory_ = nullptr;
};


#pragma once
#include"WinApp.h"
#include"DirectXCommon.h"
#include<memory>
#include"SceneManager.h"
#include"SceneFactory.h"
/*
* エンジン側のゲームの流れ
*/
class Framework
{
public:
	virtual ~Framework() = default;

	//初期化
	virtual void Initialize();

	//終了
	virtual void Finalize();

	//更新
	virtual void Update();

	//描画
	virtual void Draw()=0;

	//終了フラグのチェック
	virtual bool IsEndRequst() { return endRequst_; }

	virtual DirectXCommon* GetDxCommon() {return dxCommon_.get() ; }

	//ゲームのエンジン側の流れ
	void Run();
protected: 
	SceneManager* sceneManager_ = nullptr;

	bool endRequst_ = false;

	std::unique_ptr<WinApp> winApp_;

	std::unique_ptr<DirectXCommon> dxCommon_=nullptr;

	std::unique_ptr<AbstractSceneFactory> sceneFactory_ = nullptr;
};


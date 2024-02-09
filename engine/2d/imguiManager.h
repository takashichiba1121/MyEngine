#pragma once
#include"WinApp.h"
#include"DirectXCommon.h"
/*
* imguiの制御クラス
*/
class imguiManager
{
public: // 静的メンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	static void StaticInitialize(MyEngine::WinApp* winApp, DirectXCommon* dxCommon);

	/// <summary>
	/// 終了
	/// </summary>
	static void Finalize();

		/// <summary>
	/// 画面への描画
	/// </summary>
	static void Draw();
public: // メンバ関数
	/// <summary>
	/// ImGui受付開始
	/// </summary>
	void Begin();

	/// <summary>
	/// ImGui受付終了
	/// </summary>
	void End();

private://メンバ変数

	//SRV用デスクリプタヒープ
	static Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> sSrvHeap;

	static DirectXCommon* sDxCommon;

	static MyEngine::WinApp* sWinApp;
};


#pragma once
#include"WinApp.h"
#include"DirectXCommon.h"
class imguiManager
{
public:

	static void StaticInitialize(WinApp* winApp, DirectXCommon* dxCommon);

	/// <summary>
	/// 終了
	/// </summary>
	static void Finalize();

	/// <summary>
	/// ImGui受付開始
	/// </summary>
	void Begin();

	/// <summary>
	/// ImGui受付終了
	/// </summary>
	void End();

	/// <summary>
	/// 画面への描画
	/// </summary>
	static void Draw();

private:
	//SRV用デスクリプタヒープ
	static Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> sSrvHeap;

	static DirectXCommon* sDxCommon;

	static WinApp* sWinApp;
};


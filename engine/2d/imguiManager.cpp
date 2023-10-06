#include "imguiManager.h"
#include"imgui.h"
#include"imgui_impl_win32.h"
#include"imgui_impl_dx12.h"

Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> imguiManager::sSrvHeap;

DirectXCommon* imguiManager::sDxCommon;

WinApp* imguiManager::sWinApp;

void imguiManager::StaticInitialize(WinApp* winApp, DirectXCommon* dxCommon)
{
	HRESULT result;

	sWinApp = winApp;

	sDxCommon = dxCommon;

	//Imguiのコンテキストを生成
	ImGui::CreateContext();
	//ImGuiのスタイルを設定
	ImGui::StyleColorsDark();
	 
	ImGui_ImplWin32_Init(sWinApp->GetHwnd());

	//デスクリプタヒープ設定
	D3D12_DESCRIPTOR_HEAP_DESC desc = {};
	desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	desc.NumDescriptors = 1;
	desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	//デスクリプタヒープ生成
	result = sDxCommon->GetDevice()->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&sSrvHeap));
	assert(SUCCEEDED(result));

	ImGui_ImplDX12_Init(
		sDxCommon->GetDevice(),
		static_cast<int>(sDxCommon->GetBackBufferCount()),
		DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, sSrvHeap.Get(),
		sSrvHeap->GetCPUDescriptorHandleForHeapStart(),
		sSrvHeap->GetGPUDescriptorHandleForHeapStart());
}

void imguiManager::Finalize()
{
	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
	//デスクリプタヒープを解放
	sSrvHeap.Reset();
}

void imguiManager::Begin()
{
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void imguiManager::End()
{
	//描画準備
	ImGui::Render();
}

void imguiManager::Draw()
{
	ID3D12GraphicsCommandList* commandList = sDxCommon->GetCommandList();


	//デスクリプタヒープの配列をセットするコマンド
	ID3D12DescriptorHeap* ppHeaps[] = { sSrvHeap.Get() };
	commandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	//描画コマンドを発行
	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(),commandList);
}

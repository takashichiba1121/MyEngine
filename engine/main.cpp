 #include"Framework.h"
#include"MyGame.h"
#include<dxgidebug.h>

using namespace std;

struct D3DResouceLeakChecker
{
	~D3DResouceLeakChecker()
	{
		// リソースリークチェック
		Microsoft::WRL::ComPtr<IDXGIDebug1>debug;
		if (SUCCEEDED(DXGIGetDebugInterface1(0, IID_PPV_ARGS(&debug)))) {
			debug->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_ALL);
			debug->ReportLiveObjects(DXGI_DEBUG_APP, DXGI_DEBUG_RLO_ALL);
			debug->ReportLiveObjects(DXGI_DEBUG_D3D12, DXGI_DEBUG_RLO_ALL);
		}
	}
};

//windowsアプリのエントリーポイント（main関数）
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	D3DResouceLeakChecker memoryLeak;

	Framework* game = new MyGame;

	//game = std::unique_ptr<MyGame>();

	game->Run();

	delete game;

	return 0;
}
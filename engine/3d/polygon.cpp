#include"polygon.h"
#include <d3dcompiler.h>
#include <DirectXTex.h>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;


#pragma comment(lib, "d3dcompiler.lib")

using namespace DirectX;
using namespace Microsoft::WRL;

//polygonMana::Material polygonMana::material;

/// <summary>
/// 静的メンバ変数の実体
/// </summary>
ID3D12Device* PolygonMana::device = nullptr;
//UINT polygonMana::descriptorHandleIncrementSize = 0;
ID3D12GraphicsCommandList* PolygonMana::cmdList = nullptr;
ComPtr<ID3D12RootSignature> PolygonMana::rootsignature;
ComPtr<ID3D12PipelineState> PolygonMana::pipelinestate;
//ComPtr<ID3D12DescriptorHeap> polygonMana::descHeap;
//ComPtr<ID3D12Resource> polygonMana::texbuff;
//CD3DX12_CPU_DESCRIPTOR_HANDLE polygonMana::cpuDescHandleSRV;
//CD3DX12_GPU_DESCRIPTOR_HANDLE polygonMana::gpuDescHandleSRV;
Matrix4 PolygonMana::matViewPro;
//polygonMana::VertexPosNormalUv polygonMana::vertices[vertexCount];
//unsigned short polygonMana::indices[planeCount * 3];

void PolygonMana::StaticInitialize(ID3D12Device* device, int window_width, int window_height)
{
	// nullptrチェック
	assert(device);

	PolygonMana::device = device;

	Model::SetDevice(device);

	// パイプライン初期化
	InitializeGraphicsPipeline();

}

void PolygonMana::InitializeGraphicsPipeline()
{
	HRESULT result;

	Microsoft::WRL::ComPtr<ID3DBlob> vsBlob = nullptr;//頂点シェーダーオブジェクト
	Microsoft::WRL::ComPtr<ID3DBlob> psBlob = nullptr;//ピクセルシェーダーオブジェクト

	ID3DBlob* errorBlob = nullptr;//エラーオブジェクト

	//頂点シェーダーの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Resources/shaders/polygonVS.hlsl", //シェーダーファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,//インクルード可能にする
		"main", "vs_5_0",//エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, //デバッグ用設定
		0,
		&vsBlob, &errorBlob);

	//エラーなら
	if (FAILED(result)) {
		//errorBlobからエラー内容をstring型にコピー
		std::string error;
		error.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			error.begin());
		error += "\n";
		//エラー内容を出力ウインドウに表示
		OutputDebugStringA(error.c_str());
		assert(0);
	}
	//ピクセルシェーダーの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Resources/shaders/polygonPS.hlsl",
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"main", "ps_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		&psBlob, &errorBlob);
	//エラーなら
	if (FAILED(result)) {
		//errorBlobから内容をstring型にコピー
		std::string error;
		error.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			error.begin());
		error += "\n";
		//エラー内容を出力ウインドウに表示
		OutputDebugStringA(error.c_str());
		assert(0);
	}
	//頂点レイアウト
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{
		"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,
		D3D12_APPEND_ALIGNED_ELEMENT,
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0,
		},//vyz座標(一行で書いたほうが見やすい)
		{
		"COLOR",0,DXGI_FORMAT_R32G32_FLOAT,0,
		D3D12_APPEND_ALIGNED_ELEMENT,
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0,
		},//uv座標(一行で書いたほうが見やすい)
	};

	//グラフィックスパイプライン設定
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc{};
	//シェーダーの設定
	pipelineDesc.VS.pShaderBytecode = vsBlob->GetBufferPointer();
	pipelineDesc.VS.BytecodeLength = vsBlob->GetBufferSize();
	pipelineDesc.PS.pShaderBytecode = psBlob->GetBufferPointer();
	pipelineDesc.PS.BytecodeLength = psBlob->GetBufferSize();

	//サンプルマスクの設定
	pipelineDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
	//ラスタライザの設定
	pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;//カリングしない
	pipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;//ポリゴン内塗りつぶし
	pipelineDesc.RasterizerState.DepthClipEnable = true;//深度グリッピングを有効に

	//レンダーターゲットのブレンド設定
	D3D12_RENDER_TARGET_BLEND_DESC& blenddesc = pipelineDesc.BlendState.RenderTarget[0];
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;//RGBA全てのチャンネルを描画
	blenddesc.BlendEnable = true;//ブレンドを有効にする
	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;//加算
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;//ソースの値を100%使う
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;//デストの値を0%使う
	////加算合成
	//blenddesc.BlendOp = D3D12_BLEND_OP_ADD;//加算
	//blenddesc.SrcBlend = D3D12_BLEND_ONE;//ソースの値を100%使う
	//blenddesc.DestBlend = D3D12_BLEND_ONE;//デストの値を100%使う
	////減算合成
	//blenddesc.BlendOp = D3D12_BLEND_OP_REV_SUBTRACT;//デストからソースを減算
	//blenddesc.SrcBlend = D3D12_BLEND_ONE;//ソースの値を100%使う
	//blenddesc.DestBlend = D3D12_BLEND_ONE;//デストの値を100%使う
	////色反転
	//blenddesc.BlendOp = D3D12_BLEND_OP_ADD;//加算
	//blenddesc.SrcBlend = D3D12_BLEND_INV_DEST_COLOR;//1.0f-デストカラーの値
	//blenddesc.DestBlend = D3D12_BLEND_ZERO;//使わない
	//半透明合成
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;//加算
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;//ソースのアルファ値
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;//1.0f-ソースのアルファ値
	////ブレンドステート
	//pipelineDesc.BlendState.RenderTarget[0].RenderTargetWriteMask
	//	= D3D12_COLOR_WRITE_ENABLE_ALL;//RBGAすべてのチャンネルを描画
	//頂点レイアウトの設定
	pipelineDesc.InputLayout.pInputElementDescs = inputLayout;
	pipelineDesc.InputLayout.NumElements = _countof(inputLayout);
	//図形の形状設定
	pipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	//その他の設定
	pipelineDesc.NumRenderTargets = 1;//描画対象は一つ
	pipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;//0～255指定のRGBA
	pipelineDesc.SampleDesc.Count = 1; //1ピクセルにつき一回サンプリング


	//ルートパラメーターの設定
	D3D12_ROOT_PARAMETER rootParams[1] = {};
	//テクスチャレジスタ0番
	rootParams[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;//定数バッファ
	rootParams[0].Descriptor.ShaderRegister = 0;//定数バッファ番号
	rootParams[0].Descriptor.RegisterSpace = 0;//デフォルト値
	rootParams[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;//全てのシェーダーから見える

	//テクスチャサンプラー設定
	D3D12_STATIC_SAMPLER_DESC samplerDesc{};
	samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;//横繰り返し（タイリング）
	samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;//縦繰り返し（タイリング）
	samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;//奥行繰り返し（タイリング）
	samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;//ボーダーの時は黒
	samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;//全てリニア補完
	samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;//ミップマップ最大値
	samplerDesc.MinLOD = 0.0f;//ミップマップ最小値
	samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;//ピクセルシェーダーからのみ使用可能

	//ルートシグネチャ
	//ルートシグネチャの設定
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	rootSignatureDesc.pParameters = rootParams;//ルートパラメーターの先頭アドレス
	rootSignatureDesc.NumParameters = _countof(rootParams);//ルートパラメータ数
	rootSignatureDesc.pStaticSamplers = &samplerDesc;
	rootSignatureDesc.NumStaticSamplers = 1;
	//ルートシグネチャのシリアライズ
	Microsoft::WRL::ComPtr<ID3DBlob> rootSigBlob = nullptr;
	result = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0,
		&rootSigBlob, &errorBlob);
	assert(SUCCEEDED(result));
	result = device->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(&rootsignature));
	assert(SUCCEEDED(result));
	//rootSigBlob->Release();
	//パイプラインにルートシグネチャをセット
	pipelineDesc.pRootSignature = rootsignature.Get();

	//パイプランスステートの生成
	result = device->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&pipelinestate));
	assert(SUCCEEDED(result));

}

//bool polygonMana::LoadTexture(const std::string& directoryPath, const std::string& filename)
//{
//	HRESULT result = S_FALSE;
//
//	TexMetadata metadata{};
//	ScratchImage scratchImg{};
//
//	//ファイルパスを結合
//	string filepath = directoryPath + filename;
//
//	//ユニコード文字列に変換する
//	wchar_t wfilepath[128];
//	int iBufferSize = MultiByteToWideChar(CP_ACP, 0, filepath.c_str(), -1, wfilepath, _countof(wfilepath));
//
//
//	//// WICテクスチャのロード
//	//result = LoadFromWICFile(L"Resources/tex1.png", WIC_FLAGS_NONE, &metadata, scratchImg);
//	//assert(SUCCEEDED(result));
//	// WICテクスチャのロード
//	result = LoadFromWICFile(wfilepath, WIC_FLAGS_NONE, &metadata, scratchImg);
//	assert(SUCCEEDED(result));
//
//	ScratchImage mipChain{};
//	// ミップマップ生成
//	result = GenerateMipMaps(
//		scratchImg.GetImages(), scratchImg.GetImageCount(), scratchImg.GetMetadata(),
//		TEX_FILTER_DEFAULT, 0, mipChain);
//	if (SUCCEEDED(result)) {
//		scratchImg = std::move(mipChain);
//		metadata = scratchImg.GetMetadata();
//	}
//
//	// 読み込んだディフューズテクスチャをSRGBとして扱う
//	metadata.format = MakeSRGB(metadata.format);
//
//	// リソース設定
//	CD3DX12_RESOURCE_DESC texresDesc = CD3DX12_RESOURCE_DESC::Tex2D(
//		metadata.format, metadata.width, (UINT)metadata.height, (UINT16)metadata.arraySize,
//		(UINT16)metadata.mipLevels);
//
//	// ヒーププロパティ
//	CD3DX12_HEAP_PROPERTIES heapProps =
//		CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0);
//
//	// テクスチャ用バッファの生成
//	result = device->CreateCommittedResource(
//		&heapProps, D3D12_HEAP_FLAG_NONE, &texresDesc,
//		D3D12_RESOURCE_STATE_GENERIC_READ, // テクスチャ用指定
//		nullptr, IID_PPV_ARGS(&texbuff));
//	assert(SUCCEEDED(result));
//
//	// テクスチャバッファにデータ転送
//	for (size_t i = 0; i < metadata.mipLevels; i++) {
//		const Image* img = scratchImg.GetImage(i, 0, 0); // 生データ抽出
//		result = texbuff->WriteToSubresource(
//			(UINT)i,
//			nullptr,              // 全領域へコピー
//			img->pixels,          // 元データアドレス
//			(UINT)img->rowPitch,  // 1ラインサイズ
//			(UINT)img->slicePitch // 1枚サイズ
//		);
//		assert(SUCCEEDED(result));
//	}
//
//	// シェーダリソースビュー作成
//	cpuDescHandleSRV = CD3DX12_CPU_DESCRIPTOR_HANDLE(descHeap->GetCPUDescriptorHandleForHeapStart(), 0, descriptorHandleIncrementSize);
//	gpuDescHandleSRV = CD3DX12_GPU_DESCRIPTOR_HANDLE(descHeap->GetGPUDescriptorHandleForHeapStart(), 0, descriptorHandleIncrementSize);
//
//	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{}; // 設定構造体
//	D3D12_RESOURCE_DESC resDesc = texbuff->GetDesc();
//
//	srvDesc.Format = resDesc.Format;
//	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
//	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2Dテクスチャ
//	srvDesc.Texture2D.MipLevels = 1;
//
//	device->CreateShaderResourceView(texbuff.Get(), //ビューと関連付けるバッファ
//		&srvDesc, //テクスチャ設定情報
//		cpuDescHandleSRV
//	);
//	if (result == NULL) {
//		return false;
//	}
//
//	return true;
//}

void PolygonMana::UpdateViewMatrix()
{
	matViewPro = Object3d::GetMatViewPro();
}

bool PolygonMana::Initialize()
{
	// nullptrチェック
	assert(device);

	// ヒーププロパティ
	CD3DX12_HEAP_PROPERTIES heapProps = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	// リソース設定
	CD3DX12_RESOURCE_DESC resourceDesc =
		CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferDataB0) + 0xff) & ~0xff);


	HRESULT result;

	// 定数バッファの生成
	result = device->CreateCommittedResource(
		&heapProps, // アップロード可能
		D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
		IID_PPV_ARGS(&constBuffB0));
	//// 定数バッファの生成
	//result = device->CreateCommittedResource(
	//	&heapPropsB1,
	//	D3D12_HEAP_FLAG_NONE,
	//	&resourceDescB1,
	//	D3D12_RESOURCE_STATE_GENERIC_READ,
	//	nullptr,
	//	IID_PPV_ARGS(&constBuffB1));
	assert(SUCCEEDED(result));

	return true;
}

void PolygonMana::Update()
{
	HRESULT result;

	//寿命が尽きたパーティクルを全削除
	Polygons.remove_if([](polygon& x) {
		return x.frame >= x.num_frame;
		});
	//全パーティクル
	for (std::list<polygon>::iterator it = Polygons.begin(); it !=Polygons .end(); it++)
	{
		//経過フレーム数をカウント
		it->frame++;
		//速度に加速度を加算
		it->velocity = it->velocity + it->accel;
		//速度による移動
		it->position[0] = it->position[0] + it->velocity;

		it->position[1] = it->position[1] + it->velocity;

		it->position[2] = it->position[2] + it->velocity;


		float f = (float)it->frame / it->num_frame;

		//赤の線形補間
		it->color.x = (it->e_color.x - it->s_color.x) * f;
		it->color.x += it->s_color.x;
		//青の線形補間
		it->color.y = (it->e_color.y - it->s_color.y) * f;
		it->color.y += it->s_color.y;
		//緑の線形補間
		it->color.z = (it->e_color.z - it->s_color.z) * f;
		it->color.z += it->s_color.z;

		//緑の線形補間
		it->color.w = (it->e_color.w - it->s_color.w) * f;
		it->color.w += it->s_color.w;
	}
	//頂点バッファへデータ転送
	VertexPos* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	if (SUCCEEDED(result))
	{
		//パーティクルの情報を1つずつ反映
		for (std::list<polygon>::iterator it = Polygons.begin(); it != Polygons.end(); it++)
		{
			//座標
			vertMap->pos = it->position[0];
			vertMap->color = it->color;
			//次の頂点へ
			vertMap++;
			//座標
			vertMap->pos = it->position[1];
			vertMap->color = it->color;
			//次の頂点へ
			vertMap++;
			//座標
			vertMap->pos = it->position[2];
			vertMap->color = it->color;
			//次の頂点へ
			vertMap++;
		}
		vertBuff->Unmap(0, nullptr);
	}

	UpdateViewMatrix();
	// 定数バッファへデータ転送
	ConstBufferDataB0* constMap = nullptr;
	result = constBuffB0->Map(0, nullptr, (void**)&constMap);
	constMap->mat = matWorld * matViewPro;	// 行列の合成
	constBuffB0->Unmap(0, nullptr);
}

void PolygonMana::Draw()
{
	// nullptrチェック
	assert(device);
	assert(cmdList);

	// 頂点バッファの設定
	cmdList->IASetVertexBuffers(0, 1, &vbView);

	// 定数バッファビューをセット
	cmdList->SetGraphicsRootConstantBufferView(0, constBuffB0->GetGPUVirtualAddress());

	for (std::list<polygon>::iterator it = Polygons.begin(); it != Polygons.end(); it++)
	{

		cmdList->DrawInstanced(_countof(it->position), 1, 0, 0);
	}
}

void PolygonMana::Add(int life, Object3d::VertexPosNormalUv position, Object3d::VertexPosNormalUv position2, Object3d::VertexPosNormalUv position3, Vector3 velocity, Vector3 accel, Vector4 start_color, Vector4 end_color, Matrix4 matworld)
{
	//リストに要素を追加
	Polygons.emplace_front();
	//追加した要素の参照
	polygon& p = Polygons.front();
	//値のセット
	p.position[0] = position.pos;
	p.position[1] = position2.pos;
	p.position[2] = position3.pos;
	p.velocity = velocity;
	p.accel = accel;
	p.num_frame = life;
	p.s_color = start_color;
	p.e_color = end_color;
}


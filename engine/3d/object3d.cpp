#include "Object3d.h"
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

//Object3d::Material Object3d::material;

/// <summary>
/// 静的メンバ変数の実体
/// </summary>
ID3D12Device* Object3d::sDevice = nullptr;
//UINT Object3d::descriptorHandleIncrementSize = 0;
ID3D12GraphicsCommandList* Object3d::sCmdList = nullptr;
ComPtr<ID3D12RootSignature> Object3d::sRootsignature;
ComPtr<ID3D12PipelineState> Object3d::sPipelinestate;
//ComPtr<ID3D12DescriptorHeap> Object3d::descHeap;
//ComPtr<ID3D12Resource> Object3d::texbuff;
//CD3DX12_CPU_DESCRIPTOR_HANDLE Object3d::cpuDescHandleSRV;
//CD3DX12_GPU_DESCRIPTOR_HANDLE Object3d::gpuDescHandleSRV;
Matrix4 Object3d::sMatView{};
Matrix4 Object3d::sMatProjection{};
Vector3 Object3d::sEye = { 0, 0, 0.0f };
Vector3 Object3d::sTarget = { 0, 0, 0 };
Vector3 Object3d::sUp = { 0, 1, 0 };
DirectionalLight* Object3d::sLight = nullptr;
//Object3d::VertexPosNormalUv Object3d::vertices[vertexCount];
//unsigned short Object3d::indices[planeCount * 3];

float ToRadian(float angle)
{
	float PI = 3.141592f;

	return angle * (PI / 180);
}

void Object3d::StaticInitialize(ID3D12Device* device, uint32_t window_width, uint32_t window_height)
{
	// nullptrチェック
	assert(device);

	Object3d::sDevice = device;

	Model::SetDevice(device);

	// カメラ初期化
	InitializeCamera(window_width, window_height);

	// パイプライン初期化
	InitializeGraphicsPipeline();

}

void Object3d::PreDraw(ID3D12GraphicsCommandList* cmdList)
{
	// PreDrawとPostDrawがペアで呼ばれていなければエラー
	assert(Object3d::sCmdList == nullptr);

	// コマンドリストをセット
	Object3d::sCmdList = cmdList;

	// パイプラインステートの設定
	cmdList->SetPipelineState(sPipelinestate.Get());
	// ルートシグネチャの設定
	cmdList->SetGraphicsRootSignature(sRootsignature.Get());
	// プリミティブ形状を設定
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void Object3d::PostDraw()
{
	// コマンドリストを解除
	Object3d::sCmdList = nullptr;
}

void Object3d::SetEye(const Vector3& eye)
{
	Object3d::sEye = eye;

	UpdateViewMatrix();
}

void Object3d::SetTarget(const Vector3& target)
{
	Object3d::sTarget = target;

	UpdateViewMatrix();
}

void Object3d::InitializeCamera(uint32_t window_width, uint32_t window_height)
{
	// ビュー行列の生成
	sMatView = Matrix4Math::ViewMat(
		sEye,
		sTarget,
		sUp);

	// 透視投影による射影行列の生成
	sMatProjection = Matrix4Math::ProjectionMat(
		ToRadian(45.0f),
		(float)window_width / window_height,
		0.1f, 1000.0f
	);
}

void Object3d::InitializeGraphicsPipeline()
{
	HRESULT result = S_FALSE;
	ComPtr<ID3DBlob> vsBlob; // 頂点シェーダオブジェクト
	ComPtr<ID3DBlob> gsBlob;	// ピクセルシェーダオブジェクト
	ComPtr<ID3DBlob> psBlob;	// ピクセルシェーダオブジェクト
	ComPtr<ID3DBlob> errorBlob; // エラーオブジェクト

	// 頂点シェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Resources/shaders/ObjVS.hlsl",	// シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main", "vs_5_0",	// エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&vsBlob, &errorBlob);
	if (FAILED(result)) {
		// errorBlobからエラー内容をstring型にコピー
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	// ピクセルシェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Resources/shaders/ObjGS.hlsl",	// シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main", "gs_5_0",	// エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&gsBlob, &errorBlob);
	if (FAILED(result)) {
		// errorBlobからエラー内容をstring型にコピー
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	// ピクセルシェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Resources/shaders/ObjPS.hlsl",	// シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main", "ps_5_0",	// エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&psBlob, &errorBlob);
	if (FAILED(result)) {
		// errorBlobからエラー内容をstring型にコピー
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	// 頂点レイアウト
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{ // xy座標(1行で書いたほうが見やすい)
			"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{ // 法線ベクトル(1行で書いたほうが見やすい)
			"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{ // uv座標(1行で書いたほうが見やすい)
			"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
	};

	// グラフィックスパイプラインの流れを設定
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{};
	gpipeline.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get());
	gpipeline.GS = CD3DX12_SHADER_BYTECODE(gsBlob.Get());
	gpipeline.PS = CD3DX12_SHADER_BYTECODE(psBlob.Get());

	// サンプルマスク
	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // 標準設定
	// ラスタライザステート
	//gpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	gpipeline.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;//ポリゴン内塗りつぶし
	gpipeline.RasterizerState.DepthClipEnable = true;//深度グリッピングを有効に
	// デプスステンシルステート
	gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);

	// レンダーターゲットのブレンド設定
	D3D12_RENDER_TARGET_BLEND_DESC blenddesc{};
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;	// RBGA全てのチャンネルを描画
	blenddesc.BlendEnable = true;
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;

	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;

	// ブレンドステートの設定
	gpipeline.BlendState.RenderTarget[0] = blenddesc;
	gpipeline.BlendState.RenderTarget[1] = blenddesc;

	// 深度バッファのフォーマット
	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;

	// 頂点レイアウトの設定
	gpipeline.InputLayout.pInputElementDescs = inputLayout;
	gpipeline.InputLayout.NumElements = _countof(inputLayout);

	// 図形の形状設定（三角形）
	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	gpipeline.NumRenderTargets = 2;	// 描画対象は1つ
	gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0～255指定のRGBA
	gpipeline.SampleDesc.Count = 1; // 1ピクセルにつき1回サンプリング

	// デスクリプタレンジ
	CD3DX12_DESCRIPTOR_RANGE descRangeSRV;
	descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0); // t0 レジスタ

	// ルートパラメータ
	/*CD3DX12_ROOT_PARAMETER rootparams[2];
	rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);
	rootparams[1].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);*/

	CD3DX12_ROOT_PARAMETER rootparams[5] = {};
	rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);
	rootparams[1].InitAsConstantBufferView(1, 0, D3D12_SHADER_VISIBILITY_ALL);
	rootparams[2].InitAsConstantBufferView(2, 0, D3D12_SHADER_VISIBILITY_ALL);
	rootparams[3].InitAsConstantBufferView(3, 0, D3D12_SHADER_VISIBILITY_ALL);
	rootparams[4].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);

	// スタティックサンプラー
	CD3DX12_STATIC_SAMPLER_DESC samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0);

	// ルートシグネチャの設定
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
	rootSignatureDesc.Init_1_0(_countof(rootparams), rootparams, 1, &samplerDesc, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	ComPtr<ID3DBlob> rootSigBlob;
	// バージョン自動判定のシリアライズ
	result = D3DX12SerializeVersionedRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);
	// ルートシグネチャの生成
	result = sDevice->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&sRootsignature));
	assert(SUCCEEDED(result));

	gpipeline.pRootSignature = sRootsignature.Get();

	// グラフィックスパイプラインの生成
	result = sDevice->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&sPipelinestate));
	assert(SUCCEEDED(result));

}

//bool Object3d::LoadTexture(const std::string& directoryPath, const std::string& filename)
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

void Object3d::UpdateViewMatrix()
{
	// ビュー行列の生成
	sMatView = Matrix4Math::ViewMat(
		sEye,
		sTarget,
		sUp);
}

bool Object3d::Initialize()
{
	// nullptrチェック
	assert(sDevice);

	HRESULT result;

	// ヒーププロパティ
	CD3DX12_HEAP_PROPERTIES heapProps = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	{
		// リソース設定
		CD3DX12_RESOURCE_DESC resourceDesc =
			CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferDataB0) + 0xff) & ~0xff);

		// 定数バッファの生成
		result = sDevice->CreateCommittedResource(
			&heapProps, // アップロード可能
			D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
			IID_PPV_ARGS(&constBuffB0_));
		//// 定数バッファの生成
		//result = device->CreateCommittedResource(
		//	&heapPropsB1,
		//	D3D12_HEAP_FLAG_NONE,
		//	&resourceDescB1,
		//	D3D12_RESOURCE_STATE_GENERIC_READ,
		//	nullptr,
		//	IID_PPV_ARGS(&constBuffB1));
		assert(SUCCEEDED(result));
	}

	{
		// リソース設定
		CD3DX12_RESOURCE_DESC resourceDesc =
			CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferPolygonExplosion) + 0xff) & ~0xff);

		// 定数バッファの生成
		result = sDevice->CreateCommittedResource(
			&heapProps, // アップロード可能
			D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
			IID_PPV_ARGS(&constBuffB2_));
		//// 定数バッファの生成
		//result = device->CreateCommittedResource(
		//	&heapPropsB1,
		//	D3D12_HEAP_FLAG_NONE,
		//	&resourceDescB1,
		//	D3D12_RESOURCE_STATE_GENERIC_READ,
		//	nullptr,
		//	IID_PPV_ARGS(&constBuffB1));
		assert(SUCCEEDED(result));
	}
	result = constBuffB2_->Map(0, nullptr, (void**)&ConstMapPolygon_);
	return true;
}

void Object3d::Update()
{
	HRESULT result;
	Matrix4 matScale, matRot, matTrans;

	// スケール、回転、平行移動行列の計算
	matScale = Matrix4Math::scale(scale_);
	matRot = Matrix4Math::identity();
	matRot = matRot * Matrix4Math::rotateZ(ToRadian(rotation_.z));
	matRot = matRot * Matrix4Math::rotateX(ToRadian(rotation_.x));
	matRot = matRot * Matrix4Math::rotateY(ToRadian(rotation_.y));
	matTrans = Matrix4Math::translate(position_);

	// ワールド行列の合成
	matWorld_ = Matrix4Math::identity(); // 変形をリセット
	matWorld_ = matWorld_ * matScale; // ワールド行列にスケーリングを反映
	matWorld_ = matWorld_ * matRot; // ワールド行列に回転を反映
	matWorld_ = matWorld_ * matTrans; // ワールド行列に平行移動を反映

	// 親オブジェクトがあれば
	if (parent_ != nullptr) {
		// 親オブジェクトのワールド行列を掛ける
		matWorld_ *= parent_->matWorld_;
	}

	// 定数バッファへデータ転送
	ConstBufferDataB0* constMap = nullptr;
	result = constBuffB0_->Map(0, nullptr, (void**)&constMap);
	/*constMap->color = color;*/
	constMap->viewproj = sMatView * sMatProjection;	// 行列の合成
	constMap->world = matWorld_;
	constMap->cameraPos = sEye;
	constBuffB0_->Unmap(0, nullptr);

}

void Object3d::Draw()
{
	// nullptrチェック
	assert(sDevice);
	assert(Object3d::sCmdList);

	//モデルがセットされていなければ描画をスキップ
	if (model_ == nullptr) {
		return;
	}

	// 定数バッファビューをセット
	sCmdList->SetGraphicsRootConstantBufferView(0, constBuffB0_->GetGPUVirtualAddress());

	// 定数バッファビューをセット
	sCmdList->SetGraphicsRootConstantBufferView(3, constBuffB2_->GetGPUVirtualAddress());

	sLight->Draw(sCmdList,2);

	//モデルを描画
	model_->Draw(sCmdList, 1);
}

void Object3d::Draw(uint32_t texHandle)
{
	// nullptrチェック
	assert(sDevice);
	assert(Object3d::sCmdList);

	//モデルがセットされていなければ描画をスキップ
	if (model_ == nullptr) {
		return;
	}

	// 定数バッファビューをセット
	sCmdList->SetGraphicsRootConstantBufferView(0, constBuffB0_->GetGPUVirtualAddress());

	// 定数バッファビューをセット
	sCmdList->SetGraphicsRootConstantBufferView(3, constBuffB2_->GetGPUVirtualAddress());

	sLight->Draw(sCmdList, 2);

	//モデルを描画
	model_->Draw(sCmdList, 1, texHandle);
}

Matrix4 Object3d::GetMatViewPro()
{

	return sMatView * sMatProjection;
}

//void Object3d::LoadMaterial(const std::string& directoryPath, const std::string& filename)
//{
//	//ファイルストリーム
//	std::ifstream file;
//	//マテリアルファイルを開く
//	file.open(directoryPath + filename);
//	//ファイルオープン失敗をチェック
//	if (file.fail()) {
//		assert(0);
//	}
//
//	//1行ずつ読み込む
//	string line;
//	while (getline(file, line)) {
//
//		//1行分の文字列をストリームに変換
//		std::istringstream line_stream(line);
//		//半角スペース区切りで行の先頭文字列を取得
//		string key;
//		getline(line_stream, key, ' ');
//
//		//先頭のタブ文字は無視する
//		if (key[0] == '\t') {
//			key.erase(key.begin());	//先頭の文字を削除
//		}
//		//先頭文字がnewmtlならマテリアル名
//		if (key == "newmtl") {
//			//マテリアル名読み込み
//			line_stream >> material.name;
//		}
//		//先頭文字がKaならアンビエント色
//		if (key == "Ka") {
//			line_stream >> material.ambient.x;
//			line_stream >> material.ambient.y;
//			line_stream >> material.ambient.z;
//		}
//		//先頭文字がKdならディフューズ色
//		if (key == "Kd") {
//			line_stream >> material.diffuse.x;
//			line_stream >> material.diffuse.y;
//			line_stream >> material.diffuse.z;
//		}
//		//先頭文字列がKsならスペキュラー色
//		if (key == "Ks") {
//			line_stream >> material.specular.x;
//			line_stream >> material.specular.y;
//			line_stream >> material.specular.z;
//		}
//		//先頭文字列がmap_Kdならテクスチャファイル名
//		if (key == "map_Kd") {
//			//テクスチャのファイル名読み込み
//			line_stream >> material.textureFilename;
//			//テクスチャ読み込み
//			LoadTexture(directoryPath, material.textureFilename);
//		}
//	}
//	//ファイルを閉じる
//	file.close();
//}


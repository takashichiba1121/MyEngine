#include "Object3d.h"
#include <d3dcompiler.h>
#include <string>
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
ComPtr<ID3D12PipelineState> Object3d::sPipelinestateModeBack;
ComPtr<ID3D12PipelineState> Object3d::sPipelinestateModeNone;
ComPtr<ID3D12PipelineState> Object3d::sPipelinestateLight;
//ComPtr<ID3D12DescriptorHeap> Object3d::descHeap;
//ComPtr<ID3D12Resource> Object3d::texbuff;
//CD3DX12_CPU_DESCRIPTOR_HANDLE Object3d::cpuDescHandleSRV;
//CD3DX12_GPU_DESCRIPTOR_HANDLE Object3d::gpuDescHandleSRV;
Matrix4 Object3d::sMatView{};
Matrix4 Object3d::sMatProjection{};
Matrix4 Object3d::sMatBillboard{};
Matrix4 Object3d::sYMatBillboard{};
Vector3 Object3d::sEye = { 0, 0, 0.0f };
Vector3 Object3d::sTarget = { 0, 0, 0 };
Vector3 Object3d::sUp = { 0, 1, 0 };
LightGroup* Object3d::sLightGroup = nullptr;
//Object3d::VertexPosNormalUv Object3d::vertices[vertexCount];
//unsigned short Object3d::indices[planeCount * 3];

float ToRadian(float angle)
{
	float PI = 3.141592f;

	return angle * ( PI / 180 );
}

void Object3d::StaticInitialize(ID3D12Device* device,uint32_t window_width,uint32_t window_height)
{
	// nullptrチェック
	assert(device);

	Object3d::sDevice = device;

	Model::SetDevice(device);

	// カメラ初期化
	InitializeCamera(window_width,window_height);

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
	sCmdList->SetPipelineState(sPipelinestateModeBack.Get());
	// ルートシグネチャの設定
	sCmdList->SetGraphicsRootSignature(sRootsignature.Get());
	// プリミティブ形状を設定
	sCmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void Object3d::ChangePipeLine(pipelineType Type)
{
	switch ( Type )
	{
	case Object3d::CullBack:
			// パイプラインステートの設定
		sCmdList->SetPipelineState(sPipelinestateModeBack.Get());
		break;
	case Object3d::CullNone:
			// パイプラインステートの設定
		sCmdList->SetPipelineState(sPipelinestateModeNone.Get());
		break;
	case Object3d::Light:
			// パイプラインステートの設定
		sCmdList->SetPipelineState(sPipelinestateLight.Get());
		break;
	default:
		break;
	}
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

void Object3d::InitializeCamera(uint32_t window_width,uint32_t window_height)
{
	// ビュー行列の生成
	sMatView = Matrix4Math::ViewMat(
		sEye,
		sTarget,
		sUp);

	// 透視投影による射影行列の生成
	sMatProjection = Matrix4Math::ProjectionMat(
		ToRadian(45.0f),
		( float ) window_width / window_height,
		0.1f,1000.0f
	);
}

void Object3d::InitializeGraphicsPipeline()
{
	HRESULT result = S_FALSE;
	ComPtr<ID3DBlob> vsBlob; // 頂点シェーダオブジェクト
	ComPtr<ID3DBlob> gsBlob;	// ピクセルシェーダオブジェクト
	ComPtr<ID3DBlob> psBlob;	// ピクセルシェーダオブジェクト
	ComPtr<ID3DBlob> psLightBlob;	// ピクセルシェーダオブジェクト
	ComPtr<ID3DBlob> errorBlob; // エラーオブジェクト

	// 頂点シェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Resources/shaders/3D/ObjVS.hlsl",	// シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main","vs_5_0",	// エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&vsBlob,&errorBlob);
	if ( FAILED(result) )
	{
// errorBlobからエラー内容をstring型にコピー
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n(( char* ) errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	// ピクセルシェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Resources/shaders/3D/ObjGS.hlsl",	// シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main","gs_5_0",	// エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&gsBlob,&errorBlob);
	if ( FAILED(result) )
	{
// errorBlobからエラー内容をstring型にコピー
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n(( char* ) errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	// ピクセルシェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Resources/shaders/3D/ObjPS.hlsl",	// シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main","ps_5_0",	// エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&psBlob,&errorBlob);
	if ( FAILED(result) )
	{
// errorBlobからエラー内容をstring型にコピー
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n(( char* ) errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

		// ピクセルシェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Resources/shaders/3D/ObjNotLightPS.hlsl",	// シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main","ps_5_0",	// エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&psLightBlob,&errorBlob);
	if ( FAILED(result) )
	{
// errorBlobからエラー内容をstring型にコピー
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n(( char* ) errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	// 頂点レイアウト
	D3D12_INPUT_ELEMENT_DESC inputLayout[ ] = {
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
	gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;
	gpipeline.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;//ポリゴン内塗りつぶし
	gpipeline.RasterizerState.DepthClipEnable = true;//深度グリッピングを有効に
	// デプスステンシルステート
	gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);

	// レンダーターゲットのブレンド設定
	D3D12_RENDER_TARGET_BLEND_DESC blenddesc{};
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;	// RBGA全てのチャンネルを描画
	blenddesc.BlendEnable = true;
	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;

	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;

	// ブレンドステートの設定
	gpipeline.BlendState.RenderTarget[ 0 ] = blenddesc;

	// 深度バッファのフォーマット
	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;

	// 頂点レイアウトの設定
	gpipeline.InputLayout.pInputElementDescs = inputLayout;
	gpipeline.InputLayout.NumElements = _countof(inputLayout);

	// 図形の形状設定（三角形）
	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	gpipeline.NumRenderTargets = 1;	// 描画対象は1つ
	gpipeline.RTVFormats[ 0 ] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0～255指定のRGBA
	gpipeline.SampleDesc.Count = 1; // 1ピクセルにつき1回サンプリング

	// デスクリプタレンジ
	CD3DX12_DESCRIPTOR_RANGE descRangeSRV = {};
	descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV,1,0); // t0 レジスタ

	// ルートパラメータ
	/*CD3DX12_ROOT_PARAMETER rootparams[2];
	rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);
	rootparams[1].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);*/

	CD3DX12_ROOT_PARAMETER rootparams[ 5 ] = {};
	rootparams[ 0 ].InitAsConstantBufferView(0,0,D3D12_SHADER_VISIBILITY_ALL);
	rootparams[ 1 ].InitAsConstantBufferView(1,0,D3D12_SHADER_VISIBILITY_ALL);
	rootparams[ 2 ].InitAsConstantBufferView(2,0,D3D12_SHADER_VISIBILITY_ALL);
	rootparams[ 3 ].InitAsConstantBufferView(3,0,D3D12_SHADER_VISIBILITY_ALL);
	rootparams[ 4 ].InitAsDescriptorTable(1,&descRangeSRV,D3D12_SHADER_VISIBILITY_PIXEL);

	// スタティックサンプラー
	CD3DX12_STATIC_SAMPLER_DESC samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0);

	samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;

	// ルートシグネチャの設定
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
	rootSignatureDesc.Init_1_0(_countof(rootparams),rootparams,1,&samplerDesc,D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	ComPtr<ID3DBlob> rootSigBlob;
	// バージョン自動判定のシリアライズ
	result = D3DX12SerializeVersionedRootSignature(&rootSignatureDesc,D3D_ROOT_SIGNATURE_VERSION_1_0,&rootSigBlob,&errorBlob);
	// ルートシグネチャの生成
	result = sDevice->CreateRootSignature(0,rootSigBlob->GetBufferPointer(),rootSigBlob->GetBufferSize(),IID_PPV_ARGS(&sRootsignature));
	assert(SUCCEEDED(result));

	gpipeline.pRootSignature = sRootsignature.Get();

	// グラフィックスパイプラインの生成
	result = sDevice->CreateGraphicsPipelineState(&gpipeline,IID_PPV_ARGS(&sPipelinestateModeBack));
	assert(SUCCEEDED(result));

	gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;

		// グラフィックスパイプラインの生成
	result = sDevice->CreateGraphicsPipelineState(&gpipeline,IID_PPV_ARGS(&sPipelinestateModeNone));
	assert(SUCCEEDED(result));

	gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;

	gpipeline.PS= CD3DX12_SHADER_BYTECODE(psLightBlob.Get());

	// グラフィックスパイプラインの生成
	result = sDevice->CreateGraphicsPipelineState(&gpipeline,IID_PPV_ARGS(&sPipelinestateLight));
	assert(SUCCEEDED(result));
}

void Object3d::UpdateViewMatrix()
{
	// ビュー行列の生成
	sMatView = Matrix4Math::ViewMat(
		sEye,
		sTarget,
		sUp);

	//視点座標
	Vector3 eyePosition = sEye;
	//注視点座標X
	Vector3 targetPosition = sTarget;
	//(仮の)上方向
	Vector3 upVector = sUp;

	//カメラZ軸(視点方向)
	Vector3 cameraAxisZ = targetPosition - eyePosition;

	//ベクトルを正規化
	cameraAxisZ.normalize();

	//カメラのX軸(右方向)
	Vector3 cameraAxisX;
	//X軸は上方向→Z軸の外積で求まる
	cameraAxisX = upVector.cross(cameraAxisZ);
	//ベクトルを正規化
	cameraAxisX.normalize();

	//カメラのY軸(上方向)
	Vector3 cameraAxisY;
	//Y軸は上方向→Z軸の外積で求まる
	cameraAxisY = cameraAxisZ.cross(cameraAxisX);
	//ベクトルを正規化
	cameraAxisY.normalize();

	sMatBillboard = Matrix4Math::identity();

	//ビルボード行列
	sMatBillboard.m[ 0 ][ 0 ] = cameraAxisX.x;
	sMatBillboard.m[ 0 ][ 1 ] = cameraAxisX.y;
	sMatBillboard.m[ 0 ][ 2 ] = cameraAxisX.z;
	sMatBillboard.m[ 1 ][ 0 ] = cameraAxisY.x;
	sMatBillboard.m[ 1 ][ 1 ] = cameraAxisY.y;
	sMatBillboard.m[ 1 ][ 2 ] = cameraAxisY.z;
	sMatBillboard.m[ 2 ][ 0 ] = cameraAxisZ.x;
	sMatBillboard.m[ 2 ][ 1 ] = cameraAxisZ.y;
	sMatBillboard.m[ 2 ][ 2 ] = cameraAxisZ.z;
	sMatBillboard.m[ 3 ][ 3 ] = 1;

		//カメラ軸、Y軸、Z軸
	Vector3 ybillCameraAxisX,ybillCameraAxisY,ybillCameraAxisZ;

	//X軸は共通
	ybillCameraAxisX = cameraAxisX;
	//Y軸はワールド座標系のY軸
	ybillCameraAxisY = upVector.normalize();
	//Z軸はX軸→Y軸の外積で求まる
	ybillCameraAxisZ = ybillCameraAxisX.cross(ybillCameraAxisY);

		//Y軸ビルボード行列
	sYMatBillboard.m[ 0 ][ 0 ] = ybillCameraAxisX.x;
	sYMatBillboard.m[ 0 ][ 1 ] = ybillCameraAxisX.y;
	sYMatBillboard.m[ 0 ][ 2 ] = ybillCameraAxisX.z;
	sYMatBillboard.m[ 1 ][ 0 ] = ybillCameraAxisY.x;
	sYMatBillboard.m[ 1 ][ 1 ] = ybillCameraAxisY.y;
	sYMatBillboard.m[ 1 ][ 2 ] = ybillCameraAxisY.z;
	sYMatBillboard.m[ 2 ][ 0 ] = ybillCameraAxisZ.x;
	sYMatBillboard.m[ 2 ][ 1 ] = ybillCameraAxisZ.y;
	sYMatBillboard.m[ 2 ][ 2 ] = ybillCameraAxisZ.z;
	sYMatBillboard.m[ 3 ][ 3 ] = 1;
}

void Object3d::Finalize()
{
	sPipelinestateModeBack = nullptr;
	sPipelinestateModeNone = nullptr;
	sPipelinestateLight = nullptr;
	sRootsignature = nullptr;
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
			CD3DX12_RESOURCE_DESC::Buffer(( sizeof(ConstBufferDataB0) + 0xff ) & ~0xff);

		// 定数バッファの生成
		result = sDevice->CreateCommittedResource(
			&heapProps, // アップロード可能
			D3D12_HEAP_FLAG_NONE,&resourceDesc,D3D12_RESOURCE_STATE_GENERIC_READ,nullptr,
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
			CD3DX12_RESOURCE_DESC::Buffer(( sizeof(ConstBufferPolygonExplosion) + 0xff ) & ~0xff);

		// 定数バッファの生成
		result = sDevice->CreateCommittedResource(
			&heapProps, // アップロード可能
			D3D12_HEAP_FLAG_NONE,&resourceDesc,D3D12_RESOURCE_STATE_GENERIC_READ,nullptr,
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
	ConstMapPolygon_ = { 0.0f,1.0f,0.0f,0.0f };


	return true;
}

void Object3d::Update()
{
	HRESULT result;
	Matrix4 matScale,matRot,matTrans;

	// スケール、回転、平行移動行列の計算
	matScale = Matrix4Math::scale(scale_);
	matRot = Matrix4Math::identity();
	matRot = matRot * Matrix4Math::rotateZ(( rotation_.z ));
	matRot = matRot * Matrix4Math::rotateX(( rotation_.x ));
	matRot = matRot * Matrix4Math::rotateY(( rotation_.y ));
	matTrans = Matrix4Math::translate(position_);

	// ワールド行列の合成
	matWorld_ = Matrix4Math::identity(); // 変形をリセット
	matWorld_ = matWorld_ * matScale; // ワールド行列にスケーリングを反映
	matWorld_ = matWorld_ * matRot; // ワールド行列に回転を反映
	switch ( isBillboard_ )
	{
	case Object3d::On:
		matWorld_ = matWorld_ * sMatBillboard;
		break;
	case Object3d::YBillboardOn:
		matWorld_ = matWorld_ * sYMatBillboard;
		break;
	}
	matWorld_ = matWorld_ * matTrans; // ワールド行列に平行移動を反映

	// 親オブジェクトがあれば
	if ( parent_ != nullptr )
	{
// 親オブジェクトのワールド行列を掛ける
		matWorld_ *= parent_->matWorld_;
	}

	// 定数バッファへデータ転送
	ConstBufferDataB0* constMap = nullptr;
	result = constBuffB0_->Map(0,nullptr,( void** ) &constMap);
	constMap->viewproj = sMatView * sMatProjection;	// 行列の合成
	constMap->world = matWorld_;
	constMap->cameraPos = sEye;
	constMap->shininess = shininess_;
	constMap->alpha = alpha_;
	constMap->color = color_;
	constMap->uvShift = uvShift_;
	constBuffB0_->Unmap(0,nullptr);

	ConstBufferPolygonExplosion* constMap2 = nullptr;

	result = constBuffB2_->Map(0,nullptr,( void** ) &constMap2);
	constMap2->_Destruction = ConstMapPolygon_._Destruction;	// 行列の合成
	constMap2->_PositionFactor = ConstMapPolygon_._PositionFactor;
	constMap2->_RotationFactor = ConstMapPolygon_._RotationFactor;
	constMap2->_ScaleFactor = ConstMapPolygon_._ScaleFactor;
	constBuffB2_->Unmap(0,nullptr);
}

void Object3d::Draw()
{
	// nullptrチェック
	assert(sDevice);
	assert(Object3d::sCmdList);

	//モデルがセットされていなければ描画をスキップ
	if ( model_ == nullptr||isDraw_==false )
	{
		return;
	}

	// 定数バッファビューをセット
	sCmdList->SetGraphicsRootConstantBufferView(0,constBuffB0_->GetGPUVirtualAddress());

	// 定数バッファビューをセット
	sCmdList->SetGraphicsRootConstantBufferView(3,constBuffB2_->GetGPUVirtualAddress());

	sLightGroup->Draw(sCmdList,2);

		// 頂点バッファの設定
	sCmdList->IASetVertexBuffers(0,1,&vbView_);

	//モデルを描画
	model_->Draw(sCmdList,1);
}

void Object3d::Draw(uint32_t texHandle)
{
	// nullptrチェック
	assert(sDevice);
	assert(Object3d::sCmdList);

	//モデルがセットされていなければ描画をスキップ
	if ( model_ == nullptr||isDraw_==false )
	{
		return;
	}

	// 定数バッファビューをセット
	sCmdList->SetGraphicsRootConstantBufferView(0,constBuffB0_->GetGPUVirtualAddress());

	// 定数バッファビューをセット
	sCmdList->SetGraphicsRootConstantBufferView(3,constBuffB2_->GetGPUVirtualAddress());

	sLightGroup->Draw(sCmdList,2);

		// 頂点バッファの設定
	sCmdList->IASetVertexBuffers(0,1,&vbView_);

	//モデルを描画
	model_->Draw(sCmdList,1,texHandle);
}

void Object3d::CreateBuffers()
{
	HRESULT result = S_FALSE;

	uint32_t sizeVB = static_cast< uint32_t >( sizeof(Model::VertexPosNormalUv) * vertices_.size() );

	// ヒーププロパティ
	CD3DX12_HEAP_PROPERTIES heapProps = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	// リソース設定
	CD3DX12_RESOURCE_DESC resourceDesc = CD3DX12_RESOURCE_DESC::Buffer(sizeVB);

	// 頂点バッファ生成
	result = sDevice->CreateCommittedResource(
		&heapProps,D3D12_HEAP_FLAG_NONE,&resourceDesc,D3D12_RESOURCE_STATE_GENERIC_READ,nullptr,
		IID_PPV_ARGS(&vertBuff_));
	assert(SUCCEEDED(result));

	// 頂点バッファへのデータ転送
	Model::VertexPosNormalUv* vertMap = nullptr;
	result = vertBuff_->Map(0,nullptr,( void** ) &vertMap);
	if ( SUCCEEDED(result) )
	{
/*memcpy(vertMap, vertices, sizeof(vertices));*/
		std::copy(vertices_.begin(),vertices_.end(),vertMap);
		vertBuff_->Unmap(0,nullptr);
	}

	// 頂点バッファビューの作成
	vbView_.BufferLocation = vertBuff_->GetGPUVirtualAddress();
	/*vbView.SizeInBytes = sizeof(vertices);*/
	vbView_.SizeInBytes = sizeVB;
	vbView_.StrideInBytes = sizeof(vertices_[ 0 ]);
}

void Object3d::SetScale(const Vector3& scale)
{
	this->scale_ = scale;
}

void Object3d::SetModel(Model* model)
{
	model_ = model;

	vertices_ = model->GetVertices();

	initialVertices_ = model->GetVertices();

	CreateBuffers();
}

void Object3d::Setalpha(float alpha)
{
	alpha_ = alpha;
	if (alpha_>1.0f )
	{
		alpha_ = 1;
	}
	if(alpha_<0.0f)
	{
		alpha_ = 0;
	}
}

void Object3d::SetVertices(std::vector<Model::VertexPosNormalUv> vertices)
{
	HRESULT result;

	vertices_ = vertices;

			// 頂点バッファへのデータ転送
	Model::VertexPosNormalUv* vertMap = nullptr;
	result = vertBuff_->Map(0,nullptr,( void** ) &vertMap);
	if ( SUCCEEDED(result) )
	{
		std::copy(vertices_.begin(),vertices_.end(),vertMap);
		vertBuff_->Unmap(0,nullptr);
	}
}

Matrix4 Object3d::GetMatViewPro()
{

	return sMatView * sMatProjection;
}
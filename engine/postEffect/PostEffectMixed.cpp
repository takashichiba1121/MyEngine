#include "PostEffectMixed.h"
#include<d3dx12.h>
#include"WinApp.h"
#include"Texture.h"
#include"SpriteCommon.h"

using namespace DirectX;

const float PostEffectMixed::clearColor[4] = {0.0f,0.0f,0.0f,0.0f};

void PostEffectMixed::Initialize(PostEffectCommon* PECommon)
{
	this->PECommon = PECommon;
	
	HRESULT result;

	D3D12_RESOURCE_DESC texresDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,
		WinApp::window_width,
		(UINT)WinApp::window_height,
		1,0,1,0,D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET);
	{
		//テクスチャバッファの生成
		//ヒープ設定
		CD3DX12_HEAP_PROPERTIES heapProp(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK,
			D3D12_MEMORY_POOL_L0);

		CD3DX12_CLEAR_VALUE clearValue(DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, clearColor);


		result = this->PECommon->device->CreateCommittedResource(
			&heapProp,
			D3D12_HEAP_FLAG_NONE,
			&texresDesc,
			D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
			&clearValue,
			IID_PPV_ARGS(&texBuff));
		assert(SUCCEEDED(result));

		{//テクスチャを赤クリア
			//画素数(1280*720=921600ピクセル)
			const UINT pixelCount = WinApp::window_width * WinApp::window_height;
			//画像1行分のデータサイズ
			const UINT rowPitch = sizeof(UINT) * WinApp::window_width;
			//画像全体のデータサイズ
			const UINT depthPitch = rowPitch * WinApp::window_height;
			//画像イメージ
			UINT* img = new UINT[pixelCount];
			for (int i = 0; i < pixelCount; i++) { img[i] = 0xff0000ff; }


			result = texBuff->WriteToSubresource(0, nullptr,
				img, rowPitch, depthPitch);
			assert(SUCCEEDED(result));
			delete[] img;
		}
	}
	textureHandle = this->PECommon->CreateDescHeapSRV(texBuff.Get());

	//RTV用のデスクリプタヒープ設定
	D3D12_DESCRIPTOR_HEAP_DESC rtvDescHeapDesc{};

	rtvDescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvDescHeapDesc.NumDescriptors = 1;
	
	//RTV用のデスクリプタヒープを生成
	result = this->PECommon->device->CreateDescriptorHeap(&rtvDescHeapDesc, IID_PPV_ARGS(&descHeapRTV));
	assert(SUCCEEDED(result));
	//レンダーターゲットビューも設定
	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};
	//シェーダーの計算結果をSRGBに変換して書き込む
	rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
	//デスクリプタヒープにRTVを作成
	this->PECommon->device->CreateRenderTargetView(texBuff.Get(), &rtvDesc, descHeapRTV->GetCPUDescriptorHandleForHeapStart());

	//深度バッファ
	CD3DX12_RESOURCE_DESC depthResDesc =
		CD3DX12_RESOURCE_DESC::Tex2D(
			DXGI_FORMAT_D32_FLOAT,
			WinApp::window_width,
			WinApp::window_height,
			1, 0,
			1, 0,
			D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL);

	CD3DX12_HEAP_PROPERTIES HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

	CD3DX12_CLEAR_VALUE CLEAR_VALUE(DXGI_FORMAT_D32_FLOAT, 1.0f, 0);

	result = this->PECommon->device->CreateCommittedResource(
		&HEAP_PROPERTIES,
		D3D12_HEAP_FLAG_NONE,
		&depthResDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,
		&CLEAR_VALUE,
		IID_PPV_ARGS(&depthBuff));
	assert(SUCCEEDED(result));

	//DSV用のデスクリプタヒープ設定
	D3D12_DESCRIPTOR_HEAP_DESC DescHeapDesc{};
	DescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	DescHeapDesc.NumDescriptors = 1;
	//DSV用デスクリプタヒープの作成
	result = this->PECommon->device->CreateDescriptorHeap(&DescHeapDesc, IID_PPV_ARGS(&descHeapDSV));
	assert(SUCCEEDED(result));

	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	this->PECommon->device->CreateDepthStencilView(depthBuff.Get(),
		&dsvDesc,
		descHeapDSV->GetCPUDescriptorHandleForHeapStart());


	/*Sprite::Initialize();

	SetPosition({ 0,0 });

	SetScale({100,100});*/

	Vertex vertices_[4] = {
		{{-1.0f,-1.0f,0.0f},{0.0f,1.0f}},
		{{-1.0f,+1.0f,0.0f},{0.0f,0.0f}},
		{{+1.0f,-1.0f,0.0f},{1.0f,1.0f}},
		{{+1.0f,+1.0f,0.0f},{1.0f,0.0f}},
	};
	for (int i = 0; i < 4; i++)
	{
		vertices[i] = vertices_[i];
	}

	//頂点データ全体のサイズ=頂点データ一つ分のサイズ*頂点データの要素数
	UINT sizeVB = static_cast<UINT>(sizeof(vertices[0]) * _countof(vertices));
	//頂点バッファの設定
	D3D12_HEAP_PROPERTIES heapProp{};  //ヒープ設定
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD; //GPUへの転送用
	//リソース設定
	D3D12_RESOURCE_DESC resDesc{};
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeVB; //頂点データ全体のサイズ
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	//頂点バッファの生成
	result = this->PECommon->device->CreateCommittedResource(
		&heapProp, //ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&resDesc, //リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff));
	assert(SUCCEEDED(result));

	//GPU上のバッファに対応した仮想メモリ（メインメモリ上）を取得
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result));
	//前頂点に対して
	for (int i = 0; i < _countof(vertices); i++) {
		vertMap[i] = vertices[i]; //座標コピー
	}

	//GPU仮想アドレス
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	//頂点バッファのサイズ
	vbView.SizeInBytes = sizeVB;
	//頂点一つ分のデータサイズ
	vbView.StrideInBytes = sizeof(vertices[0]);

	CreatGraphicsPipelineState();
}

void PostEffectMixed::CreatGraphicsPipelineState()
{
	HRESULT result;

	Microsoft::WRL::ComPtr<ID3DBlob> vsBlob = nullptr;//頂点シェーダーオブジェクト
	Microsoft::WRL::ComPtr<ID3DBlob> psBlob = nullptr;//ピクセルシェーダーオブジェクト

	ID3DBlob* errorBlob = nullptr;//エラーオブジェクト

	//頂点シェーダーの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Resources/shaders/PostEffectMixedVS.hlsl", //シェーダーファイル名
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
		L"Resources/shaders/PostEffectMixedPS.hlsl",
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
		"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,
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

	//デスクリプタレンジの設定
	D3D12_DESCRIPTOR_RANGE descriptorRange0{};
	descriptorRange0.NumDescriptors = 1;//一度の描画に使うテクスチャが一枚なので１
	descriptorRange0.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRange0.BaseShaderRegister = 0;//テクスチャレジスタ0番
	descriptorRange0.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
	//デスクリプタレンジの設定
	D3D12_DESCRIPTOR_RANGE descriptorRange1{};
	descriptorRange1.NumDescriptors = 1;//一度の描画に使うテクスチャが一枚なので１
	descriptorRange1.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRange1.BaseShaderRegister = 1;//テクスチャレジスタ0番
	descriptorRange1.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;


	//ルートパラメーターの設定
	D3D12_ROOT_PARAMETER rootParams[2] = {};
	//テクスチャレジスタ1番
	rootParams[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;//種類
	rootParams[0].DescriptorTable.pDescriptorRanges = &descriptorRange0;//デスクリプタレンジ
	rootParams[0].DescriptorTable.NumDescriptorRanges = 1;//デスクリプタレンジ数
	rootParams[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;//全てのシェーダーから見える
	//テクスチャレジスタ1番
	rootParams[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;//種類
	rootParams[1].DescriptorTable.pDescriptorRanges = &descriptorRange1;//デスクリプタレンジ
	rootParams[1].DescriptorTable.NumDescriptorRanges = 1;//デスクリプタレンジ数
	rootParams[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;//全てのシェーダーから見える

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
	result = this->PECommon->device->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(&rootSignature));
	assert(SUCCEEDED(result));
	//rootSigBlob->Release();
	//パイプラインにルートシグネチャをセット
	pipelineDesc.pRootSignature = rootSignature.Get();

	//パイプランスステートの生成
	result = this->PECommon->device->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&pipelineState));
	assert(SUCCEEDED(result));
}

void PostEffectMixed::PreDrawScene(ID3D12GraphicsCommandList* cmdList)
{
	commandList=cmdList;

	CD3DX12_RESOURCE_BARRIER resouceBar = CD3DX12_RESOURCE_BARRIER::Transition(texBuff.Get(),
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
		D3D12_RESOURCE_STATE_RENDER_TARGET);

	//リソースバリアを変更(シェーダーリソース→描画可能)
	commandList->ResourceBarrier(1,
		&resouceBar);

	//レンダーターゲットビュー用のディスクリプタヒープのハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE rtvH = descHeapRTV->GetCPUDescriptorHandleForHeapStart();
	//深度ステンシルビュー用デスクリプタヒープのハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE dsvH = descHeapDSV->GetCPUDescriptorHandleForHeapStart();
	//レンダーターゲットをセット
	commandList->OMSetRenderTargets(1, &rtvH, false, &dsvH);

	CD3DX12_VIEWPORT viewPort(0.0f, 0.0f, WinApp::window_width, WinApp::window_height);

	//ビューポートの設定
	commandList->RSSetViewports(1, &viewPort);

	CD3DX12_RECT rect(0, 0, WinApp::window_width, WinApp::window_height);

	//シザリング矩形の設定
	commandList->RSSetScissorRects(1, &rect);

	//全画面のクリア
	commandList->ClearRenderTargetView(rtvH, clearColor, 0, nullptr);
	//深度バッファのクリア
	commandList->ClearDepthStencilView(dsvH, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
}

void PostEffectMixed::Draw(uint32_t textureHandle)
{
	//パイプラインステートとルートシグネチャの設定コマンド
	commandList->SetPipelineState(pipelineState.Get());
	commandList->SetGraphicsRootSignature(rootSignature.Get());

	//SRVヒープの設定コマンド
	ID3D12DescriptorHeap* ppHeaps[] = { this->PECommon->descHeapSRV.Get()};
	commandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	//プリミティブ形状の設定コマンド
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	//頂点バッファビューの設定コマンド
	commandList->IASetVertexBuffers(0, 1, &vbView);
	{
		//画像描画
		//SRVヒープの先頭ハンドルを取得（SRVを指しているはず）
		D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = this->PECommon->descHeapSRV->GetGPUDescriptorHandleForHeapStart();
		UINT incrementSize = this->PECommon->device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
		srvGpuHandle.ptr += incrementSize * this->textureHandle;
		commandList->SetGraphicsRootDescriptorTable(0, srvGpuHandle);
	}
	{
		//画像描画
	//SRVヒープの先頭ハンドルを取得（SRVを指しているはず）
		D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = this->PECommon->descHeapSRV->GetGPUDescriptorHandleForHeapStart();
		UINT incrementSize = this->PECommon->device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
		srvGpuHandle.ptr += incrementSize * textureHandle;
		commandList->SetGraphicsRootDescriptorTable(1, srvGpuHandle);
	}
	//描画コマンド
	commandList->DrawInstanced(_countof(vertices), 1, 0, 0);//すべての頂点を使って描画
}

void PostEffectMixed::PostDrawScene()
{
	CD3DX12_RESOURCE_BARRIER RESOURCE_BARRIER= CD3DX12_RESOURCE_BARRIER::Transition(texBuff.Get(),
		D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);

	commandList->ResourceBarrier(1, &RESOURCE_BARRIER);
}

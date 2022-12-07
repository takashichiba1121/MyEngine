#include "SpriteCommon.h"

void SpriteCommon::Initialize(DirectXCommon* dxCommon)
{
	HRESULT result;

	Microsoft::WRL::ComPtr<ID3DBlob> vsBlob = nullptr;//頂点シェーダーオブジェクト
	Microsoft::WRL::ComPtr<ID3DBlob> psBlob = nullptr;//ピクセルシェーダーオブジェクト

	this->dxCommon = dxCommon;
	ID3DBlob* errorBlob = nullptr;//エラーオブジェクト

	//頂点シェーダーの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Resources/shaders/SpriteVS.hlsl", //シェーダーファイル名
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
		L"Resources/shaders/SpritePS.hlsl",
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
	rootParams[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;//定数バッファ
	rootParams[0].Descriptor.ShaderRegister = 0;//定数バッファ番号
	rootParams[0].Descriptor.RegisterSpace = 0;//デフォルト値
	rootParams[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;//全てのシェーダーから見える

	//ルートシグネチャ
	//ルートシグネチャの設定
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	rootSignatureDesc.pParameters = rootParams;//ルートパラメーターの先頭アドレス
	rootSignatureDesc.NumParameters = _countof(rootParams);//ルートパラメータ数
	//ルートシグネチャのシリアライズ
	Microsoft::WRL::ComPtr<ID3DBlob> rootSigBlob = nullptr;
	result = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0,
		&rootSigBlob, &errorBlob);
	assert(SUCCEEDED(result));
	result = dxCommon->GetDevice()->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(&rootSignature));
	assert(SUCCEEDED(result));
	rootSigBlob->Release();
	//パイプラインにルートシグネチャをセット
	pipelineDesc.pRootSignature = rootSignature.Get();

	//パイプランスステートの生成
	result = dxCommon->GetDevice()->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&pipelineState));
	assert(SUCCEEDED(result));
}

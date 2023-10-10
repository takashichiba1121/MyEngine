 #include "Sound.h"

Microsoft::WRL::ComPtr<IXAudio2>Sound::sXAudio2;
IXAudio2MasteringVoice* Sound::sMasterVoice;

Sound::~Sound()
{
	//バッファのメモリを解放
	delete[] soundData_.pBuffer_;

	soundData_.pBuffer_ = 0;
	soundData_.bufferSize_ = 0;
	soundData_.wfex_ = {};
}

void Sound::StaticInitialize()
{

	HRESULT result;
	result = XAudio2Create(&sXAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);
	result = sXAudio2->CreateMasteringVoice(&sMasterVoice);

}

void Sound::StaticFinalize()
{
	// xaudio2の解放
	sXAudio2.Reset();
}

//音声データの読み込み
void Sound::Load(const char* filename) {

	//-------①ファイルオープン-------//

	//ファイル入力ストリームのインスタンス
	std::ifstream file;
	//.wavファイルをバイナリモードで開く
	file.open(filename, std::ios_base::binary);
	//ファイルオープン失敗を検出する
	assert(file.is_open());

	//-------②.wavデータ読み込み-------//

	//RIFFヘッダーの読み込み
	RiffHeader riff;
	file.read((char*)&riff, sizeof(riff));

	//ファイルがRIFFかチェック
	if (strncmp(riff.chunk.id, "RIFF", 4) != 0) {
		assert(0);
	}

	//タイプがWAVEかチェック
	if (strncmp(riff.type, "WAVE", 4) != 0) {
		assert(0);
	}

	//Formatチャンクの読み込み
	FormatChunk format = {};

	//チャンクヘッダーの確認
	file.read((char*)&format, sizeof(ChunkHeader));
	if (strncmp(format.chunk.id, "fmt ", 4) != 0) {
		assert(0);
	}

	//チャンク本体の読み込み
	assert(format.chunk.size <= sizeof(format.fmt));
	file.read((char*)&format.fmt, format.chunk.size);

	//Dataチャンクの読み込み
	ChunkHeader data;
	file.read((char*)&data, sizeof(data));

	//bextチャンクを検出した場合
	if (strncmp(data.id, "bext", 4) == 0) {
		//読み込み位置をJUNKチャンクの終わるまで進める
		file.seekg(data.size, std::ios_base::cur);
		//再読み込み
		file.read((char*)&data, sizeof(data));
	}

	//JUNKチャンクを検出した場合
	if (strncmp(data.id, "JUNK", 4) == 0) {
		//読み込み位置をJUNKチャンクの終わるまで進める
		file.seekg(data.size, std::ios_base::cur);
		//再読み込み
		file.read((char*)&data, sizeof(data));
	}

	//JUNKチャンクを検出した場合
	if (strncmp(data.id, "junk", 4) == 0) {
		//読み込み位置をJUNKチャンクの終わるまで進める
		file.seekg(data.size, std::ios_base::cur);
		//再読み込み
		file.read((char*)&data, sizeof(data));
	}
	//LISTチャンクを検出した場合
	if (strncmp(data.id, "LIST", 4) == 0) {
		//読み込み位置をJUNKチャンクの終わるまで進める
		file.seekg(data.size, std::ios_base::cur);
		//再読み込み
		file.read((char*)&data, sizeof(data));
	}

	if (strncmp(data.id, "data", 4) != 0) {
		assert(0);
	}

	//Dataチャンクのデータ部(波形データ)の読み込み
	char* pBuffer = new char[data.size];
	file.read(pBuffer, data.size);

	//Waveファイルを閉じる
	file.close();

	//-------③読み込んだ音声データをreturn-------//

	//returnする為の音声データ
	SoundData soundData = {};

	soundData.wfex_ = format.fmt;
	soundData.pBuffer_ = reinterpret_cast<BYTE*>(pBuffer);
	soundData.bufferSize_ = data.size;

	soundData_ = soundData;

}

//------サウンドの再生-------//

//音声再生
void Sound::Play(bool loop, float volume) {
	HRESULT result;

	//波形フォーマットを元にSourceVoiceの生成
	result = sXAudio2->CreateSourceVoice(&pSourceVoice_, &soundData_.wfex_);
	assert(SUCCEEDED(result));

	//再生する波形データの設定
	buf_.pAudioData = soundData_.pBuffer_;
	buf_.AudioBytes = soundData_.bufferSize_;

	pSourceVoice_->SetVolume(volume);

	if (loop)
	{
		buf_.LoopCount = XAUDIO2_LOOP_INFINITE;
	}
	buf_.Flags = XAUDIO2_END_OF_STREAM;


	//波形データの再生
	result = pSourceVoice_->SubmitSourceBuffer(&buf_);
	result = pSourceVoice_->Start();

}

//----------音声ストップ------------//
void Sound::Stop()
{
	HRESULT result;
	if (pSourceVoice_ != nullptr)
	{
		result = pSourceVoice_->Stop();
		result = pSourceVoice_->FlushSourceBuffers();
		result = pSourceVoice_->SubmitSourceBuffer(&buf_);
	}
}
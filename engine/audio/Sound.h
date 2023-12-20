/*
* 音クラス
*/

#pragma once

#include <xaudio2.h>
#pragma comment(lib,"xaudio2.lib")
#include <fstream>
#include <array>
#include <cstdint>
#include <set>
#include <string>
#include <map>
#include <combaseapi.h>
#include <Windows.h>
#include <cassert>
#include <iterator>
#include <wrl.h>


//チャンクヘッダ
struct ChunkHeader {
	char id[4];		//チャンク毎のID
	uint32_t size;	//チャンクサイズ 
};

//RIFFヘッダチャンク
struct RiffHeader {
	ChunkHeader chunk;	//"RIFF"
	char type[4];		//"WAVE"
};

//FMTチャンク
struct FormatChunk {
	ChunkHeader chunk;	//"fmt"
	WAVEFORMATEX fmt;	//波形フォーマット
};

class Sound
{
public:
	static Microsoft::WRL::ComPtr<IXAudio2> sXAudio2;
	static IXAudio2MasteringVoice* sMasterVoice;
	~Sound();

	// 初期化
	static void StaticInitialize();
	//終了処理
	static void StaticFinalize();
	//音声読み込み
	void Load(const char* filename);
	//音声再生
	void Play(bool loop = false, float volume = 1.0f);
	// 音声停止
	void Stop();


private:
	//音声データ
	struct SoundData {
		//波形フォーマット
		WAVEFORMATEX wfex_;
		//バッファの先頭アドレス
		BYTE* pBuffer_;
		//バッファサイズ
		uint32_t bufferSize_;
	};

	SoundData soundData_;

	IXAudio2SourceVoice* pSourceVoice_ = nullptr;

	//再生する波形データの設定
	XAUDIO2_BUFFER buf_{};

	bool isPlay_=false;
};
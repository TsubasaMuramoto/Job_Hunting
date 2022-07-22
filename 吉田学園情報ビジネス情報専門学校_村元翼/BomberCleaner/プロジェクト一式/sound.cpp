//===========================================================
// サウンド処理 [sound.cpp]
// Author : 村元翼
//===========================================================
#include "sound.h"

CSound::PARAM const CSound::m_aParam[SOUND_LABEL_MAX] =
{
	//=======================================================================================
	// ＢＧＭ
	//=======================================================================================
	{ "data/BGM/Title_BGM.wav"	, XAUDIO2_LOOP_INFINITE },			// タイトルのBGM
	{ "data/BGM/Game_BGM.wav"	, XAUDIO2_LOOP_INFINITE },			// ゲームのBGM
	{ "data/BGM/Result_BGM.wav"	, XAUDIO2_LOOP_INFINITE },			// リザルトのBGM
	
	//=======================================================================================
	// ＳＥ
	//=======================================================================================
	{ "data/SE/Decide_SE.wav"		,	XAUDIO2_NO_LOOP_REGION },		// 決定音
	{ "data/SE/Jump_SE.wav"			,	XAUDIO2_NO_LOOP_REGION },		// ジャンプ音
	{ "data/SE/Start_SE.wav"		,	XAUDIO2_NO_LOOP_REGION },		// スタート音
	{ "data/SE/Ignition_SE.wav"		,	XAUDIO2_LOOP_INFINITE },		// 発火音
	{ "data/SE/Explosion2_SE.wav"	,	XAUDIO2_NO_LOOP_REGION },		// 爆発音
	{ "data/SE/Destroy_SE.wav"		,	XAUDIO2_NO_LOOP_REGION },		// 破壊音
	{ "data/SE/Cursor_SE.wav"		,	XAUDIO2_NO_LOOP_REGION },		// カーソル音
	{ "data/SE/Cancel_SE.wav"		,	XAUDIO2_NO_LOOP_REGION },		// キャンセル音
	{ "data/SE/TimeUp_SE.wav"		,	XAUDIO2_NO_LOOP_REGION },		// タイムアップ音
	{ "data/SE/Count2_SE.wav"		,	XAUDIO2_NO_LOOP_REGION },		// カウント音
	{ "data/SE/GameStart_SE.wav"	,	XAUDIO2_NO_LOOP_REGION },		// ゲーム開始音
	{ "data/SE/CarryUp_SE.wav"		,	XAUDIO2_NO_LOOP_REGION },		// 持ち上げる音
	{ "data/SE/Throw_SE.wav"		,	XAUDIO2_NO_LOOP_REGION },		// 投げる音
	{ "data/SE/Fall_SE.wav"			,	XAUDIO2_NO_LOOP_REGION },		// 落ちる音

};

//=============================================================================
// コンストラクタ
//=============================================================================
CSound::CSound()
{
}

//=============================================================================
// デストラクタ
//=============================================================================
CSound::~CSound()
{
}

//=============================================================================
// 初期化
//=============================================================================
HRESULT CSound::InitSound(void)
{
	HRESULT hr;

	// COMライブラリの初期化
	CoInitializeEx(NULL, COINIT_MULTITHREADED);

	// XAudio2オブジェクトの作成
	hr = XAudio2Create(&m_pXAudio2, 0);
	if(FAILED(hr))
	{
		// COMライブラリの終了処理
		CoUninitialize();
		return E_FAIL;
	}
	
	// マスターボイスの生成
	hr = m_pXAudio2->CreateMasteringVoice(&m_pMasteringVoice);
	if(FAILED(hr))
	{
		if(m_pXAudio2)
		{
			// XAudio2オブジェクトの開放
			m_pXAudio2->Release();
		}

		// COMライブラリの終了処理
		CoUninitialize();
		return E_FAIL;
	}

	// サウンドデータの初期化
	for(int nCnt = 0; nCnt < SOUND_LABEL_MAX; nCnt++)
	{
		HANDLE hFile;
		DWORD dwChunkSize = 0;
		DWORD dwChunkPosition = 0;
		DWORD filetype;
		WAVEFORMATEXTENSIBLE wfx;
		XAUDIO2_BUFFER buffer;

		// バッファのクリア
		memset(&wfx, 0, sizeof(WAVEFORMATEXTENSIBLE));
		memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));

		// サウンドデータファイルの生成
		hFile = CreateFile(m_aParam[nCnt].filename, GENERIC_READ,FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);

		if(hFile == INVALID_HANDLE_VALUE)
		{
			return HRESULT_FROM_WIN32(GetLastError());
		}
		if(SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
		{
			return HRESULT_FROM_WIN32(GetLastError());
		}
	
		// WAVEファイルのチェック
		hr = CheckChunk(hFile, 'FFIR', &dwChunkSize, &dwChunkPosition);
		if(FAILED(hr))
		{
			return S_FALSE;
		}
		hr = ReadChunkData(hFile, &filetype, sizeof(DWORD), dwChunkPosition);
		if(FAILED(hr))
		{
			return S_FALSE;
		}
		if(filetype != 'EVAW')
		{
			return S_FALSE;
		}
	
		// フォーマットチェック
		hr = CheckChunk(hFile, ' tmf', &dwChunkSize, &dwChunkPosition);
		if(FAILED(hr))
		{
			return S_FALSE;
		}
		hr = ReadChunkData(hFile, &wfx, dwChunkSize, dwChunkPosition);
		if(FAILED(hr))
		{
			return S_FALSE;
		}

		// オーディオデータ読み込み
		hr = CheckChunk(hFile, 'atad', &m_aSizeAudio[nCnt], &dwChunkPosition);
		if(FAILED(hr))
		{
			return S_FALSE;
		}
		m_apDataAudio[nCnt] = (BYTE*)malloc(m_aSizeAudio[nCnt]);
		hr = ReadChunkData(hFile, m_apDataAudio[nCnt], m_aSizeAudio[nCnt], dwChunkPosition);
		if(FAILED(hr))
		{
			return S_FALSE;
		}
	
		m_pXAudio2->CreateSourceVoice(&m_apSourceVoice[nCnt], &(wfx.Format));

		// バッファの設定
		memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
		buffer.AudioBytes = m_aSizeAudio[nCnt];
		buffer.pAudioData = m_apDataAudio[nCnt];
		buffer.Flags      = XAUDIO2_END_OF_STREAM;
		buffer.LoopCount  = m_aParam[nCnt].nCntLoop;

		m_apSourceVoice[nCnt]->SubmitSourceBuffer(&buffer);

		// ファイルをクローズ
		CloseHandle(hFile);
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CSound::UninitSound(void)
{
	// 一時停止
	for(int nCnt = 0; nCnt < SOUND_LABEL_MAX; nCnt++)
	{
		if(m_apSourceVoice[nCnt])
		{
			// 一時停止
			m_apSourceVoice[nCnt]->Stop(0);
	
			// ソースボイスの破棄
			m_apSourceVoice[nCnt]->DestroyVoice();
	
			// オーディオデータの開放
			free(m_apDataAudio[nCnt]);
		}
	}
	
	// マスターボイスの破棄
	m_pMasteringVoice->DestroyVoice();
	
	if(m_pXAudio2)
	{
		// XAudio2オブジェクトの開放
		m_pXAudio2->Release();
	}
	
	// COMライブラリの終了処理
	CoUninitialize();
}

//=============================================================================
// セグメント再生(停止)
//=============================================================================
HRESULT CSound::PlaySound(SOUND_LABEL label)
{
	XAUDIO2_VOICE_STATE xa2state;
	XAUDIO2_BUFFER       buffer;

	// バッファの設定
	memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
	buffer.AudioBytes = m_aSizeAudio[label];
	buffer.pAudioData = m_apDataAudio[label];
	buffer.Flags      = XAUDIO2_END_OF_STREAM;
	buffer.LoopCount  = m_aParam[label].nCntLoop;

	// 状態取得
	m_apSourceVoice[label]->GetState(&xa2state);

	if(xa2state.BuffersQueued != 0)
	{	// 再生中
		// 一時停止
		m_apSourceVoice[label]->Stop(0);

		// クリア
		m_apSourceVoice[label]->FlushSourceBuffers();
	}

	// 登録
	m_apSourceVoice[label]->SubmitSourceBuffer(&buffer);

	// 再生
	m_apSourceVoice[label]->Start( 0 );

	//m_apSourceVoice[label]->SetVolume

	return S_OK;
}

//=============================================================================
// セグメント停止(指定した音のみ)
//=============================================================================
void CSound::StopSound(SOUND_LABEL label)
{
	XAUDIO2_VOICE_STATE xa2state;

	// 状態取得
	m_apSourceVoice[label]->GetState(&xa2state);
	if(xa2state.BuffersQueued != 0)
	{// 再生中
		// 一時停止
		m_apSourceVoice[label]->Stop(0);

		// クリア
		m_apSourceVoice[label]->FlushSourceBuffers();
	}
}

//=============================================================================
// セグメント停止(全て)
//=============================================================================
void CSound::StopSound(void)
{
	for(int nCnt = 0; nCnt < SOUND_LABEL_MAX; nCnt++)
	{
		if(m_apSourceVoice[nCnt])
		{
			// 一時停止
			m_apSourceVoice[nCnt]->Stop(0);

			// クリア
			m_apSourceVoice[nCnt]->FlushSourceBuffers();
		}
	}
}

//=============================================================================
// チャンクのチェック
//=============================================================================
HRESULT CSound::CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition)
{
	HRESULT hr = S_OK;
	DWORD dwRead;
	DWORD dwChunkType;
	DWORD dwChunkDataSize;
	DWORD dwRIFFDataSize = 0;
	DWORD dwFileType;
	DWORD dwBytesRead = 0;
	DWORD dwOffset = 0;
	
	if(SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{// ファイルポインタを先頭に移動
		return HRESULT_FROM_WIN32(GetLastError());
	}
	
	while(hr == S_OK)
	{
		if(ReadFile(hFile, &dwChunkType, sizeof(DWORD), &dwRead, NULL) == 0)
		{// チャンクの読み込み
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		if(ReadFile(hFile, &dwChunkDataSize, sizeof(DWORD), &dwRead, NULL) == 0)
		{// チャンクデータの読み込み
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		switch(dwChunkType)
		{
		case 'FFIR':
			dwRIFFDataSize  = dwChunkDataSize;
			dwChunkDataSize = 4;
			if(ReadFile(hFile, &dwFileType, sizeof(DWORD), &dwRead, NULL) == 0)
			{// ファイルタイプの読み込み
				hr = HRESULT_FROM_WIN32(GetLastError());
			}
			break;

		default:
			if(SetFilePointer(hFile, dwChunkDataSize, NULL, FILE_CURRENT) == INVALID_SET_FILE_POINTER)
			{// ファイルポインタをチャンクデータ分移動
				return HRESULT_FROM_WIN32(GetLastError());
			}
		}

		dwOffset += sizeof(DWORD) * 2;
		if(dwChunkType == format)
		{
			*pChunkSize         = dwChunkDataSize;
			*pChunkDataPosition = dwOffset;

			return S_OK;
		}

		dwOffset += dwChunkDataSize;
		if(dwBytesRead >= dwRIFFDataSize)
		{
			return S_FALSE;
		}
	}
	
	return S_OK;
}

//=============================================================================
// チャンクデータの読み込み
//=============================================================================
HRESULT CSound::ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset)
{
	DWORD dwRead;
	
	if(SetFilePointer(hFile, dwBufferoffset, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{// ファイルポインタを指定位置まで移動
		return HRESULT_FROM_WIN32(GetLastError());
	}

	if(ReadFile(hFile, pBuffer, dwBuffersize, &dwRead, NULL) == 0)
	{// データの読み込み
		return HRESULT_FROM_WIN32(GetLastError());
	}
	
	return S_OK;
}
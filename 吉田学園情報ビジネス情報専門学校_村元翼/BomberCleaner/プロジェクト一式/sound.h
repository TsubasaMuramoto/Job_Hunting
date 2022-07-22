//===========================================================
// サウンド処理 [sound.h]
// Author : 村元翼
//===========================================================
#ifndef _SOUND_H_
#define _SOUND_H_
#include "main.h"

//===========================================================
// サウンドクラス
//===========================================================
class CSound
{
public:
	CSound();
	~CSound();

	//=================================
	// サウンドファイル
	//=================================
	typedef enum
	{
		//--------------------------
		// ＢＧＭ
		//--------------------------
		SOUND_LABEL_BGM_TITLE = 0,			// タイトルのBGM
		SOUND_LABEL_BGM_GAME,				// ゲームのBGM
		SOUND_LABEL_BGM_RESULT,				// リザルトのBGM

		//--------------------------
		// ＳＥ
		//--------------------------
		SOUND_LABEL_SE_DECIDE,				// 決定音
		SOUND_LABEL_SE_JUMP,				// ジャンプ音
		SOUND_LABEL_SE_START,				// スタート音
		SOUND_LABEL_SE_IGNITION,			// 発火音
		SOUND_LABEL_SE_EXPLOSION,			// 爆発音
		SOUND_LABEL_SE_DESTROY,				// 破壊音
		SOUND_LABEL_SE_CURSOR,				// カーソル音
		SOUND_LABEL_SE_CANCEL,				// キャンセル音
		SOUND_LABEL_SE_TIMEUP,				// タイムアップ音
		SOUND_LABEL_SE_COUNT,				// カウント音
		SOUND_LABEL_SE_GAMESTART,			// ゲーム開始音
		SOUND_LABEL_SE_CARRYUP,				// 持ち上げる音
		SOUND_LABEL_SE_THROW,				// 投げる音
		SOUND_LABEL_SE_FALL,				// 落ちる音
		SOUND_LABEL_MAX

	} SOUND_LABEL;

	//=============================================================================
	// メソッド
	//=============================================================================
	HRESULT InitSound(void);																				// 初期化			
	void	UninitSound(void);																				// 終了
	HRESULT PlaySound(SOUND_LABEL label);																	// 再生
	void	StopSound(SOUND_LABEL label);																	// 停止
	void	StopSound(void);																				// 停止
	void	ControllVoice(SOUND_LABEL label, float fVolume) { m_apSourceVoice[label]->SetVolume(fVolume); }	// 音量調整

private:
	//=============================================================================
	// パラメータ構造体定義
	//=============================================================================
	typedef struct
	{
		char *filename;		// ファイル名
		int nCntLoop;		// ループカウント
	} PARAM;

	// 各音素材のパラメータ
	const static PARAM m_aParam[SOUND_LABEL_MAX];

	//=============================================================================
	// メソッド
	//=============================================================================
	HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
	HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

	//=============================================================================
	// メンバ変数
	//=============================================================================
	IXAudio2 *m_pXAudio2 = NULL;								// XAudio2オブジェクトへのインターフェイス
	IXAudio2MasteringVoice *m_pMasteringVoice = NULL;			// マスターボイス
	IXAudio2SourceVoice *m_apSourceVoice[SOUND_LABEL_MAX] = {};	// ソースボイス
	BYTE *m_apDataAudio[SOUND_LABEL_MAX] = {};					// オーディオデータ
	DWORD m_aSizeAudio[SOUND_LABEL_MAX] = {};					// オーディオデータサイズ

};
#endif

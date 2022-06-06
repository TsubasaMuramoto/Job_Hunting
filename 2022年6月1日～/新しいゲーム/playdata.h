//===========================================
// プレイデータ管理処理
//===========================================
#ifndef _PLAY_DATA_H_
#define _PLAY_DATA_H_
#include "main.h"

//================================================
// マクロ定義
//================================================

//================================================
// クラスの定義
//================================================
class CPlayData
{
public:
	CPlayData();			//コンストラクタ
	~CPlayData();			//デストラクタ

	HRESULT Init(void);		// 初期化
	void Uninit(void);		// 終了

	static void SetScore(int nScore);		// スコアの設定
	static int GetScore(void);				// スコアの取得
	static void SetWinNum(int nWinNum);		// 勝った個体識別番号の設定
	static int GetWinNum(void);				// 勝った個体識別番号の取得

private:
	static int m_nScore;					// スコア
	static int m_nWinNum;					// 勝った個体識別番号
};
#endif
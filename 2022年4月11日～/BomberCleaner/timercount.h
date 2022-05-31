//=============================================================================
//
// 時間経過のクラス [timercount.h]
// Author : 村元翼
//
//=============================================================================
#ifndef _TIMERCOUNT_H_
#define _TIMERCOUNT_H_

//*****************************************************************************
//インクルードファイル
//*****************************************************************************
#include"scene.h"
#include "manager.h"

//*****************************************************************************
//マクロ定義
//*****************************************************************************
#define MAX_TIME (3)			// 数字のケタ数
#define SPACE_TIME (56)			// 数字の間隔
#define REDUCE_TIME (1000)		// 何秒でタイム減らすか
#define TIMEUP_WIDTH (500.0f)	// タイムアップ表示の幅
#define TIMEUP_HEIGHT (100.0f)	// タイムアップ表示の高さ
#define INTERVAL_TIME (100)		// タイムアップ表示が出てから画面遷移までの時間
#define TIME_COUNTER (90)

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CNumber;

//*****************************************************************************
//クラス定義
//*****************************************************************************
class CTimerCount : public CScene
{
public:
	CTimerCount(OBJTYPE nPriority = OBJTYPE_NUMBER);	// コンストラクタ
	~CTimerCount();										// デストラクタ

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);	// 初期化処理
	void Uninit(void);									// 終了処理
	void Update(void);									// 更新処理
	void Draw(void);									// 描画処理

	void AddTimerCount(int nValue);						// 時間の加算
	int GetTimerCount(void);							// 時間の取得
	bool GetLimit() { return m_bLimit; }				// 終了時間の取得

	// 時間の生成
	static CTimerCount *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, int time, bool bStop);

private:
	CNumber		*m_apNumber[MAX_TIME];// 数字のインスタンス
	int			m_nTimerCount;		  // 時間
	int			m_IntervalTime;		  // タイムアップ後の間
	D3DXVECTOR3 m_size;				  // 大きさ
	D3DXVECTOR3 m_pos;				  // 位置
	D3DXCOLOR	m_col;				  // 色
	bool		m_bFade;			  // フェード判定
	bool		m_bStop;			  // リザルトのときは時間停止
	bool		m_bLimit;			  // 時間制限用
	DWORD		m_dwCurrentTime;	  // 経過時間
	DWORD		m_dwExecLastTime;	  // 現在の時間
	CManager::MODE			m_mode;

};

#endif // !_TIME_H_


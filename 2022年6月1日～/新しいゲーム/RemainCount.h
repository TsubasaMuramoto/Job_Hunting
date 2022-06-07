//=============================================================================
//
// 残機カウントのクラス [RemainCount.h]
// Author : 村元翼
//
//=============================================================================
#ifndef _REMAINCOUNT_H_
#define _REMAINCOUNT_H_

//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#include "scene.h"
#include "manager.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_REMAIN_DIGIT	(2)	// 残機の桁
#define PLAYER_REMAIN		(2)	// プレイヤー残機
#define REMAIN_SIZE			(D3DXVECTOR3(150.0f,100.0f,0.0f))

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CNumber;
class CUi;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CRemainCount : public CScene
{
public:
	//------------------------------------------
	// 構造体
	//------------------------------------------
	enum class REMAIN
	{
		MODE_RETRY = 0,
		MODE_GAMEOVER,
		MODE_NOBODY,
		MODE_MAX
	};

	CRemainCount(OBJTYPE nPriority = OBJTYPE_NUMBER);	// コンストラクタ
	~CRemainCount();									// デストラクタ

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);	// 初期化処理
	void Uninit(void);									// 終了処理
	void Update(void);									// 更新処理
	void Draw(void);									// 描画処理

	void RemainChange(void);
	void SumAndDiffRemain(int nValue)	{ m_nRemain += nValue; }		// 残機の加算減算
	static void SetRemainChange()		{ m_bChange = true; }			// リトライの設定
	static int GetRemain()				{ return m_nRemain; }			// 残機の取得

	// 生成
	static CRemainCount *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size);

private:
	CNumber				*m_apNumber[MAX_REMAIN_DIGIT];	// 数字のインスタンス
	static int			m_nRemain;						// 残機
	static bool			m_bChange;						// 残機変化判定
	int					m_nFrame;						// フレーム
	D3DXVECTOR3			m_size;							// 大きさ
	D3DXVECTOR3			m_pos;							// 位置
	D3DXCOLOR			m_col;							// 色
	bool				m_bFade;						// フェード判定
	REMAIN				m_RemainMode;					// 残機別モード
};

#endif


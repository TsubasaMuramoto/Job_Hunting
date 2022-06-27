//--------------------------------------
// リザルトの処理
//--------------------------------------
#ifndef _RESULT_H_
#define _RESULT_H_
#include "main.h"

//--------------------------------------
// マクロ定義
//--------------------------------------			
#define RESULT_RANKING_POSY			(80.0f)				// ランキングのposY

//--------------------------------------
// 前方宣言
//--------------------------------------
class CBg;
class CUi;
class CRanking;

//--------------------------------------
// リザルトクラスの生成
//--------------------------------------
class CResult
{
public:
	CResult();				// コンストラクタ
	~CResult();				// デストラクタ
	HRESULT Init(void);		// 初期化
	void Uninit(void);		// 終了
	void Update(void);		// 更新
	void Draw(void);		// 描画

private:
	CRanking	*m_pRanking;		// ランキングクラス
	CUi			*m_pTitleLogo;		// タイトルロゴUI
	CUi			*m_pReturnTitle;	// タイトルに戻るUI
	CBg			*m_pBgResult;		// リザルト背景
	int			m_nSelectType;		// 現在の選択
	int			m_nFrame;			// フレーム数

};
#endif
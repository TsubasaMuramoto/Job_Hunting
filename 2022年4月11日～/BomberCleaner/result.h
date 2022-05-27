//--------------------------------------
// リザルトの処理
//--------------------------------------
#ifndef _RESULT_H_
#define _RESULT_H_
#include "main.h"
#include "scene.h"

//--------------------------------------
// マクロ定義
//--------------------------------------
#define MAX_RESULT (4)
#define RESULT_RANKING_POSY				(80.0f)				//ランキングのposY

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

	//--------------------------------------
	// 選択肢構造体
	//--------------------------------------
	typedef enum
	{
		GAME_RETRY = 0,
		TITLE,
		MAX,
	}TYPE;

	//--------------------------------------
	// メソッド
	//--------------------------------------
	CResult();
	~CResult();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	CRanking			*m_pRanking;
	CUi					*m_pUi;							
	CUi					*m_pReturnTitle;				// タイトルに戻るUI
	CBg					*m_pBgResult;
	int					m_nSelectType;					// 現在の選択
	int					m_nFrame;						// フレーム数

};


#endif //_RESULT_H_
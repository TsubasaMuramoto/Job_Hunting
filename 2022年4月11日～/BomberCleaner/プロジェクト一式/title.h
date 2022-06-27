//===============================================
// タイトルの処理
// Author:村元翼
//===============================================
#ifndef _TITLE_H_
#define _TITLE_H_

//===============================================
// インクルードファイル
//===============================================
#include "main.h"
#include "scene.h"
#include "Ui.h"

//===============================================
// マクロ定義
//===============================================
#define MAX_SCROLL_UI		(2)		// スクロールUIの最大数
#define MAX_TUTO			(3)		// チュートリアルUIの最大数
#define KEY_EXP_FONTSIZE	(36)	// キー説明のフォントサイズ

//===============================================
// 前方宣言
//===============================================
class CTexture;
class CStringManager;

//===============================================
// タイトルクラスの定義
//===============================================
class CTitle
{
public:
	//------------------------------------
	// タイトル画面のページ
	//------------------------------------
	typedef enum
	{
		PAGE_01 = 0,
		PAGE_02,
		PAGE_03,
		PAGE_MAX

	}TITLE_PAGE;

	//------------------------------------
	// チュートリアルのページ
	//------------------------------------
	typedef enum
	{
		TUTORIAL_PAGE_01 = 0,
		TUTORIAL_PAGE_02,
		TUTORIAL_PAGE_03,
		TUTORIAL_PAGE_MAX

	}TUTORIAL_PAGE;

	//------------------------------------
	// 操作説明のページ
	//------------------------------------
	typedef enum
	{
		OPR_INSTR_01 = 0,
		OPR_INSTR_02,
		OPR_INSTR_03,
		OPR_INSTR_04,
		OPR_INSTR_05,
		OPR_INSTR_06,
		OPR_INSTR_07,
		OPR_INSTR_08,
		OPR_INSTR_09,
		OPR_INSTR_10,
		OPR_INSTR_11,
		OPR_INSTR_12,
		OPR_INSTR_MAX

	}OPR_INSTR;

	//------------------------------------
	// ゲームモード選択の識別番号
	//------------------------------------
	typedef enum
	{
		TITLE_SELECT_01 = 0,
		TITLE_SELECT_02,
		TITLE_SELECT_03,
		TITLE_SELECT_MAX

	}TITLE_SELECT;

	CTitle();						// コンストラクタ
	~CTitle();						// デストラクタ
	HRESULT Init(void);				// 初期化
	void Uninit(void);				// 終了
	void Update(void);				// 更新
	void Draw(void);				// 描画

	void TutorialDisplay(CUi **pTutoUI,CUi **pScr,const CTexture *pTex);									// チュートリアル表示処理関数
	void CursorMove(int &nNowSel, const int &nFirst, const int &nLast, const bool &bMinus);					// カーソル移動関数
	void KeyExpCreate(CStringManager **ppKeyExp, const char *cStr,const int& nFontSize = KEY_EXP_FONTSIZE);	// キー説明文字列生成関数

private:
	//--------------------------------------
	// ゲーム選択UIパラメータ
	//--------------------------------------
	typedef struct
	{
		D3DXVECTOR3		pos, size;	// 位置・サイズ
		string			Tex;		// テクスチャ名
		CUi::UI_TYPE	UiType;		// UIのタイプ

	}SELECT_PARAM;
	
	const static SELECT_PARAM	m_SelectParam[TITLE_SELECT_MAX];// ゲーム選択UIパラメータ
	CStringManager				*m_pKeyExp;						// キー説明
	CUi							*m_pScrollUI[MAX_SCROLL_UI];	// ロールUI	
	CUi							*m_pPressEnter;					// PressEnterUIクラス
	CUi							*m_pTitleLogo;					// タイトルロゴUIクラス
	CUi							*m_pTutorial;					// チュートリアル画面UIクラス
	CUi							*m_pSelectUI[TITLE_SELECT_MAX];	// 選択肢UIクラス
	TITLE_PAGE					m_Page;							// タイトルページ構造体
	TUTORIAL_PAGE				m_TutoPage;						// チュートリアルページ構造体
	int							m_nSelect;						// モード選択に使う
	int							m_nSelectKey;					// キー説明の選択に使う
	bool						m_bOPRLock;						// 操作ロック判定
};
#endif
//--------------------------------------
// PAUSEの処理
// Author:村元翼
//--------------------------------------
#ifndef _PAUSE_H
#define _PAUSE_H
#include "main.h"
#include "scene.h"
#include "texture.h"

//-------------------------------------------
// 前方宣言
//-------------------------------------------
class CScene2D;
class CBg;

//---------------------------------------------
// ポーズのクラス
//---------------------------------------------
class CPause : public CScene
{
public:
	//-----------------------------------------
	// ポーズフェードの状態構造体
	//-----------------------------------------
	typedef enum
	{
		FADE_IN = 0,	// フェードイン
		FADE_OUT,		// フェードアウト
	}PAUSE_FADE;

	//-----------------------------------------
	// ポーズの選択状態列挙
	//-----------------------------------------
	typedef enum
	{
		SELECT_RETURNGAME = 0,		// ゲームに戻る
		SELECT_RETRY,				// リトライ
		SELECT_RETURNTITLE,			// タイトルに戻る
		SELECT_MAX
	}PAUSE_SELECT;

	CPause(OBJTYPE nPriority = OBJTYPE_PAUSE);			// コンストラクタ
	~CPause();											// デストラクタ

	HRESULT Init(D3DXVECTOR3 pos,D3DXVECTOR3 size);		// 初期化
	void Uninit(void);									// 終了
	void Update(void);									// 更新
	void Draw(void);									// 描画
	static CPause *Create(D3DXVECTOR3 pos);				// インスタンス生成処理

private:
	D3DXVECTOR3		m_pos;							// 位置
	D3DXVECTOR3		m_Speed;						// 速度
	float			m_fAlpha;						// ポーズ背景の透明度
	int				m_nPauseType;					// ポーズ選択
	bool			m_bUninit;						// pauseを終了させるか
	bool			m_bNextMode;					// 次のモードにいくか
	bool			m_bMoveCursor;					// カーソルが動いているか
	CScene2D		*m_pPolygon[SELECT_MAX];		// ポーズ選択のポリゴン
	CScene2D		*m_pCursor;						// 選択カーソルのポリゴン
	CScene2D		*m_pTutorialUI;					// 操作説明UI
	static CBg		*m_pBg;							// ポーズ背景
};
#endif
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
// マクロ定義
//-------------------------------------------

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
		FADE_IN = 0,
		FADE_OUT,
	}PAUSE_FADE;

	//-----------------------------------------
	// ポーズの選択状態列挙
	//-----------------------------------------
	typedef enum
	{
		SELECT_RETURNGAME = 0,
		SELECT_RETRY,
		SELECT_RETURNTITLE,
		SELECT_MAX
	}PAUSE_SELECT;

	CPause(OBJTYPE nPriority = OBJTYPE_PAUSE);
	~CPause();
	HRESULT Init(D3DXVECTOR3 pos,D3DXVECTOR3 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CPause *Create(D3DXVECTOR3 pos);			// インスタンス生成処理

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


#endif //_PAUSE_H
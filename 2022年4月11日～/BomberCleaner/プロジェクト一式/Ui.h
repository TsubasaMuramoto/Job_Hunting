//=========================================
// UI作成
// Author:村元翼
//=========================================
#ifndef _UI_H_
#define _UI_H_
#include "scene.h"
#include "scene2D.h"
#include "control.h"

//=========================================
// クラス生成
//=========================================
class CUi : public CScene2D
{
public:
	//------------------------------
	// UIタイプの構造体
	//------------------------------
	typedef enum
	{
		UI_TYPE_NONE = 0,	// 無し
		UI_TYPE_BLINK,		// 点滅
		UI_TYPE_START,		// スタート
		UI_TYPE_ZOOM,		// ズーム
		UI_TYPE_MAX

	} UI_TYPE;

	CUi(OBJTYPE nPriority = OBJTYPE_UI);				// コンストラクタ
	~CUi();												// デストラクタ

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);	// 初期化
	void Uninit(void);									// 終了
	void Update(void);									// 更新
	void Draw(void);									// 描画

	// UIの生成
	static CUi *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, UI_TYPE type = UI_TYPE_NONE);

	// 取得関数
	UI_TYPE GetUiType(void) { return UI_TYPE(); }

private:
	CControl					*m_pControl;	// コントロールクラス
	UI_TYPE						m_UiType;		// UIタイプ
	D3DXVECTOR3					m_pos;			// ポリゴンの位置
	D3DXVECTOR3					m_size;			// サイズ
	D3DXCOLOR					m_col;			// カラー
	bool						m_bRelease;		// 終了判定

};
#endif
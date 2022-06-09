//=============================================================================
//
// テキスト表示のクラス [TextureFont.h]
// Author : 村元翼
//
//=============================================================================
#ifndef _TEXTUREFONT_H_
#define _TEXTUREFONT_H_

//*****************************************************************************
//インクルードファイル
//*****************************************************************************
#include "scene.h"
#include "scene2D.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define FONT_POS (D3DXVECTOR3(50.0f,600.0f,0.0f))

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CFont : public CScene2D
{
public:
	CFont(OBJTYPE nPriority = OBJTYPE_NUMBER);					// コンストラクタ
	~CFont();													// デストラクタ

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);			// 初期化処理
	void Uninit(void);											// 終了処理
	void Update(void);											// 更新処理
	void Draw(void);											// 描画処理

	static CFont *Create(D3DXVECTOR3 pos, int nfontSize,const char* str, const char* font = "ＭＳ 明朝");	// 生成関数

private:
	int m_nFontSize;
	const char* m_str;
	const char* m_font;
	D3DXVECTOR3 m_pos;
	
};
#endif

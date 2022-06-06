//=============================================================================
//
// ビルボードクラス [billboard.h]
// Author : 村元翼
//
//=============================================================================
#ifndef _BILLBOARD_H_
#define _BILLBOARD_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "main.h"
#include "scene.h"
#include "manager.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define FVF_VERTEX_2D	(D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1)	// 頂点フォーマット
#define NUM_VERTEX		(4)											// 頂点数
#define SCREEN_CENTER_X (SCREEN_WIDTH / 2)							// スクリーンX座標の中心
#define SCREEN_CENTER_Y (SCREEN_HEIGHT / 2)							// スクリーンY座標の中心

//*****************************************************************************
// ビルボードクラス
//*****************************************************************************
class CBillboard : public CScene
{
public:

	CBillboard(OBJTYPE nPriority = OBJTYPE_BILLBOARD);			// コンストラクタ
	~CBillboard();										// デストラクタ

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);	// 初期化処理
	void Uninit(void);									// 終了処理
	void Update(void);									// 更新処理
	void Draw(void);									// 描画処理

	// ビルボードの生成
	static CBillboard *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXCOLOR col);

	//---------------------------------------
	// Setter & Getter
	//---------------------------------------
	void SetTex(int nAnim, int nPartU);
	void SetPos(D3DXVECTOR3 pos, D3DXVECTOR3 scale);
	void SetColVal(D3DCOLORVALUE colval);
	D3DXVECTOR3 GetPos(void)						{ return m_pos; }
	void BindTexture(LPDIRECT3DTEXTURE9 pTexture)	{ m_pTexture = pTexture; }

private:
	LPDIRECT3DTEXTURE9		m_pTexture = nullptr;	// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff = nullptr;	// 頂点バッファへのポインタ
	D3DXVECTOR3				m_pos;					// 位置
	D3DXVECTOR3				m_size;					// サイズ
	D3DXMATRIX				m_mtxWorld;				// ワールドマトリックス
	D3DCOLORVALUE			m_colval;				// カラー
};
#endif //!_SCENE2D_H_


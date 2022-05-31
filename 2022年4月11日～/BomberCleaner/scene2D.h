//*****************************************************************************
// ポリゴンの処理
// Author:村元翼
//*****************************************************************************
#ifndef _SCENE2D_H
#define _SCENE2D_H

//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#include "main.h"
#include "scene.h"
#include "texture.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define FVF_VERTEX_2D (D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1)

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;	// 頂点座標
	float		rhw;	// 1.0fで固定
	D3DCOLOR	col;	// 頂点カラー
	D3DXVECTOR2 tex;	// テクスチャ座標
} VERTEX_2D;

//*****************************************************************************
// クラスの定義
//*****************************************************************************
class CScene2D : public CScene
{
public:
	CScene2D(OBJTYPE nPriority = OBJTYPE_POLYGON2D);		// コンストラクタ
	~CScene2D();											// デストラクタ

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);// 初期化
	void Uninit(void);								// 終了
	void Update(void);								// 更新
	void Draw(void);								// 描画
	
	// 生成
	static CScene2D *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size,OBJTYPE objtype = OBJTYPE_POLYGON2D);

	//------------------------------------------------------------------------------
	// 設定・取得関数
	//------------------------------------------------------------------------------
	// 位置設定
	void SetScalePos(D3DXVECTOR3 pos, D3DXVECTOR3 scale);
	void SetPos(D3DXVECTOR3 vtx0, D3DXVECTOR3 vtx1, D3DXVECTOR3 vtx2, D3DXVECTOR3 vtx3);	
	void SetPos(D3DXVECTOR3 pos, D3DXVECTOR3 scale,bool bScroll = true);
	void SetPos(D3DXVECTOR3 pos, D3DXVECTOR3 scale, D3DXVECTOR3 Rotvtx0, D3DXVECTOR3 Rotvtx1, D3DXVECTOR3 Rotvtx2, D3DXVECTOR3 Rotvtx3);
	// テクスチャ設定
	void SetTex(int nAnim,int nPartU);
	void SetTex(float fSpeedX, float fSpeedY);
	void SetTex(float nPatternX, float nPatternY, float fSizeX,float fSizeY);
	// カラー設定
	void SetCol(D3DXCOLOR col);
	// テクスチャ取得
	void BindTexture(LPDIRECT3DTEXTURE9 pTexture) { m_pTexture = pTexture; }

	static D3DXVECTOR3 Move(D3DXVECTOR3 *pPos, float angle, float speed);		// 移動

private:

protected:
	LPDIRECT3DTEXTURE9		m_pTexture;				// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;				// 頂点バッファへのポインタ
};
#endif

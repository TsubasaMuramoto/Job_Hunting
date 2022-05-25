//--------------------------------------
// 背景
// Author:村元翼
//--------------------------------------
#ifndef _BG_H
#define _BG_H
#include "scene2D.h"
#include "texture.h"

//-------------------------------------
// 前方宣言
//-------------------------------------
class CScene;

//-------------------------------------
// クラス生成
//-------------------------------------
class CBg : public CScene2D
{
public:
	CBg(OBJTYPE nPriority = OBJTYPE_BG);
	~CBg();
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);	// 初期化
	void Uninit(void);									// 終了
	void Update(void);									// 更新
	void Draw(void);									// 描画

	// インスタンス生成処理
	static CBg *Create(	D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f), 
						D3DXVECTOR3 size = D3DXVECTOR3(0.0f, 0.0f, 0.0f),
						bool bScrol = true,
						CScene::OBJTYPE objtype = CScene::OBJTYPE_BG ); 


	//-------------------------------------
	// Getter & Setter
	//-------------------------------------
	void BindTexture(LPDIRECT3DTEXTURE9 pTex)	{ m_pTexture = pTex; }
	D3DXVECTOR3 GetTex(void)					{ return m_Tex; }
	D3DXVECTOR3 GetPos(void)					{ return m_pos; }

private:
	LPDIRECT3DTEXTURE9			m_pTexture;		// テクスチャへのポインタ
	D3DXVECTOR3					m_Tex;			// テクスチャをスクロールする変数
	D3DXVECTOR3					m_pos;			// 位置
	bool						m_bScroll;		// スクロール切り替え
};


#endif // !_BG_H


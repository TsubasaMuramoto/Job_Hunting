//==================================
// メッシュクラスの作成
// Author: 村元翼
//==================================
#ifndef _MESH_H_
#define _MESH_H_
#include "main.h"

//==============================================
// マクロ定義
//==============================================

//==============================================
// 前方宣言
//==============================================
class CScene;

//==============================================
// クラス定義
//==============================================
class CMesh
{
public:
	//--------------------------------------------
	// コンストラクタ・デストラクタ
	//--------------------------------------------
	CMesh();
	~CMesh();

	//--------------------------------------------
	// 設定・取得関数
	//--------------------------------------------
	void SetPos(CScene *pScene,D3DXVECTOR3 pos, D3DXVECTOR3 size);					// 頂点座標の設定
	//void SetTex(int nAnim, int nPartU);											// テクスチャの設定
	void SetTex(float fSpeedX, float fSpeedY);										// テクスチャの設定
	void SetCol(CScene *pScene, D3DXCOLOR col);										// カラーの設定
	void AddRot(D3DXVECTOR3 rot); 														// 回転の加算

	void BindTexture(LPDIRECT3DTEXTURE9 pTexture)	{ m_pTexture = pTexture; }		// テクスチャ設定処理
	D3DXMATRIX	GetMatrix()							{ return m_mtxWorld; }			// ワールドマトリックスの取得
	D3DXVECTOR3 GetLocalVtx(int nID)				{ return m_vtxLocal[nID]; }		// ローカル頂点座標の取得

protected:
	vector<D3DXVECTOR3>			m_vtxLocal;								// ローカル頂点座標
	vector<D3DXVECTOR3>			m_vtxWorld;								// ワールド頂点座標
	vector<D3DXMATRIX>			m_mtxVec;								// ワールド頂点マトリックス
	D3DXVECTOR3					m_posMS;								// 位置
	D3DXVECTOR3					m_rotMS;								// 回転
	D3DXVECTOR3					m_sizeMS;								// サイズ
	int							m_nLine;								// 横のポリゴン数
	int							m_nVertical;							// 縦のポリゴン数

	LPDIRECT3DTEXTURE9		m_pTexture;									// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;									// 頂点バッファへのポインタ
	LPDIRECT3DINDEXBUFFER9	m_pVtxIndexBuff;							// 頂点インデックスバッファへのポインタ
	D3DXMATRIX				m_mtxWorld;									// ワールドマトリックス
};

#endif


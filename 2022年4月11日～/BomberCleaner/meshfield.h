//==================================
// メッシュフィールドの作成
// Author: 村元翼
//==================================
#ifndef _MESHFIELD_H
#define _MESHFIELD_H
#include "main.h"
#include "scene.h"
#include "Scene3D.h"
#include "texture.h"

//==============================================
// マクロ定義
//==============================================
#define MESH_VTX			(4)

//*****************************************************************************
// 構造体定義
//*****************************************************************************

//------------------------------------------------------------------------------
//クラスの定義
//------------------------------------------------------------------------------
class CMeshField : public CScene
{
public:
	//--------------------------------------------
	// コンストラクタ・デストラクタ
	//--------------------------------------------
	CMeshField(OBJTYPE nPriority = OBJTYPE_MESH);
	~CMeshField();

	//--------------------------------------------
	// メソッド
	//--------------------------------------------
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//--------------------------------------------
	// セッター・ゲッター
	//--------------------------------------------
	void BindTexture(LPDIRECT3DTEXTURE9 pTexture)	{ m_pTexture = pTexture; }			// テクスチャ設定処理
	void SetPos(D3DXVECTOR3 pos, D3DXVECTOR3 scale);									// 頂点座標の設定
	void SetTex(int nAnim, int nPartU);													// テクスチャの設定
	void SetTex(float fSpeedX, float fSpeedY);											// テクスチャの設定
	void SetCol(D3DXCOLOR col);															// カラーの設定
	D3DXMATRIX	GetMatrix()							{ return m_mtxWorld; }				// ワールドマトリックスの取得
	D3DXVECTOR3 GetLocalVtx(int nID)				{ return m_vtxLocal[nID]; }			// ローカル頂点座標の取得

	static CMeshField *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 rot, int nLine, int nVertical);	// シーン3Dの生成
	void AllCollision(void);
	bool LineCollisionMesh(CScene *pScene, const int *pnVtx);													// 線とメッシュの当たり判定
	bool ShadowCollisionMesh(CScene *pScene, const int *pnVtx);																	// 影との当たり判定
	void ProcessByObjtype(CScene *pScene,D3DXVECTOR3 &pos);
	// 設定・取得処理


private:
	vector<D3DXVECTOR3>			m_vtxLocal;								// ローカル頂点座標
	vector<D3DXVECTOR3>			m_vtxWorld;								// ワールド頂点座標
	vector<D3DXMATRIX>			m_mtxVec;								// ワールド頂点マトリックス
	D3DXVECTOR3					m_pos;									// 位置
	D3DXVECTOR3					m_rot;									// 回転
	D3DXVECTOR3					m_size;									// サイズ
	int							m_nLine;								// 横のポリゴン数
	int							m_nVertical;							// 縦のポリゴン数

protected:

	LPDIRECT3DTEXTURE9		m_pTexture = NULL;				// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff = NULL;				// 頂点バッファへのポインタ
	LPDIRECT3DINDEXBUFFER9	m_pVtxIndexBuff = nullptr;		// 頂点インデックスバッファへのポインタ
	D3DXMATRIX				m_mtxWorld;
};


#endif // !_MESHFIELD_H


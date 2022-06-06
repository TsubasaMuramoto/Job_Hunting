//==================================
// シーン3Dの作成
// Author:
//==================================
#ifndef _SCENE3D_H
#define _SCENE3D_H
#include "main.h"
#include "scene.h"
#include "texture.h"

//==============================================
// マクロ定義
//==============================================
#define FVF_VERTEX_3D (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_DIFFUSE|D3DFVF_TEX1)
#define ALLOWABLE_ERROR		(5.0f)		// 許容誤差(床が斜面でも抜けないようにする)
#define PERCENT_EXTRA		(3.0f)		// 割り増し誤差
#define FIELD_VTX			(4)
#define MESH_VTX			(4)

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;//頂点座標
	D3DXVECTOR3 nor;//法線ベクトル
	D3DCOLOR	col;//頂点カラー
	D3DXVECTOR2 tex;//テクスチャ座標

} VERTEX_3D;

//------------------------------------------------------------------------------
// クラスの定義
//------------------------------------------------------------------------------
class CScene3D : public CScene
{
public:

	//--------------------------------------------
	// コンストラクタ・デストラクタ
	//--------------------------------------------
	CScene3D(OBJTYPE nPriority = OBJTYPE_POLYGON3D);
	~CScene3D();

	//--------------------------------------------
	// メソッド
	//--------------------------------------------
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//--------------------------------------------
	// 設定・取得関数
	//--------------------------------------------
	void BindTexture(LPDIRECT3DTEXTURE9 pTexture) { m_pTexture = pTexture; }			// テクスチャ設定処理
	void SetPos(D3DXVECTOR3 pos, D3DXVECTOR3 scale);									// 頂点座標の設定
	void SetTex(int nAnim, int nPartU);													// テクスチャの設定
	void SetTex(float fSpeedX, float fSpeedY);											// テクスチャの設定
	void SetCol(D3DXCOLOR col);															// カラーの設定
	D3DXMATRIX	GetMatrix()						{ return m_mtxWorld; }					// ワールドマトリックスの取得
	D3DXVECTOR3 GetLocalVtx(int nID)			{ return m_vtxLocal[nID]; }				// ローカル頂点座標の取得

	static CScene3D *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 rot);		// シーン3Dの生成

private:
	D3DXVECTOR3 m_vtxLocal[4];								// ローカル頂点座標保存

protected:

	LPDIRECT3DTEXTURE9		m_pTexture = NULL;				// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff = NULL;				// 頂点バッファへのポインタ
	D3DXMATRIX m_mtxWorld;
	D3DXVECTOR3 m_pos;										// 位置
	D3DXVECTOR3 m_rot;										// 回転
	D3DXVECTOR3 m_size;										// サイズ
};
#endif
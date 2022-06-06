//==================================
// メッシュスフィアの作成
// Author: 村元翼
//==================================
#ifndef _MESHSPHERE_H_
#define _MESHSPHERE_H_
#include "main.h"
#include "scene.h"
#include "Scene3D.h"
#include "texture.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define SPHERE_RADIUS	(100.0f)													// 球半径
#define SPHERE_SIZE		(D3DXVECTOR3(SPHERE_RADIUS,SPHERE_RADIUS,SPHERE_RADIUS))	// 球サイズ

//*****************************************************************************
// 構造体定義
//*****************************************************************************

//------------------------------------------------------------------------------
//クラスの定義
//------------------------------------------------------------------------------
class CMeshSphere : public CScene
{
public:
	//--------------------------------------------
	// コンストラクタ・デストラクタ
	//--------------------------------------------
	CMeshSphere(OBJTYPE nPriority = OBJTYPE_MESH);
	~CMeshSphere();

	//--------------------------------------------
	// メソッド
	//--------------------------------------------
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//--------------------------------------------
	// 設定・取得処理
	//--------------------------------------------
	void BindTexture(LPDIRECT3DTEXTURE9 pTexture) { m_pTexture = pTexture; }			// テクスチャ設定処理
	void SetPos(D3DXVECTOR3 pos, D3DXVECTOR3 scale);									// 頂点座標の設定
	void SetTex(int nAnim, int nPartU);													// テクスチャの設定
	void SetTex(float fSpeedX, float fSpeedY);											// テクスチャの設定
	void SetCol(D3DXCOLOR col);															// カラーの設定
	void SetRot(D3DXVECTOR3 rot)					{ m_rot = rot; }					// 回転の設定
	void SetScale(D3DXVECTOR3 scale)				{ m_scale = scale; }				// スケールの設定
	D3DXVECTOR3 GetScale()							{ return m_scale; }
	D3DXMATRIX	GetMatrix()							{ return m_mtxWorld; }				// ワールドマトリックスの取得
	D3DXVECTOR3 GetLocalVtx(int nID)				{ return m_vtxLocal[nID]; }			// ローカル頂点座標の取得

	bool SphereCollisionSphere(float fRadius, CScene *pScene);
	static CMeshSphere *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 rot, int nLine, int nVertical,bool bInsideDraw, CScene::OBJTYPE objtype);	// シーン3Dの生成
	bool LineCollisionMesh(CScene *pScene, const int *nVtx);

private:
	vector<D3DXVECTOR3> m_vtxLocal;								// ローカル頂点座標
	vector<D3DXVECTOR3> m_vtxWorld;								// ワールド頂点座標
	vector<D3DXMATRIX>  m_mtxVec;								// ワールド頂点マトリックス
	D3DXVECTOR3 m_pos;											// 位置
	D3DXVECTOR3 m_rot;											// 回転
	D3DXVECTOR3 m_size;											// サイズ
	D3DXVECTOR3 m_scale;										// スケール
	int m_nLine;												// 横のポリゴン数
	int m_nVertical;											// 縦のポリゴン数
	bool m_bInsideDraw;											// 内側に描画するか
	CScene::OBJTYPE objtype;									// オブジェクトタイプ

protected:

	LPDIRECT3DTEXTURE9		m_pTexture = NULL;				// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff = NULL;				// 頂点バッファへのポインタ
	LPDIRECT3DINDEXBUFFER9	m_pVtxIndexBuff = nullptr;		// 頂点インデックスバッファへのポインタ
	D3DXMATRIX				m_mtxWorld;
};


#endif // !_MESHSPHERE_H_



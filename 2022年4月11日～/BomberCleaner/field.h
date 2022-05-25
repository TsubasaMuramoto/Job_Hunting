//==================================
// フィールドの作成
// Author:村元翼
//==================================
#ifndef _FIELD_H_
#define _FIELD_H_
#include "main.h"
#include "scene3D.h"
#include "texture.h"

//==============================================================================
// マクロ定義
//==============================================================================
//#define _OX_EPSILON_		(0.000001f) // 誤差
//#define ALLOWABLE_ERROR		(5.0f)	// 許容誤差(床が斜面でも抜けないようにする)


//==============================================================================
// 前方宣言
//==============================================================================

//------------------------------------------------------------------------------
// クラスの定義
//------------------------------------------------------------------------------
class CField : public CScene3D
{
public:
	CField();
	~CField();
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CField *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 rot);
	bool DotCollisionField(CScene *pScene);
	bool LineCollisionField(CScene *pScene);
	D3DXMATRIX *GetVtxMtx(void)	{ return &m_vtxMtxWorld[0]; }		// 頂点ワールドマトリックスの取得
	void SetVtxMtxWorld(D3DXVECTOR3 vtxPos, int nCntVtx);		// 頂点ワールドマトリックスの設定

private:
	D3DXMATRIX m_vtxMtxWorld[FIELD_VTX];
	D3DXVECTOR3 m_vtx[FIELD_VTX];
	D3DXVECTOR3 m_vtxRot[FIELD_VTX];
	CScene3D	*m_pScene3D;
};


#endif // !_FIELD_H_

//==================================
// フィールドの作成
// Author:村元翼
//==================================
#ifndef _FIELD_H_
#define _FIELD_H_
#include "main.h"
#include "scene3D.h"

//==============================================================================
// 前方宣言
//==============================================================================

//------------------------------------------------------------------------------
// クラスの定義
//------------------------------------------------------------------------------
class CField : public CScene3D
{
public:
	CField();											// コンストラクタ
	~CField();											// デストラクタ

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);	// 初期化
	void Uninit(void);									// 終了
	void Update(void);									// 更新
	void Draw(void);									// 描画

	// 生成
	static CField *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 rot);

	bool DotCollisionField(CScene *pScene);							// 点と床の当たり判定
	bool LineCollisionField(CScene *pScene);						// 線と床の当たり判定
	D3DXMATRIX *GetVtxMtx(void)	{ return &m_vtxMtxWorld[0]; }		// 頂点ワールドマトリックスの取得
	void SetVtxMtxWorld(D3DXVECTOR3 vtxPos, int nCntVtx);			// 頂点ワールドマトリックスの設定

private:
	D3DXMATRIX m_vtxMtxWorld[FIELD_VTX];	// 床の頂点ワールドマトリックス
	D3DXVECTOR3 m_vtx[FIELD_VTX];			// 床の頂点座標情報
	D3DXVECTOR3 m_vtxRot[FIELD_VTX];		// 床の頂点回転情報
	CScene3D	*m_pScene3D;				// シーン3Dクラス
};


#endif // !_FIELD_H_

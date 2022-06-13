//==============================================================================
// メッシュクラスの作成
// Author:村元翼
//==============================================================================
#include "mesh.h"
#include "Scene3D.h"

//=============================================================================
// コンストラクタ
//=============================================================================
CMesh::CMesh()
{
	// 各種初期化
	m_pTexture = nullptr;						// テクスチャポインタ
	m_pVtxBuff = nullptr;						// 頂点バッファポインタ
	m_pVtxIndexBuff = nullptr;					// 頂点インデックスバッファポインタ
	m_posMS = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 位置
	m_rotMS = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 回転
	m_sizeMS = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// サイズ
}

//=============================================================================
// デストラクタ
//=============================================================================
CMesh::~CMesh()
{

}

//------------------------------------------------------------
// 頂点座標の設定
//------------------------------------------------------------
void CMesh::SetPos(CScene *pScene,D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	pScene->SetPos(pos);
	pScene->SetSize(size);
	m_posMS = pos;
	m_sizeMS = size;
}

//------------------------------------------------------------
// テクスチャの設定(背景動かす用)
//------------------------------------------------------------
void CMesh::SetTex(float fSpeedX, float fSpeedY)
{
	VERTEX_3D *pVtx;

	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標
	int nNum = 0;
	for (int nVertical = 0; nVertical < m_nVertical + 1; nVertical++)
	{
		for (int nLine = 0; nLine < m_nLine + 1; nLine++, nNum++)
		{
			//テクスチャ
			pVtx[nNum].tex = D3DXVECTOR2(0.0f + ((1.0f / m_nLine) * nLine) + fSpeedX, 0.0f + ((1.0f / m_nVertical) * nVertical) + fSpeedY);
		}
	}
	m_pVtxBuff->Unlock();
}

//------------------------------------------------------------
// カラーの設定
//------------------------------------------------------------
void CMesh::SetCol(CScene *pScene, D3DXCOLOR col)
{
	pScene->SetCol(col);

	VERTEX_3D *pVtx;

	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標
	int nNum = 0;
	for (int nVertical = 0; nVertical < m_nVertical + 1; nVertical++)
	{
		for (int nLine = 0; nLine < m_nLine + 1; nLine++, nNum++)
		{
			pVtx[nNum].col = D3DXCOLOR(col.r, col.g, col.b, col.a);
		}
	}

	m_pVtxBuff->Unlock();
}

//------------------------------------------------------------
// 回転の設定
//------------------------------------------------------------
void CMesh::AddRot(D3DXVECTOR3 rot)
{
	m_rotMS += rot;

	//++++++++++++++++++++++++++++++//
	//			Y軸回転限界		  　//
	//++++++++++++++++++++++++++++++//
	if (m_rotMS.y < -D3DX_PI)
	{
		m_rotMS.y = D3DX_PI;
	}
	else if (m_rotMS.y > D3DX_PI)
	{
		m_rotMS.y = -D3DX_PI;
	}

	//++++++++++++++++++++++++++++++//
	//			X軸回転限界		  　//
	//++++++++++++++++++++++++++++++//
	if (m_rotMS.x < -D3DX_PI)
	{
		m_rotMS.x = D3DX_PI;
	}
	else if (m_rotMS.x > D3DX_PI)
	{
		m_rotMS.x = -D3DX_PI;
	}

	//++++++++++++++++++++++++++++++//
	//			Z軸回転限界		  　//
	//++++++++++++++++++++++++++++++//
	if (m_rotMS.z < -D3DX_PI)
	{
		m_rotMS.z = D3DX_PI;
	}
	else if (m_rotMS.z > D3DX_PI)
	{
		m_rotMS.z = -D3DX_PI;
	}
}

////------------------------------------------------------------
////テクスチャの設定
////------------------------------------------------------------
//void CMesh::SetTex(int nAnim, int nPartU)
//{
//	VERTEX_3D *pVtx;
//
//	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
//
//	pVtx[0].tex = D3DXVECTOR2(0.0f + (1.0f / nPartU)*nAnim, 0.0);
//	pVtx[1].tex = D3DXVECTOR2((1.0f / nPartU)*(nAnim + 1), 0.0f);
//	pVtx[2].tex = D3DXVECTOR2(0.0f + (1.0f / nPartU)*nAnim, 1.0f);
//	pVtx[3].tex = D3DXVECTOR2((1.0f / nPartU)*(nAnim + 1), 1.0f);
//
//	m_pVtxBuff->Unlock();
//
//}
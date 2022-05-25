//==============================================================================
// フィールドの作成
// Author:村元翼
//==============================================================================
#include "Scene3D.h"
#include "scene.h"
#include "renderer.h"
#include "manager.h"
#include "shadow.h"

//=============================================================================
// コンストラクタ
//=============================================================================
CShadow::CShadow(OBJTYPE nPriority) : CScene3D(nPriority)
{
	// 各種初期化
	m_pTexture = nullptr;							// テクスチャポインタ
	m_pVtxBuff = nullptr;							// 頂点バッファポインタ
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 位置
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// サイズ
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 回転
	m_OriginSize = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 元のサイズ
}

//=============================================================================
// デストラクタ
//=============================================================================
CShadow::~CShadow()
{

}

//=============================================================================
// インスタンス生成処理
//=============================================================================
CShadow *CShadow::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 rot)
{
	// インスタンス生成
	CShadow *pShadow = nullptr;
	if (!pShadow)
	{
		// 動的メモリ確保
		pShadow = new CShadow();

		// NULLチェック
		if (pShadow)
		{
			pShadow->m_pos = pos;			// 位置
			pShadow->m_size = size;			// 大きさ
			pShadow->m_rot = rot;			// 回転

			pShadow->Init(pos, size);	// 初期化
		}
	}

	return pShadow;
}

//=============================================================================
// ポリゴンの初期化処理
//=============================================================================
HRESULT CShadow::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	m_OriginSize = size;
	CScene3D::Init(pos, size);
	CScene3D::BindTexture(CManager::GetInstance()->GetTexture()->GetTexture("TEX_TYPE_EFFECT_MOVE"));
	return S_OK;
}

//=============================================================================
// ポリゴンの終了処理
//=============================================================================
void CShadow::Uninit(void)
{
	CScene3D::Uninit();
}

//=============================================================================
// ポリゴンの更新処理
//=============================================================================
void CShadow::Update(void)
{
	m_pos.x = CScene::GetPosOld().x;
	m_pos.z = CScene::GetPosOld().z;
	float Y = CScene::GetPosOld().y;

	CScene::SetPosOld({ m_pos.x, Y, m_pos.z });
	CScene3D::Update();


	/*float OldY = CScene::GetPosOld().y;
	float NewY = CScene::GetPos().y;
	float fLength = abs(OldY - NewY);

	if (fLength > 1.0f)
	{
		m_size.x *= fLength;
		m_size.z *= fLength;
		CScene3D::SetPos(m_pos, m_size);
	}
	else if (fLength <= 1.0f)
	{
		m_size.x = m_OriginSize.x;
		m_size.z = m_OriginSize.z;
	}*/

	Y -= GRAVITY;
	CScene::SetPos({ m_pos.x, Y, m_pos.z });
	//SetGravity(GRAVITY);
}

//=============================================================================
// ポリゴンの描画処理
//=============================================================================
void CShadow::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	CManager::GetInstance()->GetRenderer()->SetSubSynthesis(true, pDevice);

	CScene3D::Draw();

	CManager::GetInstance()->GetRenderer()->SetSubSynthesis(false, pDevice);
}

void CShadow::SetGravity(float GravitySpeed)
{
	m_pos.y -= GravitySpeed;
}


////================================================
//// ワールドマトリックス設定処理
////================================================
//void CShadow::SetVtxMtxWorld(D3DXVECTOR3 vtxPos, int nCntVtx)
//{
//	//デバイスのポインタ
//	LPDIRECT3DDEVICE9 pDevice;
//	//デバイスの取得
//	pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();
//
//	// 計算用マトリックス
//	D3DXMATRIX mtxRot, mtxTrans;
//
//	// ワールドマトリックスの初期化
//	D3DXMatrixIdentity(&m_vtxMtxWorld[nCntVtx]);
//
//	//向きを反映
//	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_vtxRot[nCntVtx].y, m_vtxRot[nCntVtx].x, m_vtxRot[nCntVtx].z);
//	D3DXMatrixMultiply(&m_vtxMtxWorld[nCntVtx], &m_vtxMtxWorld[nCntVtx], &mtxRot);
//
//	//位置を反映
//	D3DXMatrixTranslation(&mtxTrans, vtxPos.x, vtxPos.y, vtxPos.z);
//	D3DXMatrixMultiply(&m_vtxMtxWorld[nCntVtx], &m_vtxMtxWorld[nCntVtx], &mtxTrans);
//
//	D3DXMATRIX mtxParent;		//親のマトリックス
//	mtxParent = m_mtxWorld;
//
//	//算出した各パーツのワールドマトリックスと親のワールドマトリックスを掛け合わせる
//	D3DXMatrixMultiply(&m_vtxMtxWorld[nCntVtx], &m_vtxMtxWorld[nCntVtx], &mtxParent);
//
//	//ワールドマトリックスの設定
//	pDevice->SetTransform(D3DTS_WORLD, &m_vtxMtxWorld[nCntVtx]);
//}
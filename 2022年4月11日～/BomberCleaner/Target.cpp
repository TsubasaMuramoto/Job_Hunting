//========================================
// ターゲット処理
// Author : 村元翼
//========================================
#include "Target.h"
#include "model.h"
#include "manager.h"
#include "renderer.h"

//=============================================================================
// コンストラクタ
//=============================================================================
CTarget::CTarget(OBJTYPE nPriority) : CScene(nPriority)
{
	memset(&m_pModel, 0, sizeof(m_pModel));
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 位置
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 回転
	m_scale = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// スケール
}

//=============================================================================
// デストラクタ
//=============================================================================
CTarget::~CTarget()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CTarget::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	m_pos = pos;
	m_rot = rot;
	CScene::SetPos(m_pos);

	return S_OK;
}
//=============================================================================
// 終了処理
//=============================================================================
void CTarget::Uninit(void)
{
	// モデル破棄
	if (m_pModel)
	{
		m_pModel->Uninit();
		delete m_pModel;
		m_pModel = nullptr;
	}

	// オブジェクトの破棄
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CTarget::Update(void)
{
	if (m_pModel)
	{
		m_pModel->Update();

		if (m_pModel->GetHitBool())
		{
			//CScore *pScore = CGame::GetScore();
			//pScore->AddScore(TARGET_SCORE);
			//Uninit();
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CTarget::Draw(void)
{
	// デバイスのポインタ
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// 現在のマテリアルを取得
	D3DXMATRIX mtxRot, mtxTrans, mtxScale;	// 計算用マトリックス

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// 大きさを反映
	//D3DXMatrixScaling(&mtxScale, m_scale.x, m_scale.y, m_scale.z);
	//D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScale);

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// 各パーツのワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// モデルの描画
	if (m_pModel)
	{
		m_pModel->Draw();
	}
}

//=============================================================================
// インスタンス生成処理
//=============================================================================
CTarget *CTarget::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, int nXType)
{
	// デバイスのポインタ
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// インスタンス生成
	CTarget *pTarget = nullptr;
	if (!pTarget)
	{
		pTarget = new CTarget();
		if (pTarget)
		{
			pTarget->m_scale = scale;
			pTarget->Init(pos, rot);		// 初期化

												// 角度をラジアンに変換
			float rot_x = D3DXToRadian(rot.x);
			float rot_y = D3DXToRadian(rot.y);
			float rot_z = D3DXToRadian(rot.z);

			if (!pTarget->m_pModel)
			{
				// 爆弾スポナーモデルの生成
				pTarget->m_pModel = CModel::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(rot_x, rot_y, rot_z), scale, nXType);
				// 親モデルをNULLに設定
				pTarget->m_pModel->SetParent(NULL);
				// モデルタイプの設定
				pTarget->CScene::SetModelType(CScene::MODTYPE_TARGET);
				// 爆弾スポナーサイズの設定
				pTarget->SetSize(pTarget->m_pModel->GetSize());
			}
		}
	}
	return pTarget;
}
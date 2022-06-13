//===========================================
// 障害物モデル
//===========================================
#include "Obstacle.h"
#include "model.h"
#include "manager.h"
#include "renderer.h"
#include "player.h"
#include "fade.h"
#include "Ui.h"
#include "RemainCount.h"
#include "effect_blast.h"

//===========================================
// コンストラクタ
//===========================================
CObstacle::CObstacle(OBJTYPE nPriority) : CModel_Spawner(nPriority)
{
	m_pUI = nullptr;
	m_nFrame = 0;
	m_bDoOnce = false;
	m_bRetry = false;
}

//===========================================
// デストラクタ
//===========================================
CObstacle::~CObstacle()
{

}

//===========================================
// 生成
//===========================================
CObstacle *CObstacle::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, int nXType)
{
	// デバイスのポインタ
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// インスタンス生成
	CObstacle *pObstacle = nullptr;

	if (!pObstacle)
	{
		pObstacle = new CObstacle();
		// NULLチェック
		if (pObstacle)
		{
			pObstacle->Init(pos, rot);		// 初期化

			// 角度をラジアンに変換
			float rot_x = D3DXToRadian(rot.x);
			float rot_y = D3DXToRadian(rot.y);
			float rot_z = D3DXToRadian(rot.z);

			if (!pObstacle->m_pModel)
			{
				// モデルの生成
				pObstacle->m_pModel = CModel::Create({ 0.0f, 0.0f, 0.0f }, { rot_x, rot_y, rot_z }, scale, nXType);
				// 親モデルをNULLに設定
				pObstacle->m_pModel->SetParent(NULL);
				// モデルタイプの設定
				pObstacle->CScene::SetModelType(CScene::MODTYPE_NORMAL);
				// シングルモデルサイズの設定
				pObstacle->SetSize(pObstacle->m_pModel->GetSize());
			}
		}
	}

	return pObstacle;
}

//===========================================
// 初期化関数
//===========================================
HRESULT CObstacle::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	CModel_Spawner::Init(pos, rot);
	return S_OK;
}


//===========================================
// 終了関数
//===========================================
void CObstacle::Uninit()
{
	CModel_Spawner::Uninit();

	// UI削除
	if (m_pUI)
	{
		m_pUI->Uninit();
		delete m_pUI;
		m_pUI = nullptr;
	}
}

//===========================================
// 更新関数
//===========================================
void CObstacle::Update()
{
	if (m_pModel)
	{
		m_pModel->Update();

		if (m_pModel->GetPlayerHitBool() && !m_bDoOnce)
		{
			m_bDoOnce = true;
			CPlayer *pPlayer = (CPlayer*)CScene::GetScene(OBJTYPE_PLAYER);
			if (pPlayer)
			{
				pPlayer->SetUninit();
				CBlast::Create(pPlayer->GetPos(), pPlayer->GetSize(), { 0.0f,1.0f,1.0f,1.0f }, 20, 20);
				CRemainCount::SetRemainChange();
			}
		}
	}
}

//===========================================
// 描画関数
//===========================================
void CObstacle::Draw()
{
	// デバイスのポインタ
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// 現在のマテリアルを取得
	D3DXMATRIX mtxRotModel, mtxTransModel;	// 計算用マトリックス
	D3DXMATRIX mtxParent;					// 親のマトリックス

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRotModel, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRotModel);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTransModel, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTransModel);

	// 各パーツのワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// モデルの描画
	if (m_pModel)
	{
		m_pModel->Draw();
	}
}

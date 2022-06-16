//===========================================
// ゴール地点のモデル
//===========================================
#include "Gate.h"
#include "model.h"
#include "manager.h"
#include "renderer.h"
#include "fade.h"
#include "Ui.h"
#include "ClearGauge.h"

//===========================================
// コンストラクタ
//===========================================
CGate::CGate(OBJTYPE nPriority) : CModel_Spawner(nPriority)
{
	m_scale = { 0.0f,0.0f,0.0f };
	m_pUI = nullptr;
	m_nFrame = 0;
}

//===========================================
// デストラクタ
//===========================================
CGate::~CGate()
{

}

//===========================================
// 生成関数
//===========================================
CGate *CGate::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, int nXType)
{
	// デバイスのポインタ
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// インスタンス生成
	CGate *pGate = nullptr;
	if (!pGate)
	{
		pGate = new CGate();
		// NULLチェック
		if (pGoal)
		{
			pGate->m_scale = scale;
			pGate->Init(pos, rot);		// 初期化

										// 角度をラジアンに変換
			float rot_x = D3DXToRadian(rot.x);
			float rot_y = D3DXToRadian(rot.y);
			float rot_z = D3DXToRadian(rot.z);

			if (!pGate->m_pModel)
			{
				// モデルの生成
				pGate->m_pModel = CModel::Create({ 0.0f, 0.0f, 0.0f }, { rot_x, rot_y, rot_z }, scale, nXType);
				// 親モデルをNULLに設定
				pGate->m_pModel->SetParent(NULL);
				// モデルタイプの設定
				pGate->CScene::SetModelType(CScene::MODTYPE_NORMAL);
				// シングルモデルサイズの設定
				pGate->SetSize(pGate->m_pModel->GetSize());
			}
		}
	}

	return pGate;
}


//===========================================
// 初期化関数
//===========================================
HRESULT CGate::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	CModel_Spawner::Init(pos, rot);
	return S_OK;
}

//===========================================
// 終了関数
//===========================================
void CGate::Uninit(void)
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
void CGate::Update(void)
{
	if (m_pModel)
	{
		m_pModel->Update();

#if(1)
		if (m_pModel->GetPlayerHitBool() || CClearGauge::GetClear())		// ゴールに当たるとリザルトにいく
		{
			// ゴールメッセージの表示
			if (m_nFrame == 0)
			{
				m_pUI = CUi::Create(SCREEN_CENTER, { 3.0f, 1.0f, 0.0f }, CUi::UI_TYPE_ZOOM);
				m_pUI->BindTexture(CManager::GetInstance()->GetTexture()->GetTexture("TEX_TYPE_END"));
			}

			// リザルトへ行く
			if (m_nFrame == GO_RESULT_FRAME)
			{
				CManager::GetInstance()->SetStop(true);
				CManager::GetInstance()->GetFade()->SetFade(CManager::MODE::RESULT);
			}

			// フレームを進める
			m_nFrame++;
		}
#endif
	}
}

//===========================================
// 描画関数
//===========================================
void CGate::Draw(void)
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

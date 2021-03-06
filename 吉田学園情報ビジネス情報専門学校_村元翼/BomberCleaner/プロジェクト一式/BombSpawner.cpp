//========================================
// 爆弾スポナーの処理
// Author : 村元翼
//========================================
#include "BombSpawner.h"
#include "Bomb.h"
#include "model.h"
#include "manager.h"
#include "renderer.h"
#include "LoadX.h"

//===========================================
// マクロ定義
//===========================================
#define SPAWN_TIME		(120)		// スポーン間隔時間

//=============================================================================
// コンストラクタ
//=============================================================================
CBombSpawner::CBombSpawner(OBJTYPE nPriority) : CScene(nPriority)
{
	memset(&m_pBomb, 0, sizeof(m_pBomb));		// 爆弾クラス
	memset(&m_pModel, 0, sizeof(m_pModel));		// モデルクラス
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 位置
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 回転
	m_scale = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// スケール
	m_nSpawnTime = 0;							// スポーンタイム
	m_bIsThere = false;
}

//=============================================================================
// デストラクタ
//=============================================================================
CBombSpawner::~CBombSpawner()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CBombSpawner::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	m_pos = pos;			// 位置
	m_rot = rot;			// 回転
	CScene::SetPos(m_pos);	// 位置を設定

	return S_OK;
}
//=============================================================================
// 終了処理
//=============================================================================
void CBombSpawner::Uninit(void)
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
void CBombSpawner::Update(void)
{
	// モデル更新
	if (m_pModel)
	{
		m_pModel->Update();

		// 爆弾がスポナーの所にいない状態の処理
		if (!m_bIsThere)
		{
			m_nSpawnTime--;
			if (m_nSpawnTime <= 0)	// 爆弾を生成
			{
				if (!m_pBomb)
				{
					m_pBomb = CBomb::Create({ m_pos.x,m_pos.y + (m_pModel->GetSize().y * m_scale.y),m_pos.z }, m_rot, m_scale, CManager::GetInstance()->GetLoadX()->GetNum("MODTYPE_BOMB"));
				}
				m_nSpawnTime = SPAWN_TIME;		// 敵スポーンタイマーをリセット
				m_bIsThere = true;				// 爆弾が出現している状態にする
			}
		}
		else	// 爆弾がスポナーの所にいる状態の処理
		{
			if (m_pBomb)
			{
				if (m_pBomb->GetThrow())	// 爆弾を投げた判定を取得
				{
					m_pBomb = nullptr;		// 爆弾破棄
					m_bIsThere = false;		// 爆弾がスポナーの場所にいない状態にする
				}
			}
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CBombSpawner::Draw(void)
{
	// デバイスのポインタ
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// 現在のマテリアルを取得
	D3DXMATRIX mtxRot, mtxTrans, mtxScale;	// 計算用マトリックス

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

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
CBombSpawner *CBombSpawner::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, int nXType)
{
	// デバイスのポインタ
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// インスタンス生成
	CBombSpawner *pBombSpanwer = nullptr;
	if (!pBombSpanwer)
	{
		pBombSpanwer = new CBombSpawner();
		if (pBombSpanwer)
		{
			pBombSpanwer->m_scale = scale;		// スケール
			pBombSpanwer->Init(pos, rot);		// 初期化

			// 角度をラジアンに変換
			float rot_x = D3DXToRadian(rot.x);
			float rot_y = D3DXToRadian(rot.y);
			float rot_z = D3DXToRadian(rot.z);

			if (!pBombSpanwer->m_pModel)
			{
				// 爆弾スポナーモデルの生成
				pBombSpanwer->m_pModel = CModel::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(rot_x, rot_y, rot_z), scale, nXType);
				// 親モデルをNULLに設定
				pBombSpanwer->m_pModel->SetParent(NULL);
				// モデルタイプの設定
				pBombSpanwer->CScene::SetModelType(CScene::MODTYPE_BOMBSPAWNER);
				// 爆弾スポナーサイズの設定
				pBombSpanwer->SetSize(pBombSpanwer->m_pModel->GetSize());
			}
		}
	}
	return pBombSpanwer;
}
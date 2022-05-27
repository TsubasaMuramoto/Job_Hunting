//========================================
// 爆弾の処理
// Author : 村元翼
//========================================
#include "Bomb.h"
#include "model.h"
#include "manager.h"
#include "renderer.h"
#include "effect.h"
#include "LoadX.h"
#include "effect_blast.h"
#include "control_throw.h"
#include "meshfield.h"
#include "shadow.h"

//===========================================
// マクロ定義
//===========================================
#define BOMB_TIME				(240)								// 爆発時間
#define SHRINK_AND_SWELL_SPEED	(0.05f)								// 伸縮速度
#define MAX_SCALE_X				(1.1f)								// 最大スケール
#define DIFFUSE_SPEED			(0.05f)								// 色変更速度

//=============================================================================
// コンストラクタ
//=============================================================================
CBomb::CBomb(OBJTYPE nPriority) : CScene(nPriority)
{
	memset(&m_pShadow, 0, sizeof(m_pShadow));
	memset(&m_pControl, 0, sizeof(m_pControl));
	memset(&m_pModel, 0, sizeof(m_pModel));
	m_pos			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 位置
	m_rot			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 回転
	m_scale			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// スケール
	m_Emissive		= D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);	// 拡散光
	m_nBombTime		= BOMB_TIME;
	m_fSphereRot	= 0.0f;
	m_fSphereSize	= 1.0f;
	m_fSphereAlpha	= 1.0f;
	m_bIgnition		= false;
	m_bShrink		= false;
	m_bBlink		= false;
}

//=============================================================================
// デストラクタ
//=============================================================================
CBomb::~CBomb()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CBomb::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	m_pos = pos;
	m_rot = rot;
	m_fOriginScaleX = m_scale.x;	// スケールXを保存(初期サイズ)

	CScene::SetPos(m_pos);

	return S_OK;
}
//=============================================================================
// 終了処理
//=============================================================================
void CBomb::Uninit(void)
{
	// モデル破棄
	if (m_pModel)
	{
		m_pModel->Uninit();
		delete m_pModel;
		m_pModel = nullptr;
	}

	// コントロール破棄
	if (m_pControl)
	{
		m_pControl->Uninit();
		delete m_pControl;
		m_pControl = nullptr;
	}

	// 影の破棄
	if (m_pShadow)
	{
		m_pShadow->Uninit();
		m_pShadow = nullptr;
	}

	// オブジェクトの破棄
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CBomb::Update(void)
{
	CScene::SetPosOld(m_pos);	// 移動前の位置設定
	m_pos = CScene::GetPos();	// 移動後の位置取得

	// 爆弾がある状態の処理
	if (m_pModel)
	{
		m_pModel->Update();

		// 移動限界
		if (m_pos.x > MOVELIMIT)
		{
			m_pos.x = MOVELIMIT;
		}
		if (m_pos.x < -MOVELIMIT)
		{
			m_pos.x = -MOVELIMIT;
		}
		if (m_pos.z > MOVELIMIT)
		{
			m_pos.z = MOVELIMIT;
		}
		if (m_pos.z < -MOVELIMIT)
		{
			m_pos.z = -MOVELIMIT;
		}

		// 着火状態処理
		if (m_bIgnition)
		{
			// 発火のエフェクト
			CEffect::Create({ m_pos.x,m_pos.y + (m_pModel->GetSize().y * m_scale.y),m_pos.z }, IGNITION_SIZE, IGNITION_COL, IGNITION_SUB, 2);
			Ignition();
		}

		// 投げている状態の処理
		if (m_bThrow)
		{
			if (m_pControl)
			{
				m_pControl->Update(this);
			}

			// プレイヤーのシーン取得
			CScene *pScene = CScene::GetScene(CScene::OBJTYPE_MESH);

			float fHitTime = 0.0f;
			D3DXVECTOR3 Hitpos = {0.0f,0.0f,0.0f};
				// シーンがnullになるまで通る
			while (pScene)
			{
				// 次のシーンを取得
				CScene *pSceneNext = CScene::GetSceneNext(pScene);

				float fRad = 0.0f;
				// 当たり判定
				if (m_pModel->CalcParticlePlaneCollision(fRad, &CScene::GetPosOld(), &CScene::GetPos(), &D3DXVECTOR3(0.0f, 1.0f, 0.0f), &pScene->GetPos(), &fHitTime, &Hitpos))
				{
					m_bThrow = false;
				}

				// 次のシーンを現在のシーンにする
				pScene = pSceneNext;
			}
		}

		if (m_pShadow)	// シャドウの位置設定
		{
			m_pShadow->CScene::SetPosOld({ m_pos.x ,m_pos.y, m_pos.z });
		}
	}

	//CScene::SetPos(m_pos);
}

//=============================================================================
// 描画処理
//=============================================================================
void CBomb::Draw(void)
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
CBomb *CBomb::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, int nXType)
{
	// デバイスのポインタ
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// インスタンス生成
	CBomb *pBomb = nullptr;
	if (!pBomb)
	{
		pBomb = new CBomb();
		if (pBomb)
		{
			pBomb->m_scale = scale;
			pBomb->Init(pos, rot);		// 初期化

			// 角度をラジアンに変換
			float rot_x = D3DXToRadian(rot.x);
			float rot_y = D3DXToRadian(rot.y);
			float rot_z = D3DXToRadian(rot.z);

			if (!pBomb->m_pModel)
			{
				// 爆弾モデルの生成
				pBomb->m_pModel = CModel::Create(D3DXVECTOR3(0.0f,0.0f,0.0f), D3DXVECTOR3(rot_x, rot_y, rot_z), scale, nXType);
				// 親モデルをNULLに設定
				pBomb->m_pModel->SetParent(NULL);
				// モデルタイプの設定
				pBomb->CScene::SetModelType(CScene::MODTYPE_BOMB);
				// 爆弾サイズの設定
				pBomb->SetSize(pBomb->m_pModel->GetSize());

				// 影の設定
				//if (!pBomb->m_pShadow)
				//{
				//	pBomb->m_pShadow = CShadow::Create({ pos.x , pos.y ,pos.z }, { pBomb->GetSize().x,0.0f,pBomb->GetSize().z }, rot);
				//}
			}
		}
	}
	return pBomb;
}

//=============================================================================
// 爆発状態処理
//=============================================================================
void CBomb::Explosion(void)
{
	m_bThrow = false;

	// 爆発球の生成
	if (m_pModel)
	{
		float fRad = m_pModel->GetSize().x;
		CBlast::Create(m_pos, { 30.0f ,30.0f,30.0f }/*{ fRad, fRad ,fRad }*/, { 1.0f,0.0f,0.0f,1.0f }, 20, 20);
	}

	// 終了
	Uninit();
}

//=============================================================================
// 発火状態処理
//=============================================================================
void CBomb::Ignition(void)
{
	m_nBombTime--;

	// 爆弾サイズ変更処理
	if (!m_bShrink)		// 爆弾が膨らむ
	{
		m_scale.x += SHRINK_AND_SWELL_SPEED;
		m_scale.y += SHRINK_AND_SWELL_SPEED;
		m_scale.z += SHRINK_AND_SWELL_SPEED;
		if (m_scale.x > m_fOriginScaleX + MAX_SCALE_X)
		{
			m_bShrink = !m_bShrink;
		}
	}
	else				// 爆弾が縮む
	{
		m_scale.x -= SHRINK_AND_SWELL_SPEED;
		m_scale.y -= SHRINK_AND_SWELL_SPEED;
		m_scale.z -= SHRINK_AND_SWELL_SPEED;
		if (m_scale.x < m_fOriginScaleX)
		{
			m_bShrink = !m_bShrink;
		}
	}

	// 発光色変更処理
	if (!m_bBlink)		// 赤くなる
	{
		m_Emissive.r += DIFFUSE_SPEED;
		if (m_Emissive.r >= 1.0f)
		{
			m_bBlink = !m_bBlink;
		}
	}
	else				// 黒くなる
	{
		m_Emissive.r -= DIFFUSE_SPEED;
		if (m_Emissive.r <= 0.0f)
		{
			m_bBlink = !m_bBlink;
		}
	}

	m_pModel->SetEmissive(m_Emissive);
	m_pModel->SetScale(m_scale);

	if (m_nBombTime <= 0)	// 0で爆発する
	{
		Explosion();
	}
}

//=============================================================================
// 放射時の処理
//=============================================================================
void CBomb::Throw(D3DXVECTOR3 ThrowDir)
{
	if (!m_bThrow)
	{
		m_pControl = CControlThrow::Create();
		m_bThrow = true;
		m_bIgnition = true;
		m_ThrowDir = ThrowDir;
		D3DXVec3Normalize(&m_ThrowDir, &m_ThrowDir);
	}
}
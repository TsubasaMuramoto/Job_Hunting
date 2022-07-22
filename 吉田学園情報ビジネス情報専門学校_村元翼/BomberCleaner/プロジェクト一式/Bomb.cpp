//=============================================================================
// 爆弾の処理
// Author : 村元翼
//=============================================================================
#include "Bomb.h"
#include "model.h"
#include "manager.h"
#include "renderer.h"
#include "effect.h"
#include "effect_blast.h"
#include "control_throw.h"
#include "shadow.h"
#include "sound.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define BOMB_TIME				(240)								// 爆発時間
#define SHRINK_AND_SWELL_SPEED	(0.05f)								// 伸縮速度
#define MAX_SCALE_X				(1.1f)								// 最大スケール
#define DIFFUSE_SPEED			(0.05f)								// 色変更速度

//=============================================================================
// コンストラクタ
//=============================================================================
CBomb::CBomb(OBJTYPE nPriority) : CScene(nPriority)
{
	memset(&m_pShadow, 0, sizeof(m_pShadow));				// シャドウ
	memset(&m_pControl, 0, sizeof(m_pControl));				// コントロール
	memset(&m_pModel, 0, sizeof(m_pModel));					// モデル
	m_pos			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 位置
	m_rot			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 回転
	m_scale			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// スケール
	m_Emissive		= D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);	// 拡散光
	m_nBombTime		= BOMB_TIME;							// 爆発までの時間
	m_fSphereRot	= 0.0f;									// 爆風の回転
	m_fSphereSize	= 1.0f;									// 爆風のサイズ
	m_fSphereAlpha	= 1.0f;									// 爆風のα値
	m_bIgnition		= false;								// 着火判定
	m_bShrink		= false;								// 伸縮判定
	m_bBlink		= false;								// 点滅判定
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
	// 位置・回転取得
	m_pos = pos;
	m_rot = rot;
	m_fOriginScaleX = m_scale.x;	// スケールXを保存(初期サイズ)

	CScene::SetPos(m_pos);			// 位置設定

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
			// コントロールクラス更新
			if (m_pControl)
			{
				m_pControl->Update(this);
			}
		}

		if (m_pShadow)	// 影の位置設定
		{
			m_pShadow->CScene::SetPosOld({ m_pos.x ,m_pos.y, m_pos.z });
		}
	}
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
			pBomb->m_scale = scale;		// スケール
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
	// 爆発音
	CSound *pSound = CManager::GetInstance()->GetSound();
	pSound->PlaySound(CSound::SOUND_LABEL_SE_EXPLOSION);

	// 投げている状態をfalseにする
	m_bThrow = false;

	// 爆発球の生成
	if (m_pModel)
	{
		float fRad = m_pModel->GetSize().x;
		CBlast::Create(m_pos, { 30.0f ,30.0f,30.0f }/*{ fRad, fRad ,fRad }*/, { 1.0f,0.0f,0.0f,1.0f }, 20, 20);
	}
}

//=============================================================================
// 発火状態処理
//=============================================================================
void CBomb::Ignition(void)
{
	// 爆発までのカウント
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

	if (m_nBombTime <= 0)	// 0で爆発処理
	{
		CSound *pSound = CManager::GetInstance()->GetSound();
		pSound->StopSound(CSound::SOUND_LABEL_SE_IGNITION);	// 発火音

		Explosion();
		Uninit();
	}
}

//=============================================================================
// 投げた時の処理
//=============================================================================
void CBomb::Throw(D3DXVECTOR3 ThrowDir)
{
	if (!m_bThrow)
	{
		CSound *pSound = CManager::GetInstance()->GetSound();
		pSound->PlaySound(CSound::SOUND_LABEL_SE_IGNITION);				// 発火音
		pSound->ControllVoice(CSound::SOUND_LABEL_SE_IGNITION, 0.2f);

		m_pControl	= CControlThrow::Create();				// 投げた時の移動クラス
		m_bThrow	= true;									// 投げている状態にする
		m_bIgnition = true;									// 発火状態
		m_ThrowDir	= ThrowDir;								// 投げた向きを取得
		D3DXVec3Normalize(&m_ThrowDir, &m_ThrowDir);		// 向きを単位化
	}
}
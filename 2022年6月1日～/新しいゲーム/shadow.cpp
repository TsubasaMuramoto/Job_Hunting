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
			pShadow->Init(pos, size);		// 初期化
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

	Y -= GRAVITY;
	CScene::SetPos({ m_pos.x, Y, m_pos.z });
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
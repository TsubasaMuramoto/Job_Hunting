//=============================================================================
//
// エフェクト [effect.cpp]
// Author : 村元翼
//
//=============================================================================
//=============================================================================
//インクルードファイル
//=============================================================================
#include "Scene3D.h"
#include "manager.h"
#include "renderer.h"
#include "effect.h"
#include "control_fade.h"
#include "control_rise.h"
#include "control_spread.h"

//=============================================================================
// 静的メンバ変数
//=============================================================================

//=============================================================================
// コンストラクタ
//=============================================================================
CEffect::CEffect(OBJTYPE nPriority) : CScene(nPriority)
{
	// メンバ変数のクリア
	m_pos = { 0.0f,0.0f,0.0f };					// 位置
	m_size = { 0.0f,0.0f,0.0f };				// 大きさ
	m_col = { 0.0f,0.0f,0.0f,0.0f };			// 色
	m_fSub = 0.0f;								// 減少量
}

//=============================================================================
// デストラクタ
//=============================================================================
CEffect::~CEffect()
{
}

//=============================================================================
// 生成処理
//=============================================================================
CEffect *CEffect::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXCOLOR col, float fSub, int nType)
{
	// インスタンス生成
	CEffect *pEffect = nullptr;
	if (!pEffect)
	{
		pEffect = new CEffect(OBJTYPE_EFFECT);
		if (pEffect)
		{
			pEffect->CScene::SetCol(col);
			pEffect->m_fSub = fSub;
			pEffect->Init(pos ,size);

			// ビルボードのタイプ
			switch (nType)
			{
			case EFFECT_NONE:		// 無し
				break;

			case EFFECT_FADE:		// フェードアウト
				//pEffect->m_pControl = CControlFade::Create();
				break;

			case EFFECT_BURN:		// 発火
				pEffect->m_pControl = CControlRise::Create();
				break;

			case EFFECT_EXPLOSION:	// 爆発
				pEffect->m_pControl = CControlSpread::Create();
			}
		}
	}
	return pEffect;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CEffect::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	// デバイス・テクスチャの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();
	BindTexture(CManager::GetInstance()->GetTexture()->GetTexture("TEX_TYPE_EFFECT_MOVE"));

	m_pos = pos;
	m_size = size;
	CScene::SetPos(m_pos);
	CScene::SetSize(m_size);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	VERTEX_3D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos = D3DXVECTOR3(-m_size.x, +m_size.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(+m_size.x, +m_size.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-m_size.x, -m_size.y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(+m_size.x, -m_size.y, 0.0f);

	// 法線ベクトルの設定
	pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

	// 頂点カラーの設定
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	// テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// 頂点バッファのアンロック
	m_pVtxBuff->Unlock();

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CEffect::Uninit(void)
{
	// コントロール破棄
	if (m_pControl)
	{
		m_pControl->Uninit();
		delete m_pControl;
		m_pControl = nullptr;
	}

	// 頂点バッファの破棄
	if (m_pVtxBuff)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CEffect::Update(void)
{
	// タイプに合わせた動きをする
	if (m_pControl)
	{
		m_pControl->Update(this);
	}

	m_pos = CScene::GetPos();
	m_size = CScene::GetSize();
	m_col = CScene::GetCol();

	// サイズの縮小
	if (m_size.x > 0.0f && m_size.y > 0.0f)
	{
		m_size.x -= m_fSub;
		m_size.y -= m_fSub;
	}

	// 透明度のフェードアウト
	if (m_col.a > 0.0f)
	{
		m_col.a -= 0.001f;
	}

	CScene::SetPos(m_pos);
	CEffect::SetSize(m_size);
	CEffect::SetCol(m_col);

	// ビルボードの終了
	if (m_col.a <= 0.0f || (m_size.x <= 0.0f && m_size.y <= 0.0f))
	{
		Uninit();
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CEffect::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();
	D3DXMATRIX mtxView, mtxTrans;											// 計算用マトリックス

	CManager::GetInstance()->GetRenderer()->SetZTest(true, pDevice);		// ビルボードがモデルの前に描画されないようにする
	CManager::GetInstance()->GetRenderer()->SetAddSynthesis(true, pDevice);	// アルファブレンドによる加算合成
	CManager::GetInstance()->GetRenderer()->SetAlphaTest(true, pDevice);	// アルファテスト
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);							// ライトを無効化

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// ビューマトリックスを取得
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);

	// ポリゴンをカメラに対して正面に向ける
	D3DXMatrixInverse(&m_mtxWorld, NULL, &mtxView);  //逆行列を求める
	m_mtxWorld._41 = 0.0f;
	m_mtxWorld._42 = 0.0f;
	m_mtxWorld._43 = 0.0f;

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));  //頂点バッファをデバイスのデータストリームに設定

	pDevice->SetFVF(FVF_VERTEX_3D); //頂点フォーマットの設定

	// テクスチャの設定
	pDevice->SetTexture(0, m_pTexture);

	// ポリゴンの描画
	pDevice->DrawPrimitive
	(
		D3DPT_TRIANGLESTRIP,
		0,
		2
	);

	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);							// ライティング有効
	CManager::GetInstance()->GetRenderer()->SetZTest(false, pDevice);		// Zテスト無効
	CManager::GetInstance()->GetRenderer()->SetAlphaTest(false, pDevice);	// αテスト無効
	CManager::GetInstance()->GetRenderer()->SetAddSynthesis(false, pDevice);// 加算合成無効
}

//*****************************************************************************
// 位置設定
//*****************************************************************************
void CEffect::SetSize(D3DXVECTOR3 size)
{
	m_size = size;
	CScene::SetSize(m_size);

	VERTEX_3D *pVtx; //頂点バッファへのポインタ

	 // 頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos = D3DXVECTOR3(-m_size.x, +m_size.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(+m_size.x, +m_size.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-m_size.x, -m_size.y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(+m_size.x, -m_size.y, 0.0f);

	//頂点バッファのアンロック
	m_pVtxBuff->Unlock();
}

//*****************************************************************************
// 色設定
//*****************************************************************************
void CEffect::SetCol(D3DXCOLOR col)
{
	m_col = col;
	CScene::SetCol(m_col);

	// 頂点情報へのポインタ
	VERTEX_3D *pVtx;

	// 頂点座標をロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//テクスチャ座標を更新
	pVtx[0].col = col;
	pVtx[1].col = col;
	pVtx[2].col = col;
	pVtx[3].col = col;

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}
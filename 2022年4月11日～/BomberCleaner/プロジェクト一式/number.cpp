//=============================================================================
//
// 数字の処理 [number.cpp]
// Author : 村元翼
//
//=============================================================================
#include "number.h"
#include "manager.h"
#include "renderer.h"

//=============================================================================
// コンストラクタ
//=============================================================================
CNumber::CNumber(OBJTYPE nPriority) : CScene2D(nPriority)
{
}

//=============================================================================
//デストラクタ
//=============================================================================
CNumber::~CNumber()
{
}

//=============================================================================
//　数字の生成
//=============================================================================
CNumber *CNumber::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	// 数字のインスタンス生成
	CNumber *pNumber = nullptr;
	if (!pNumber)
	{
		pNumber = new CNumber;
		if (pNumber)
		{
			pNumber->Init(pos, size);																		// 初期化
			pNumber->BindTexture(CManager::GetInstance()->GetTexture()->GetTexture("TEX_TYPE_NUMBER"));		// テクスチャ取得
		}
	}
	return pNumber;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CNumber::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	// シーン2D初期化
	CScene2D::Init(pos,size);

	// デバイスのポインタ
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,	// 確保するバッファのサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	// 頂点情報へのポインタ
	VERTEX_2D * pVtx;

	// 頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void **)&pVtx, 0);

	// 頂点座標の設定	
	pVtx[0].pos = D3DXVECTOR3(pos.x - size.x, pos.y - size.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(pos.x + size.x, pos.y - size.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(pos.x - size.x, pos.y + size.y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(pos.x + size.x, pos.y + size.y, 0.0f);

	// テクスチャの設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);

	// rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// 頂点カラー
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CNumber::Uninit(void)
{
	// 頂点バッファの破棄
	if (m_pVtxBuff)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}

	// シーン2D終了
	CScene2D::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CNumber::Update(void)
{
	// シーン2D更新
	CScene2D::Update();
}

//=============================================================================
// 描画処理
//=============================================================================
void CNumber::Draw(void)
{
	// シーン2D描画
	CScene2D::Draw();

	// デバイスを取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// アルファテスト有効
	CManager::GetInstance()->GetRenderer()->SetAlphaTest(true, pDevice);			

	// 頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// ポリゴンの描画
	pDevice->DrawPrimitive
	(
		D3DPT_TRIANGLESTRIP,
		0,												// 開始する頂点のインデックス,
		2												// 描画するプリミティブ数
	);

	// アルファテスト無効
	CManager::GetInstance()->GetRenderer()->SetAlphaTest(false, pDevice);			
}

//=============================================================================
// 色の設定
//=============================================================================
void CNumber::SetColor(D3DXCOLOR col)
{
	// カラー取得
	m_col = col;

	// 頂点情報へのポインタ
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void **)&pVtx, 0);

	// 頂点カラー
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//=============================================================================
// 数字の設定
//=============================================================================
void CNumber::SetNumber(int nNumber)
{
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void **)&pVtx, 0);

	pVtx[0].tex = D3DXVECTOR2(nNumber * 0.1f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(nNumber * 0.1f + 0.1f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(nNumber * 0.1f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(nNumber * 0.1f + 0.1f, 1.0f);

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}
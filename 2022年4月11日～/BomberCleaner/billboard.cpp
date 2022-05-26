//=============================================================================
//
// ビルボードクラス [billboard.cpp]
//
//=============================================================================
#include "Scene3D.h"
#include "manager.h"
#include "renderer.h"
#include "billboard.h"

//*****************************************************************************
// コンストラクタ
//*****************************************************************************
CBillboard::CBillboard(OBJTYPE nPriority) : CScene(nPriority)
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_colval = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
}

//*****************************************************************************
// デストラクタ
//*****************************************************************************
CBillboard::~CBillboard()
{

}

//*****************************************************************************
// 初期化
//*****************************************************************************
HRESULT CBillboard::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

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

//*****************************************************************************
// 終了処理
//*****************************************************************************
void CBillboard::Uninit(void)
{
	// 頂点バッファの破棄
	if (m_pVtxBuff)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}
	Release();
}

//*****************************************************************************
// 更新処理
//*****************************************************************************
void CBillboard::Update(void)
{
	m_pos = CScene::GetPos();

	if (m_pVtxBuff)
	{
		SetPos(m_pos, m_size);
		SetColVal(m_colval);
	}
}

//*****************************************************************************
// 描画処理
//*****************************************************************************
void CBillboard::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();
	D3DXMATRIX mtxView, mtxTrans;											// 計算用マトリックス

	CManager::GetInstance()->GetRenderer()->SetZTest(true, pDevice);		// ビルボードがモデルの前に描画されないようにする
	CManager::GetInstance()->GetRenderer()->SetAddSynthesis(true,pDevice);	// アルファブレンドによる加算合成
	CManager::GetInstance()->GetRenderer()->SetAlphaTest(true, pDevice);	// アルファテスト
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);							// ライトを無効化

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	//ビューマトリックスを取得
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);

	//ポリゴンをカメラに対して正面に向ける
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
// 生成処理
//*****************************************************************************
CBillboard *CBillboard::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXCOLOR col)
{
	CBillboard *pBillboard = nullptr;
	if (!pBillboard)
	{
		pBillboard = new CBillboard;
		if (pBillboard)
		{
			pBillboard->m_colval = col;
			pBillboard->Init(pos, size);																		// 初期化
			pBillboard->BindTexture(CManager::GetInstance()->GetTexture()->GetTexture("TEX_TYPE_EFFECT_MOVE"));	// テクスチャ取得
		}
	}
	return pBillboard;
}

//*****************************************************************************
// 位置設定
//*****************************************************************************
void CBillboard::SetPos(D3DXVECTOR3 pos, D3DXVECTOR3 scale)
{
	m_pos = pos;
	m_size = scale;
	CScene::SetPos(m_pos);
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
void CBillboard::SetColVal(D3DCOLORVALUE colval)
{
	m_colval = colval;
	SetCol(m_colval);

	// 頂点情報へのポインタ
	VERTEX_3D *pVtx;

	// 頂点座標をロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//テクスチャ座標を更新
	pVtx[0].col = D3DXCOLOR(m_colval.r, m_colval.g,m_colval.b, m_colval.a);
	pVtx[1].col = D3DXCOLOR(m_colval.r, m_colval.g,m_colval.b, m_colval.a);
	pVtx[2].col = D3DXCOLOR(m_colval.r, m_colval.g,m_colval.b, m_colval.a);
	pVtx[3].col = D3DXCOLOR(m_colval.r, m_colval.g,m_colval.b, m_colval.a);

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

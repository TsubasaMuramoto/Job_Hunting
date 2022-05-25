//========================================================================
// ポリゴンの処理
//========================================================================
#include "scene2D.h"
#include "renderer.h"
#include "manager.h"
#include "texture.h"
#include "game.h"
#include "bg.h"

//=============================================================================
// コンストラクタ
//=============================================================================
CScene2D::CScene2D(OBJTYPE nPriority) :CScene(nPriority)
{
	m_pTexture = nullptr;
	m_pVtxBuff = nullptr;
}

//=============================================================================
// デストラクタ
//=============================================================================
CScene2D::~CScene2D()
{

}

//=============================================================================
//インスタンス生成処理
//=============================================================================
CScene2D *CScene2D::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, CScene::OBJTYPE objtype)
{
	// インスタンス生成
	CScene2D *pScene2D = nullptr;

	if (!pScene2D)
	{
		// 動的メモリ確保
		pScene2D = new CScene2D(objtype);

		// NULLチェック
		if (pScene2D)
		{
			pScene2D->Init(pos,size);																	// 初期化
			pScene2D->BindTexture(CManager::GetInstance()->GetTexture()->GetTexture("TEX_TYPE_FLOOR"));	// テクスチャ取得
			pScene2D->SetPos(pos, size);																// 位置設定
		}
	}
	return pScene2D;
}

//=============================================================================
// ポリゴンの初期化
//=============================================================================
HRESULT CScene2D::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	// デバイスのポインタ

	CScene::SetPos(pos);
	CScene::SetSize(size);

	 // 頂点情報を設定
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL
	);

	// 頂点バッファのポインタ
	VERTEX_2D *pVtx;

	// 頂点バッファのロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// バッファの生成(位置)
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// テクスチャ
	pVtx[0].tex = D3DXVECTOR2(0.0, 0.0);
	pVtx[1].tex = D3DXVECTOR2(1.0, 0.0);
	pVtx[2].tex = D3DXVECTOR2(0.0, 1.0);
	pVtx[3].tex = D3DXVECTOR2(1.0, 1.0);

	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// カラー
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// アンロック
	m_pVtxBuff->Unlock();

	return S_OK;
}

//=============================================================================
// ポリゴンの終了処理
//=============================================================================
void CScene2D::Uninit(void)
{
	// 頂点バッファの破棄
	if (m_pVtxBuff)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}

	// オブジェクトの破棄
	Release();
}

//=============================================================================
// ポリゴンの更新処理
//=============================================================================
void CScene2D::Update(void)
{

}

//=============================================================================
// ポリゴンの描画処理
//=============================================================================
void CScene2D::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	// デバイスのポインタ

	// 頂点フォーマットの設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));		
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, m_pTexture);

	// ポリゴンの描画
	pDevice->DrawPrimitive
	(
		D3DPT_TRIANGLESTRIP,
		0,																// 描画を開始する最初の頂点インデックス
		2																// ポリゴンの数
	);	
}

//------------------------------------------------------------
// 頂点座標の設定(不定形ポリゴンver.)
//------------------------------------------------------------
void CScene2D::SetPos(D3DXVECTOR3 vtx0, D3DXVECTOR3 vtx1, D3DXVECTOR3 vtx2, D3DXVECTOR3 vtx3)
{
	VERTEX_2D *pVtx;

	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// バッファの生成
	pVtx[0].pos = vtx0;
	pVtx[1].pos = vtx1;
	pVtx[2].pos = vtx2;
	pVtx[3].pos = vtx3;

	m_pVtxBuff->Unlock();
}

//------------------------------------------------------------
// 長方形ポリゴン(trueでスクロール込み)
//------------------------------------------------------------
void CScene2D::SetPos(D3DXVECTOR3 pos, D3DXVECTOR3 scale, bool bScroll)
{
	D3DXVECTOR3 Pos = { 0.0f,0.0f,0.0f };
	Pos = pos;

	CScene::SetPos(Pos);
	CScene::SetSize(scale);

	VERTEX_2D *pVtx;

	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// バッファの生成
	pVtx[0].pos = D3DXVECTOR3(Pos.x - (scale.x / 2), Pos.y - (scale.y / 2), 0.0f);
	pVtx[1].pos = D3DXVECTOR3(Pos.x + (scale.x / 2), Pos.y - (scale.y / 2), 0.0f);
	pVtx[2].pos = D3DXVECTOR3(Pos.x - (scale.x / 2), Pos.y + (scale.y / 2), 0.0f);
	pVtx[3].pos = D3DXVECTOR3(Pos.x + (scale.x / 2), Pos.y + (scale.y / 2), 0.0f);

	m_pVtxBuff->Unlock();
}

//------------------------------------------------------------
// 長方形ポリゴン(回転込み)
//------------------------------------------------------------
void CScene2D::SetPos(D3DXVECTOR3 pos, D3DXVECTOR3 scale, D3DXVECTOR3 Rotvtx0, D3DXVECTOR3 Rotvtx1, D3DXVECTOR3 Rotvtx2, D3DXVECTOR3 Rotvtx3)
{
	CScene::SetPos(pos);
	CScene::SetSize(scale);

	VERTEX_2D *pVtx;

	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// バッファの生成
	pVtx[0].pos = D3DXVECTOR3(pos.x + Rotvtx0.x, pos.y + Rotvtx0.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(pos.x + Rotvtx1.x, pos.y + Rotvtx1.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(pos.x + Rotvtx2.x, pos.y + Rotvtx2.y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(pos.x + Rotvtx3.x, pos.y + Rotvtx3.y, 0.0f);

	m_pVtxBuff->Unlock();
}

//--------------------------------------------------------------------
// pos原点左端
//--------------------------------------------------------------------
void CScene2D::SetScalePos(D3DXVECTOR3 pos, D3DXVECTOR3 scale)
{
	CScene::SetPos(pos);
	CScene::SetSize(scale);

	VERTEX_2D *pVtx;

	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//バッファの生成
	pVtx[0].pos = D3DXVECTOR3(pos.x,			pos.y - scale.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(pos.x + scale.x,	pos.y - scale.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(pos.x,			pos.y + scale.y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(pos.x + scale.x,	pos.y + scale.y, 0.0f);

	m_pVtxBuff->Unlock();

}

//------------------------------------------------------------
// テクスチャの設定
//------------------------------------------------------------
void CScene2D::SetTex(int nAnim, int nPartU)
{
	VERTEX_2D *pVtx;

	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].tex = D3DXVECTOR2(0.0f + (1.0f / nPartU)* nAnim,	  0.0f);
	pVtx[1].tex = D3DXVECTOR2(		 (1.0f / nPartU)*(nAnim + 1), 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f + (1.0f / nPartU)* nAnim,	  1.0f);
	pVtx[3].tex = D3DXVECTOR2(		 (1.0f / nPartU)*(nAnim + 1), 1.0f);

	m_pVtxBuff->Unlock();

}
void CScene2D::SetTex(float nPatternX,float nPatternY, float fSizeX, float fSizeY)
{
	VERTEX_2D *pVtx;

	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].tex = D3DXVECTOR2(nPatternX * fSizeX		 , nPatternY * fSizeY);
	pVtx[1].tex = D3DXVECTOR2(nPatternX * fSizeX + fSizeX, nPatternY * fSizeY);
	pVtx[2].tex = D3DXVECTOR2(nPatternX * fSizeX		 , nPatternY * fSizeY + fSizeY);
	pVtx[3].tex = D3DXVECTOR2(nPatternX * fSizeX + fSizeX, nPatternY * fSizeY + fSizeY);

	m_pVtxBuff->Unlock();

}
void CScene2D::SetTex(float fSpeedX, float fSpeedY)
{
	VERTEX_2D *pVtx;

	CScene::SetTex(D3DXVECTOR2(fSpeedX, fSpeedY));

	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].tex = D3DXVECTOR2(0.0f + fSpeedX, 0.0f + fSpeedY);
	pVtx[1].tex = D3DXVECTOR2(1.0f + fSpeedX, 0.0f + fSpeedY);
	pVtx[2].tex = D3DXVECTOR2(0.0f + fSpeedX, 1.0f + fSpeedY);
	pVtx[3].tex = D3DXVECTOR2(1.0f + fSpeedX, 1.0f + fSpeedY);

	m_pVtxBuff->Unlock();
}

//------------------------------------------------------------
// カラーの設定
//------------------------------------------------------------
void CScene2D::SetCol(D3DXCOLOR col)
{
	CScene::SetCol(col);

	VERTEX_2D *pVtx;

	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].col = D3DXCOLOR(col.r, col.g, col.b, col.a);
	pVtx[1].col = D3DXCOLOR(col.r, col.g, col.b, col.a);
	pVtx[2].col = D3DXCOLOR(col.r, col.g, col.b, col.a);
	pVtx[3].col = D3DXCOLOR(col.r, col.g, col.b, col.a);

	m_pVtxBuff->Unlock();

}

//------------------------------------------------------------
// ポリゴンの移動計算
//------------------------------------------------------------
D3DXVECTOR3 CScene2D::Move(D3DXVECTOR3 *pPos, float angle, float speed)
{
	D3DXVECTOR3 move;
	float fAngle = angle;
	float fSpeed = speed;

	move.x = sinf(-fAngle) * fSpeed;
	move.y = cosf(-fAngle) * fSpeed;

	*pPos -= move;

	return move;
}

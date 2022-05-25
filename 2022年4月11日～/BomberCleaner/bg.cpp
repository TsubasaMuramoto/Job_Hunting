//---------------------------------------------------
// 背景の処理
// Author:村元翼
//---------------------------------------------------
#include "bg.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "scene2D.h"
#include "keyboard.h"
#include "game.h"

//--------------------------
// コンストラクタ
//----------------------------
CBg::CBg(OBJTYPE nPriority):CScene2D(nPriority)
{
	m_Tex = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_bScroll = false;
}

//--------------------------
// デストラクタ
//----------------------------
CBg::~CBg()
{

}

//--------------------------
// 初期化
//----------------------------
HRESULT CBg::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	m_pos = pos;

	// テクスチャ取得
	CScene2D::BindTexture(CManager::GetInstance()->GetTexture()->GetTexture("TEX_TYPE_FLOOR"));
	
	CScene2D::Init(m_pos,size);		// 初期化
	CScene2D::SetPos				// 位置設定
	(
		D3DXVECTOR3(0.0f,0.0f, 0.0f),
		D3DXVECTOR3(SCREEN_WIDTH,0.0f, 0.0f),
		D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f),
		D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f)
	);

	CScene2D::SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));	// カラー設定
	return S_OK;
}

//--------------------------
// 終了
//----------------------------
void CBg::Uninit(void)
{
	CScene2D::Uninit();
}

//--------------------------
// 更新
//----------------------------
void CBg::Update(void)
{
	CScene2D::Update();

	if (m_bScroll)
	{
		CScene2D::SetTex(m_Tex.x, m_Tex.y);			// テクスチャ更新
	}	
}

//--------------------------
//描画
//----------------------------
void CBg::Draw(void)
{
	CScene2D::Draw();
}

//---------------------------------------------------------------
//インスタンス生成処理
//---------------------------------------------------------------
CBg *CBg::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size,bool bScrol, CScene::OBJTYPE objtype)
{
	//インスタンス生成
	CBg *pBg = new CBg(objtype);

	if (pBg)
	{
		// 情報の取得
		pBg->m_bScroll = bScrol;

		// 初期化処理
		pBg->Init(pos, size);
	}
	return pBg;
}


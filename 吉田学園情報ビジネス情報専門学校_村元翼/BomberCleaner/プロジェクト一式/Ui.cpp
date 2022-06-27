//================================================
// UI処理
// Author:村元翼
//================================================
#include "ui.h"
#include "control_blink.h"
#include "control_zoom.h"
#include "manager.h"
#include "renderer.h"

//================================================
// デフォルトコンストラクタ
//================================================
CUi::CUi(OBJTYPE nPriority) : CScene2D(nPriority)
{
	m_pTexture = nullptr;
	m_pControl = nullptr;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_UiType = UI_TYPE_NONE;
}

//================================================
// デストラクタ
//================================================
CUi::~CUi()
{

}

//================================================
// 初期化処理
//================================================
HRESULT CUi::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	// 位置・サイズ設定
	m_pos = pos;
	m_size = size;
	CScene::SetPos(m_pos);
	CScene::SetSize(m_size);

	CScene2D::Init(m_pos, m_size);	// シーン2D初期化

	return S_OK;
}

//================================================
// 終了処理
//================================================
void CUi::Uninit(void)
{
	// コントロール破棄
	if (m_pControl)
	{
		m_pControl->Uninit();
		delete m_pControl;
		m_pControl = nullptr;
	}

	// シーン2D破棄
	CScene2D::Uninit();
}

//================================================
// 更新処理
//================================================
void CUi::Update(void)
{
	// 位置・サイズ・カラー取得
	m_pos = GetPos();
	m_size = GetSize();
	m_col = GetCol();

	// 位置・サイズ更新
	if (m_pVtxBuff)
	{
		CScene2D::SetPos(m_pos, m_size);
	}

	// コントロール更新
	if (m_pControl)
	{
		m_pControl->Update(this);
	}

}


//================================================
// 描画処理
//================================================
void CUi::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	// デバイスのポインタ
	CManager::GetInstance()->GetRenderer()->SetAlphaTest(true, pDevice);				// αテスト有効

	CScene2D::Draw();																	// シーン2D描画

	CManager::GetInstance()->GetRenderer()->SetAlphaTest(false, pDevice);				// αテスト無効
}


//================================================
// 生成処理
//================================================
CUi *CUi::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, UI_TYPE type)
{
	// UIインスタンス生成
	CUi *pUi = nullptr;
	if (!pUi)
	{
		pUi = new CUi;

		if (pUi)
		{
			// タイプ別処理
			switch (type)
			{
			case UI_TYPE_BLINK:	// 点滅
				pUi->m_pControl = CControlBlink::Create();
				break;

			case UI_TYPE_ZOOM:	// ズーム
				pUi->m_pControl = CControlZoom::Create();
				break;
			}

			pUi->m_UiType = type;								// UIタイプ
			pUi->Init(pos, size);								// 初期化処理
			pUi->SetPos(pos, size, false);						// 位置設定
			pUi->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));		// カラー設定
		}
	}
	return pUi;
}



//-------------------------------------------------------------------
// フェードの処理
// Author:村元翼
//-------------------------------------------------------------------
#include "fade.h"
#include "bg.h"
#include "texture.h"
#include "manager.h"

//------------------------------------------------------------------
// 静的メンバ変数宣言
//-------------------------------------------------------------------
CBg							*CFade::m_pBg = nullptr;
CFade::FADE					CFade::m_fade = CFade::FADE_IN;
CManager::MODE				CFade::m_modeNext = CManager::MODE::TITLE;
float						CFade::m_colorAlpha = 1.0f;

//--------------------------------------------
// コンストラクタ
//--------------------------------------------
CFade::CFade(OBJTYPE nPriority) :CScene(nPriority)
{
	
}

//--------------------------------------------
// デストラクタ
//--------------------------------------------
CFade::~CFade()
{

}

//---------------------------------------------------------------
// インスタンス生成処理
//---------------------------------------------------------------
CFade *CFade::Create(CManager::MODE modeNext,D3DXVECTOR3 pos,D3DXVECTOR3 size)
{
	//インスタンス生成
	CFade *pFade = nullptr;

	if (pFade == nullptr)
	{
		// 動的メモリ確保
		pFade = new CFade(OBJTYPE_FADE);

		if (pFade != nullptr)
		{
			pFade->m_modeNext = modeNext;								// 次に移るモード
			pFade->Init(pos,size);										// 初期化
		}
	}
	return pFade;
}

//--------------------------------------------
// 初期化
//--------------------------------------------
HRESULT CFade::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	// オブジェクト生成
	m_pBg = CBg::Create(pos, size, false, CScene::OBJTYPE_FADE);

	return S_OK;
}

//--------------------------------------------
// 終了
//--------------------------------------------
void CFade::Uninit(void)
{
	// オブジェクト終了
	if (m_pBg != nullptr)
	{
		m_pBg->Uninit();
		delete m_pBg;
		m_pBg = nullptr;
	}

	// シーンの破棄
	Release();
}

//--------------------------------------------
// フェードの更新処理
//--------------------------------------------
void CFade::Update(void)
{
	if (m_fade != FADE_NONE)
	{
		// カラー設定
		m_pBg->SetCol(D3DXCOLOR(0.0f, 0.0f, 0.0f, m_colorAlpha));

		// フェード画面を消す
		if (m_fade == FADE_IN)
		{
			m_colorAlpha -= FADE_IN_SPEED;
			if (m_colorAlpha <= 0.0f)
			{
				m_colorAlpha = 0.0f;
				m_fade = FADE_NONE;
			}

		}

		// フェード画面を出す
		else if (m_fade == FADE_OUT)
		{
			m_colorAlpha += FADE_OUT_SPEED;

			if (m_colorAlpha >= 1.0f)
			{
				m_colorAlpha = 1.0f;
				m_fade = FADE_IN;

				//モードの設定
				CManager::SetMode(m_modeNext);
			}
		}
	}
}

//--------------------------------------------
// 描画
//--------------------------------------------
void CFade::Draw(void)
{

}

//--------------------------------------------
//フェードの設定処理
//--------------------------------------------
void CFade::SetFade(CManager::MODE modeNext)
{
	m_fade = FADE_OUT;
	m_modeNext = modeNext;
	m_colorAlpha = 0.0f;
}

//--------------------------------------------
//フェードの取得処理
//--------------------------------------------
CFade::FADE CFade::GetFade(void)
{
	return m_fade;
}

//=============================================
// リザルト画面の処理
// Author:村元翼
//=============================================
#include "result.h"
#include "texture.h"
#include "manager.h"
#include "keyboard.h"
#include "ranking.h"
#include "fade.h"
#include "bg.h"
#include "Ui.h"
#include "XInput.h"
#include "DirectInput.h"

//=============================================
// マクロ定義
//=============================================
#define RETURN_TITLE_TIME (240)

//=============================================
// コンストラクタ
//=============================================
CResult::CResult()
{
	m_nFrame = 0;
	m_nSelectType = 0;
}

//=============================================
// デストラクタ
//=============================================
CResult::~CResult()
{
}

//=============================================
// 初期化
//=============================================
HRESULT CResult::Init(void)
{
	CBg::Create();

	m_pUi = CUi::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, 100.0f, 0.0f), D3DXVECTOR3(600.0f, 300.0f, 0.0f), CUi::UI_TYPE_NONE);
	m_pUi->BindTexture(CManager::GetInstance()->GetTexture()->GetTexture("TEX_TYPE_TITLE"));

	//ランキングの生成
	m_pRanking = CRanking::Create
	(
		D3DXVECTOR3(400.0f + RANKING_POLYGON_WIDTH * MAX_SCORE, RESULT_RANKING_POSY, 0.0f),
		D3DXVECTOR3(RANKING_POLYGON_WIDTH, RANKING_POLYGON_HEIGHT, 0.0f)
	);

	return S_OK;
}
//=============================================
// 終了
//=============================================
void CResult::Uninit(void)
{
	if (m_pUi)
	{
		m_pUi->Uninit();
		m_pUi = nullptr;
	}

	if (m_pRanking)
	{
		m_pRanking->Uninit();
		m_pRanking = nullptr;
	}
}

//=============================================
// 更新
//=============================================
void CResult::Update(void)
{

	if(m_nFrame >= RETURN_TITLE_TIME)
	{
		CManager::SetModeBool(CManager::MODE::TITLE);
	}

	else
	{
		m_nFrame++;
	}
	//if (CManager::SetModeBool(CManager::MODE::TITLE))
	//{

	//}
}

//=============================================
// 描画
//=============================================
void CResult::Draw(void)
{

}


//=============================================================================
//
// 残機カウントのクラス [RemainCount.cpp]
// Author : 村元翼
//
//=============================================================================

//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#include "RemainCount.h"
#include "number.h"
#include "fade.h"
#include "Ui.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define MAX_REMAIN (2)
#define NEXT_NUMBER_SPACE (100)

//=============================================================================
// 静的メンバ変数の初期化
//=============================================================================
int		CRemainCount::m_nRemain = MAX_REMAIN;
bool	CRemainCount::m_bChange = false;

//=============================================================================
// コンストラクタ
//=============================================================================
CRemainCount::CRemainCount(OBJTYPE nPriority) : CScene(nPriority)
{
	memset(m_apNumber, 0, sizeof(m_apNumber));
	m_pos = { 0.0f,0.0f,0.0f };
	m_size = { 0.0f,0.0f,0.0f };
	m_col = { 0.0f,0.0f,0.0f,0.0f };
	m_bFade = false;
	m_bChange = false;
	m_nFrame = 0;
	m_RemainMode = REMAIN::MODE_RETRY;
}

//=============================================================================
//デストラクタ
//=============================================================================
CRemainCount::~CRemainCount()
{

}

//=============================================================================
//初期化処理
//=============================================================================
HRESULT CRemainCount::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	m_pos = pos;
	m_size = size;
	CScene::SetPos(m_pos);
	CScene::SetSize(m_size);

	return S_OK;
}

//=============================================================================
//終了処理
//=============================================================================
void CRemainCount::Uninit(void)
{
	// 2Dポリゴンの破棄
	for (int nCnt = 0; nCnt < MAX_REMAIN_DIGIT; nCnt++)
	{
		if (m_apNumber[nCnt])
		{
			m_apNumber[nCnt]->Uninit();
			m_apNumber[nCnt] = nullptr;
		}
	}

	// オブジェクトの破棄
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CRemainCount::Update(void)
{
	// 残機が0になる(ゲームオーバー)
	if (m_bChange)
	{
		RemainChange();
	}
}

//=============================================================================
//描画処理
//=============================================================================
void CRemainCount::Draw(void)
{
	// 2Dポリゴンの描画
	for (int nCnt = 0; nCnt < MAX_REMAIN_DIGIT; nCnt++)
	{
		if (m_apNumber[nCnt])
		{
			m_apNumber[nCnt]->Draw();
		}
	}
}

//=============================================================================
// 時間の生成
//=============================================================================
CRemainCount *CRemainCount::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	// 時間のインスタンス生成
	CRemainCount *pRemainCount = new CRemainCount;

	// 時間の準備
	if (pRemainCount)
	{
		pRemainCount->Init(pos, size);			// 初期化
	}

	return pRemainCount;
}

//=============================================================================
// 時間の生成
//=============================================================================
void CRemainCount::RemainChange(void)
{
	if (m_nFrame == 0)
	{
		// 変化前の残機
		for (int nCnt = 0; nCnt < MAX_REMAIN_DIGIT; nCnt++)
		{
			if (!m_apNumber[nCnt])
			{
				m_apNumber[nCnt] = CNumber::Create(D3DXVECTOR3(m_pos.x + (m_size.x * nCnt), m_pos.y, 0.0f), D3DXVECTOR3(m_size.x, m_size.y, 0.0f));
				m_apNumber[nCnt]->SetNumber((m_nRemain) % (int)pow(10, MAX_REMAIN_DIGIT - nCnt) / (int)pow(10, (MAX_REMAIN_DIGIT - 1) - nCnt));
			}
		}
		m_nRemain--;	// 残機を減らす
	}

	else if (m_nFrame == GO_RESULT_FRAME)
	{
		// 変化後の残機
		for (int nCnt = 0; nCnt < MAX_REMAIN_DIGIT; nCnt++)
		{
			if (m_apNumber[nCnt])
			{
				m_apNumber[nCnt]->SetNumber(m_nRemain % (int)pow(10, MAX_REMAIN_DIGIT - nCnt) / (int)pow(10, (MAX_REMAIN_DIGIT - 1) - nCnt));
			}
		}
	}

	else if (m_nFrame == GO_RESULT_FRAME * 2)
	{
		// フェードの取得
		CFade *pFade = CManager::GetInstance()->GetFade();

		CManager::GetInstance()->SetStop(true);
		m_bChange = false;

		if (m_nRemain <= 0)
		{
			m_RemainMode = REMAIN::MODE_GAMEOVER;	// ゲームオーバー
		}

		switch (m_RemainMode)
		{
		case REMAIN::MODE_RETRY:	// リトライ
			pFade->SetFade(CManager::MODE::GAME);
			break;

		case REMAIN::MODE_GAMEOVER:	// ゲームオーバー
			pFade->SetFade(CManager::MODE::RESULT);
			break;
		}
	
	}

	m_nFrame++; // フレームを進める
}

//=============================================================================
//
// 時間経過の処理 [timercount.cpp]
// Author : 土居大輝
//
//=============================================================================

//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#include "timercount.h"
#include "number.h"
#include "fade.h"

//=============================================================================
//コンストラクタ
//=============================================================================
CTimerCount::CTimerCount(OBJTYPE nPriority) : CScene(nPriority)
{
	m_bStop = false;
	m_dwCurrentTime = 0;
	// 現在の時間を取得
	m_dwExecLastTime = timeGetTime();
	m_bFade = false;
	m_bLimit = false;
}

//=============================================================================
//デストラクタ
//=============================================================================
CTimerCount::~CTimerCount()
{
}

//=============================================================================
//初期化処理
//=============================================================================
HRESULT CTimerCount::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	m_pos = pos;
	m_size = size;
	CScene::SetPos(m_pos);
	CScene::SetSize(m_size);

	// 数字の生成
	for (int nCntTime = 0; nCntTime < MAX_TIME; nCntTime++)
	{
		m_apNumber[nCntTime] = CNumber::Create(D3DXVECTOR3(m_pos.x + m_size.x + SPACE_TIME * nCntTime, m_pos.y, 0.0f), D3DXVECTOR3(m_size.x, m_size.y, 0.0f));
	}

	return S_OK;
}

//=============================================================================
//終了処理
//=============================================================================
void CTimerCount::Uninit(void)
{
	// 2Dポリゴンの破棄
	for (int nCntTime = 0; nCntTime < MAX_TIME; nCntTime++)
	{
		if (m_apNumber[nCntTime])
		{
			m_apNumber[nCntTime]->Uninit();
			m_apNumber[nCntTime] = nullptr;
		}
	}

	// オブジェクトの破棄
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CTimerCount::Update(void)
{
	if (m_bStop == false)
	{
		// 現在の時間を取得
		m_dwCurrentTime = timeGetTime();

		// フェードの取得
		CFade *pFade = CManager::GetInstance()->GetFade();

		// 時間経過処理（1秒で減らす）
		if ((m_dwCurrentTime - m_dwExecLastTime) >= REDUCE_TIME && m_nTimerCount != 0)
		{
			AddTimerCount(-1);
			m_dwCurrentTime = 0;
			m_dwExecLastTime = timeGetTime();
		}

		// 数字の設定
		for (int nCnt = 0; nCnt < MAX_TIME; nCnt++)
		{
			m_apNumber[nCnt]->SetNumber(m_nTimerCount % (int)pow(10, MAX_TIME - nCnt) / (int)pow(10, (MAX_TIME - 1) - nCnt));
			//m_apNumber[1]->SetNumber(m_nTimerCount % (int)pow(10, 2) / (int)pow(10, 1));
			//m_apNumber[2]->SetNumber(m_nTimerCount % (int)pow(10, 1));
		}
		// 時間切れになったら
		if (m_nTimerCount == 0)
		{
			m_bLimit = true;

			if (!m_bFade)
			{
				// 画面遷移 / フェード処理
				//pFade->SetFade(CManager::MODE::RESULT);
				m_bFade = true;
			}

		}

		if (m_bLimit)
		{

		}

		//if (m_nTime <= 30)
		//{
		//	//サウンド取得
		//	//CSound *pSound = CManager::GetSound();

		//	// 残り時間少なくなると色を変える
		//	m_col.a = 1.0f;
		//	m_col.r = 1.0f;
		//	m_apNumber[0]->SetColor(m_col);
		//	m_apNumber[1]->SetColor(m_col);
		//	m_apNumber[2]->SetColor(m_col);
		//}
		//m_mode = CManager::GetMode();
		//if (m_mode == CManager::MODE_GAME)
		//{
		//	// 時間切れ
		//	if (m_nTime == 0)
		//	{
		//		if (m_bflag == false)
		//		{
		//			CUI::Create(D3DXVECTOR3(SCREEN_CENTER_X, SCREEN_CENTER_Y, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(TIMEUP_WIDTH, TIMEUP_HEIGHT, 0.0f), CTexture::TEXTURETYPE_TIMEUP);
		//			m_bflag = true;
		//		}
		//		m_IntervalTime++;
		//		if (m_IntervalTime >= INTERVAL_TIME)
		//		{
		//			// リザルト行き
		//			if (m_fade == false)
		//			{
		//				pFade->SetFade(CManager::MODE_RESULT);
		//				m_fade = true;
		//				m_bStop = true;
		//			}
		//		}
		//		m_nTime = 0;
		//	}
		//}
	}
}

//=============================================================================
//描画処理
//=============================================================================
void CTimerCount::Draw(void)
{
	// 2Dポリゴンの描画
	for (int nCntTime = 0; nCntTime < MAX_TIME; nCntTime++)
	{
		m_apNumber[nCntTime]->Draw();
	}
}

//=============================================================================
// 時間の加算
//=============================================================================
void CTimerCount::AddTimerCount(int nValue)
{
	m_nTimerCount += nValue;
}
//=============================================================================
// 時間の取得
//=============================================================================
int CTimerCount::GetTimerCount(void)
{
	return m_nTimerCount;
}

//=============================================================================
// 時間の生成
//=============================================================================
CTimerCount *CTimerCount::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, int time, bool bStop)
{
	// 時間のインスタンス生成
	CTimerCount *pTimerCount = new CTimerCount;

	// 時間の準備
	if (pTimerCount != nullptr)
	{
		pTimerCount->m_bStop = bStop;		// 停止するか
		pTimerCount->m_nTimerCount = time;	// 時間
		pTimerCount->Init(pos,size);		// 初期化
	}
	return pTimerCount;
}

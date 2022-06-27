//=============================================================================
// 時間経過の処理 [timercount.cpp]
// Author : 村元翼
//=============================================================================

//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#include "timercount.h"
#include "number.h"
#include "fade.h"

//=============================================================================
// コンストラクタ
//=============================================================================
CTimerCount::CTimerCount(OBJTYPE nPriority) : CScene(nPriority)
{
	m_dwCurrentTime = 0;
	m_dwExecLastTime = timeGetTime();	// 現在の時間を取得
	m_bFade = false;					// フェード判定
	m_bLimit = false;					// 時間切れ判定
	m_bStop = false;					// ストップ判定
}

//=============================================================================
// デストラクタ
//=============================================================================
CTimerCount::~CTimerCount()
{
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CTimerCount::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	// 位置・サイズ設定
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
// 終了処理
//=============================================================================
void CTimerCount::Uninit(void)
{
	// 2Dポリゴンの破棄
	for (int nCnt = 0; nCnt < MAX_TIME; nCnt++)
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
void CTimerCount::Update(void)
{
	// ストップ状態判定
	if (!m_bStop)
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
		}
		// 時間切れになったら
		if (m_nTimerCount == 0)
		{
			m_bLimit = true;

			if (!m_bFade)
			{
				// 画面遷移 / フェード処理
				m_bFade = true;
			}

		}

		if (m_bLimit)
		{

		}
	}
}

//=============================================================================
// 描画処理
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
	CTimerCount *pTimerCount = nullptr;
	if (!pTimerCount)
	{
		pTimerCount = new CTimerCount;

		if (pTimerCount)
		{
			pTimerCount->m_bStop = bStop;		// 停止するか
			pTimerCount->m_nTimerCount = time;	// 時間
			pTimerCount->Init(pos, size);		// 初期化
		}
	}
	return pTimerCount;
}

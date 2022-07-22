//================================================
// スコアの作成
// Author:村元翼
//================================================
#include "manager.h"
#include "keyboard.h"
#include "scene.h"
#include "score.h"
#include "number.h"
#include "playdata.h"

//================================================
// コンストラクタ
//================================================
CScore::CScore(OBJTYPE nPriority) : CScene(nPriority)
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 位置
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// サイズ
	m_nAddScore = ADDSCORE_FORMER;			// 加算するスコアの初期化
	m_nScore = 0;							// スコアの初期化
}

//================================================
// デストラクタ
//================================================
CScore::~CScore()
{

}

//================================================
// スコアの生成
//================================================
CScore *CScore::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	// インスタンス生成
	CScore *pScore = nullptr;
	if (!pScore)
	{
		if (pScore = new CScore)
		{
			pScore->Init(pos, size);	// 初期化
		}
	}

	return pScore;
}

//================================================
// 初期化
//================================================
HRESULT CScore::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	// 数字生成
	for (int nCnt = 0; nCnt < MAX_SCORE; nCnt++)
	{
		m_apNumber[nCnt] = CNumber::Create(D3DXVECTOR3(pos.x + nCnt * size.x, pos.y, pos.z), size);
		m_apNumber[nCnt]->BindTexture(CManager::GetInstance()->GetTexture()->GetTexture("TEX_TYPE_NUMBER"));
		m_apNumber[nCnt]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		m_nAll++;
	}

	// 位置・サイズ設定
	m_pos = pos;
	m_size = size;
	SetPos(m_pos);
	SetSize(m_size);

	return S_OK;
}

//================================================
// 終了
//================================================
void CScore::Uninit()
{
	// ナンバー破棄
	for (int nCnt = 0; nCnt < MAX_SCORE; nCnt++)
	{
		if (m_apNumber[nCnt])
		{
			m_apNumber[nCnt]->Uninit();
			m_apNumber[nCnt] = nullptr;
		}
	}

	m_nAll = 0;
	Release();
}

//================================================
// 更新
//================================================
void CScore::Update()
{
	m_pos = CScene::GetPos();

#if _DEBUG
	CInputkeyboard *pKey = CManager::GetInstance()->GetKeyboard();
	if (pKey->GetTrigger(DIK_0))
	{
		AddScore(m_nAddScore);	// スコアの加算
	}
#endif
	SetScore();
}

//================================================
// 描画
//================================================
void CScore::Draw()
{
	for (int nCnt = 0; nCnt < MAX_SCORE; nCnt++)
	{
		if (m_apNumber[nCnt])
		{
			m_apNumber[nCnt]->Draw();
		}
	}
}

//================================================
// スコアの設定
//================================================
void CScore::SetScore(int nScore)
{
	m_nScore = nScore;
}

//================================================
// スコアの設定
//================================================
void CScore::SetScore()
{
	for (int nCnt = 0, nDecrement = 1; nCnt < m_nAll; nCnt++, nDecrement *= 10)
	{
		m_apNumber[nCnt]->SetNumber(m_nScore % ((int)pow(10, m_nAll) / nDecrement) / ((int)pow(10, m_nAll - 1) / nDecrement));
	}
}

//================================================
// スコア位置設定処理
//================================================
void CScore::SetScorePos(D3DXVECTOR3 pos)
{
	//位置設定
	m_pos = pos;

	for (int nCnt = 0; nCnt < MAX_SCORE; nCnt++)
	{
		m_apNumber[nCnt]->SetPos({ pos.x - (m_size.x * MAX_SCORE / 2) + (m_size.x * nCnt) + (m_size.x / 2.0f), pos.y, 0.0f }, m_size, false);
	}

	//位置を設定
	SetPos(m_pos);
}


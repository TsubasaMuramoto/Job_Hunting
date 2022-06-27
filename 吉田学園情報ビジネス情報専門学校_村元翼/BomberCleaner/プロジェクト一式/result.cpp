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
#include "score.h"
#include "sound.h"

//=============================================
// マクロ定義
//=============================================
#define RETURN_TITLE_TIME (120)									// タイトルに戻るUIが出るまでのフレーム
#define RETURN_TITLE_POS  (D3DXVECTOR3(1150.0f,650.0f,0.0f))	// タイトルに戻るUIの位置
#define RETURN_TITLE_SIZE (D3DXVECTOR3( 450.0f,150.0f,0.0f))	// タイトルに戻るUIのサイズ

//=============================================
// コンストラクタ
//=============================================
CResult::CResult()
{
	m_pReturnTitle = nullptr;
	m_pTitleLogo = nullptr;
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
	// 背景生成
	CBg::Create();

	// タイトルロゴを表示
	m_pTitleLogo = CUi::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, 100.0f, 0.0f), D3DXVECTOR3(800.0f, 200.0f, 0.0f), CUi::UI_TYPE_NONE);
	m_pTitleLogo->BindTexture(CManager::GetInstance()->GetTexture()->GetTexture("TEX_TYPE_TITLE"));

	// ランキングの生成
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
	// タイトルロゴUI破棄
	if (m_pTitleLogo)
	{
		m_pTitleLogo->Uninit();
		m_pTitleLogo = nullptr;
	}

	// タイトルに戻るUIの破棄
	if (m_pReturnTitle)
	{
		m_pReturnTitle->Uninit();
		m_pReturnTitle = nullptr;
	}

	// ランキング破棄
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
	if (m_pRanking)
	{
		m_pRanking->Update();
	}

	// タイトルに戻れるまで少し時間を置く
	if (m_nFrame >= RETURN_TITLE_TIME)
	{
		// タイトルに戻るUIを表示
		if (!m_pReturnTitle)
		{
			m_pReturnTitle = CUi::Create(RETURN_TITLE_POS, RETURN_TITLE_SIZE, CUi::UI_TYPE_BLINK);
			m_pReturnTitle->BindTexture(CManager::GetInstance()->GetTexture()->GetTexture("TEX_TYPE_RETURN_TITLE"));
		}
		if (CManager::SetModeBool(CManager::MODE::TITLE))	// ゲームモード遷移
		{
			CManager::GetInstance()->GetSound()->PlaySound(CSound::SOUND_LABEL_SE_DECIDE);	// 決定音
		}
	}

	else
	{
		m_nFrame++;
	}

	// デバッグ時にRを押すとランキングがリセットされる
#ifdef _DEBUG
	if (CManager::GetInstance()->GetKeyboard()->GetTrigger(DIK_R))
	{
		m_pRanking->DeleteRanking();
	}
#endif
}

//=============================================
// 描画
//=============================================
void CResult::Draw(void)
{
	if (m_pRanking)
	{
		m_pRanking->Draw();
	}
}


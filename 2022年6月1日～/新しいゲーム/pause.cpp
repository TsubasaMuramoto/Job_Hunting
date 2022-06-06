//---------------------------------------------------
// ポーズの処理
//---------------------------------------------------
#include "pause.h"
#include "XInput.h"
#include "keyboard.h"
#include "manager.h"
#include "renderer.h"
#include "scene2D.h"
#include "texture.h"
#include "fade.h"
#include "bg.h"
//#include "sound.h"
#include <assert.h>

//-------------------------------------------
// マクロ定義
//-------------------------------------------
#define PAUSE_INERVAL (100.0f)
#define PAUSE_BG_ALPHA (0.8f)
#define SELECT_HEIGHT (300.0f)
#define SELECT_SIZE (D3DXVECTOR3(350.0f,60.0f,0.0f))
#define CURSOR_SIZE (D3DXVECTOR3(50.0f, 100.0f, 0.0f))

//--------------------------
// 静的メンバ変数の初期化
//----------------------------
CBg *CPause::m_pBg = nullptr;

//--------------------------
// コンストラクタ
//----------------------------
CPause::CPause(OBJTYPE nPriority) :CScene(nPriority)
{
	// メンバ変数の初期化
	memset(m_pPolygon, 0, sizeof(m_pPolygon));
	m_pCursor = nullptr;
	m_pTutorialUI = nullptr;
	m_bNextMode = false;
	m_bUninit = false;
	m_bMoveCursor = true;
	m_nPauseType = 0;
	m_fAlpha = 0.0f;
}

//--------------------------
// デストラクタ
//----------------------------
CPause::~CPause()
{

}

//--------------------------
// 初期化
//----------------------------
HRESULT CPause::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	m_pos = pos;
	CScene::SetPos(m_pos);

	// ポーズ背景の生成とカラーの設定
	m_pBg = CBg::Create({ 0.0f,0.0f,0.0f }, { 0.0f,0.0f,0.0f }, false, CScene::OBJTYPE_PAUSE);
	m_pBg->SetCol(D3DXCOLOR(0.0f, 0.0f, 0.0f, PAUSE_BG_ALPHA));

	// 選択肢のポリゴン生成
	int nType = 0;
	for (int nCnt = 0; nCnt < SELECT_MAX; nCnt++)
	{
		if (!m_pPolygon[nCnt])
		{
			m_pPolygon[nCnt] = CScene2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SELECT_HEIGHT + (PAUSE_INERVAL * nCnt), 0.0f), SELECT_SIZE,OBJTYPE_PAUSE);

			switch (nCnt)
			{
			case SELECT_RETURNGAME:
				m_pPolygon[nCnt]->BindTexture(CManager::GetInstance()->GetTexture()->GetTexture("TEX_TYPE_KOUZI"));
				break;

			case SELECT_RETRY:
				m_pPolygon[nCnt]->BindTexture(CManager::GetInstance()->GetTexture()->GetTexture("TEX_TYPE_KOUZI"));
				break;

			case SELECT_RETURNTITLE:
				m_pPolygon[nCnt]->BindTexture(CManager::GetInstance()->GetTexture()->GetTexture("TEX_TYPE_KOUZI"));
				break;
			}

		}
	}

	// カーソルのポリゴン生成
	if (!m_pCursor)
	{
		m_pCursor = CScene2D::Create(m_pos, CURSOR_SIZE, OBJTYPE_PAUSE);
		m_pCursor->BindTexture(CManager::GetInstance()->GetTexture()->GetTexture("TEX_TYPE_KOUZI"));
	}

	return S_OK;
}

//--------------------------
// 終了
//----------------------------
void CPause::Uninit(void)
{
	for (int nCnt = 0; nCnt < SELECT_MAX; nCnt++)
	{
		if (m_pPolygon[nCnt])
		{
			m_pPolygon[nCnt]->Uninit();
			m_pPolygon[nCnt] = nullptr;
		}
	}

	if (m_pCursor)
	{
		m_pCursor->Uninit();
		m_pCursor = nullptr;
	}

	// ポーズ背景の破棄
	if (m_pBg)
	{
		m_pBg->Uninit();
		m_pBg = nullptr;
	}

	CManager::GetInstance()->SetPause(false);	// ポーズの状態をfalseにする
	CManager::GetInstance()->SetStop(false);	// ポーズの状態をfalseにする
	Release();									// オブジェクトの破棄
}

//--------------------------
// 更新
//--------------------------
void CPause::Update(void)
{
	// オブジェクト取得
	CXInput *pXInput = CManager::GetInstance()->GetXInput();
	CInputkeyboard *pKey = CManager::GetInstance()->GetKeyboard();
	//CSound *pSound = CManager::GetInstance()->GetSound();

	// 上に行く
	if (pKey->GetTrigger(DIK_W) || pXInput->GetButtonTrigger(XINPUT_GAMEPAD_DPAD_UP))
	{
		// ポーズのタイプをずらす
		m_nPauseType--;
		if (m_nPauseType < 0)
		{
			m_nPauseType = SELECT_MAX - 1;
		}
		m_bMoveCursor = true;
	}

	// 下に行く
	if (pKey->GetTrigger(DIK_S) || pXInput->GetButtonTrigger(XINPUT_GAMEPAD_DPAD_DOWN))
	{
		// ポーズのタイプをずらす
		m_nPauseType++;
		if (m_nPauseType >= SELECT_MAX)
		{
			m_nPauseType = 0;
		}
		m_bMoveCursor = true;
	}

	// カーソルの位置を変える
	if (m_bMoveCursor)
	{
		if (m_pCursor)
		{
			//pSound->PlaySound(pSound->SOUND_LABEL_SE_MOVE_CURSOR);

			switch (m_nPauseType)
			{
			case SELECT_RETURNGAME:		// ゲーム画面に戻る
				m_pCursor->SetPos(D3DXVECTOR3(m_pPolygon[0]->GetPos().x - m_pPolygon[0]->GetSize().x, m_pPolygon[0]->GetPos().y, 0.0f), m_pCursor->GetSize());
				break;

			case SELECT_RETRY:			// リトライ
				m_pCursor->SetPos(D3DXVECTOR3(m_pPolygon[1]->GetPos().x - m_pPolygon[1]->GetSize().x, m_pPolygon[1]->GetPos().y, 0.0f), m_pCursor->GetSize());
				break;

			case SELECT_RETURNTITLE:	// タイトルに戻る
				m_pCursor->SetPos(D3DXVECTOR3(m_pPolygon[2]->GetPos().x - m_pPolygon[2]->GetSize().x, m_pPolygon[2]->GetPos().y, 0.0f), m_pCursor->GetSize());
				break;
			}

			m_bMoveCursor = false;
		}
	}

	// 選択を決定する
	if ((pKey->GetTrigger(DIK_RETURN) || pXInput->GetButtonTrigger(XINPUT_GAMEPAD_A)) && !m_bNextMode)
	{
		//pSound->PlaySound(pSound->SOUND_LABEL_SE_DECIDE);

		switch (m_nPauseType)
		{
		case SELECT_RETURNGAME:		// ゲームに戻る
			CManager::GetInstance()->SetPause(false);
			CManager::GetInstance()->SetStop(false);
			break;
		case SELECT_RETRY:			// リトライ
			CFade::SetFade(CManager::MODE::GAME);
			break;
		case SELECT_RETURNTITLE:	// タイトル画面
			CFade::SetFade(CManager::MODE::TITLE);
			break;
		}
		m_bNextMode = true;
	}
}

//--------------------------
// 描画
//----------------------------
void CPause::Draw(void)
{

}

//---------------------------------------------------------------
// インスタンス生成処理
//---------------------------------------------------------------
CPause *CPause::Create(D3DXVECTOR3 pos)
{
	// インスタンス生成
	CPause *pPause = nullptr;
	if (!pPause)
	{
		pPause = new CPause(OBJTYPE_PAUSE);			// メモリ確保
		if (pPause)
		{
			pPause->Init(pos, {0.0f,0.0f,0.0f});	// 初期化処理
		}
	}

	return pPause;
}

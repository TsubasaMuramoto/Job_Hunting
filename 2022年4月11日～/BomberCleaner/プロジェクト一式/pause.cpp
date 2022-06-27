//===================================================
// ポーズの処理
// Author:村元翼
//===================================================
#include "pause.h"
#include "XInput.h"
#include "keyboard.h"
#include "manager.h"
#include "renderer.h"
#include "scene2D.h"
#include "texture.h"
#include "fade.h"
#include "bg.h"
#include <assert.h>

//===================================================
// マクロ定義
//===================================================
#define PAUSE_INERVAL (100.0f)							// ポーズインターバル
#define PAUSE_BG_ALPHA (0.8f)							// ポーズ背景のα値
#define SELECT_HEIGHT (300.0f)							// 選択肢の高さ
#define SELECT_SIZE (D3DXVECTOR3(300.0f,120.0f,0.0f))	// 選択肢のサイズ
#define CURSOR_SIZE (D3DXVECTOR3(50.0f, 80.0f, 0.0f))	// カーソルの高さ

//===================================================
// 静的メンバ変数の初期化
//===================================================
CBg *CPause::m_pBg = nullptr;	// 背景クラス

//===================================================
// コンストラクタ
//===================================================
CPause::CPause(OBJTYPE nPriority) :CScene(nPriority)
{
	// メンバ変数の初期化
	memset(m_pSelect, 0, sizeof(m_pSelect));	// ポリゴンクラス
	m_pCursor			= nullptr;				// カーソル(ポリゴン)クラス
	m_pTutorialUI		= nullptr;				// UIクラス
	m_bNextMode			= false;				// 次のモードに行く時の判定
	m_bUninit			= false;				// 終了判定
	m_bMoveCursor		= true;					// カーソル移動判定
	m_nPauseType		= 0;					// ポーズ選択状態を管理する変数
	m_fAlpha			= 0.0f;					// α値をいじるための変数
}

//===================================================
// デストラクタ
//===================================================
CPause::~CPause()
{

}

//===================================================
// 初期化
//===================================================
HRESULT CPause::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	// 位置設定
	m_pos = pos;
	CScene::SetPos(m_pos);

	// ポーズ背景の生成とカラーの設定
	m_pBg = CBg::Create({ 0.0f,0.0f,0.0f }, { 0.0f,0.0f,0.0f }, false, CScene::OBJTYPE_PAUSE);
	m_pBg->SetCol(D3DXCOLOR(0.0f, 0.0f, 0.0f, PAUSE_BG_ALPHA));

	// テクスチャクラス取得
	CTexture *pTex = CManager::GetInstance()->GetTexture();

	// 選択肢のポリゴン生成
	int nType = 0;
	for (int nCnt = 0; nCnt < SELECT_MAX; nCnt++)
	{
		if (!m_pSelect[nCnt])
		{
			m_pSelect[nCnt] = CScene2D::Create
			(
				D3DXVECTOR3(SCREEN_WIDTH / 2.0f, 
				SELECT_HEIGHT + (PAUSE_INERVAL * nCnt), 0.0f), 
				SELECT_SIZE,OBJTYPE_PAUSE
			);

			// テクスチャ取得
			switch (nCnt)
			{
			case SELECT_RETURNGAME:
				m_pSelect[nCnt]->BindTexture(pTex->GetTexture("TEX_TYPE_PAUSE01"));
				break;

			case SELECT_RETRY:
				m_pSelect[nCnt]->BindTexture(pTex->GetTexture("TEX_TYPE_PAUSE02"));
				break;

			case SELECT_RETURNTITLE:
				m_pSelect[nCnt]->BindTexture(pTex->GetTexture("TEX_TYPE_PAUSE03"));
				break;
			}

		}
	}

	// カーソルのポリゴン生成
	if (!m_pCursor)
	{
		m_pCursor = CScene2D::Create(m_pos, CURSOR_SIZE, OBJTYPE_PAUSE);
		m_pCursor->BindTexture(pTex->GetTexture("TEX_TYPE_PAUSE_CURSOR"));
	}

	return S_OK;
}

//===================================================
// 終了
//===================================================
void CPause::Uninit(void)
{
	// 選択肢の破棄
	for (int nCnt = 0; nCnt < SELECT_MAX; nCnt++)
	{
		if (m_pSelect[nCnt])
		{
			m_pSelect[nCnt]->Uninit();
			m_pSelect[nCnt] = nullptr;
		}
	}

	// カーソル破棄
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

//===================================================
// 更新
//===================================================
void CPause::Update(void)
{
	if (!m_bNextMode)
	{
		// 入力系クラス取得
		CXInput *pXInput = CManager::GetInstance()->GetXInput();
		CInputkeyboard *pKey = CManager::GetInstance()->GetKeyboard();

		// 選択を上にずらす
		if (CInput::PressAnyAction(CInput::ACTION_UP))
		{
			// ポーズのタイプをずらす
			m_nPauseType--;
			if (m_nPauseType < 0)
			{
				m_nPauseType = SELECT_MAX - 1;
			}
			m_bMoveCursor = true;
		}

		// 選択を下にずらす
		if (CInput::PressAnyAction(CInput::ACTION_DOWN))
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
				switch (m_nPauseType)
				{
				case SELECT_RETURNGAME:		// ゲーム画面に戻る
					m_pCursor->SetPos(D3DXVECTOR3(m_pSelect[0]->GetPos().x - (m_pSelect[0]->GetSize().x), m_pSelect[0]->GetPos().y, 0.0f), m_pCursor->GetSize());
					break;

				case SELECT_RETRY:			// リトライ
					m_pCursor->SetPos(D3DXVECTOR3(m_pSelect[1]->GetPos().x - (m_pSelect[1]->GetSize().x), m_pSelect[1]->GetPos().y, 0.0f), m_pCursor->GetSize());
					break;

				case SELECT_RETURNTITLE:	// タイトルに戻る
					m_pCursor->SetPos(D3DXVECTOR3(m_pSelect[2]->GetPos().x - (m_pSelect[2]->GetSize().x), m_pSelect[2]->GetPos().y, 0.0f), m_pCursor->GetSize());
					break;
				}

				m_bMoveCursor = false;
			}
		}

		// 選択を決定する
		if ((pKey->GetTrigger(DIK_RETURN) || pXInput->GetButtonTrigger(XINPUT_GAMEPAD_A)))
		{
			switch (m_nPauseType)
			{
			case SELECT_RETURNGAME:		// ゲームに戻る
				CManager::GetInstance()->SetPause(false);	// オブジェクト停止状態をfalseにする
				CManager::GetInstance()->SetStop(false);	// ポーズ破棄をONにする
				break;
			case SELECT_RETRY:			// リトライ
				CFade::SetFade(CManager::MODE::GAME);
				break;
			case SELECT_RETURNTITLE:	// タイトル画面
				CFade::SetFade(CManager::MODE::TITLE);
				break;
			}
			m_bNextMode = true;		// ループ防止のためにtrueにする
		}
	}
}

//===================================================
// 描画
//===================================================
void CPause::Draw(void)
{

}

//===================================================
// インスタンス生成処理
//===================================================
CPause *CPause::Create(D3DXVECTOR3 pos)
{
	// インスタンス生成
	CPause *pPause = nullptr;
	if (!pPause)
	{
		pPause = new CPause(OBJTYPE_PAUSE);			// メモリ確保
		if (pPause)
		{
			pPause->Init(pos, {0.0f,0.0f,0.0f});	// 初期化
		}
	}

	return pPause;
}

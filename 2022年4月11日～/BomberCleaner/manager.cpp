//================================================
// manager.cppの作成
// Author:村元翼
//================================================
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "camera.h"
#include "light.h"
#include "texture.h"
#include "keyboard.h"
#include "title.h"
#include "game.h"
#include "result.h"
#include "fade.h"
#include "XInput.h"
#include "DirectInput.h"
#include "playdata.h"
#include "LoadX.h"
#include "pause.h"

//================================================
// 静的メンバ変数の初期化
//================================================
CManager*		CManager::s_pManager				= nullptr;		// シングルトンマネージャ
CRenderer*		CManager::m_pRenderer				= nullptr;		// レンダラー
CCamera*		CManager::m_apCamera[MAX_CAMERA]	= { nullptr };	// カメラ
CLight*			CManager::m_pLight[MAX_LIGHT]		= { nullptr };	// ライト
CTexture*		CManager::m_pTexture				= nullptr;		// テクスチャ
CInputkeyboard* CManager::m_pInputKeyboard			= nullptr;		// キーボード
CXInput*		CManager::m_pXInput					= nullptr;		// XInput
CDInput*		CManager::m_pDInput					= nullptr;		// DInput
CTitle*			CManager::m_pTitle					= nullptr;		// タイトル
CGame*			CManager::m_pGame					= nullptr;		// チュートリアル
CResult*		CManager::m_pResult					= nullptr;		// リザルト
CPause*			CManager::m_pPause					= nullptr;		// ポーズ
CFade*			CManager::m_pFade					= nullptr;		// フェード
CPlayData*		CManager::m_pPlayData				= nullptr;		// プレイデータ
CLoadX*			CManager::m_pLoadX					= nullptr;		// X読み込みクラス
CManager::MODE	CManager::m_mode					= MODE::GAME;	// モード
bool			CManager::m_bOnlyOnce				= false;
bool			CManager::m_bStop = false;
bool			CManager::m_bPause = false;
HWND			CManager::m_hWnd = nullptr;

//================================================
// コンストラクタ
//================================================
CManager::CManager()
{

}

//================================================
// デストラクタ
//================================================
CManager::~CManager()
{

}

//================================================
// マネージャ初期化
//================================================
HRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd, bool bReturn)
{
	m_hWnd = hWnd;

	// レンダラーの生成
	if (!m_pRenderer)
	{
		m_pRenderer = new CRenderer;
		if (m_pRenderer)
		{
			m_pRenderer->Init(hWnd, bReturn);
		}
	}

	//プレイデータクラスの生成
	if (!m_pPlayData)
	{
		m_pPlayData = new CPlayData;

		if (m_pPlayData)
		{
			m_pPlayData->Init();
		}
	}

	// X読み込みクラスの生成
	if (!m_pLoadX)
	{
		m_pLoadX = new CLoadX;

		if (m_pLoadX)
		{
			m_pLoadX->Init();
		}
	}

	// キーボードの生成
	if (!m_pInputKeyboard)
	{
		m_pInputKeyboard = new CInputkeyboard;
		if (m_pInputKeyboard)
		{
			m_pInputKeyboard->Init(hInstance, hWnd);
		}
	}

	// Xinput生成
	if (!m_pXInput)
	{
		m_pXInput = new CXInput;
	}

	// Dinput生成
	if (!m_pDInput)
	{
		m_pDInput = new CDInput;
		if (m_pDInput)
		{
			m_pDInput->Init(hInstance, hWnd);
		}
	}

	// ライトの生成
	for (int nCnt = 0; nCnt < MAX_LIGHT; nCnt++)
	{
		if (!m_pLight[nCnt])
		{
			m_pLight[nCnt] = new CLight;
		}
	}

	m_pLight[0]->Init({ 0.0f, 0.0f, 0.0f	},	{ 1.0f, 1.0f, 1.0f, 1.0f }, { 0.2f, -0.8f, -0.4f });
	m_pLight[1]->Init({ 0.0f, 0.0f, 0.0f	},	{ 1.0f, 1.0f, 1.0f, 1.0f }, { -0.2f, -0.8f, 0.4f });
	m_pLight[2]->Init({ 0.0f, 0.0f, 0.0f	},	{ 1.0f, 1.0f, 1.0f, 1.0f }, { 0.0f, 0.4f, 0.0f	});
	m_pLight[3]->Init({ 0.0f, 0.0f, -100.0f },	{ 1.0f, 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, -1.0f });

	// テクスチャの生成
	if (!m_pTexture)
	{
		m_pTexture = new CTexture;
		if (m_pTexture)
		{
			m_pTexture->Init();
		}
	}

	// フェードの生成
	if (!m_pFade)
	{
		m_pFade = CFade::Create(m_mode);
	}

	// モードの設定
	SetMode(m_mode);

	return S_OK;
}

//================================================
// マネージャ終了
//================================================
void CManager::Uninit(void)
{
	//-----------------------------------------------
	// 全ての終了処理
	//-----------------------------------------------
	CScene::ReleaseAll();

	//*******************************//
	//  各種オブジェクトの終了処理   //
	//*******************************//
	// テクスチャの破棄
	if (m_pTexture)
	{
		m_pTexture->Uninit();
		delete m_pTexture;
		m_pTexture = nullptr;
	}

	// フェードの破棄
	if (m_pFade)
	{
		m_pFade->Uninit();
		delete m_pFade;
		m_pFade = nullptr;
	}

	// ライト破棄
	for (int nCnt = 0; nCnt < MAX_LIGHT; nCnt++)
	{
		if (m_pLight[nCnt])
		{
			delete m_pLight[nCnt];
			m_pLight[nCnt] = nullptr;
		}
	}

	// カメラの破棄
	for (int nCnt = 0; nCnt < MAX_CAMERA; nCnt++)
	{
		if (m_apCamera[nCnt])
		{
			m_apCamera[nCnt]->Uninit();
			delete m_apCamera[nCnt];
			m_apCamera[nCnt] = nullptr;
		}
	}

	// レンダラー破棄
	if (m_pRenderer)
	{
		m_pRenderer->Uninit();
		delete m_pRenderer;
		m_pRenderer = nullptr;
	}

	// キーボードの破棄
	if (m_pInputKeyboard)
	{
		m_pInputKeyboard->Uninit();
		delete m_pInputKeyboard;
		m_pInputKeyboard = nullptr;
	}

	// DInputの破棄
	if (m_pDInput)
	{
		m_pDInput->Uninit();
		delete m_pDInput;
		m_pDInput = nullptr;
	}

	// プレイデータの破棄
	if (m_pPlayData)
	{
		m_pPlayData->Uninit();
		delete m_pPlayData;
		m_pPlayData = nullptr;
	}

	// X読み込みクラスの破棄
	if (m_pLoadX)
	{
		m_pLoadX->Uninit();
		delete m_pLoadX;
		m_pLoadX = nullptr;
	}

	// シングルトンマネージャの破棄
	if (s_pManager)
	{
		delete s_pManager;
		s_pManager = nullptr;
	}
}

//================================================
// マネージャ更新
//================================================
void CManager::Update(void)
{
	// レンダラーの更新
	if (m_pRenderer)
	{
		m_pRenderer->Update();
	}

	//キーボードの更新
	if (m_pInputKeyboard)
	{
		m_pInputKeyboard->Update();
	}

	// ゲームパッドの更新
	if (m_pXInput)
	{
		m_pXInput->UpdateGamepad();
	}

	// DInputの更新
	if (m_pDInput)
	{
		m_pDInput->Update();
	}

	// ストップ中は入力無効
	if (!m_bStop && !m_bPause)
	{
		// カメラの更新処理
		for (int nCnt = 0; nCnt < MAX_CAMERA; nCnt++)
		{
			if (m_apCamera[nCnt])
			{
				m_apCamera[nCnt]->Update();
			}
		}
	}

	//*******************************//
	//		　モード更新処理		 //
	//*******************************//
	switch (m_mode)
	{
	case MODE::TITLE:		// タイトル
		if (m_pTitle)
		{
			m_pTitle->Update();
		}
		break;

	case MODE::GAME:		// ゲーム
		if (m_pGame)
		{
			m_pGame->Update();

			if (!m_bStop)
			{
				// ポーズ切り替え(Pを押す)
				if (m_pInputKeyboard->GetTrigger(DIK_P) || m_pXInput->GetButtonTrigger(XINPUT_GAMEPAD_START))
				{
					m_bPause = !m_bPause;		// ポーズを切り替える

					if (m_bPause)
					{
						// ポーズの生成
						if (!m_pPause)
						{
							m_pPause = CPause::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
						}
					}

					else
					{
						// ポーズの生成
						if (m_pPause)
						{
							// deleteはSceneで行う
							m_pPause->Uninit();
							m_pPause = nullptr;
						}
					}
				}
			}
		}
		break;

	case MODE::RESULT:	// リザルト
		if (m_pResult)
		{
			m_pResult->Update();
		}
		break;
	}
}

//================================================
// マネージャ描画
//================================================
void CManager::Draw(void)
{
	// レンダラー描画
	if (m_pRenderer)
	{
		m_pRenderer->Draw();
	}

	// カメラ描画
	for (int nCnt = 0; nCnt < MAX_CAMERA; nCnt++)
	{
		if (m_apCamera[nCnt])
		{
			m_apCamera[nCnt]->SetCamera();
		}
	}
}

//================================================
// インスタンス取得処理
//================================================
CManager *CManager::GetInstance(void)
{
	if (!s_pManager)
	{
		s_pManager = new CManager;
	}
	return s_pManager;
}

//================================================
// モードのセット
//================================================
void CManager::SetMode(MODE mode)
{
	//-----------------------------------------------
	// 現在のモードを破棄(UninitはScene.cppで行う)
	//-----------------------------------------------
	switch (m_mode)
	{
	case MODE::TITLE:		// タイトル
		if (m_pTitle)
		{
			m_pTitle->Uninit();
			delete m_pTitle;
			m_pTitle = nullptr;
		}
		break;

	case MODE::GAME:		// ゲーム
		if (m_pGame)
		{
			// カメラの破棄
			for (int nCntCamera = 0; nCntCamera < MAX_CAMERA; nCntCamera++)
			{
				if (m_apCamera[nCntCamera])
				{
					m_apCamera[nCntCamera]->Uninit();
					delete m_apCamera[nCntCamera];
					m_apCamera[nCntCamera] = nullptr;
				}
			}

			// ポーズの破棄
			if (m_pPause)
			{
				m_pPause->Uninit();
				m_pPause = nullptr;
			}

			m_pGame->Uninit();
			delete m_pGame;
			m_pGame = nullptr;
		}
		break;

	case MODE::RESULT:		// リザルト
		if (m_pResult)
		{
			m_pResult->Uninit();
			delete m_pResult;
			m_pResult = nullptr;
		}
		break;
	}

	//----------------------------------
	// 全てのオブジェクトの破棄
	//----------------------------------
	CScene::ReleaseAll();

	//----------------------------------
	// 次のモードにする
	//----------------------------------
	switch (mode)
	{
	case MODE::TITLE:		// タイトル
		if (!m_pTitle)
		{
			m_pTitle = new CTitle;
			if (m_pTitle)
			{
				m_pTitle->Init();
			}
		}
		break;

	case MODE::GAME:		// ゲーム
		if (!m_pGame)
		{
			m_pGame = new CGame;
			if (m_pGame)
			{
				//カメラの生成
				for (int nCnt = 0; nCnt < MAX_CAMERA; nCnt++)
				{
					m_apCamera[nCnt] = CCamera::Create(
										D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(D3DX_PI*-0.3f, 0.0f, 0.0f),
										(float)(SCREEN_WIDTH / MAX_CAMERA * nCnt), 0.0f,
										(float)(SCREEN_WIDTH / MAX_CAMERA), (float)SCREEN_HEIGHT, nCnt);
				}
				m_pGame->Init();
			}
		}
		break;

	case MODE::RESULT:		// リザルト
		if (!m_pResult)
		{
			m_pResult = new CResult;
			if (m_pResult)
			{
				m_pResult->Init();
			}
		}
		break;
	}

	// 次のモードを現在のモードにする
	m_mode = mode;
	m_bOnlyOnce = false;
}

//================================================
// モードのセット判定
//================================================
bool CManager::SetModeBool(MODE modeNext)
{
	//=========================================================
	// 画面遷移(ENTERまたはSTARTボタンを押す)
	//=========================================================
	if ((CInput::PressAnyAction(CInput::ACTION_ENTER) || CInput::PressAnyAction(CInput::ACTION_ATTACK)) && !m_bOnlyOnce)
	{
		m_pFade->SetFade(modeNext);						// ゲームモードへ
		m_bOnlyOnce = true;								// ENTER連打防止
		return true;
	}

	return false;
}

//=========================================================
//ゲーム画面の処理
//=========================================================
#include "game.h"
#include "fade.h"
#include "manager.h"
#include "meshfield.h"
#include "meshcylinder.h"
#include "meshsphere.h"
#include "timercount.h"
#include "stage.h"
#include "score.h"
#include "Scene3D.h"
#include "keyboard.h"
#include "XInput.h"
#include "DirectInput.h"
#include "Bomb.h"
#include "BombSpawner.h"
#include "LoadX.h"
#include "Ui.h"
#include "sound.h"

//=========================================================
// マクロ定義
//=========================================================
#define COUNT_DOWN_FRAME	(60)							// カウントダウンのフレーム
#define UI_SIZE				(D3DXVECTOR3(1.0f,1.0f,0.0f))	// UIサイズ
#define FADE_FRAME			(90)							// 終了してからリザルトにフェードするまでのフレーム

//=========================================================
// 静的メンバ変数の初期化
//=========================================================
CScore *CGame::m_pScore = nullptr;

//=========================================================
// コンストラクタ
//=========================================================
CGame::CGame()
{
	m_nFrame			= 0;		// フレーム
	m_pTimer			= nullptr;	// タイマークラス
	m_pMeshCylinder		= nullptr;	// メッシュ筒
	m_pMeshField		= nullptr;	// メッシュ床
	m_pMeshSphere		= nullptr;	// メッシュ球
	m_pMeshWall			= nullptr;	// メッシュ壁
	m_pStage			= nullptr;	// ステージクラス
	m_pUI				= nullptr;	// UIクラス
	m_bOnce				= false;	// 一度のみ通る変数
	m_bStart			= false;	// スタート判定変数
}

//=========================================================
// デストラクタ
//=========================================================
CGame::~CGame()
{

}

//=========================================================
// 初期化
//=========================================================
HRESULT CGame::Init(void)
{

#ifdef _DEBUG
	CBombSpawner::Create({ 800.0f,0.0f,0.0f }, { 0.0f,0.0f,0.0f }, { 1.0f,1.0f,1.0f }, 5);

	CBomb::Create({ 0.0f,	10.0f, -300.0f	},	{ 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }, CManager::GetInstance()->GetLoadX()->GetNum("MODTYPE_BOMB"));
	CBomb::Create({ 100.0f, 10.0f, -300.0f	},	{ 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }, CManager::GetInstance()->GetLoadX()->GetNum("MODTYPE_BOMB"));
	CBomb::Create({ -100.0f,10.0f, -300.0f	},	{ 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }, CManager::GetInstance()->GetLoadX()->GetNum("MODTYPE_BOMB"));
	CBomb::Create({ 0.0f,	10.0f, -400.0f	},	{ 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }, CManager::GetInstance()->GetLoadX()->GetNum("MODTYPE_BOMB"));
	CBomb::Create({ 100.0f, 10.0f, -400.0f	},	{ 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }, CManager::GetInstance()->GetLoadX()->GetNum("MODTYPE_BOMB"));
	CBomb::Create({ -100.0f,10.0f, -400.0f	},	{ 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }, CManager::GetInstance()->GetLoadX()->GetNum("MODTYPE_BOMB"));
#endif

	// ステージ生成
	if (!m_pStage)
	{
		m_pStage = new CStage;
		if (m_pStage)
		{
			m_pStage->Init();									// 初期化
			m_pStage->SetStage("data/TEXT/StageSet000.txt");	// モデル設定のファイル名
		}
	}

// タイマーカウント生成
#ifdef _DEBUG
	m_pTimer = CTimerCount::Create(D3DXVECTOR3(SCREEN_WIDTH - 250.0f, 50.0f, 0.0f), D3DXVECTOR3(50.0f, 30.0f, 0.0f), 10, false);

	m_pMeshField = CMeshField::Create({ 100.0f,100.0f,100.0f }, { 300.0f,0.0f,300.0f }, { 40.0f,0.0f,0.0f }, 6, 6);
	m_pMeshField->BindTexture(CManager::GetInstance()->GetTexture()->GetTexture("TEX_TYPE_CAUTION"));
#else
	m_pTimer = CTimerCount::Create(D3DXVECTOR3(SCREEN_WIDTH - 250.0f, 50.0f, 0.0f), D3DXVECTOR3(50.0f, 30.0f, 0.0f), 60, false);
#endif

	// スコア生成
	m_pScore = CScore::Create(D3DXVECTOR3(SCREEN_WIDTH - 300.0f, SCREEN_HEIGHT - 50.0f, 0.0f), D3DXVECTOR3(50.0f, 30.0f, 0.0f));

	return S_OK;
}

//=========================================================
// 終了
//=========================================================
void CGame::Uninit(void)
{
	// ストップ解除
	if (CManager::GetInstance()->GetStop())
	{
		CManager::GetInstance()->SetStop(false);
	}
}

//=========================================================
// 更新
//=========================================================
void CGame::Update(void)
{
	CTexture *pTex = CManager::GetInstance()->GetTexture();
	CSound *pSound = CManager::GetInstance()->GetSound();

	//----------------------------------------------------
	// 開始カウントダウン
	//----------------------------------------------------
#ifndef _DEBUG
	if (!m_bStart)
	{
		// フレーム0の時
		if (m_nFrame == 0)
		{
			CManager::GetInstance()->SetStop(true);
		}

		m_nFrame++;		// フレームカウント

		// カウント３
		if (m_nFrame == COUNT_DOWN_FRAME) 
		{
			m_pUI = CUi::Create(SCREEN_CENTER, UI_SIZE, CUi::UI_TYPE_ZOOM);
			m_pUI->BindTexture(pTex->GetTexture("TEX_TYPE_NUMBER_THREE"));

			pSound->PlaySound(CSound::SOUND_LABEL_SE_COUNT);	// カウント音
		}

		// カウント２
		else if (m_nFrame == COUNT_DOWN_FRAME * 2) 
		{
			SetUiDelete();
			m_pUI = CUi::Create(SCREEN_CENTER, UI_SIZE, CUi::UI_TYPE_ZOOM);
			m_pUI->BindTexture(pTex->GetTexture("TEX_TYPE_NUMBER_TWO"));

			pSound->PlaySound(CSound::SOUND_LABEL_SE_COUNT);	// カウント音
		}

		// カウント１
		else if (m_nFrame == COUNT_DOWN_FRAME * 3) 
		{
			SetUiDelete();
			m_pUI = CUi::Create(SCREEN_CENTER, UI_SIZE, CUi::UI_TYPE_ZOOM);
			m_pUI->BindTexture(pTex->GetTexture("TEX_TYPE_NUMBER_ONE"));

			pSound->PlaySound(CSound::SOUND_LABEL_SE_COUNT);	// カウント音

		}

		// カウント０
		else if (m_nFrame == COUNT_DOWN_FRAME * 4) 
		{
			SetUiDelete();
			m_pUI = CUi::Create(SCREEN_CENTER, {UI_SIZE.x * 3,UI_SIZE.y,0.0f}, CUi::UI_TYPE_ZOOM);
			m_pUI->BindTexture(pTex->GetTexture("TEX_TYPE_GAMESTART"));

			pSound->PlaySound(CSound::SOUND_LABEL_SE_GAMESTART);	// スタート音
		}

		// ゲームスタート
		else if (m_nFrame == COUNT_DOWN_FRAME * 5)
		{
			SetUiDelete();
			m_nFrame = 0;
			m_bStart = true;
			CManager::GetInstance()->SetStop(false);
		}
	}
#endif

	//----------------------------------------------------
	// 終了サイン
	//----------------------------------------------------
	if (m_pTimer->GetLimit())
	{
		if (!m_bOnce)
		{
			m_pUI = CUi::Create(SCREEN_CENTER, { UI_SIZE.x * 3,UI_SIZE.y,0.0f }, CUi::UI_TYPE_ZOOM);
			m_pUI->BindTexture(pTex->GetTexture("TEX_TYPE_END"));
			CManager::GetInstance()->SetStop(true);
			m_bOnce = true;

			pSound->PlaySound(CSound::SOUND_LABEL_SE_TIMEUP);	// 終了音
		}

		m_nFrame++;	// フレームカウント

		if (m_nFrame == FADE_FRAME)
		{
			SetUiDelete();
			CManager::GetInstance()->GetFade()->SetFade(CManager::MODE::RESULT);
		}
	}

}
//=========================================================
// 描画
//=========================================================
void CGame::Draw(void)
{

}

//=========================================================
// UI削除関数
//=========================================================
void CGame::SetUiDelete(void)
{
	if (m_pUI)
	{
		m_pUI->Uninit();
		delete m_pUI;
		m_pUI = nullptr;
	}
}






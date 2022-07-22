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
#include "playdata.h"
#include "StringManager.h"

//=========================================================
// マクロ定義
//=========================================================
#define COUNT_DOWN_FRAME	(60)										// カウントダウンのフレーム
#define SKY_SIZE			(D3DXVECTOR3(10000.0f,10000.0f,10000.0f))	// 空サイズ
#define UI_SIZE				(D3DXVECTOR3(1.0f,1.0f,0.0f))				// UIサイズ
#define NUMBER_SIZE			(D3DXVECTOR3(50.0f, 30.0f, 0.0f))			// スコアサイズ
#define FADE_FRAME			(90)										// 終了してからリザルトにフェードするまでのフレーム
#define GAME_TIME			(60)										// ゲームタイム

//=========================================================
// 静的メンバ変数の初期化
//=========================================================

//=========================================================
// コンストラクタ
//=========================================================
CGame::CGame()
{
	m_nFrame			= 0;		// フレーム
	m_pScore			= nullptr;	// スコアクラス
	m_pAddScore			= nullptr;	// 追加スコアクラス
	m_pTimer			= nullptr;	// タイマークラス
	m_pMeshCylinder		= nullptr;	// メッシュ筒
	m_pMeshField		= nullptr;	// メッシュ床
	m_pMeshSphere		= nullptr;	// メッシュ球
	m_pMeshWall			= nullptr;	// メッシュ壁
	m_pStage			= nullptr;	// ステージクラス
	m_pUI				= nullptr;	// UIクラス
	m_bOnce				= false;	// 一度のみ通る変数
	m_bStart			= false;	// スタート判定変数
	m_bEnd				= false;	// エンド判定変数
	m_bFrameCount		= false;	// フレームカウント変数
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
	// 空生成
	if (!m_pMeshSphere)
	{
		m_pMeshSphere = CMeshSphere::Create({ 0.0f,0.0f,0.0f }, SKY_SIZE, { 0.0f,0.0f,0.0f }, 10, 10, true, CScene::OBJTYPE_MESH);
		m_pMeshSphere->BindTexture(CManager::GetInstance()->GetTexture()->GetTexture("TEX_TYPE_SKY"));
	}

	// ステージ生成
	if (!m_pStage)
	{
		m_pStage = new CStage;
		if (m_pStage)
		{
			m_pStage->Init();									// 初期化
			m_pStage->SetStage("data/TEXT/StageSet001.txt");	// モデル設定のファイル名
		}
	}

#ifdef _DEBUG
	// タイマー生成
	if (!m_pTimer)
	{
		m_pTimer = CTimerCount::Create(D3DXVECTOR3(SCREEN_WIDTH - 250.0f, 50.0f, 0.0f), NUMBER_SIZE, 10, false);
	}
	//m_pMeshField = CMeshField::Create({ 100.0f,100.0f,100.0f }, { 300.0f,0.0f,300.0f }, { 0.0f,0.0f,0.0f }, 6, 6);
	//m_pMeshField->BindTexture(CManager::GetInstance()->GetTexture()->GetTexture("TEX_TYPE_CAUTION"));

	CBombSpawner::Create({ 800.0f,0.0f,0.0f }, { 0.0f,0.0f,0.0f }, { 1.0f,1.0f,1.0f }, 5);

	CBomb::Create({ 0.0f,	10.0f, -300.0f }, { 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }, CManager::GetInstance()->GetLoadX()->GetNum("MODTYPE_BOMB"));
	CBomb::Create({ 100.0f, 10.0f, -300.0f }, { 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }, CManager::GetInstance()->GetLoadX()->GetNum("MODTYPE_BOMB"));
	CBomb::Create({ -100.0f,10.0f, -300.0f }, { 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }, CManager::GetInstance()->GetLoadX()->GetNum("MODTYPE_BOMB"));
	CBomb::Create({ 0.0f,	10.0f, -400.0f }, { 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }, CManager::GetInstance()->GetLoadX()->GetNum("MODTYPE_BOMB"));
	CBomb::Create({ 100.0f, 10.0f, -400.0f }, { 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }, CManager::GetInstance()->GetLoadX()->GetNum("MODTYPE_BOMB"));
	CBomb::Create({ -100.0f,10.0f, -400.0f }, { 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }, CManager::GetInstance()->GetLoadX()->GetNum("MODTYPE_BOMB"));
#else
	// タイマー生成
	if (!m_pTimer)
	{
		m_pTimer = CTimerCount::Create(D3DXVECTOR3(SCREEN_WIDTH - 250.0f, 50.0f, 0.0f), NUMBER_SIZE, GAME_TIME, false);
	}
#endif

	// スコア生成
	if (!m_pScore)
	{
		m_pScore = CScore::Create(D3DXVECTOR3(SCREEN_WIDTH - 300.0f, SCREEN_HEIGHT - 50.0f, 0.0f), NUMBER_SIZE);
	}

	return S_OK;
}

//=========================================================
// 終了
//=========================================================
void CGame::Uninit(void)
{
	CManager::GetPlayData()->SetScore(m_pScore->GetScore());	// プレイデータにスコアを設定

	// ストップ解除
	if (CManager::GetInstance()->GetStop())
	{
		CManager::GetInstance()->SetStop(false);
	}

	//------------------------------------
	// オブジェクトを破棄
	//------------------------------------
	// スコア
	if (m_pScore)
	{
		m_pScore = nullptr;
	}

	// 追加スコア
	if (m_pAddScore)
	{
		m_pAddScore = nullptr;
	}

	// メッシュフィールド
	if (m_pMeshField)
	{
		m_pMeshField->Uninit();
		m_pMeshField = nullptr;
	}

	// メッシュウォール
	if (m_pMeshWall)
	{
		m_pMeshWall->Uninit();
		m_pMeshWall = nullptr;
	}

	// メッシュスフィア
	if (m_pMeshSphere)
	{
		m_pMeshSphere->Uninit();
		m_pMeshSphere = nullptr;
	}

	// メッシュシリンダー
	if (m_pMeshCylinder)
	{
		m_pMeshCylinder->Uninit();
		m_pMeshCylinder = nullptr;
	}

	// タイマー
	if (m_pTimer)
	{
		m_pTimer->Uninit();
		m_pTimer = nullptr;
	}

	// ステージ
	if (m_pStage)
	{
		m_pStage->Uninit();
		delete m_pStage;
		m_pStage = nullptr;
	}

	// UI
	SetUiDelete();
}

//=========================================================
// 更新
//=========================================================
void CGame::Update(void)
{
	//----------------------------------------------------
	// 開始カウントダウン
	//----------------------------------------------------
#ifndef _DEBUG
	if (!m_bStart)
	{
		StartGame();
	}
#endif

	//----------------------------------------------------
	// 終了サイン
	//----------------------------------------------------
	if (m_bEnd)
	{
		EndGame();
	}

	else
	{
		if (m_pTimer->GetLimit())
		{
			m_bEnd = true;
		}

		// 全てのオブジェクトを破壊し終えたら
		CScene *pScene = CScene::GetScene(CScene::OBJTYPE_MODEL);
		if (!pScene)
		{
			m_bEnd = true;
		}

#ifdef _DEBUG
		if (CInput::PressAnyAction(CInput::ACTION_ENTER))
		{
			m_bEnd = true;
		}
#endif
	}
}
//=========================================================
// 描画
//=========================================================
void CGame::Draw(void)
{

}

//=========================================================
// 開始処理
//=========================================================
void CGame::StartGame(void)
{
	// サウンドクラス取得
	CSound *pSound = CManager::GetInstance()->GetSound();

	// フレーム0の時
	if (m_nFrame == 0)
	{
		CManager::GetInstance()->SetStop(true);
		m_pTimer->SetStop(true);
	}

	// フレームカウント
	m_nFrame++;		

	// カウント３
	if (m_nFrame == COUNT_DOWN_FRAME)
	{
		SetUiCreate("TEX_TYPE_NUMBER_THREE", 1);
		pSound->PlaySound(CSound::SOUND_LABEL_SE_COUNT);		// カウント音
	}

	// カウント２
	else if (m_nFrame == COUNT_DOWN_FRAME * 2)
	{
		SetUiCreate("TEX_TYPE_NUMBER_TWO", 1);
		pSound->PlaySound(CSound::SOUND_LABEL_SE_COUNT);		// カウント音
	}

	// カウント１
	else if (m_nFrame == COUNT_DOWN_FRAME * 3)
	{
		SetUiCreate("TEX_TYPE_NUMBER_ONE", 1);
		pSound->PlaySound(CSound::SOUND_LABEL_SE_COUNT);		// カウント音
	}

	// カウント０
	else if (m_nFrame == COUNT_DOWN_FRAME * 4)
	{
		SetUiCreate("TEX_TYPE_GAMESTART", 3);
		pSound->PlaySound(CSound::SOUND_LABEL_SE_GAMESTART);	// スタート音
	}

	// ゲームスタート
	else if (m_nFrame == COUNT_DOWN_FRAME * 5)
	{
		SetUiDelete();
		m_nFrame = 0;
		m_bStart = true;
		CManager::GetInstance()->SetStop(false);
		m_pTimer->SetStop(false);
	}
}

//=========================================================
// 終了処理
//=========================================================
void CGame::EndGame(void)
{
	CTexture *pTex = CManager::GetInstance()->GetTexture();
	CSound *pSound = CManager::GetInstance()->GetSound();

	if (!m_bOnce)
	{
		pSound->PlaySound(CSound::SOUND_LABEL_SE_TIMEUP);	// 終了音

		// 終了UI生成
		if (!m_pUI)
		{
			m_pUI = CUi::Create(SCREEN_CENTER, { UI_SIZE.x * 3,UI_SIZE.y,0.0f }, CUi::UI_TYPE_ZOOM);
			m_pUI->BindTexture(pTex->GetTexture("TEX_TYPE_END"));
		}

		// ストップ判定ON
		m_pTimer->SetStop(true);
		CManager::GetInstance()->SetStop(true);

		m_bOnce = true;
		m_bFrameCount = true;
	}

	// フレームカウントを行うか
	if (m_bFrameCount)
	{
		m_nFrame++;	
	}

	if (m_nFrame == FADE_FRAME)
	{
		// 追加スコアの生成
		if (!m_pAddScore)
		{
			CStringManager::Create({ SCREEN_WIDTH / 2 - 200.0f,SCREEN_HEIGHT / 2 - 150.0f,0.0f }, 64, 64, "残りタイム×100","ＭＳゴシック");
			m_pAddScore = CScore::Create({ SCREEN_WIDTH / 2 - NUMBER_SIZE.x * MAX_SCORE,SCREEN_HEIGHT / 2,0.0f }, NUMBER_SIZE * 2);
			m_pAddScore->SetScore(ADDSCORE_FORMER * m_pTimer->GetTimerCount());
		}
		SetUiDelete();
	}

	else if (m_nFrame == FADE_FRAME * 2)
	{
		// 加算スコアの計算が終わるまで一度カウントを止める
		if (m_bFrameCount)
		{
			m_bFrameCount = false;
		}

		// スコア	
		if (m_pAddScore->GetScore() > 0)
		{
			m_pAddScore->AddScore(-10);
			m_pScore->AddScore(10);
		}
		else
		{
			m_bFrameCount = true;	// フレームカウントを進める
		}
	}

	else if (m_nFrame == FADE_FRAME * 3)
	{
		CManager::GetInstance()->GetFade()->SetFade(CManager::MODE::RESULT);
	}
}


//=========================================================
// UI生成関数
//=========================================================
void CGame::SetUiCreate(const char* TexStr, float fSizeMulti)
{
	// テクスチャクラス取得
	CTexture *pTex = CManager::GetInstance()->GetTexture();

	SetUiDelete();

	if (!m_pUI)
	{
		m_pUI = CUi::Create(SCREEN_CENTER, { UI_SIZE.x * fSizeMulti,UI_SIZE.y,0.0f }, CUi::UI_TYPE_ZOOM);
		m_pUI->BindTexture(pTex->GetTexture(TexStr));
	}
}

//=========================================================
// UI削除関数
//=========================================================
void CGame::SetUiDelete(void)
{
	if (m_pUI)
	{
		m_pUI->Uninit();
		m_pUI = nullptr;
	}
}






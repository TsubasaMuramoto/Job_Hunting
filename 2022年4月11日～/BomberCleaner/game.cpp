//=========================================================
//ゲーム画面の処理
//=========================================================
#include "game.h"
#include "texture.h"
#include "manager.h"
#include "fade.h"
#include "meshfield.h"
#include "meshcylinder.h"
#include "meshsphere.h"
#include "timercount.h"
#include "stage.h"
#include "score.h"
#include "Scene3D.h"
#include "field.h"
#include "keyboard.h"
#include "XInput.h"
#include "DirectInput.h"
#include "Bomb.h"
#include "BombSpawner.h"
#include "LoadX.h"
#include "Ui.h"

//=========================================================
// マクロ定義
//=========================================================
#define COUNT_DOWN_FRAME (60)
#define UI_SIZE (D3DXVECTOR3(1.0f,1.0f,0.0f))

//=========================================================
// 静的メンバ変数の初期化
//=========================================================
CScore *CGame::m_pScore = nullptr;

//=========================================================
// コンストラクタ
//=========================================================
CGame::CGame()
{
	m_nFrame = 0;
	m_CameraRot = { 0.0f,0.0f,0.0f };
	m_pTimer = nullptr;
	m_pMeshCylinder = nullptr;
	m_pMeshField = nullptr;
	m_pMeshSphere = nullptr;
	m_pMeshWall = nullptr;
	m_pStage = nullptr;
	m_pUI = nullptr;
	m_bOnce = false;
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
	CBombSpawner::Create({ 800.0f,0.0f,0.0f }, { 0.0f,0.0f,0.0f }, { 1.0f,1.0f,1.0f }, 5);
	//CBomb::Create(D3DXVECTOR3(0.0f, 10.0f, -200.0f), D3DXVECTOR3(0.0f,0.0f,0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f), "data/MODEL/Bomb001.x");
	CBomb::Create({ 0.0f,	10.0f, -300.0f	},	{ 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }, CManager::GetInstance()->GetLoadX()->GetNum("MODTYPE_BOMB"));
	CBomb::Create({ 100.0f, 10.0f, -300.0f	},	{ 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }, CManager::GetInstance()->GetLoadX()->GetNum("MODTYPE_BOMB"));
	CBomb::Create({ -100.0f,10.0f, -300.0f	},	{ 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }, CManager::GetInstance()->GetLoadX()->GetNum("MODTYPE_BOMB"));
	CBomb::Create({ 0.0f,	10.0f, -400.0f	},	{ 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }, CManager::GetInstance()->GetLoadX()->GetNum("MODTYPE_BOMB"));
	CBomb::Create({ 100.0f, 10.0f, -400.0f	},	{ 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }, CManager::GetInstance()->GetLoadX()->GetNum("MODTYPE_BOMB"));
	CBomb::Create({ -100.0f,10.0f, -400.0f	},	{ 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }, CManager::GetInstance()->GetLoadX()->GetNum("MODTYPE_BOMB"));

	/*CMeshField::Create({ 600.0f,50.0f,100.0f }, { 300.0f,0.0f,300.0f }, { 0.0f,0.0f,0.0f }, 1, 1);
	CMeshField::Create({ 700.0f,120.0f,100.0f }, { 300.0f,0.0f,300.0f }, { 0.0f,0.0f,0.0f }, 1, 1);
	CMeshField::Create({ 900.0f,100.0f,100.0f }, { 300.0f,0.0f,300.0f }, { 0.0f,0.0f,0.0f }, 1, 1);*/

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

	/*CMeshField::Create({ 100.0f,50.0f,100.0f }, { 300.0f,0.0f,300.0f }, { 0.0f,0.0f,0.0f }, 1, 1);
	CMeshField::Create({ 100.0f,120.0f,100.0f }, { 300.0f,0.0f,300.0f }, { 0.0f,0.0f,0.0f }, 1, 1);
	CMeshField::Create({ 100.0f,180.0f,100.0f }, { 300.0f,0.0f,300.0f }, { 0.0f,0.0f,0.0f }, 6, 6);
	CMeshField::Create({ -200.0f,100.0f,100.0f }, { 300.0f,0.0f,300.0f }, { 0.0f,0.0f,0.0f }, 1, 1);*/

	// タイマーカウント生成
#ifdef _DEBUG
	m_pTimer = CTimerCount::Create(D3DXVECTOR3(SCREEN_WIDTH - 250.0f, 50.0f, 0.0f), D3DXVECTOR3(50.0f, 30.0f, 0.0f), 10, false);
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

}

//=========================================================
//更新
//=========================================================
void CGame::Update(void)
{
#if (1)
	if (!m_bStart && !m_bEnd)
	{
		//CCamera *pCamera = CManager::GetInstance()->GetCamera(0);
		//m_CameraRot = pCamera->GetRot();
		//m_CameraRot.y += 0.1f;
		//pCamera->SetRot(m_CameraRot);

		if (m_nFrame == 0)
		{
			CManager::GetInstance()->SetStop(true);
		}

		m_nFrame++;

		if (m_nFrame == COUNT_DOWN_FRAME) 
		{
			m_pUI = CUi::Create(SCREEN_CENTER, UI_SIZE, CUi::UI_TYPE_ZOOM);
			m_pUI->BindTexture(CManager::GetInstance()->GetTexture()->GetTexture("TEX_TYPE_NUMBER_THREE"));
		}
		else if (m_nFrame == COUNT_DOWN_FRAME * 2) 
		{
			SetUiDelete();
			m_pUI = CUi::Create(SCREEN_CENTER, UI_SIZE, CUi::UI_TYPE_ZOOM);
			m_pUI->BindTexture(CManager::GetInstance()->GetTexture()->GetTexture("TEX_TYPE_NUMBER_TWO"));
		}
		else if (m_nFrame == COUNT_DOWN_FRAME * 3) 
		{
			SetUiDelete();
			m_pUI = CUi::Create(SCREEN_CENTER, UI_SIZE, CUi::UI_TYPE_ZOOM);
			m_pUI->BindTexture(CManager::GetInstance()->GetTexture()->GetTexture("TEX_TYPE_NUMBER_ONE"));
		}
		else if (m_nFrame == COUNT_DOWN_FRAME * 4) 
		{
			SetUiDelete();
			m_pUI = CUi::Create(SCREEN_CENTER, {UI_SIZE.x * 3,UI_SIZE.y,0.0f}, CUi::UI_TYPE_ZOOM);
			m_pUI->BindTexture(CManager::GetInstance()->GetTexture()->GetTexture("TEX_TYPE_GAMESTART"));
		}
		else if (m_nFrame == COUNT_DOWN_FRAME * 5)
		{
			SetUiDelete();
			m_nFrame = 0;
			m_bStart = true;
			CManager::GetInstance()->SetStop(false);
		}
	}
#endif
	if (m_pTimer->GetLimit())
	{
		if (!m_bOnce)
		{
			m_pUI = CUi::Create(SCREEN_CENTER, { UI_SIZE.x * 3,UI_SIZE.y,0.0f }, CUi::UI_TYPE_ZOOM);
			m_pUI->BindTexture(CManager::GetInstance()->GetTexture()->GetTexture("TEX_TYPE_END"));
			CManager::GetInstance()->SetStop(true);
			m_bOnce = true;
		}
		m_nFrame++;
		if (m_nFrame == 90)
		{
			SetUiDelete();
			CManager::GetInstance()->GetFade()->SetFade(CManager::MODE::RESULT);
		}
	}

}
//=========================================================
// 描画
//--------------------------------------------
void CGame::Draw(void)
{

}

bool CGame::GetStart(void) {
	return m_bStart;
}
bool CGame::GetEnd(void) {
	return m_bEnd;
}

void CGame::SetUiDelete(void)
{
	if (m_pUI)
	{
		m_pUI->Uninit();
		delete m_pUI;
		m_pUI = nullptr;
	}
}






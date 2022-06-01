//=============================================================================
// カメラの作成
// Author:村元翼
//=============================================================================
#include "camera.h"
#include "renderer.h"
#include "manager.h"
#include "keyboard.h"
#include "player.h"
#include "XInput.h"
#include "DirectInput.h"

//==============================================================================
// マクロ定義
//==============================================================================
#define CAMERA_DISTANCE			(500.0f)	// カメラ距離
#define CAMERA_DISTANCE_MIN		(50.0f)		// カメラ距離最小
#define CAMERA_DISTANCE_MAX		(500.0f)	// カメラ距離最大
#define CAMERA_SPEED			(0.04f)		// カメラ移動速度
#define LONG_SPEED				(2.0f)		// カメラ遠近移動速度
#define POSR_Y_MOVE				(2.0f)		// カメラ注視点移動速度
#define POSR_Y_MOVEHEIGHT		(150.0f)	// カメラ注視点が動き始める高さ
#define _OX_EPSILON_			(0.000001f) // 誤差

//=============================================================================
// コンストラクタ
//=============================================================================
CCamera::CCamera()
{

}

//=============================================================================
// デストラクタ
//=============================================================================
CCamera::~CCamera()
{

}
//=============================================================================
// カメラの初期化処理
//=============================================================================
HRESULT CCamera::Init(void)
{
	m_fLong = CAMERA_DISTANCE;						// 視点注視点の距離
	m_posV = D3DXVECTOR3(0.0f, 100.0f, -m_fLong);	// カメラ視点
	m_posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// カメラ注視点
	m_vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);			// カメラベクトル
	m_rot = D3DXVECTOR3(2.0f, 0.0f, 0.0f);			// カメラ回転
	m_bPosR_Move = false;
	return S_OK;
}

//=============================================================================
// カメラの終了処理
//=============================================================================
void CCamera::Uninit(void)
{

}

//=============================================================================
// カメラの更新処理
//=============================================================================
void CCamera::Update(void)
{
	// カメラの移動
	MoveCamera();
}

//================================================
// カメラの生成処理
//================================================
CCamera *CCamera::Create(D3DXVECTOR3 m_pos, D3DXVECTOR3 rot, float viewportX, float viewportY, float viewportWidth, float viewporHeight, int nID)
{
	//インスタンスの生成
	CCamera *pCamera = nullptr;
	if (!pCamera)
	{
		// 動的メモリ確保
		pCamera = new CCamera;

		// NULLチェック
		if (pCamera)
		{
			pCamera->Init();	// 初期化
		}
	}
	return pCamera;
}

//================================================
// カメラの設定
//================================================
void CCamera::SetCamera(void)
{
	LPDIRECT3DDEVICE9 pDevice;										// デバイスのポインタ
	pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	// デバイス取得

	//ビューポートの設定
	//pDevice->SetViewport(&m_viewport);

	// プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxProjection);

	// プロジェクションマトリックスを作成
	D3DXMatrixPerspectiveFovLH(&m_mtxProjection,
								D3DXToRadian(50.0f),							//カメラの画角
								(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,		//画面比率!!!ビューポートを使うときビューポートの幅高さを使う
								10.0f,											//手前の描画制限
								10000.0f);										//奥の描画制限

	// プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &m_mtxProjection);

	// ビューマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxView);

	// ビューマトリックスを作成
	D3DXMatrixLookAtLH
	(
		&m_mtxView,
		&m_posV,
		&m_posR,
		&m_vecU
	);

	// ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &m_mtxView);
}

//===============================================================
// カメラの移動
//===============================================================
void CCamera::MoveCamera(void)
{
	// 入力デバイスの取得
	CInputkeyboard *pKey = CManager::GetInstance()->GetKeyboard();
	CDInput *pDInput = CManager::GetInstance()->GetDInput();

	//++++++++++++++++++++++++++++++//
	//			視点の回転			//
	//++++++++++++++++++++++++++++++//
	if (CInput::PressAnyAction(CInput::ACTION_R_LEFT))
	{	// 左へ回転
		m_rot.y += CAMERA_SPEED;
		if (m_rot.y > D3DX_PI)
		{
			m_rot.y = -D3DX_PI;
		}
	}
	if (CInput::PressAnyAction(CInput::ACTION_R_RIGHT))
	{	// 右へ回転
		m_rot.y += -CAMERA_SPEED;
		if (m_rot.y < -D3DX_PI)
		{
			m_rot.y = D3DX_PI;
		}
	}

	if (CInput::PressAnyAction(CInput::ACTION_R_DOWN))
	{	// 下へ回転
		m_rot.x += -CAMERA_SPEED;
		if (m_rot.x < 0.0f + D3DX_PI / 2)
		{
			m_rot.x = 0.0f + D3DX_PI / 2 + _OX_EPSILON_;
		}
	}
	if (CInput::PressAnyAction(CInput::ACTION_R_UP))
	{	// 上へ回転
		m_rot.x += CAMERA_SPEED;
		if (m_rot.x > D3DX_PI)
		{
			m_rot.x = D3DX_PI - _OX_EPSILON_;
		}
	}

	// カメラを近づける
	if (pDInput->GetPress(pDInput->BUTTON_R2))
	{
		m_fLong -= LONG_SPEED;
		if (m_fLong <= CAMERA_DISTANCE_MIN)
		{
			m_fLong = CAMERA_DISTANCE_MIN;
		}
	}

	// カメラを遠ざける
	else if (pDInput->GetPress(pDInput->BUTTON_L2))
	{
		m_fLong += LONG_SPEED;
		if (m_fLong >= CAMERA_DISTANCE_MAX)
		{
			m_fLong = CAMERA_DISTANCE_MAX;
		}
	}

#ifdef _DEBUG
	//++++++++++++++++++++++++++++//
	//		  カメラの移動	      //
	//++++++++++++++++++++++++++++//
	//左移動
	if (pKey->GetPress(DIK_A))
	{		
		m_posV.x -= cosf(-m_rot.y) * 2.0f;
		m_posV.z -= sinf(-m_rot.y) * 2.0f;
		m_posR.x -= cosf(-m_rot.y) * 2.0f;
		m_posR.z -= sinf(-m_rot.y) * 2.0f;
	}

	//右移動
	if (pKey->GetPress(DIK_D))
	{
		m_posV.x += cosf(-m_rot.y) * 2.0f;
		m_posV.z += sinf(-m_rot.y) * 2.0f;
		m_posR.x += cosf(-m_rot.y) * 2.0f;
		m_posR.z += sinf(-m_rot.y) * 2.0f;
	}

	//奥移動
	if (pKey->GetPress(DIK_W))
	{
		m_posV.x += sinf(m_rot.y) * 2.0f;
		m_posV.z += cosf(m_rot.y) * 2.0f;
		m_posR.x += sinf(m_rot.y) * 2.0f;
		m_posR.z += cosf(m_rot.y) * 2.0f;
	}

	//手前移動
	if (pKey->GetPress(DIK_S))
	{	
		m_posV.x -= sinf(m_rot.y) * 2.0f;
		m_posV.z -= cosf(m_rot.y) * 2.0f;
		m_posR.x -= sinf(m_rot.y) * 2.0f;
		m_posR.z -= cosf(m_rot.y) * 2.0f;
	}
#endif
	//++++++++++++++++++++++++++++//
	//		カメラ移動限界		  //
	//++++++++++++++++++++++++++++//
	if (m_posV.x > MOVELIMIT)
	{
		m_posV.x = MOVELIMIT;
	}
	else if (m_posV.x < -MOVELIMIT)
	{
		m_posV.x = -MOVELIMIT;
	}
	if (m_posV.z > MOVELIMIT)
	{
		m_posV.z = MOVELIMIT;
	}
	else if (m_posV.z < -MOVELIMIT)
	{
		m_posV.z = -MOVELIMIT;
	}
}

//=============================================================================
// プレイヤーにカメラが追従する処理
//=============================================================================
void CCamera::SetPlayerCamera(CPlayer *pPlayer)
{
	// NULLチェック
	if (pPlayer)
	{
		//----------------------------------------------------------
		// カメラ注視点のY座標の追従を行う処理
		//----------------------------------------------------------
		float fLong = pPlayer->GetPos().y - m_posR.y;

		if (m_bPosR_Move)	// 追従する状態
		{
			float fMove = (fLong / 20.0f);
			if (fMove <= 0.01f && fMove >= -0.01f)
			{
				m_bPosR_Move = false;
				m_posR.y = pPlayer->GetPos().y;
			}
			else
			{
				m_posR.y += fMove;	// Y座標を動かす
			}

		}

		else	// 追従しない状態
		{
			if (fLong != 0.0f && !pPlayer->GetJump())
			{
				m_bPosR_Move = true;
			}
		}

		// 注視点の位置をプレイヤーの位置とする(y座標は例外)
		m_posR.x = pPlayer->GetPos().x;
		m_posR.z = pPlayer->GetPos().z;

		// 視点の場所を注視点を元に移動(極座標回転)
		m_posV.x = pPlayer->GetPos().x - m_fLong * sinf(m_rot.x) * sinf(m_rot.y);
		m_posV.z = pPlayer->GetPos().z - m_fLong * sinf(m_rot.x) * cosf(m_rot.y);
		m_posV.y = pPlayer->GetPos().y - m_fLong * cosf(m_rot.x);
	}
}

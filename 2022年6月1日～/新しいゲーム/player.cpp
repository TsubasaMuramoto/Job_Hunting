#include "main.h"
#include "manager.h"
#include "LoadX.h"
#include "renderer.h"
#include "player.h"
#include "model.h"
#include "keyboard.h"
#include "camera.h"
#include "Scene3D.h"
#include "XInput.h"
#include "DirectInput.h"
#include "effect.h"
#include "model_spawner.h"
#include "meshfield.h"
#include "shadow.h"
#include "mouse.h"
#include <assert.h>

//=============================================================================
// マクロ定義
//=============================================================================
#define PLAYER_SHADOWSIZE	(D3DXVECTOR3(m_size.x * 1.5f,0.0f,m_size.z * 1.5f))	// プレイヤー影サイズ
#define MOVE_EFFECTSIZE		(D3DXVECTOR3(10.0f,10.0f,0.0f))
#define MARK_SIZE			(D3DXVECTOR3(10.0f,40.0f,0.0f))						// 目印のサイズ
#define ROTATE_POWER		(0.005f)											// 回転量
#define CUBE_ROTATE_POWER	(0.002f)											// キューブ状態時の回転量

//=============================================================================
// 静的メンバ変数の初期化
//=============================================================================
const float CPlayer::m_fMaxSpeed = MAX_SPEED;

//=============================================================================
// コンストラクタ
//=============================================================================
CPlayer::CPlayer(OBJTYPE nPriority) : CScene(nPriority)
{
	m_pModel		= nullptr;
	m_pBillboard	= nullptr;
	m_Direction		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_pos			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Oldpos		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Speed			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fGravity		= 0.0f;
	m_fGravitySpeed = GRAVITY_SPEED;
	m_fAngle		= 0.0f;
	m_bJump			= false;
	m_bUninit		= false;
	m_bSwitch		= true;
	m_state			= PLAYER_STATE::BALL;
}

//=============================================================================
// デストラクタ
//=============================================================================
CPlayer::~CPlayer()
{

}

//=============================================================================
// 生成
//=============================================================================
CPlayer *CPlayer::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType)
{
	// インスタンス生成
	CPlayer *pPlayer = nullptr;
	if (!pPlayer)
	{
		pPlayer = new CPlayer;
		if (pPlayer)
		{
			pPlayer->m_nType = nType;		// プレイヤータイプ
			pPlayer->Init(pos, rot);		// 初期化
		}
	}
	return pPlayer;
}

//=============================================================================
// 初期化
//=============================================================================
HRESULT CPlayer::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	// 位置・回転保存
	m_pos = pos;
	m_rot = rot;

	if (!m_pModel)
	{
		// 角度をラジアンに変換
		float rot_x = D3DXToRadian(rot.x);
		float rot_y = D3DXToRadian(rot.y);
		float rot_z = D3DXToRadian(rot.z);

		// プレイヤーモデル生成
		m_pModel = CModel::Create({ 0.0f,0.0f,0.0f }, { rot_x,rot_y,rot_z }, { 1.0f,1.0f,1.0f }, m_nType, false);
		// モデルタイプの設定
		CScene::SetModelType(CScene::MODTYPE_PLAYER);
		// プレイヤーサイズの設定
		CScene::SetSize(m_pModel->GetSize());
	}

	// サイズ取得
	m_size = m_pModel->GetSize();

	// 影の設定
	if (!m_pShadow)
	{
		m_pShadow = CShadow::Create({m_pos.x , m_pos.y - (m_size.y / 2),m_pos.z}, PLAYER_SHADOWSIZE, { 0.0f,0.0f,0.0f });
	}

	// 位置・回転設定
	CScene::SetPos(m_pos);
	CScene::SetSize(m_size);

	// 回転用マトリックスの初期化
	D3DXMatrixIdentity(&m_mtxRot);

	return S_OK;
}

//=============================================================================
// 終了
//=============================================================================
void CPlayer::Uninit()
{
	// プレイヤーモデル破棄
	ModelDelete();

	// 影の破棄
	if (m_pShadow)
	{
		m_pShadow->Uninit();
		m_pShadow = nullptr;
	}

	// オブジェクト破棄
	Release();
}

//=============================================================================
// 更新
//=============================================================================
void CPlayer::Update()
{
	// シーンに保存した位置を取得
	 m_pos = CScene::GetPos();

	 // 現在の位置を最後の位置にする
	 m_Oldpos = m_pos;
	 CScene::SetPosOld(m_Oldpos);

	 // カメラの取得
	 CCamera *pCamera = CManager::GetInstance()->GetCamera(0);
#ifdef _DEBUG
	//-------------------------------------------
	// カメラ追従関数の呼び出し
	//-------------------------------------------
	if (m_bSwitch)
	{
		pCamera->SetPlayerCamera(this);
	}
	else
	{
		pCamera->NoFollowingPlayer();
	}

	//-------------------------------------------
	// カメラ追従切り替え
	//-------------------------------------------
	if (CManager::GetInstance()->GetKeyboard()->GetTrigger(DIK_Q))
	{
		 m_bSwitch = !m_bSwitch;
		 if (m_bSwitch)
		 {
			 pCamera->SetLong(CAMERA_DISTANCE);
			 pCamera->SetRot(CAMERA_ROT);
		 }
		 else
		 {
			 
		 }
	}

#else
	 CManager::GetInstance()->GetCamera(0)->SetPlayerCamera(this);
#endif

	//------------------------------------------
	// プレイヤー制御系関数
	//------------------------------------------
	Move();														// 移動
	Action();													// アクション
	Gravity(m_pos, m_fGravity, m_fGravitySpeed, m_bJump);		// 重力

	// シーンに位置を設定する
	CScene::SetPos(m_pos);

	if (m_pModel)	// プレイヤーモデル更新
	{
		m_pModel->Update();
	}

	if (m_pShadow)	// シャドウの位置設定
	{
		m_pShadow->CScene::SetPosOld({ m_pos.x ,m_pos.y, m_pos.z });
	}

	if (m_bUninit)
	{
		Uninit();
	}
}

//==========================================================================================================
// 描画
//==========================================================================================================
void CPlayer::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice(); // デバイスのポインタ
	D3DXMATRIX mtxRotModel, mtxTransModel;											 // 計算用マトリックス

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);
	
	// クォータニオンで生成した回転行列を掛ける
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxRot, &m_mtxWorld);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTransModel, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTransModel);

	// 各パーツのワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// モデルの描画
	m_pModel->Draw();
}

//==========================================================================================================
// 移動処理
//==========================================================================================================
void CPlayer::Move(void)
{
#if(1)
	m_Speed.x += ACCELERATION;
#endif

	//=============================================================================
	// 移動する(座標・回転更新)
	//=============================================================================
	if (m_Speed.x != 0.0f)
	{
		m_pos.x += m_Speed.x;

		// 移動のエフェクト
		CEffect::Create(m_pos, MOVE_EFFECTSIZE, { 1.0f, 1.0f, 1.0f ,1.0f }, 0.1f, 1);
	}

#if (0)
	//==========================================================================================================
	// 移動入力判定
	//==========================================================================================================
	if (InputDirection(DIRECTION_FORWARD) || InputDirection(DIRECTION_BACK) || InputDirection(DIRECTION_RIGHT) || InputDirection(DIRECTION_LEFT))
	{
		//  奥
		if (InputDirection(DIRECTION_FORWARD))
		{
			m_Speed.z += ACCELERATION;
		}
		// 手前
		else if (InputDirection(DIRECTION_BACK))
		{
			m_Speed.z -= ACCELERATION;
		}
		// 右
		if (InputDirection(DIRECTION_RIGHT))
		{
			m_Speed.x += ACCELERATION;
		}
		// 左
		else if (InputDirection(DIRECTION_LEFT))
		{
			m_Speed.x -= ACCELERATION;
		}

	}
#endif

	//=============================================================================
	// 状態別移動処理
	//=============================================================================
	switch (m_state)
	{
	case PLAYER_STATE::BALL:
		m_fAngle -= ROTATE_POWER;					// 回転量を足す(引く)
		Quaternion();								// クォータニオン回転
		break;

	case PLAYER_STATE::CUBE:
		if (m_bJump)
		{
			m_fAngle -= CUBE_ROTATE_POWER;			// キューブ時の回転
			Quaternion();							// クォータニオン回転
		}
		else
		{
			D3DXMatrixIdentity(&m_mtxRot);			// 回転初期化
		}
		break;

	case PLAYER_STATE::AIRSHIP:
		if (m_bJump)
		{
			m_fAngle = D3DX_PI / 4;					// 飛行船時の回転
			Quaternion();							// クォータニオン回転
		}
		else
		{
			D3DXMatrixIdentity(&m_mtxRot);			// 回転初期化
		}
		break;
	}

	Inertia(m_Speed);								// 慣性
	SpeedAndRotLimit(m_Speed, m_rot, m_fMaxSpeed);	// 速度と回転限界
}

//==========================================================================================================
// クォータニオン回転処理関数
//==========================================================================================================
void CPlayer::Quaternion(void)
{
	D3DXVECTOR3 moveVec = { m_pos - m_Oldpos };			// 移動ベクトル

	// 軸ベクトル(移動ベクトルと垂直のベクトル)
	D3DXVECTOR3 Axis = {0.0f,0.0f,0.0f};
	Axis.x = -moveVec.z;
	Axis.z = moveVec.x;

	if (m_Speed.x != 0.0f || m_Speed.z != 0.0f)
	{
		D3DXQUATERNION QuaAns = { 0.0f,0.0f,0.0f,1.0f };	// 単位クォータニオン
		D3DXQuaternionRotationAxis(&QuaAns, &Axis, D3DXToDegree(m_fAngle));
		D3DXMatrixRotationQuaternion(&m_mtxRot, &QuaAns);
	}
}

//==========================================================================================================
// プレイヤーの慣性
//==========================================================================================================
void CPlayer::Inertia(D3DXVECTOR3 &speed)
{
	//------------------------------------------------
	// 左右入力していない状態
	//------------------------------------------------
	// 0に戻り続ける処理
	if (speed.x > 0.0f)
	{
		speed.x -= INERTIA;

		if (speed.x <= 0.0f)
		{
			speed.x = 0.0f;
		}
	}

	else if (speed.x < 0.0f)
	{
		speed.x += INERTIA;

		if (speed.x >= 0.0f)
		{
			speed.x = 0.0f;
		}
	}

	//------------------------------------------------
	// 上下入力していない状態
	//------------------------------------------------
	// 0に戻り続ける処理
	if (speed.z > 0.0f)
	{
		speed.z -= INERTIA;

		if (speed.z <= 0.0f)
		{
			speed.z = 0.0f;
		}
	}

	else if (speed.z < 0.0f)
	{
		speed.z += INERTIA;

		if (speed.z >= 0.0f)
		{
			speed.z = 0.0f;
		}
	}

	//------------------------------------------------
	// ジャンプを押していない場合
	//------------------------------------------------
	// 0に戻り続ける処理
	if (speed.y > 0.0f)
	{
		speed.y -= INERTIA;

		if (speed.y <= 0.0f)
		{
			speed.y = 0.0f;
		}
	}

	else if (speed.y < 0.0f)
	{
		speed.y += INERTIA;

		if (speed.y >= 0.0f)
		{
			speed.y = 0.0f;
		}
	}
}

//==========================================================================================================
// 速度 ＆ 回転の限界
//==========================================================================================================
void CPlayer::SpeedAndRotLimit(D3DXVECTOR3 &speed, D3DXVECTOR3 &rot,const float fMaxSpeed)
{
	//==========================================================================================================
	// 回転限界
	//==========================================================================================================
	if (rot.x < -D3DX_PI)
	{
		rot.x = D3DX_PI;
	}
	else if (rot.x > D3DX_PI)
	{
		rot.x = -D3DX_PI;
	}
	if (rot.z < -D3DX_PI)
	{
		rot.z = D3DX_PI;
	}
	else if (rot.z > D3DX_PI)
	{
		rot.z = -D3DX_PI;
	}

	if (m_fAngle > D3DX_PI)
	{
		m_fAngle = -D3DX_PI;
	}
	else if (m_fAngle < -D3DX_PI)
	{
		m_fAngle = D3DX_PI;
	}

	//==========================================================================================================
	// 速度限界
	//==========================================================================================================
	if (speed.x >= fMaxSpeed)
	{
		speed.x = fMaxSpeed;
	}
	else if (speed.x <= -fMaxSpeed)
	{
		speed.x = -fMaxSpeed;
	}
	if (speed.y >= fMaxSpeed)
	{
		speed.y = fMaxSpeed;
	}
	else if (speed.y <= -fMaxSpeed)
	{
		speed.y = -fMaxSpeed;
	}
	if (speed.z >= fMaxSpeed)
	{
		speed.z = fMaxSpeed;
	}
	else if (speed.z <= -fMaxSpeed)
	{
		speed.z = -fMaxSpeed;
	}
}

//==========================================================================================================
// プレイヤーアクション
//==========================================================================================================
void CPlayer::Action(void)
{
	CMouse *pMouse = CManager::GetInstance()->GetMouse();
	CInputkeyboard *pKey = CManager::GetInstance()->GetKeyboard();

	switch (m_state)
	{
	case PLAYER_STATE::BALL:
	case PLAYER_STATE::CUBE:
		// ジャンプ
		if ((pMouse->GetTrigger(pMouse->MOUSE_LEFT) || CInput::PressAnyAction(CInput::ACTION_SPACE)) && !m_bJump)	// Aボタン
		{
			m_fGravity = JUMP;
			m_bJump = true;
		}
		break;

	case PLAYER_STATE::AIRSHIP:
		if (pMouse->GetPress(pMouse->MOUSE_LEFT))
		{
			m_Speed.y += ACCELERATION;
			m_fGravity = m_Speed.y;
			m_bJump = true;
		}
		break;
	}



#ifdef _DEBUG
	// モード切替
	if (pKey->GetTrigger(DIK_0))
	{
		SetState(PLAYER_STATE::BALL);
	}
	else if (pKey->GetTrigger(DIK_1))
	{
		SetState(PLAYER_STATE::CUBE);
	}
	else if (pKey->GetTrigger(DIK_2))
	{
		SetState(PLAYER_STATE::AIRSHIP);
	}
#endif
}

//==========================================================================================================
// プレイヤー状態設定関数
//==========================================================================================================
void CPlayer::SetState(PLAYER_STATE state)
{
	// モデル終了
	ModelDelete();

	// モデルを切り替える
	if (!m_pModel)
	{
		CLoadX *pLoad = CManager::GetInstance()->GetLoadX();

		switch (state)
		{
		case PLAYER_STATE::BALL:
			m_nType = pLoad->GetNum("PLAYER_ONE");
			break;

		case PLAYER_STATE::CUBE:
			m_nType = pLoad->GetNum("PLAYER_TWO");			
			break;

		case PLAYER_STATE::AIRSHIP:
			m_nType = pLoad->GetNum("PLAYER_THREE");
			break;
		}

		// モデル生成
		m_pModel = CModel::Create({ 0.0f,0.0f,0.0f }, { 0.0f,0.0f,0.0f }, { 1.0f,1.0f,1.0f }, m_nType, false);

		// プレイヤーサイズの設定
		CScene::SetSize(m_pModel->GetSize());

		// 
		m_pos.y = m_pModel->GetSize().y / 2;

		// 回転用マトリックスの初期化
		D3DXMatrixIdentity(&m_mtxRot);
	}

	m_state = state;
}

//==========================================================================================================
// 重力の処理(プレイヤーに重力を減らす値(初期値は減らしたい正の数)を持たして、第三引数に入れて)
//==========================================================================================================
void CPlayer::Gravity(D3DXVECTOR3& pos, float& fGravity, const float& fGravitySpeed, bool& bJump)
{
	// 重力加算
	if (fGravitySpeed > 0.0f)
	{
		fGravity -= fGravitySpeed;
		pos.y += m_fGravity;
	}

	// 床についたかどうか
	if (pos.y <= 0.0f)
	{
		// 位置を固定する
		pos.y = 0.0f;
		fGravity = 0.0f;
		bJump = false;
	}
}

//==========================================================================================================
// モデル終了処理
//==========================================================================================================
void CPlayer::ModelDelete(void)
{
	if (m_pModel)
	{
		m_pModel->Uninit();
		delete m_pModel;
		m_pModel = nullptr;
	}
}

//==========================================================================================================
// 移動方向入力関数
//==========================================================================================================
bool CPlayer::InputDirection(const MOVE_DIRECTION &moveDir)
{
	// クラスの取得
	CDInput *pDInput = CManager::GetInstance()->GetDInput();
	CXInput *pXInput = CManager::GetInstance()->GetXInput();
	CInputkeyboard *pKey = CManager::GetInstance()->GetKeyboard();

	switch (moveDir)
	{
	case DIRECTION_FORWARD:
		return pKey->GetPress(DIK_W) || pXInput->GetGamePad()->m_state.Gamepad.sThumbLY >= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE || pDInput->GetGamepad().lY <= -DINPUT_STICK_RANGE;

	case DIRECTION_BACK:
		return pKey->GetPress(DIK_S) || pXInput->GetGamePad()->m_state.Gamepad.sThumbLY <= -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE || pDInput->GetGamepad().lY >= DINPUT_STICK_RANGE;

	case DIRECTION_LEFT:
		return pKey->GetPress(DIK_A ) || pXInput->GetGamePad()->m_state.Gamepad.sThumbLX <= -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE || pDInput->GetGamepad().lX <= -DINPUT_STICK_RANGE;

	case DIRECTION_RIGHT:
		return pKey->GetPress(DIK_D) || pXInput->GetGamePad()->m_state.Gamepad.sThumbLX >= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE || pDInput->GetGamepad().lX >= DINPUT_STICK_RANGE;

	default:
		assert(moveDir <= -1 || moveDir >= DIRECTION_MAX);
		return false;
	}
}

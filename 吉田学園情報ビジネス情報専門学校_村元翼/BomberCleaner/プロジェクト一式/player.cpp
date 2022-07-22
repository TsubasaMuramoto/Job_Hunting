//=============================================================================
// プレイヤーの処理
// Author : 村元翼
//=============================================================================
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "player.h"
#include "model.h"
#include "keyboard.h"
#include "camera.h"
#include "XInput.h"
#include "DirectInput.h"
#include "effect.h"
#include "Bomb.h"
#include "model_spawner.h"
#include "shadow.h"
#include "billboard.h"
#include "sound.h"
#include <assert.h>

//=============================================================================
// マクロ定義
//=============================================================================
#define PLAYER_SHADOWSIZE	(D3DXVECTOR3(m_size.x * 1.5f,0.0f,m_size.z * 1.5f))	// プレイヤー影サイズ
#define MOVE_EFFECTSIZE		(D3DXVECTOR3(5.0f,5.0f,0.0f))						// 移動エフェクトサイズ
#define CARRY_RANGE			(600.0f)											// 持ち運べる範囲
#define CARRY_RANGE_DIST	(10000.0f)											// 持ち運べる範囲(2点間の距離と比較する)
#define MARK_SIZE			(D3DXVECTOR3(10.0f,40.0f,0.0f))						// 目印のサイズ
#define MAX_PATTERN			(5)													// 目印のテクスチャパターンの数
#define DEATH_POSY			(-200.0f)											// 死亡Y座標

//=============================================================================
// コンストラクタ
//=============================================================================
CPlayer::CPlayer(OBJTYPE nPriority) : CScene(nPriority)
{
	//-----------------------------------------------
	// 変数初期化
	//-----------------------------------------------
	m_pBomb			= nullptr;							// 爆弾クラス
	m_pModel		= nullptr;							// モデルクラス
	m_pBillboard	= nullptr;							// ビルボードクラス
	m_Direction		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 向き
	m_pos			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 位置
	m_Oldpos		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 最後の位置
	m_Speed			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 速度
	m_fMaxSpeed		= MAX_SPEED;						// 最大速度
	m_fGravity		= 0.0f;								// 現在の重力
	m_fGravitySpeed = GRAVITY_SPEED;					// 重力加速度
	m_nFrame		= 0;								// フレーム数
	m_nPattern		= 0;								// テクスチャパターン
	m_fAngle		= 0.0f;								// 角度
	m_bJump			= false;							// ジャンプ判定
	m_state			= STATE_NORMAL;						// 状態
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
			pPlayer->m_posInit = pos;		// 初期位置保存
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
	// モデルクラス破棄
	if (m_pModel)
	{
		m_pModel->Uninit();
		delete m_pModel;
		m_pModel = nullptr;
	}

	// 爆弾クラスの破棄
	if (m_pBomb)
	{
		m_pBomb = nullptr;
	}

	// 影クラスの破棄
	if (m_pShadow)
	{
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

	// カメラ追従関数の呼び出し
	CManager::GetInstance()->GetCamera(0)->SetPlayerCamera(this);

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
}

//=============================================================================
// 描画
//=============================================================================
void CPlayer::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice(); // デバイスのポインタ
	D3DXMATRIX mtxTrans;															 // 計算用マトリックス

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);
	
	// クォータニオンで生成した回転行列を掛ける
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxRot, &m_mtxWorld);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// 各パーツのワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// モデルの描画
	if (m_pModel)
	{
		m_pModel->Draw();
	}
}

//=============================================================================
// プレイヤーの慣性
//=============================================================================
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
}

//==========================================================================================================
// 移動処理
//==========================================================================================================
void CPlayer::Move(void)
{
	//=============================================================================
	// 移動する(座標・回転更新)
	//=============================================================================
	if (m_Speed.x != 0.0f || m_Speed.z != 0.0f)
	{
		float fLength = sqrtf((m_Speed.x * m_Speed.x) + (m_Speed.z * m_Speed.z));
		float move_x = m_Speed.x / fLength;
		float move_z = m_Speed.z / fLength;
		move_x *= m_Speed.x;
		move_z *= m_Speed.z;

		//---------------------------------------------------
		// X移動(横移動)
		//---------------------------------------------------
		if (m_Speed.x != 0.0f)
		{
			m_fAngle -= move_x * ROTATING_VELOCITY;
			m_pos.x += move_x * sinf(m_rot.y);					// 移動はプレイヤーの向きに影響する
			m_pos.z += move_x * cosf(m_rot.y);
		}

		//---------------------------------------------------
		// Z移動(縦移動)
		//---------------------------------------------------
		if (m_Speed.z != 0.0f)
		{
			m_fAngle -= move_z * ROTATING_VELOCITY;
			m_pos.x += move_z * sinf(m_rot.y);
			m_pos.z += move_z * cosf(m_rot.y);
		}

		// 移動のエフェクト
		CEffect::Create(m_pos, MOVE_EFFECTSIZE, { 1.0f, 1.0f, 1.0f ,1.0f }, 0.1f, 1);
	}

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

		// プレイヤーの角度を移動量の大きさで計算
		float Ang = atan2f(m_Speed.x,m_Speed.z);
		// カメラの向いてる方向に向かってまっすぐになるようにカメラの向きをアングルに足す
		CCamera *pCamera = CManager::GetInstance()->GetCamera(0);
		Ang = Ang + pCamera->GetRot().y;
		m_rot.y = Ang;

	}

	Inertia(m_Speed);								// 慣性
	SpeedAndRotLimit(m_Speed, m_rot, m_fMaxSpeed);	// 速度と回転限界
	Quaternion();									// クォータニオン回転
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
		D3DXQUATERNION QuaAns = { 0.0f,0.0f,0.0f,1.0f };						// 単位クォータニオン
		D3DXQuaternionRotationAxis(&QuaAns, &Axis, D3DXToDegree(m_fAngle));		// 回転角度と軸から回転クォータニオンを求める
		D3DXMatrixRotationQuaternion(&m_mtxRot, &QuaAns);						// クォータニオンから回転行列を作成する
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
// 持ち運ぶ処理
//==========================================================================================================
bool CPlayer::Carry(void)
{
	//--------------------------------------------------------------
	// プレイヤーから一番近い爆弾を取得する
	//--------------------------------------------------------------
	CScene *pSaveScene = nullptr;
	CScene *pScene = CScene::GetScene(OBJTYPE_BOMB);

	float fSaveDist;		// プレイヤーと爆弾の距離を保存する変数
	while (pScene)
	{
		// 次のシーンを取得
		CScene *pSceneNext = CScene::GetSceneNext(pScene);

		if (pScene->GetModelType() == MODTYPE_BOMB)
		{
			float fDist;							// プレイヤーと爆弾の距離を取る変数
			D3DXVECTOR3 BombPos = pScene->GetPos();	// オブジェクトの位置取得

			// 距離を測る
			fDist = ((BombPos.x - m_pos.x) * (BombPos.x - m_pos.x)) + ((BombPos.y - m_pos.y) * (BombPos.y - m_pos.y)) + ((BombPos.z - m_pos.z) * (BombPos.z - m_pos.z));

			// プレイヤーの爆弾取得範囲内にいる
			if (fDist < CARRY_RANGE_DIST)
			{
				CBomb *pBomb = (CBomb*)pScene;
				// 取得したオブジェクトの距離と現在保存されている距離を測る
				if ((!pSaveScene || fDist < fSaveDist) && !pBomb->GetThrow())
				{
					fSaveDist = fDist;
					pSaveScene = pScene;
				}
			}
		}

		// 次のシーンを現在のシーンにする
		pScene = pSceneNext;
	}


	//--------------------------------------------------------------
	// 範囲内にオブジェクト(爆弾)が存在している場合
	//--------------------------------------------------------------
	if (pSaveScene)
	{
		m_nFrame++;	// フレームカウント

		// 位置と高さの作成
		D3DXVECTOR3 pos = pSaveScene->GetPos();
		float fHeight = pSaveScene->GetSize().y * 1.5f;

		// 現在選択されている爆弾に目印(ビルボード)をつける
		if (!m_pBillboard)
		{
			m_pBillboard = CBillboard::Create({ pos.x,pos.y + fHeight ,pos.z }, MARK_SIZE, { 1.0f,1.0f,1.0f,1.0f });	// ビルボードの生成
			m_pBillboard->BindTexture(CManager::GetInstance()->GetTexture()->GetTexture("TEX_TYPE_ALLOW"));				// テクスチャの取得
			m_pBillboard->SetTex(0, MAX_PATTERN);																		// テクスチャの設定
		}
		else
		{
			m_pBillboard->CScene::SetPos({ pos.x,pos.y + fHeight ,pos.z });	// 目印の位置設定
		}

		// 爆弾を持ちあげる
		if (CInput::PressAnyAction(CInput::ACTION_ATTACK))
		{
			m_pBomb = (CBomb*)pSaveScene;
			if (m_pBillboard)
			{
				m_pBillboard->Uninit();
				m_pBillboard = nullptr;
			}
			return true;
		}
	}
	else
	{
		// 範囲に爆弾がないなら目印を消す
		if (m_pBillboard)
		{
			m_pBillboard->Uninit();
			m_pBillboard = nullptr;
		}
	}
	return false;
}

//==========================================================================================================
// プレイヤーアクション
//==========================================================================================================
void CPlayer::Action(void)
{
	CSound *pSound = CManager::GetInstance()->GetSound();

	//--------------------------------------------------------------
	// 状態別処理
	//--------------------------------------------------------------
	switch (m_state)
	{
	case STATE_NORMAL:	// 通常
		if (Carry())
		{
			pSound->PlaySound(CSound::SOUND_LABEL_SE_CARRYUP);	// 持ち上げる音
			m_state = STATE_HOLD;
		}

		break;

	case STATE_HOLD:	// 保持
		if (m_pBomb)
		{
			m_pBomb->SetPos({ m_pos.x,m_pos.y + m_size.y,m_pos.z });

			// 攻撃ボタンの入力
			if (CInput::PressAnyAction(CInput::ACTION_ATTACK))
			{	
				pSound->PlaySound(CSound::SOUND_LABEL_SE_THROW);	// 投げる音	

				D3DXVECTOR3 moveVec = m_pos - m_Oldpos;				// 移動ベクトルを作る
				m_pBomb->Throw(moveVec);							// プレイヤーの移動方向に投げる
				m_pBomb = nullptr;									// 爆弾を破棄(メモリ自体は破棄しない)
				m_state = STATE_NORMAL;								// プレイヤーの状態を通常に戻す
			}
		}
		break;
	}

	// ジャンプ
	if (CInput::PressAnyAction(CInput::ACTION_SPACE) && !m_bJump)	// Aボタン
	{
		pSound->PlaySound(CSound::SOUND_LABEL_SE_JUMP);			// ジャンプ音

		m_fGravity = JUMP;										// 初期ジャンプ移動量
		m_bJump = true;											// ジャンプ判定ON
	}
}

//==========================================================================================================
// 重力の処理(プレイヤーに重力を減らす値(初期値は減らしたい正の数)を持たして、第三引数に入れて)
//==========================================================================================================
void CPlayer::Gravity(D3DXVECTOR3& pos, float& fGravity, const float& fGravitySpeed, bool&bJump)
{
	// 重力加算
	if (fGravitySpeed > 0.0f)
	{
		fGravity -= fGravitySpeed;
		pos.y += m_fGravity;
	}

	// 死亡判定についたかどうか
	if (pos.y <= DEATH_POSY)
	{
		// サウンドクラスの取得
		CSound *pSound = CManager::GetInstance()->GetSound();
		pSound->PlaySound(CSound::SOUND_LABEL_SE_FALL);

		// 位置を初期位置に戻す
		pos = m_posInit;
		fGravity = 0.0f;
		bJump = false;
	}
}

//==========================================================================================================
// 移動方向入力関数
//==========================================================================================================
bool CPlayer::InputDirection(const MOVE_DIRECTION &moveDir)
{
	// 入力系クラスの取得
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
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
// �}�N����`
//=============================================================================
#define PLAYER_SHADOWSIZE	(D3DXVECTOR3(m_size.x * 1.5f,0.0f,m_size.z * 1.5f))	// �v���C���[�e�T�C�Y
#define MOVE_EFFECTSIZE		(D3DXVECTOR3(10.0f,10.0f,0.0f))						// �ړ��G�t�F�N�g�T�C�Y
#define MARK_SIZE			(D3DXVECTOR3(10.0f,40.0f,0.0f))						// �ڈ�̃T�C�Y
#define ROTATE_POWER		(0.005f)											// ��]��
#define CUBE_ROTATE_POWER	(0.002f)											// �L���[�u��Ԏ��̉�]��
#define INERTIA				(0.1f)												// ����
#define ACCELERATION		(0.2f)												// �����x
#define JUMP				(13.0f)												// �d��
#define BOUNCE				(8.0f)												// 

//=============================================================================
// �ÓI�����o�ϐ��̏�����
//=============================================================================
const float CPlayer::m_fMaxSpeed = 5.0f;			// �ő呬�x
const float CPlayer::m_fGravitySpeed = 0.6f;		// �d�͂̋���
const float CPlayer::m_fFallSpeed = 0.4f;			// �������x

//=============================================================================
// �R���X�g���N�^
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
	m_fAngle		= 0.0f;
	m_bJump			= false;
	m_bUninit		= false;
	m_bSwitch		= true;
	m_state			= PLAYER_STATE::BALL;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CPlayer::~CPlayer()
{

}

//=============================================================================
// ����
//=============================================================================
CPlayer *CPlayer::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType)
{
	// �C���X�^���X����
	CPlayer *pPlayer = nullptr;
	if (!pPlayer)
	{
		pPlayer = new CPlayer;
		if (pPlayer)
		{
			pPlayer->m_nType = nType;		// �v���C���[�^�C�v
			pPlayer->Init(pos, rot);		// ������
		}
	}
	return pPlayer;
}

//=============================================================================
// ������
//=============================================================================
HRESULT CPlayer::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	// �ʒu�E��]�ۑ�
	m_pos = pos;
	m_rot = rot;

	if (!m_pModel)
	{
		// �p�x�����W�A���ɕϊ�
		float rot_x = D3DXToRadian(rot.x);
		float rot_y = D3DXToRadian(rot.y);
		float rot_z = D3DXToRadian(rot.z);

		// �v���C���[���f������
		m_pModel = CModel::Create({ 0.0f,0.0f,0.0f }, { rot_x,rot_y,rot_z }, { 1.0f,1.0f,1.0f }, m_nType, false);
		// ���f���^�C�v�̐ݒ�
		CScene::SetModelType(CScene::MODTYPE_PLAYER);
		// �v���C���[�T�C�Y�̐ݒ�
		CScene::SetSize(m_pModel->GetSize());
	}

	// �T�C�Y�擾
	m_size = m_pModel->GetSize();

	// �e�̐ݒ�
	if (!m_pShadow)
	{
		m_pShadow = CShadow::Create({m_pos.x , m_pos.y - (m_size.y / 2),m_pos.z}, PLAYER_SHADOWSIZE, { 0.0f,0.0f,0.0f });
	}

	// �ʒu�E��]�ݒ�
	CScene::SetPos(m_pos);
	CScene::SetSize(m_size);

	// ��]�p�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxRot);

	return S_OK;
}

//=============================================================================
// �I��
//=============================================================================
void CPlayer::Uninit()
{
	// �v���C���[���f���j��
	ModelDelete();

	// �e�̔j��
	if (m_pShadow)
	{
		m_pShadow->Uninit();
		m_pShadow = nullptr;
	}

	// �I�u�W�F�N�g�j��
	Release();
}

//=============================================================================
// �X�V
//=============================================================================
void CPlayer::Update()
{
	// �V�[���ɕۑ������ʒu���擾
	 m_pos = CScene::GetPos();

	 // ���݂̈ʒu���Ō�̈ʒu�ɂ���
	 m_Oldpos = m_pos;
	 CScene::SetPosOld(m_Oldpos);

	 // �J�����̎擾
	 CCamera *pCamera = CManager::GetInstance()->GetCamera(0);
#ifdef _DEBUG
	//-------------------------------------------
	// �J�����Ǐ]�֐��̌Ăяo��
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
	// �J�����Ǐ]�؂�ւ�
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
	// �v���C���[����n�֐�
	//------------------------------------------
	Move();														// �ړ�
	Action();													// �A�N�V����

	// �V�[���Ɉʒu��ݒ肷��
	CScene::SetPos(m_pos);

	if (m_pModel)	// �v���C���[���f���X�V
	{
		m_pModel->Update();
	}

	if (m_pShadow)	// �V���h�E�̈ʒu�ݒ�
	{
		m_pShadow->CScene::SetPosOld({ m_pos.x ,m_pos.y, m_pos.z });
	}

	if (m_bUninit)
	{
		Uninit();
	}
}

//==========================================================================================================
// �`��
//==========================================================================================================
void CPlayer::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice(); // �f�o�C�X�̃|�C���^
	D3DXMATRIX mtxRotModel, mtxTransModel;											 // �v�Z�p�}�g���b�N�X

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);
	
	// �N�H�[�^�j�I���Ő���������]�s����|����
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxRot, &m_mtxWorld);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTransModel, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTransModel);

	// �e�p�[�c�̃��[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// ���f���̕`��
	m_pModel->Draw();
}

//==========================================================================================================
// �ړ�����
//==========================================================================================================
void CPlayer::Move(void)
{
#if(1)
	m_Speed.x += ACCELERATION;
#endif

	//=============================================================================
	// �ړ�����(���W�E��]�X�V)
	//=============================================================================
	if (m_Speed.x != 0.0f)
	{
		m_pos.x += m_Speed.x;

		// �ړ��̃G�t�F�N�g
		CEffect::Create(m_pos, MOVE_EFFECTSIZE, { 1.0f, 1.0f, 1.0f ,1.0f }, 0.1f, 1);
	}

	//=============================================================================
	// ��ԕʈړ�����
	//=============================================================================
	switch (m_state)
	{
	case PLAYER_STATE::BALL:
		m_fAngle -= ROTATE_POWER;					// ��ɉ�]������
		Quaternion();					
		break;

	case PLAYER_STATE::CUBE:
		if (m_bJump)
		{
			m_fAngle -= CUBE_ROTATE_POWER;			// �W�����v���ɉ�]
			Quaternion();
		}
		else
		{
			D3DXMatrixIdentity(&m_mtxRot);			// ���n���ɉ�]������
		}
		break;

	case PLAYER_STATE::AIRSHIP:
		m_fAngle = D3DXToRadian(atan2(m_Speed.y, m_Speed.x));	// �c���̈ړ��ʂ���p�x�����߂�
		Quaternion();											// �N�H�[�^�j�I����]
		break;

	case PLAYER_STATE::UFO:
		break;
	}

	Inertia(m_Speed);								// ����
	SpeedAndRotLimit(m_Speed, m_rot, m_fMaxSpeed);	// ���x�Ɖ�]���E
}

//==========================================================================================================
// �v���C���[�̊���
//==========================================================================================================
void CPlayer::Inertia(D3DXVECTOR3 &speed)
{
	//------------------------------------------------
	// ���E���͂��Ă��Ȃ����
	//------------------------------------------------
	// 0�ɖ߂葱���鏈��
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
	// �㉺���͂��Ă��Ȃ����
	//------------------------------------------------
	// 0�ɖ߂葱���鏈��
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
	// �W�����v�������Ă��Ȃ��ꍇ
	//------------------------------------------------
	// 0�ɖ߂葱���鏈��
	//if (speed.y > 0.0f)
	//{
	//	speed.y -= INERTIA;

	//	if (speed.y <= 0.0f)
	//	{
	//		speed.y = 0.0f;
	//	}
	//}

	//else if (speed.y < 0.0f)
	//{
	//	speed.y += INERTIA;

	//	if (speed.y >= 0.0f)
	//	{
	//		speed.y = 0.0f;
	//	}
	//}
}

//==========================================================================================================
// �N�H�[�^�j�I����]�����֐�
//==========================================================================================================
void CPlayer::Quaternion(void)
{
	D3DXVECTOR3 moveVec = { m_pos - m_Oldpos };			// �ړ��x�N�g��

	// ���x�N�g��(�ړ��x�N�g���Ɛ����̃x�N�g��)
	D3DXVECTOR3 Axis = {0.0f,0.0f,0.0f};
	Axis.x = -moveVec.z;
	Axis.z = moveVec.x;

	if (m_Speed.x != 0.0f || m_Speed.z != 0.0f)
	{
		D3DXQUATERNION QuaAns = { 0.0f,0.0f,0.0f,1.0f };	// �P�ʃN�H�[�^�j�I��
		D3DXQuaternionRotationAxis(&QuaAns, &Axis, D3DXToDegree(m_fAngle));
		D3DXMatrixRotationQuaternion(&m_mtxRot, &QuaAns);
	}
}

//==========================================================================================================
// ���x �� ��]�̌��E
//==========================================================================================================
void CPlayer::SpeedAndRotLimit(D3DXVECTOR3 &speed, D3DXVECTOR3 &rot,const float fMaxSpeed)
{
	//==========================================================================================================
	// ��]���E
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
	// ���x���E
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
// �v���C���[�A�N�V����
//==========================================================================================================
void CPlayer::Action(void)
{
	CMouse *pMouse = CManager::GetInstance()->GetMouse();
	CInputkeyboard *pKey = CManager::GetInstance()->GetKeyboard();

	switch (m_state)
	{
	case PLAYER_STATE::BALL:
	case PLAYER_STATE::CUBE:
		// �W�����v
		if ((pMouse->GetTrigger(pMouse->MOUSE_LEFT) || CInput::PressAnyAction(CInput::ACTION_SPACE)) && !m_bJump)	// A�{�^��
		{
			m_fGravity = JUMP;
			m_bJump = true;
		}
		Gravity(m_pos, m_fGravity, m_fGravitySpeed, m_bJump);		// �d��
		break;

	case PLAYER_STATE::AIRSHIP:
		if (pMouse->GetPress(pMouse->MOUSE_LEFT) || pKey->GetPress(DIK_SPACE))
		{
			m_Speed.y += ACCELERATION;	// �㏸����
			m_bJump = true;				// �W�����v��Ԃɂ���
		}
		else
		{
			m_Speed.y -= ACCELERATION;	// ������
		}

		if (!m_bJump)
		{
			m_Speed.y = 0.0f;	// ���n���͉��������Ȃ��悤�ɃX�s�[�h0�ɂ���
		}

		m_fGravity = m_Speed.y;										// �d�͂ɃX�s�[�h����
		Gravity(m_pos, m_fGravity, m_fGravitySpeed, m_bJump);		// �d�͊֐�
		break;

	case PLAYER_STATE::UFO:
		// ���˂�
		if ((pMouse->GetTrigger(pMouse->MOUSE_LEFT) || CInput::PressAnyAction(CInput::ACTION_SPACE)))
		{
			m_fGravity = BOUNCE;
			m_bJump = true;
		}
		Gravity(m_pos, m_fGravity, m_fFallSpeed, m_bJump);		// �d��
		break;
	}


#ifdef _DEBUG
	// ���[�h�ؑ�
	if (pKey->GetTrigger(DIK_1))
	{
		SetState(PLAYER_STATE::BALL);
	}
	else if (pKey->GetTrigger(DIK_2))
	{
		SetState(PLAYER_STATE::CUBE);
	}
	else if (pKey->GetTrigger(DIK_3))
	{
		SetState(PLAYER_STATE::AIRSHIP);
	}
	else if (pKey->GetTrigger(DIK_4))
	{
		SetState(PLAYER_STATE::UFO);
	}
#endif
}

//==========================================================================================================
// �v���C���[��Ԑݒ�֐�
//==========================================================================================================
void CPlayer::SetState(PLAYER_STATE state)
{
	// ���f���I��
	ModelDelete();

	// ���f����؂�ւ���
	if (!m_pModel)
	{
		char *pStr = nullptr;

		switch (state)
		{
		case PLAYER_STATE::BALL:
			pStr = "PLAYER_ONE";
			break;

		case PLAYER_STATE::CUBE:
			pStr = "PLAYER_TWO";
			break;

		case PLAYER_STATE::AIRSHIP:
			pStr = "PLAYER_THREE";
			break;

		case PLAYER_STATE::UFO:
			pStr = "PLAYER_FOUR";
			break;
		}

		// X�t�@�C���̎�ސݒ�
		CLoadX *pLoad = CManager::GetInstance()->GetLoadX();
		m_nType = pLoad->GetNum(pStr);

		// ���f������
		m_pModel = CModel::Create({ 0.0f,0.0f,0.0f }, { 0.0f,0.0f,0.0f }, { 1.0f,1.0f,1.0f }, m_nType, false);
		// �v���C���[�T�C�Y�̐ݒ�
		CScene::SetSize(m_pModel->GetSize());
		// �ʒu����
		//m_pos.y += m_pModel->GetSize().y / 2;
		// ��]�p�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&m_mtxRot);
	}

	m_state = state;	// ��Ԑݒ�
}

//==========================================================================================================
// �d�͂̏���(�v���C���[�ɏd�͂����炷�l(�����l�͌��炵�������̐�)���������āA��O�����ɓ����)
//==========================================================================================================
void CPlayer::Gravity(D3DXVECTOR3& pos, float& fGravity, const float& fGravitySpeed, bool& bJump)
{
	// �d�͉��Z
	if (fGravitySpeed > 0.0f)
	{
		fGravity -= fGravitySpeed;
		pos.y += fGravity;
	}

	// ���ɂ������ǂ���
	if (pos.y <= 0.0f)
	{
		// �ʒu���Œ肷��
		pos.y = 0.0f;
		fGravity = 0.0f;
		bJump = false;
	}
}

//==========================================================================================================
// ���f���I������
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
// �ړ��������͊֐�
//==========================================================================================================
bool CPlayer::InputDirection(const MOVE_DIRECTION &moveDir)
{
	// �N���X�̎擾
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

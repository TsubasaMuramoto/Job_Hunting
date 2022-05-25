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
#include "Bomb.h"
#include "model_spawner.h"
#include "meshfield.h"
#include "shadow.h"
#include <assert.h>

//=============================================================================
// �}�N����`
//=============================================================================
#define PLAYER_SHADOWSIZE (D3DXVECTOR3(m_size.x * 1.5f,0.0f,m_size.z * 1.5f))

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CPlayer::CPlayer(OBJTYPE nPriority) : CScene(nPriority)
{
	// memset(m_pMotion, NULL, sizeof(m_pMotion));
	// memset(m_pBomb, NULL, sizeof(m_pBomb));

	m_Direction		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_pos			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Oldpos		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Speed			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fGravity		= 0.0f;
	m_fGravitySpeed = GRAVITY_SPEED;
	m_fMaxSpeed		= MAX_SPEED;
	m_bJump			= false;
	m_state			= STATE_NORMAL;
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
	// �ʒu�擾
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

	// �v���C���[���ݒ�
	CScene::SetPos(m_pos);
	CScene::SetSize(m_size);

	return S_OK;
}

//=============================================================================
// �I��
//=============================================================================
void CPlayer::Uninit()
{
	// �v���C���[���f���j��
	if (m_pModel)
	{
		m_pModel->Uninit();
		delete m_pModel;
		m_pModel = nullptr;
	}

	// �{���j��
	if (m_pBomb)
	{
		m_pBomb = nullptr;
	}

	// �e�̔j��
	if (m_pShadow)
	{
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

	// �J�����Ǐ]�֐��̌Ăяo��
	CManager::GetInstance()->GetCamera(0)->SetPlayerCamera(this);

	//------------------------------------------
	// �ړ��n�֐�
	//------------------------------------------
	Move();														// �ړ�
	Jump(m_fGravity, m_bJump);									// �W�����v
	Gravity(m_pos, m_fGravity, m_fGravitySpeed, m_bJump);		// �d��

	switch (m_state)
	{

	case STATE_NORMAL:
		if (CInput::PressAnyAction(CInput::ACTION_ATTACK))
		{
			//m_pBomb = CBomb::Create({ m_pos.x,m_pos.y + m_size.y,m_pos.z }, { 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }, CManager::GetInstance()->GetLoadX()->GetNum("MODTYPE_BOMB"));

			if (Carry())
			{
				m_state = STATE_HOLD;
			}
		}

		break;

	case STATE_HOLD:
		if (m_pBomb)
		{
			m_pBomb->SetPos({ m_pos.x,m_pos.y + m_size.y,m_pos.z });

			if (CInput::PressAnyAction(CInput::ACTION_ATTACK))
			{
				D3DXVECTOR3 moveVec = m_pos - m_Oldpos;
				m_pBomb->Throw(moveVec);
				m_state = STATE_NORMAL;
			}
		}
		break;
	}

	// �V�[���Ɉʒu��ݒ肷��
	CScene::SetPos(m_pos);

	if (m_pModel)	// �v���C���[���f���X�V
	{
		m_pModel->Update();
	}

	if (m_pShadow)	// �V���h�E�̈ʒu�ݒ�
	{
		m_pShadow->CScene::SetPosOld({ m_pos.x ,m_pos.y,m_pos.z });
	}
}

//=============================================================================
// �`��
//=============================================================================
void CPlayer::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice(); // �f�o�C�X�̃|�C���^
	D3DXMATRIX mtxRotModel, mtxTransModel;											 // �v�Z�p�}�g���b�N�X

	// �e�p�[�c�̃��[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	// �����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRotModel, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRotModel);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTransModel, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTransModel);

	// �e�p�[�c�̃��[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// ���f���̕`��
	m_pModel->Draw();
}

//=============================================================================
// �v���C���[�̊���
//=============================================================================
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
}

//-----------------------------------------------------------------
// �ړ�����
//-----------------------------------------------------------------
void CPlayer::Move(void)
{
	CCamera *pCamera = CManager::GetInstance()->GetCamera(0);

	//=============================================================================
	// �ړ�����(���W�E��]�X�V)
	//=============================================================================
	if (m_Speed.x != 0.0f || m_Speed.z != 0.0f)
	{
		float fLength = sqrtf((m_Speed.x * m_Speed.x) + (m_Speed.z * m_Speed.z));
		float move_x = m_Speed.x / fLength;
		float move_z = m_Speed.z / fLength;
		move_x *= m_Speed.x;
		move_z *= m_Speed.z;

		if (m_Speed.x != 0.0f)
		{
			if (m_Speed.x > 0.0f)
			{
				m_pos.x += move_x * sinf(m_rot.y);					// �ړ��̓v���C���[�̌����ɉe������
				m_pos.z += move_x * cosf(m_rot.y);
				m_rot.x += move_x * cosf(0.0f) * ROTATING_VELOCITY;
				m_rot.z += move_x * sinf(0.0f) * ROTATING_VELOCITY;
			}

			else if (m_Speed.x < 0.0f)
			{
				m_pos.x += move_x * sinf(m_rot.y);
				m_pos.z += move_x * cosf(m_rot.y);
				m_rot.x -= move_x * cosf(D3DX_PI) * ROTATING_VELOCITY;
				m_rot.z -= move_x * sinf(D3DX_PI) * ROTATING_VELOCITY;
			}

		}

		if (m_Speed.z != 0.0f)
		{
			if (m_Speed.z > 0.0f)
			{
				m_pos.x += move_z * sinf(m_rot.y);
				m_pos.z += move_z * cosf(m_rot.y);
				m_rot.x += move_z * cosf(0.0f) * ROTATING_VELOCITY;
				m_rot.z += move_z * sinf(0.0f) * ROTATING_VELOCITY;
			}

			else if (m_Speed.z < 0.0f)
			{
				m_pos.x += move_z * sinf(m_rot.y);
				m_pos.z += move_z * cosf(m_rot.y);
				m_rot.x -= move_z * cosf(D3DX_PI) * ROTATING_VELOCITY;
				m_rot.z -= move_z * sinf(D3DX_PI) * ROTATING_VELOCITY;
			}
		}

		// �ړ��̃G�t�F�N�g
		CEffect::Create(m_pos, { 5.0f, 5.0f, 0.0f }, { 1.0f, 1.0f, 1.0f ,1.0f},0.1f, 1);
	}

	//==========================================================================================================
	// �ړ����͔���
	//==========================================================================================================
	if (InputDirection(DIRECTION_FORWARD) || InputDirection(DIRECTION_BACK) || InputDirection(DIRECTION_RIGHT) || InputDirection(DIRECTION_LEFT))
	{
		//  ��
		if (InputDirection(DIRECTION_FORWARD))
		{
			m_Speed.z += ACCELERATION;
		}
		// ��O
		else if (InputDirection(DIRECTION_BACK))
		{
			m_Speed.z -= ACCELERATION;
		}
		// �E
		if (InputDirection(DIRECTION_RIGHT))
		{
			m_Speed.x += ACCELERATION;
		}
		// ��
		else if (InputDirection(DIRECTION_LEFT))
		{
			m_Speed.x -= ACCELERATION;
		}

		// �v���C���[�̊p�x���ړ��ʂ̑傫���Ōv�Z
		float Ang = atan2f(m_Speed.x,m_Speed.z);
		// �J�����̌����Ă�����Ɍ������Ă܂������ɂȂ�悤�ɃJ�����̌������A���O���ɑ���
		Ang = Ang + pCamera->GetRot().y;
		m_rot.y = Ang;
	}

	Inertia(m_Speed);								// ����
	SpeedAndRotLimit(m_Speed, m_rot, m_fMaxSpeed);	// ���x�Ɖ�]���E
}

//-----------------------------------------------------------------------------------------------
// ���x �� ��]�̌��E
//-----------------------------------------------------------------------------------------------
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

	if (speed.z >= fMaxSpeed)
	{
		speed.z = fMaxSpeed;
	}

	else if (speed.z <= -fMaxSpeed)
	{
		speed.z = -fMaxSpeed;
	}
}

//-----------------------------------------------------------------------------------------------
// �����^�ԏ���
//-----------------------------------------------------------------------------------------------
bool CPlayer::Carry(void)
{
	CScene *pSaveScene = nullptr;
	CScene *pScene = CScene::GetScene(OBJTYPE_MODEL);

	// �V�[����null�ɂȂ�܂Œʂ�
	while (pScene)
	{
		// ���̃V�[�����擾
		CScene *pSceneNext = CScene::GetSceneNext(pScene);

		if (pScene->GetModelType() == MODTYPE_BOMB)
		{
			if (m_pModel->SphereCollisionSphere(200.0f, pScene))
			{
				pSaveScene = pScene;
			}
		}

		// ���̃V�[�������݂̃V�[���ɂ���
		pScene = pSceneNext;
	}

	if (pSaveScene)
	{
		m_pBomb = (CBomb*)pSaveScene;
		return true;
	}
	return false;
}

//-----------------------------------------------------------------------------------------------
// �W�����v�̏���(�v���C���[�ɍ��W�����v�ł��邩��bool(�����l��false)���������āA�������ɓ����)
//-----------------------------------------------------------------------------------------------
void CPlayer::Jump(float &fGravity, bool& bJump)
{
	// �W�����v�̈ړ��ʂɉ��Z
	if (CInput::PressAnyAction(CInput::ACTION_SPACE) && !bJump)	// A�{�^��
	{
		fGravity = JUMP;
		bJump = true;
	}
}

//-----------------------------------------------------------------------------------------------
// �d�͂̏���(�v���C���[�ɏd�͂����炷�l(�����l�͌��炵�������̐�)���������āA��O�����ɓ����)
//-----------------------------------------------------------------------------------------------
void CPlayer::Gravity(D3DXVECTOR3& pos, float& fGravity, const float& fGravitySpeed, bool&bJump)
{
	// �d�͉��Z
	if (fGravitySpeed > 0.0f)
	{
		fGravity -= fGravitySpeed;
		pos.y += m_fGravity;
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

//-----------------------------------------------------------------------------------------------
// �ړ��������͊֐�
//-----------------------------------------------------------------------------------------------
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


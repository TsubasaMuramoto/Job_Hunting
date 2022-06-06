//=============================================================================
// �J�����̍쐬
// Author:������
//=============================================================================
#include "camera.h"
#include "renderer.h"
#include "manager.h"
#include "keyboard.h"
#include "player.h"
#include "XInput.h"
#include "DirectInput.h"
#include "mouse.h"

//==============================================================================
// �}�N����`
//==============================================================================
#define CAMERA_DISTANCE			(2000.0f)	// �J��������
#define CAMERA_DISTANCE_MIN		(50.0f)		// �J���������ŏ�
#define CAMERA_DISTANCE_MAX		(5000.0f)	// �J���������ő�
#define CAMERA_MOVE_SPEED		(10.0f)		// �J�����ړ����x
#define CAMERA_ROT_SPEED		(0.04f)		// �J������]���x(PAD)
#define MOUSE_ROT_SPEED			(0.01f)		// �J������]���x(MOUSE)
#define LONG_SPEED				(10.0f)		// �J�������߈ړ����x
#define POSR_Y_MOVE				(2.0f)		// �J���������_�ړ����x
#define POSR_Y_MOVEHEIGHT		(150.0f)	// �J���������_�������n�߂鍂��
#define _OX_EPSILON_			(0.000001f) // �덷

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CCamera::CCamera()
{

}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CCamera::~CCamera()
{

}
//================================================
// �J�����̐�������
//================================================
CCamera *CCamera::Create(D3DXVECTOR3 m_pos, D3DXVECTOR3 rot, float viewportX, float viewportY, float viewportWidth, float viewporHeight, int nID)
{
	//�C���X�^���X�̐���
	CCamera *pCamera = nullptr;
	if (!pCamera)
	{
		// ���I�������m��
		pCamera = new CCamera;

		// NULL�`�F�b�N
		if (pCamera)
		{
			pCamera->Init();	// ������
		}
	}
	return pCamera;
}

//=============================================================================
// �J�����̏���������
//=============================================================================
HRESULT CCamera::Init(void)
{
	m_fLong = CAMERA_DISTANCE;						// ���_�����_�̋���
	m_posV = D3DXVECTOR3(0.0f, 100.0f, -m_fLong);	// �J�������_
	m_posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// �J���������_
	m_vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);			// �J�����x�N�g��
	m_rot = D3DXVECTOR3(2.0f, 0.0f, 0.0f);			// �J������]
	m_bPosR_Move = false;
	return S_OK;
}

//=============================================================================
// �J�����̏I������
//=============================================================================
void CCamera::Uninit(void)
{

}

//=============================================================================
// �J�����̍X�V����
//=============================================================================
void CCamera::Update(void)
{
	// �J�����̈ړ�
	MoveCamera();
	RotLimit();
}

//================================================
// �J�����̐ݒ�
//================================================
void CCamera::SetCamera(void)
{
	LPDIRECT3DDEVICE9 pDevice;										// �f�o�C�X�̃|�C���^
	pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	// �f�o�C�X�擾

	// �r���[�|�[�g�̐ݒ�
	//pDevice->SetViewport(&m_viewport);

	// �v���W�F�N�V�����}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxProjection);

	// �v���W�F�N�V�����}�g���b�N�X���쐬
	D3DXMatrixPerspectiveFovLH
	(
		&m_mtxProjection,
		D3DXToRadian(50.0f),							// �J�����̉�p
		(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,		// ��ʔ䗦!!!�r���[�|�[�g���g���Ƃ��r���[�|�[�g�̕��������g��
		10.0f,											// ��O�̕`�搧��
		10000.0f										// ���̕`�搧��
	);							

	// �v���W�F�N�V�����}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_PROJECTION, &m_mtxProjection);

	// �r���[�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxView);

	// �r���[�}�g���b�N�X���쐬
	D3DXMatrixLookAtLH
	(
		&m_mtxView,
		&m_posV,
		&m_posR,
		&m_vecU
	);

	// �r���[�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_VIEW, &m_mtxView);
}

//===============================================================
// �J�����̈ړ�
//===============================================================
void CCamera::MoveCamera(void)
{
	// ���̓f�o�C�X�̎擾
	CInputkeyboard *pKey = CManager::GetInstance()->GetKeyboard();
	CDInput *pDInput = CManager::GetInstance()->GetDInput();
	CMouse *pMouse = CManager::GetInstance()->GetMouse();

	if (CInput::PressAnyAction(CInput::ACTION_R_DOWN))
	{	// ���։�]
		m_rot.x += -CAMERA_ROT_SPEED;
		if (m_rot.x < 0.0f + D3DX_PI / 2)
		{
			m_rot.x = 0.0f + D3DX_PI / 2 + _OX_EPSILON_;
		}
	}
	if (CInput::PressAnyAction(CInput::ACTION_R_UP))
	{	// ��։�]
		m_rot.x += CAMERA_ROT_SPEED;
		if (m_rot.x > D3DX_PI)
		{
			m_rot.x = D3DX_PI - _OX_EPSILON_;
		}
	}

	// �J�������߂Â���
	if (pDInput->GetPress(pDInput->BUTTON_R2))
	{
		m_fLong -= LONG_SPEED;
		if (m_fLong <= CAMERA_DISTANCE_MIN)
		{
			m_fLong = CAMERA_DISTANCE_MIN;
		}
	}

	// �J��������������
	else if (pDInput->GetPress(pDInput->BUTTON_L2))
	{
		m_fLong += LONG_SPEED;
		if (m_fLong >= CAMERA_DISTANCE_MAX)
		{
			m_fLong = CAMERA_DISTANCE_MAX;
		}
	}

}

//=============================================================================
// �v���C���[�ɃJ�������Ǐ]���鏈��
//=============================================================================
void CCamera::SetPlayerCamera(CPlayer *pPlayer)
{
	// NULL�`�F�b�N
	if (pPlayer)
	{
		//----------------------------------------------------------
		// �J���������_��Y���W�̒Ǐ]���s������
		//----------------------------------------------------------
		float fLong = pPlayer->GetPos().y - m_posR.y;

		if (m_bPosR_Move)	// �Ǐ]������
		{
			float fMove = (fLong / 20.0f);
			if (fMove <= 0.01f && fMove >= -0.01f)
			{
				m_bPosR_Move = false;
				m_posR.y = pPlayer->GetPos().y;
			}
			else
			{
				m_posR.y += fMove;	// Y���W�𓮂���
			}

		}

		else	// �Ǐ]���Ȃ����
		{
			if (fLong != 0.0f && !pPlayer->GetJump())
			{
				m_bPosR_Move = true;
			}
		}

		// �����_�̈ʒu���v���C���[�̈ʒu�Ƃ���(y���W�͗�O)
		m_posR.x = pPlayer->GetPos().x;
		m_posR.z = pPlayer->GetPos().z;

		// ���_�̏ꏊ���v���C���[�����Ɉړ�(�ɍ��W��])
		m_posV.x = pPlayer->GetPos().x - m_fLong * sinf(m_rot.x) * sinf(m_rot.y);
		m_posV.z = pPlayer->GetPos().z - m_fLong * sinf(m_rot.x) * cosf(m_rot.y);
		m_posV.y = pPlayer->GetPos().y - m_fLong * cosf(m_rot.x);
	}
}

//=============================================================================
// �v���C���[�ɃJ�������Ǐ]���Ă��Ȃ����̏���
//=============================================================================
void CCamera::NoFollowingPlayer(void)
{
	//****************************//
	//		   ���_�̈ړ�		  //
	//****************************//
	if (CInput::PressAnyAction(CInput::ACTION_RIGHT))		// �E
	{
		m_posR.x += CAMERA_MOVE_SPEED * cosf(-m_rot.y);
		m_posR.z += CAMERA_MOVE_SPEED * sinf(-m_rot.y);
		m_posV.x += CAMERA_MOVE_SPEED * cosf(-m_rot.y);
		m_posV.z += CAMERA_MOVE_SPEED * sinf(-m_rot.y);
	}

	else if (CInput::PressAnyAction(CInput::ACTION_LEFT))	// ��
	{
		m_posR.x -= CAMERA_MOVE_SPEED * cosf(-m_rot.y);
		m_posR.z -= CAMERA_MOVE_SPEED * sinf(-m_rot.y);
		m_posV.x -= CAMERA_MOVE_SPEED * cosf(-m_rot.y);
		m_posV.z -= CAMERA_MOVE_SPEED * sinf(-m_rot.y);
	}

	if (CInput::PressAnyAction(CInput::ACTION_UP))			// ��
	{
		m_posR.x += CAMERA_MOVE_SPEED * sinf(m_rot.y);
		m_posR.z += CAMERA_MOVE_SPEED * cosf(m_rot.y);
		m_posV.x += CAMERA_MOVE_SPEED * sinf(m_rot.y);
		m_posV.z += CAMERA_MOVE_SPEED * cosf(m_rot.y);
	}
	else if (CInput::PressAnyAction(CInput::ACTION_DOWN))	// ��O
	{
		m_posR.x -= CAMERA_MOVE_SPEED * sinf(m_rot.y);
		m_posR.z -= CAMERA_MOVE_SPEED * cosf(m_rot.y);
		m_posV.x -= CAMERA_MOVE_SPEED * sinf(m_rot.y);
		m_posV.z -= CAMERA_MOVE_SPEED * cosf(m_rot.y);
	}

	//****************************//
	//	�@	�}�E�X�ł̉�]		  //
	//****************************//
	CMouse *pMouse = CManager::GetInstance()->GetMouse();
	float CameraPosRX = (float)pMouse->GetMousePos().lX;
	float CameraPosRZ = (float)pMouse->GetMousePos().lY;

	if (CameraPosRZ > 0.0f || CameraPosRZ < -0.0f)
	{
		m_rot.x += (MOUSE_ROT_SPEED / 3) * CameraPosRZ;
	}

	if (CameraPosRX > 0.0f || CameraPosRX < -0.0f)
	{
		m_rot.y += (MOUSE_ROT_SPEED / 3) * CameraPosRX;
	}

#if(1)
	//++++++++++++++++++++++++++++++//
	//			���_�̉�]			//
	//++++++++++++++++++++++++++++++//
	if (CInput::PressAnyAction(CInput::ACTION_R_LEFT))
	{	// ���։�]
		m_rot.y += CAMERA_ROT_SPEED;
		if (m_rot.y > D3DX_PI)
		{
			m_rot.y = -D3DX_PI;
		}
	}
	if (CInput::PressAnyAction(CInput::ACTION_R_RIGHT))
	{	// �E�։�]
		m_rot.y += -CAMERA_ROT_SPEED;
		if (m_rot.y < -D3DX_PI)
		{
			m_rot.y = D3DX_PI;
		}
	}
#endif

	// ���_�̏ꏊ�𒍎��_�����Ɉړ�(�ɍ��W��])
	m_posV.x = m_posR.x - m_fLong * sinf(m_rot.x) * sinf(m_rot.y);
	m_posV.z = m_posR.z - m_fLong * sinf(m_rot.x) * cosf(m_rot.y);
	m_posV.y = m_posR.y - m_fLong * cosf(m_rot.x);
}

//=============================================================================
// �J������]���E
//=============================================================================
void CCamera::RotLimit(void)
{
	//++++++++++++++++++++++++++++++//
	//			����]			�@�@//
	//++++++++++++++++++++++++++++++//
	if (m_rot.y < -D3DX_PI)
	{
		m_rot.y = D3DX_PI;
	}
	else if (m_rot.y > D3DX_PI)
	{
		m_rot.y = -D3DX_PI;
	}

	//++++++++++++++++++++++++++++++//
	//			�c��]			�@�@//
	//++++++++++++++++++++++++++++++//
	if (m_rot.x < 0.0f)
	{
		m_rot.x = _OX_EPSILON_;
	}
	else if (m_rot.x > D3DX_PI)
	{
		m_rot.x = D3DX_PI - _OX_EPSILON_;
	}
}
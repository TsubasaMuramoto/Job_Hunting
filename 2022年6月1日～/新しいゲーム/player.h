//=============================================================
// �v���C���[�̍쐬
// Author:������
//=============================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

//=============================================================
// �C���N���[�h�t�@�C��
//=============================================================
#include "scene.h"

//=============================================================
// �}�N����`
//=============================================================


//=============================================================
// �O���錾
//=============================================================
class CModel;
class CShadow;
class CBillboard;

//=============================================================
// �N���X��`
//=============================================================
class CPlayer : public CScene
{
public:
	//------------------------------------
	// ���͏��\����
	//------------------------------------
	typedef enum
	{
		DIRECTION_FORWARD = 0,	// �O
		DIRECTION_BACK,			// ��
		DIRECTION_RIGHT,		// �E
		DIRECTION_LEFT,			// ��
		DIRECTION_MAX

	}MOVE_DIRECTION;

	//------------------------------------
	// �v���C���[��ԍ\����
	//------------------------------------
	enum class PLAYER_STATE
	{
		BALL = 0,		// �{�[��
		CUBE,			// �L���[�u
		AIRSHIP,		// ��s�@
		UFO,			// UFO
		MAX
	};

	CPlayer(OBJTYPE nPriority = CScene::OBJTYPE_PLAYER);		// �R���X�g���N�^
	~CPlayer();													// �f�X�g���N�^

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot);				// ������
	void Uninit(void);											// �I��
	void Update(void);											// �X�V
	void Draw(void);											// �`��

	// ����
	static CPlayer *Create(D3DXVECTOR3 pos,D3DXVECTOR3 rot, int nType);

	//---------------------------------------------------
	// �ړ��n�֐�
	//---------------------------------------------------
	void Move(void);																				// �ړ�
	void Inertia(D3DXVECTOR3 &speed);																// ����
	void Gravity(D3DXVECTOR3& pos, float& fGravity, const float& fGravitySpeed, bool &bJump);		// �d��
	void Action(void);																				// �A�N�V����
	bool InputDirection(const MOVE_DIRECTION &moveDir);												// �ړ��������͊֐�
	void SpeedAndRotLimit(D3DXVECTOR3 &speed, D3DXVECTOR3 &rot, const float fMaxSpeed);				// �ړ��E��]���E																			// ���e�����グ�֐�
	void Quaternion(void);																			// �N�H�[�^�j�I����]�֐�
	void ModelDelete(void);

	//---------------------------------------------------
	// �ݒ�E�擾�֐�
	//---------------------------------------------------
	D3DXVECTOR3 GetPos(void)					{ return m_pos; }
	D3DXVECTOR3 GetRot(void)					{ return m_rot; }
	D3DXVECTOR3 GetSpeed(void)					{ return m_Speed; }
	bool GetJump()								{ return m_bJump; }
	void SetUninit()							{ m_bUninit = true; }
	void SetSpeed(D3DXVECTOR3 speed)			{ m_Speed = speed; }
	void SetJump(bool bJump)					{ m_bJump = bJump; }
	void SetGravity(float fGravity,bool bJump)	{ m_fGravity = fGravity , m_bJump = bJump; }
	void SetState(PLAYER_STATE state);

private:
	const static float	m_fMaxSpeed;					// �ő�X�s�[�h
	const static float	m_fGravitySpeed;				// �d�͉����x
	const static float  m_fFallSpeed;					// �������x
	CBillboard			*m_pBillboard;					// �r���{�[�h�N���X
	CShadow				*m_pShadow;						// �V���h�E�N���X
	CModel				*m_pModel;						// ���f���N���X
	D3DXVECTOR3			m_pos,m_Oldpos;					// �ʒu�E�Ō�̈ʒu
	D3DXVECTOR3			m_rot;							// ��]
	D3DXVECTOR3			m_size;							// �T�C�Y
	D3DXVECTOR3			m_Speed, m_Direction;			// ���x�E����			
	float				m_fGravity;						// �d��
	float				m_fAngle;						// ��]����p�x
	int					m_nType;						// �^�C�v
	bool				m_bJump;						// �W�����v����
	bool				m_bSwitch;						// �J�����Ǐ]�؂�ւ�
	bool				m_bUninit;						// �I������
	D3DXMATRIX			m_mtxWorld;						// ���[���h�}�g���b�N�X
	D3DXMATRIX			m_mtxRot;						// �N�H�[�^�j�I���̌v�Z�Ɏg���}�g���b�N�X
	PLAYER_STATE		m_state;						// �v���C���[�̏��
};

//=============================================================
// ���Z�q�̃I�[�o�[���[�h
//=============================================================
//CPlayer::PLAYER_STATE& operator++(CPlayer::PLAYER_STATE& State)
//{
//	switch (State)
//	{
//	case CPlayer::PLAYER_STATE::BALL:		return State = CPlayer::PLAYER_STATE::CUBE;
//	case CPlayer::PLAYER_STATE::CUBE:		return State = CPlayer::PLAYER_STATE::AIRPLANE;
//	case CPlayer::PLAYER_STATE::AIRPLANE:	return State = CPlayer::PLAYER_STATE::BALL;
//	default:								return State = CPlayer::PLAYER_STATE::BALL;
//	}
//}
//
//CPlayer::PLAYER_STATE& operator--(CPlayer::PLAYER_STATE& State)
//{
//	switch (State)
//	{
//	case CPlayer::PLAYER_STATE::BALL:		return State = CPlayer::PLAYER_STATE::AIRPLANE;
//	case CPlayer::PLAYER_STATE::CUBE:		return State = CPlayer::PLAYER_STATE::BALL;
//	case CPlayer::PLAYER_STATE::AIRPLANE:	return State = CPlayer::PLAYER_STATE::CUBE;
//	default:								return State = CPlayer::PLAYER_STATE::BALL;
//	}
//}

#endif 

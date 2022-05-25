//--------------------------------------
// PAUSE�̏���
// Author:������
//--------------------------------------
#ifndef _PAUSE_H
#define _PAUSE_H
#include "main.h"
#include "scene.h"
#include "texture.h"

//-------------------------------------------
// �}�N����`
//-------------------------------------------

//-------------------------------------------
// �O���錾
//-------------------------------------------
class CScene2D;
class CBg;

//---------------------------------------------
// �|�[�Y�̃N���X
//---------------------------------------------
class CPause : public CScene
{
public:
	//-----------------------------------------
	// �|�[�Y�t�F�[�h�̏�ԍ\����
	//-----------------------------------------
	typedef enum
	{
		FADE_IN = 0,
		FADE_OUT,
	}PAUSE_FADE;

	//-----------------------------------------
	// �|�[�Y�̑I����ԗ�
	//-----------------------------------------
	typedef enum
	{
		SELECT_RETURNGAME = 0,
		SELECT_RETRY,
		SELECT_RETURNTITLE,
		SELECT_MAX
	}PAUSE_SELECT;

	CPause(OBJTYPE nPriority = OBJTYPE_PAUSE);
	~CPause();
	HRESULT Init(D3DXVECTOR3 pos,D3DXVECTOR3 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CPause *Create(D3DXVECTOR3 pos);			// �C���X�^���X��������

private:
	D3DXVECTOR3		m_pos;							// �ʒu
	D3DXVECTOR3		m_Speed;						// ���x
	float			m_fAlpha;						// �|�[�Y�w�i�̓����x
	int				m_nPauseType;					// �|�[�Y�I��
	bool			m_bUninit;						// pause���I�������邩
	bool			m_bNextMode;					// ���̃��[�h�ɂ�����
	bool			m_bMoveCursor;					// �J�[�\���������Ă��邩
	CScene2D		*m_pPolygon[SELECT_MAX];		// �|�[�Y�I���̃|���S��
	CScene2D		*m_pCursor;						// �I���J�[�\���̃|���S��
	CScene2D		*m_pTutorialUI;					// �������UI
	static CBg		*m_pBg;							// �|�[�Y�w�i

};


#endif //_PAUSE_H
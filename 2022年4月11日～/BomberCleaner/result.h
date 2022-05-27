//--------------------------------------
// ���U���g�̏���
//--------------------------------------
#ifndef _RESULT_H_
#define _RESULT_H_
#include "main.h"
#include "scene.h"

//--------------------------------------
// �}�N����`
//--------------------------------------
#define MAX_RESULT (4)
#define RESULT_RANKING_POSY				(80.0f)				//�����L���O��posY

//--------------------------------------
// �O���錾
//--------------------------------------
class CBg;
class CUi;
class CRanking;

//--------------------------------------
// ���U���g�N���X�̐���
//--------------------------------------
class CResult
{
public:

	//--------------------------------------
	// �I�����\����
	//--------------------------------------
	typedef enum
	{
		GAME_RETRY = 0,
		TITLE,
		MAX,
	}TYPE;

	//--------------------------------------
	// ���\�b�h
	//--------------------------------------
	CResult();
	~CResult();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	CRanking			*m_pRanking;
	CUi					*m_pUi;							
	CUi					*m_pReturnTitle;				// �^�C�g���ɖ߂�UI
	CBg					*m_pBgResult;
	int					m_nSelectType;					// ���݂̑I��
	int					m_nFrame;						// �t���[����

};


#endif //_RESULT_H_
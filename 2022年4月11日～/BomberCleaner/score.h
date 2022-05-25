//--------------------------------------------------------------
// �X�R�A�̍쐬
// Author : ������
//--------------------------------------------------------------
#ifndef _SCORE_H_
#define _SCORE_H_
#include "scene.h"

//--------------------------------------------------------------
// �O���錾
//--------------------------------------------------------------
class CNumber;

//--------------------------------------------------------------
// �}�N����`
//--------------------------------------------------------------
#define MAX_SCORE (6)			// �ő�`�文��
#define ADD_SCORE (10)			// ���Z�X�R�A

//---------------------------------------------------------------
// �X�R�A�N���X(�V�[���̔h��)
//---------------------------------------------------------------
class CScore : public CScene
{
public:
	CScore(OBJTYPE nPriority = OBJTYPE_NUMBER);
	~CScore();

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CScore	*Create(D3DXVECTOR3 pos, D3DXVECTOR3 fsize);			// �X�R�A�̐���
	void			AddScore(int AddScore) { m_nScore += AddScore; };		// �X�R�A�̉��Z

	//---------------------------------
	// Getter & Setter
	//---------------------------------
	void			SetScore(void);											// �X�R�A�̕`��ݒ�
	void			SetScore(int nScore);
	void			SetScorePos(D3DXVECTOR3 pos);
	CNumber			*GetNumber(int nNumber) { return m_apNumber[nNumber]; }
	int				GetScore(void) { return m_nScore; }

private:
	D3DXVECTOR3 m_pos;												// �ʒu
	D3DXVECTOR3 m_size;												// �T�C�Y
	CNumber *m_apNumber[MAX_SCORE];									// �X�R�A�̃|���S��
	int m_nAddScore;												// �X�R�A���Z�p�ϐ�
	int m_nAll;														// �X�R�A�|���S����
	int m_nScore;													// ���݂̃X�R�A
};
#endif

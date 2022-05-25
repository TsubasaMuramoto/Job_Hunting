//-------------------------------------------
// �L�[�{�[�h�̓��͏���
//-------------------------------------------
#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_
#include "main.h"
#include "input.h"

//-------------------------------------------
// �}�N����`
//-------------------------------------------
#define NUM_KEY_MAX (256)//�L�[�̍ő吔

//------------------------------------------
// �L�[�{�[�h�̃N���X
//------------------------------------------
class CInputkeyboard : public CInput
{
public:
	CInputkeyboard();
	~CInputkeyboard();
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit(void);
	void Update(void);
	bool GetPress(int nKey);
	bool GetTrigger(int Key);

private:
	BYTE m_aKeyState[NUM_KEY_MAX];				// �L�[�{�[�h�̓��͏��(�v���X���)
	BYTE m_aKeyStateTrigger[NUM_KEY_MAX];		// �L�[�{�[�h�̓��͏��(�g���K�[���)

};

#endif _KEYBOARD_H_

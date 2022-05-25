//---------------------
// 入力処理
//---------------------
#ifndef _INPUT_H_
#define _INPUT_H_
#include "main.h"

class CInput
{
public:
	//------------------------------------
	// タイトル画面のUI
	//------------------------------------
	typedef enum
	{
		ACTION_ENTER = 0,
		ACTION_SPACE,
		ACTION_ATTACK,
		ACTION_UP,
		ACTION_DOWN,
		ACTION_RIGHT,
		ACTION_LEFT,
		ACTION_MAX

	}ACTION;

	static bool PressAnyAction(const ACTION &action);

	CInput();
	virtual ~CInput();
	virtual HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	virtual void Uninit(void);
	virtual void Update(void) = 0;

protected:
	static LPDIRECTINPUT8			m_pInput;		// DirectInputオブジェクトのポインタ
	static LPDIRECTINPUTDEVICE8		m_pJoystick;	// ジョイスティック用デバイス
	LPDIRECTINPUTDEVICE8			m_pDevice;		// 入力デバイスへのポインタ
};

#endif //_INPUT_H_

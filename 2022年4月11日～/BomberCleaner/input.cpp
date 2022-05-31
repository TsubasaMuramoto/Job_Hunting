//====================================
// 入力処理
//====================================
#include "input.h"
#include "manager.h"
#include "keyboard.h"
#include "XInput.h"
#include "DirectInput.h"

//====================================
// マクロ定義
//====================================

//====================================
// 静的メンバ変数の初期化
//====================================
LPDIRECTINPUT8 CInput::m_pInput = nullptr;	// DirectInputオブジェクトのポインタ
LPDIRECTINPUTDEVICE8 CInput::m_pJoystick = nullptr;

//====================================
// コンストラクタ
//====================================
CInput::CInput()
{
	m_pDevice = nullptr;
}

//====================================
// デストラクタ
//====================================
CInput::~CInput()
{

}

//====================================
// 初期化処理
//====================================
HRESULT CInput::Init(HINSTANCE hInstance, HWND hWnd)
{
	// 入力の生成
	if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pInput, NULL)))
	{
		return E_FAIL;
	}
	return S_OK;
}

//====================================
// 終了処理
//====================================
void CInput::Uninit(void)
{
	// 入力デバイスの開放
	if (m_pDevice)
	{
		m_pDevice->Unacquire();
		m_pDevice->Release();
		m_pDevice = nullptr;
	}

	// 入力デバイスの開放
	if (m_pJoystick)
	{
		m_pJoystick->Unacquire();
		m_pJoystick->Release();
		m_pJoystick = nullptr;
	}

	// DirectInputオブジェクトの開放
	if (m_pInput)
	{
		m_pInput->Release();
		m_pInput = nullptr;
	}
}

bool CInput::PressAnyAction(const ACTION &action)
{
	CInputkeyboard *pKey = CManager::GetInstance()->GetKeyboard();
	CXInput *pXPad = CManager::GetInstance()->GetXInput();
	CDInput *pDPad = CManager::GetInstance()->GetDInput();

	switch (action)
	{
	case ACTION_ENTER:	// 決定・ポーズなどに使う
		return pKey->GetTrigger(DIK_RETURN) || pXPad->GetButtonTrigger(XINPUT_GAMEPAD_START) || pDPad->GetTrigger(pDPad->BUTTON_START);

	case ACTION_UP:		// 上昇など
		return pKey->GetTrigger(DIK_UP) || pXPad->GetButtonTrigger(XINPUT_GAMEPAD_DPAD_UP) || pDPad->GetGamepad().lY <= -DINPUT_STICK_RANGE ||
			pKey->GetTrigger(DIK_W) || pXPad->GetGamePad()->m_state.Gamepad.sThumbLY <= -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;

	case ACTION_DOWN:	// 下降など
		return pKey->GetTrigger(DIK_DOWN) || pXPad->GetButtonTrigger(XINPUT_GAMEPAD_DPAD_DOWN) || pDPad->GetGamepad().lY >= DINPUT_STICK_RANGE ||
			pKey->GetTrigger(DIK_S) || pXPad->GetGamePad()->m_state.Gamepad.sThumbLY >= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;
		
	case ACTION_SPACE:	// キャンセル・ジャンプなど
		return (pKey->GetTrigger(DIK_SPACE) || pXPad->GetButtonTrigger(XINPUT_GAMEPAD_A) || pDPad->GetTrigger(CDInput::BUTTON_A));

	case ACTION_ATTACK:	// 攻撃など
		return (pKey->GetTrigger(DIK_K) || pXPad->GetButtonTrigger(XINPUT_GAMEPAD_B) || pDPad->GetTrigger(CDInput::BUTTON_B));

	case ACTION_LEFT:	// 左行動
		return (pKey->GetTrigger(DIK_A) || pXPad->GetButtonTrigger(XINPUT_GAMEPAD_DPAD_LEFT) || pDPad->GetGamepad().lX <= -DINPUT_STICK_RANGE) ||
				pKey->GetTrigger(DIK_LEFT) || pXPad->GetGamePad()->m_state.Gamepad.sThumbLX <= -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;

	case ACTION_RIGHT:	// 右行動
		return (pKey->GetTrigger(DIK_D) || pXPad->GetButtonTrigger(XINPUT_GAMEPAD_DPAD_RIGHT) || pDPad->GetGamepad().lX >= DINPUT_STICK_RANGE) ||
				pKey->GetTrigger(DIK_RIGHT) || pXPad->GetGamePad()->m_state.Gamepad.sThumbLX >= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;

	case ACTION_R_UP:	// 上回転など
		return pXPad->GetGamePad()->m_state.Gamepad.sThumbRY <= -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE || pDPad->GetGamepad().lRy >= DINPUT_STICK_RANGE;

	case ACTION_R_DOWN:	// 下回転など
		return pXPad->GetGamePad()->m_state.Gamepad.sThumbRY >= XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE || pDPad->GetGamepad().lRy <= -DINPUT_STICK_RANGE;

	case ACTION_R_RIGHT: // 右回転など
		return pXPad->GetGamePad()->m_state.Gamepad.sThumbRX <= -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE || pDPad->GetGamepad().lRx <= -DINPUT_STICK_RANGE;

	case ACTION_R_LEFT:	// 左回転など
		return pXPad->GetGamePad()->m_state.Gamepad.sThumbRX >= XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE || pDPad->GetGamepad().lRx >= DINPUT_STICK_RANGE;
	}

	return false;
}



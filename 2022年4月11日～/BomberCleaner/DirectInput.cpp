//===============================================
// DirectInputの処理
// Author:村元翼
//===============================================
#include "DirectInput.h"

//===============================================
// 静的メンバ変数の初期化
//===============================================
const char *CDInput::m_aStrR = nullptr;
const char *CDInput::m_aStrL = nullptr;

//===============================================
// コンストラクタ
//===============================================
CDInput::CDInput()
{

}

//===============================================
// デストラクタ
//===============================================
CDInput::~CDInput()
{

}

//===============================================
// 初期化
//===============================================
HRESULT CDInput::Init(HINSTANCE hInstance, HWND hWnd)
{
	// 入力初期化
	CInput::Init(hInstance, hWnd);

	// デバイスの列挙(ゲームパッド)
	if (FAILED(m_pInput->EnumDevices(DI8DEVCLASS_GAMECTRL, &DeviceFindCallBack, NULL, DIEDFL_ATTACHEDONLY)))
	{
		return E_FAIL;
	}

	if (m_pJoystick != nullptr)
	{
		// 協調モードを設定
		if (FAILED(m_pJoystick->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
		{
			return E_FAIL;
		}

		m_pJoystick->GetCapabilities(&m_pJoystickCaps);

		// 軸の設定
		SetAxis(hWnd);

		// キーボードへのアクセス権を獲得(入力制御開始)
		m_pJoystick->Acquire();

		// ポーリング(定期的にデバイスの値を取得する関数)
		if (FAILED(m_pJoystick->Poll()))
		{
			return E_FAIL;
		}
	}

	return S_OK;
}

//===============================================
// 終了
//===============================================
void CDInput::Uninit()
{
	CInput::Uninit();
}

//===============================================
// 更新
//===============================================
void CDInput::Update()
{
	// ゲームパッドの入力情報の構造体
	DIJOYSTATE state;
	ZeroMemory(&state, sizeof(state));

	if (m_pJoystick != nullptr)
	{
		// ポーリング
		m_pJoystick->Poll();

		// 入力デバイスからデータを取得
		if (SUCCEEDED(m_pJoystick->GetDeviceState(sizeof(state), &state)))
		{
			PrintState();

			for (int nCntKey = 0; nCntKey < BUTTON_MAX; nCntKey++)
			{
				m_aStateTrigger.rgbButtons[nCntKey] = (m_aState.rgbButtons[nCntKey] ^ state.rgbButtons[nCntKey]) & state.rgbButtons[nCntKey];
				m_aStateRelease.rgbButtons[nCntKey] = (m_aState.rgbButtons[nCntKey] ^ state.rgbButtons[nCntKey]) & ~state.rgbButtons[nCntKey];
			}
			m_aState = state;
		}

		else
		{
			m_pJoystick->Acquire();
		}
	}
}

//=============================================================================
// ボタンのトリガー情報取得
//=============================================================================
bool CDInput::GetTrigger(int nButton)
{
	return (m_aStateTrigger.rgbButtons[nButton] & 0x80) ? true : false;
}

//=============================================================================
// ボタンのプレス情報取得
//=============================================================================
bool CDInput::GetPress(int nButton)
{
	return (m_aState.rgbButtons[nButton] & 0x80) ? true : false;
}

//=============================================================================
// ボタンのリリース情報取得
//=============================================================================
bool CDInput::GetRelease(int nButton)
{
	return (m_aStateRelease.rgbButtons[nButton] & 0x80) ? true : false;
}

//===============================================
// コールバック関数
//===============================================
BOOL CALLBACK CDInput::DeviceFindCallBack(LPCDIDEVICEINSTANCE ipddi, LPVOID pvRef)
{
	// デバイス生成
	if (FAILED(m_pInput->CreateDevice(ipddi->guidInstance, &m_pJoystick, NULL)))
	{
		m_pJoystick->Release();
		return DIENUM_STOP;
	}

	// データフォーマットを設定
	if (FAILED(m_pJoystick->SetDataFormat(&c_dfDIJoystick)))
	{
		m_pJoystick->Release();
		return DIENUM_STOP;
	}

	return DIENUM_CONTINUE;	// コントローラーが2個以上の場合はDIENUM_CONTINUE
}

BOOL CALLBACK CDInput::EnumAxesCallback(LPCDIDEVICEOBJECTINSTANCE lpddoi, LPVOID pvRef)
{
	//------------------------------------
	// 軸範囲設定
	//------------------------------------
	DIPROPRANGE diprg;								// 宣言
	ZeroMemory(&diprg, sizeof(diprg));				// 初期化
	diprg.diph.dwSize = sizeof(diprg);				// サイズ設定
	diprg.diph.dwHeaderSize = sizeof(diprg.diph);	// サイズ設定
	diprg.diph.dwHow = DIPH_BYID;					// 対象
	diprg.diph.dwObj = lpddoi->dwType;				// 対象になる軸
	diprg.lMin = -DINPUT_STICK_RANGE;				// 最小値
	diprg.lMax = DINPUT_STICK_RANGE;				// 最大値
	if (FAILED(m_pJoystick->SetProperty(DIPROP_RANGE, &diprg.diph)))
	{
		return DIENUM_STOP;
	}

	return DIENUM_CONTINUE;
}


//===============================================
// 軸設定関数
//===============================================
void CDInput::SetAxis(HWND hWnd)
{
	//------------------------------------
	// 軸モード設定
	//------------------------------------
	DIPROPDWORD diprop;								// 宣言
	ZeroMemory(&diprop, sizeof(diprop));			// サイズ初期化
	diprop.diph.dwSize = sizeof(diprop);			// 使用する構造体のサイズを設定
	diprop.diph.dwHeaderSize = sizeof(diprop.diph);	// 使用する構造体のサイズを設定
	diprop.diph.dwHow = DIPH_DEVICE;				// dwObjになんの値が入ってるかの判定
	diprop.diph.dwObj = 0;							// DIPH_DIVICEなら0を代入
	diprop.dwData = DIPROPAXISMODE_ABS;				// 絶対値モードの指定(DIPROPAXISMODE_RELにしたら相対値)

	// 軸モードを変更(設定)
	if (FAILED(m_pJoystick->SetProperty(DIPROP_AXISMODE, &diprop.diph)))
	{
		throw std::runtime_error("軸モードの設定に失敗");
	}

	// オブジェクトの列挙
	m_pJoystick->EnumObjects(EnumAxesCallback, (void*)hWnd, DIDFT_AXIS);
}

void CDInput::PrintState(void)
{
	if (m_aState.lX >= DINPUT_STICK_RANGE)
	{
		PrintLeftState("→");
	}
	else if (m_aState.lX <= -DINPUT_STICK_RANGE)
	{
		PrintLeftState("←");
	}
	else if (m_aState.lY >= DINPUT_STICK_RANGE)
	{
		PrintLeftState("↓");
	}
	else if (m_aState.lY <= -DINPUT_STICK_RANGE)
	{
		PrintLeftState("↑");
	}
	else
	{
		PrintLeftState("〇");
	}

	if (m_aState.lRx >= DINPUT_STICK_RANGE)
	{
		PrintRightState("→");
	}
	else if (m_aState.lRx <= -DINPUT_STICK_RANGE)
	{
		PrintRightState("←");
	}
	else if (m_aState.lRy >= DINPUT_STICK_RANGE)
	{
		PrintRightState("↓");
	}
	else if (m_aState.lRy <= -DINPUT_STICK_RANGE)
	{
		PrintRightState("↑");
	}
	else
	{
		PrintRightState("〇");
	}
}



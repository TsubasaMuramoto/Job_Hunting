//---------------------
//キーボードの入力処理
//---------------------
#include"mouse.h"

//----------------------------------------
//コンストラクタ
//----------------------------------------
CMouse::CMouse()
{

}
//----------------------------------------
//デストラクタ
//----------------------------------------
CMouse::~CMouse()
{

}

//----------------------------------------
//初期化処理
//----------------------------------------
HRESULT CMouse::Init(HINSTANCE hInstance, HWND hWnd)
{
	CInput::Init(hInstance, hWnd);
	if (FAILED(m_pInput->CreateDevice(GUID_SysMouse, &m_pDevice, NULL)))
	{
		return E_FAIL;
	}
	// データフォーマットを設定
	if (FAILED(m_pDevice->SetDataFormat(&c_dfDIMouse2)))
	{
		return E_FAIL;
	}

	// 協調モードを設定
	if (FAILED(m_pDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	// キーボードへのアクセス権を獲得(入力制御開始)
	m_pDevice->Acquire();

	return S_OK;

}
//----------------------------------------
//終了処理
//----------------------------------------
void CMouse::Uninit(void)
{
	CInput::Uninit();
}
//----------------------------------------
//更新処理
//----------------------------------------
void CMouse::Update(void)
{
	DIMOUSESTATE2 aKeyState;
	int nCntKey;

	//入力デバイスからデータを取得
	if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(aKeyState), &aKeyState)))
	{
		for (nCntKey = 0; nCntKey < MOUSE_MAX; nCntKey++)
		{
			m_aKeyStateTrigger.rgbButtons[nCntKey] = (m_aKeyState.rgbButtons[nCntKey] ^ aKeyState.rgbButtons[nCntKey])&aKeyState.rgbButtons[nCntKey];

			m_aKeyState = aKeyState;//キーボードの入力情報の保存
		}
	}

	else
	{
		m_pDevice->Acquire();
	}
}
//----------------------------------------
// キーボードの入力情報を取得
//----------------------------------------
bool CMouse::GetPress(int nKey)
{
	return(m_aKeyState.rgbButtons[nKey] & 0x80) ? true : false;
}

bool CMouse::GetTrigger(int nKey)
{
	return (m_aKeyStateTrigger.rgbButtons[nKey] & 0x80) ? true : false;
}
//=============================================
// キーボードの入力処理
//=============================================
#include"keyboard.h"

//=============================================
// コンストラクタ
//=============================================
CInputkeyboard::CInputkeyboard()
{

}

//=============================================
// デストラクタ
//=============================================
CInputkeyboard::~CInputkeyboard()
{

}

//=============================================
// 初期化処理
//=============================================
HRESULT CInputkeyboard::Init(HINSTANCE hInstance, HWND hWnd)
{
	// 入力初期化
	CInput::Init(hInstance, hWnd);

	// デバイス生成
	if (FAILED(m_pInput->CreateDevice(GUID_SysKeyboard, &m_pDevice, NULL)))
	{
		return E_FAIL;
	}
	// データフォーマットを設定
	if (FAILED(m_pDevice->SetDataFormat(&c_dfDIKeyboard)))
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

//=============================================
// 終了処理
//=============================================
void CInputkeyboard::Uninit(void)
{
	// 入力終了
	CInput::Uninit();
}

//=============================================
// 更新処理
//=============================================
void CInputkeyboard::Update(void)
{
	// キー入力情報変数
	BYTE aKeyState[NUM_KEY_MAX];
	int nCntKey;

	// 入力デバイスからデータを取得
	if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(aKeyState), &aKeyState[0])))
	{
		for (nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
		{
			m_aKeyStateTrigger[nCntKey] = (m_aKeyState[nCntKey] ^ aKeyState[nCntKey])&aKeyState[nCntKey];

			m_aKeyState[nCntKey] = aKeyState[nCntKey];	// キーボードの入力情報の保存
		}
	}

	else
	{
		m_pDevice->Acquire();
	}
}

//=============================================
// キーボードの入力情報を取得(プレス(長押し))
//=============================================
bool CInputkeyboard::GetPress(int nKey)
{
	return(m_aKeyState[nKey] & 0x80) ? true : false;
}

//=============================================
// キーボードの入力情報を取得(トリガー(単体押し))
//=============================================
bool CInputkeyboard::GetTrigger(int nKey)
{
	return (m_aKeyStateTrigger[nKey] & 0x80) ? true : false;
}
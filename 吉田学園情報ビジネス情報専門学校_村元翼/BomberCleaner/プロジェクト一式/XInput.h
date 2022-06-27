//---------------------
// Xinput入力処理
//---------------------
#ifndef _XInput_H_
#define _XInput_H_
#include "main.h"
#include <XInput.h>

//-----------------------------------------------------------------------------
// マクロ定義
//-----------------------------------------------------------------------------
#define XINPUT_BUTTON_MAX		(32)
#define MAX_GAMECONTROLLER		(1)						// ゲームパッドの最大数
#define VIBRATION_POWER_LEFT	(32000)					// コントローラーの左モーター振動のつよさ 0-65535
#define VIBRATION_POWER_RIGHT	(16000)					// コントローラーの右モーター振動のつよさ 0-65535
#define VIBRATION_POWER_MAX		(65535)					// 最大振動
//#define XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE  (7849)	// 左スティック入力の最大値と最小値-32768〜32767
//#define XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE (8689)	// 右スティック入力の最大値と最小値-32768〜32767

//-----------------------------------------------------------------------------
// クラス定義
//-----------------------------------------------------------------------------
class CXInput
{
public:
	typedef struct
	{
		DWORD m_wButtons[XINPUT_BUTTON_MAX];	// ボタンの入力情報（プレス情報）
		XINPUT_STATE m_state;					// ボタンの状態
		XINPUT_STATE m_stateLast;				// ボタンの前の状態
		XINPUT_VIBRATION m_vibration;			// コントローラーのバイブレーション

	} GAMEPAD;

	CXInput();
	~CXInput();

	void UpdateGamepad(void);			// 更新
	bool GetGamepadPress(int nButton);	// プレス情報
	bool GetButtonTrigger(int nButton);	// トリガー情報
	GAMEPAD *GetGamePad(void);			// ゲームパッドの情報を取得

private:
	GAMEPAD	m_GamePad;					// ゲームパッドクラス
	XINPUT_STATE m_laststate;			// 最後に保存したステート

};
#endif _XInput_H_

//===============================================
// DirectInputの処理
// Author:村元翼
//===============================================
#ifndef _DIRECTINPUT_H_
#define _DIRECTINPUT_H_

//===============================================
// インクルードファイル
//===============================================
#include "main.h"
#include "input.h"

//===============================================
// マクロ定義
//===============================================
#define MAX_GAMEPAD (1)				// ゲームパッドの数
#define DINPUT_STICK_RANGE (1000)	// スティックの傾き範囲

//===============================================
// 前方宣言
//===============================================

//===============================================
// DirectInputクラスの定義
//===============================================
class CDInput : public CInput
{
public:
	//------------------------------
	// ボタン列挙
	//------------------------------
	typedef enum
	{
		BUTTON_A = 0,			// Aボタン
		BUTTON_B,				// Bボタン
		BUTTON_Y,				// Yボタン
		BUTTON_X,				// Xボタン
		BUTTON_L2,				// L2ボタン
		BUTTON_R2,				// R2ボタン
		BUTTON_L,				// Lボタン
		BUTTON_R,				// Rボタン
		BUTTON_START,			// STARTボタン
		BUTTON_SELECT,			// SELECTボタン
		BUTTON_L3,				// L3ボタン
		BUTTON_R3,				// R3ボタン
		BUTTON_BUTTOM,			// BUTTOMボタン
		BUTTON_HOME,			// HOMEボタン
		BUTTON_MAX				// ボタン数

	}DINPUT_BUTTON;

	CDInput();												// コンストラクタ
	~CDInput();												// デストラクタ

	HRESULT Init(HINSTANCE hInstance, HWND hWnd);			// 初期化
	void Uninit(void);										// 終了
	void Update(void);										// 更新

	DIJOYSTATE GetGamepad(void)		{ return m_aState; }	// ジョイパッド状態の取得
	bool GetPress(int nButton);								// 長押し入力情報の取得
	bool GetTrigger(int nButton);							// 単押し入力情報の取得
	bool GetRelease(int nButton);							// 離す入力情報の取得

	void SetAxis(HWND hWnd);								// 軸設定関数

	static BOOL CALLBACK DeviceFindCallBack(const LPCDIDEVICEINSTANCE ipddi, LPVOID pvRef);			// DInput列挙コールバック
	static BOOL CALLBACK EnumAxesCallback(const LPCDIDEVICEOBJECTINSTANCE lpddoi, LPVOID pvRef);	// DInput軸列挙コールバック

	void PrintState(void);
	static void PrintLeftState(const char *aStr)	{ m_aStrL = aStr; }
	static void PrintRightState(const char *aStr)	{ m_aStrR = aStr; }
	static const char *GetStateR()					{ return m_aStrR; }
	static const char *GetStateL()					{ return m_aStrL; }

private:
	DIDEVCAPS m_pJoystickCaps;		// 
	DIJOYSTATE m_aState;			// ゲームパッドの入力情報（プレス情報）
	DIJOYSTATE m_aStateTrigger;		// ゲームパッドの入力情報 (トリガー)
	DIJOYSTATE m_aStateRelease;		// ゲームパッドの入力情報 (リリース)
	const static char *m_aStrR;
	const static char *m_aStrL;

};
#endif //_DIRECTINPUT_H_
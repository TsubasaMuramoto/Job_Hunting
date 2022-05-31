//-------------------------------------------
// キーボードの入力処理
// Author:村元翼
//-------------------------------------------
#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_
#include "main.h"
#include "input.h"

//-------------------------------------------
// マクロ定義
//-------------------------------------------
#define NUM_KEY_MAX (256)//キーの最大数

//------------------------------------------
// キーボードのクラス
//------------------------------------------
class CInputkeyboard : public CInput
{
public:
	CInputkeyboard();									// コンストラクタ
	~CInputkeyboard();									// デストラクタ

	HRESULT Init(HINSTANCE hInstance, HWND hWnd);		// 初期化
	void Uninit(void);									// 終了
	void Update(void);									// 更新

	bool GetPress(int nKey);							// プレス情報取得
	bool GetTrigger(int Key);							// トリガー情報取得

private:
	BYTE m_aKeyState[NUM_KEY_MAX];				// キーボードの入力情報(プレス情報)
	BYTE m_aKeyStateTrigger[NUM_KEY_MAX];		// キーボードの入力情報(トリガー情報)

};
#endif

//=============================================================
// プレイヤーの作成
// Author:村元翼
//=============================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

//=============================================================
// インクルードファイル
//=============================================================
#include "scene.h"

//=============================================================
// マクロ定義
//=============================================================


//=============================================================
// 前方宣言
//=============================================================
class CModel;
class CShadow;
class CBillboard;

//=============================================================
// クラス定義
//=============================================================
class CPlayer : public CScene
{
public:
	//------------------------------------
	// 入力情報構造体
	//------------------------------------
	typedef enum
	{
		DIRECTION_FORWARD = 0,	// 前
		DIRECTION_BACK,			// 後
		DIRECTION_RIGHT,		// 右
		DIRECTION_LEFT,			// 左
		DIRECTION_MAX

	}MOVE_DIRECTION;

	//------------------------------------
	// プレイヤー状態構造体
	//------------------------------------
	enum class PLAYER_STATE
	{
		BALL = 0,		// ボール
		CUBE,			// キューブ
		AIRSHIP,		// 飛行機
		UFO,			// UFO
		MAX
	};

	CPlayer(OBJTYPE nPriority = CScene::OBJTYPE_PLAYER);		// コンストラクタ
	~CPlayer();													// デストラクタ

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot);				// 初期化
	void Uninit(void);											// 終了
	void Update(void);											// 更新
	void Draw(void);											// 描画

	// 生成
	static CPlayer *Create(D3DXVECTOR3 pos,D3DXVECTOR3 rot, int nType);

	//---------------------------------------------------
	// 移動系関数
	//---------------------------------------------------
	void Move(void);																				// 移動
	void Inertia(D3DXVECTOR3 &speed);																// 慣性
	void Gravity(D3DXVECTOR3& pos, float& fGravity, const float& fGravitySpeed, bool &bJump);		// 重力
	void Action(void);																				// アクション
	bool InputDirection(const MOVE_DIRECTION &moveDir);												// 移動方向入力関数
	void SpeedAndRotLimit(D3DXVECTOR3 &speed, D3DXVECTOR3 &rot, const float fMaxSpeed);				// 移動・回転限界																			// 爆弾持ち上げ関数
	void Quaternion(void);																			// クォータニオン回転関数
	void ModelDelete(void);

	//---------------------------------------------------
	// 設定・取得関数
	//---------------------------------------------------
	D3DXVECTOR3 GetPos(void)					{ return m_pos; }
	D3DXVECTOR3 GetRot(void)					{ return m_rot; }
	D3DXVECTOR3 GetSpeed(void)					{ return m_Speed; }
	bool GetJump()								{ return m_bJump; }
	void SetUninit()							{ m_bUninit = true; }
	void SetSpeed(D3DXVECTOR3 speed)			{ m_Speed = speed; }
	void SetJump(bool bJump)					{ m_bJump = bJump; }
	void SetGravity(float fGravity,bool bJump)	{ m_fGravity = fGravity , m_bJump = bJump; }
	void SetState(PLAYER_STATE state);

private:
	const static float	m_fMaxSpeed;					// 最大スピード
	const static float	m_fGravitySpeed;				// 重力加速度
	const static float  m_fFallSpeed;					// 落下速度
	CBillboard			*m_pBillboard;					// ビルボードクラス
	CShadow				*m_pShadow;						// シャドウクラス
	CModel				*m_pModel;						// モデルクラス
	D3DXVECTOR3			m_pos,m_Oldpos;					// 位置・最後の位置
	D3DXVECTOR3			m_rot;							// 回転
	D3DXVECTOR3			m_size;							// サイズ
	D3DXVECTOR3			m_Speed, m_Direction;			// 速度・向き			
	float				m_fGravity;						// 重力
	float				m_fAngle;						// 回転する角度
	int					m_nType;						// タイプ
	bool				m_bJump;						// ジャンプ判定
	bool				m_bSwitch;						// カメラ追従切り替え
	bool				m_bUninit;						// 終了判定
	D3DXMATRIX			m_mtxWorld;						// ワールドマトリックス
	D3DXMATRIX			m_mtxRot;						// クォータニオンの計算に使うマトリックス
	PLAYER_STATE		m_state;						// プレイヤーの状態
};

//=============================================================
// 演算子のオーバーロード
//=============================================================
//CPlayer::PLAYER_STATE& operator++(CPlayer::PLAYER_STATE& State)
//{
//	switch (State)
//	{
//	case CPlayer::PLAYER_STATE::BALL:		return State = CPlayer::PLAYER_STATE::CUBE;
//	case CPlayer::PLAYER_STATE::CUBE:		return State = CPlayer::PLAYER_STATE::AIRPLANE;
//	case CPlayer::PLAYER_STATE::AIRPLANE:	return State = CPlayer::PLAYER_STATE::BALL;
//	default:								return State = CPlayer::PLAYER_STATE::BALL;
//	}
//}
//
//CPlayer::PLAYER_STATE& operator--(CPlayer::PLAYER_STATE& State)
//{
//	switch (State)
//	{
//	case CPlayer::PLAYER_STATE::BALL:		return State = CPlayer::PLAYER_STATE::AIRPLANE;
//	case CPlayer::PLAYER_STATE::CUBE:		return State = CPlayer::PLAYER_STATE::BALL;
//	case CPlayer::PLAYER_STATE::AIRPLANE:	return State = CPlayer::PLAYER_STATE::CUBE;
//	default:								return State = CPlayer::PLAYER_STATE::BALL;
//	}
//}

#endif 

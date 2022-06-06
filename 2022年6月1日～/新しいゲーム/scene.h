//==================================================
// scene.hの作成
// Author:村元翼
//==================================================
#ifndef _SCENE_H_
#define _SCENE_H_

//==================================================
// インクルードファイル
//==================================================
#include "main.h"

//==================================================
// オブジェクトクラスの生成
//==================================================
class CScene
{
public:
	//-----------------------------
	// オブジェクトタイプ構造体
	//-----------------------------
	typedef enum
	{
		OBJTYPE_NONE = 0,		// 無し
		OBJTYPE_BG,				// 背景
		OBJTYPE_NUMBER,			// ナンバー
		OBJTYPE_POLYGON2D,		// 2Dポリゴン
		OBJTYPE_POLYGON3D,		// 3Dポリゴン
		OBJTYPE_MESH,			// メッシュ
		OBJTYPE_MODEL,			// モデル
		OBJTYPE_PLAYER,			// プレイヤー
		OBJTYPE_EFFECT,			// エフェクト
		OBJTYPE_EFFECTMESH,		// エフェクトメッシュ
		OBJTYPE_BILLBOARD,		// ビルボード
		OBJTYPE_SHADOW,			// シャドウ
		OBJTYPE_UI,				// UI
		OBJTYPE_PAUSE,			// ポーズ
		OBJTYPE_FADE,			// フェード
		OBJTYPE_MAX				

	}OBJTYPE;

	//-----------------------------
	// モデルタイプ構造体
	//-----------------------------
	typedef enum
	{
		MODTYPE_NORMAL = 0,			// 通常
		MODTYPE_GOAL,				// ゴール
		MODTYPE_OBSTACLE,			// 障害物
		MODTYPE_TARGET,				// 対象
		MODTYPE_PLAYER,				// プレイヤー
		MODTYPE_MAX					

	}MODTYPE;

	//------------------------------------
	// メッシュタイプ列挙
	//------------------------------------
	typedef enum
	{
		MESH_FIELD = 0,				// フィールド
		MESH_WALL,					// ウォール
		MESH_CYLINDER,				// シリンダー
		MESH_SPHERE,				// スフィア
		MESH_MAX

	}MESH_TYPE;

	CScene();														// デフォルトコンストラクタ
	explicit CScene(OBJTYPE nPriority);								// オーバーロードコンストラクタ
	virtual ~CScene();												// デストラクタ

	// 純粋仮想関数
	virtual HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size) = 0;	// ポリゴンの初期化
	virtual void Uninit(void) = 0;									// ポリゴンの終了
	virtual void Update(void) = 0;									// ポリゴンの更新
	virtual void Draw(void) = 0;									// ポリゴンの描画

	static void UpdateAll();										// 全てのポリゴンの更新
	static void DrawAll();											// 全てのポリゴンの描画
	static void ReleaseAll();										// 全てのポリゴンの解放

	//-----------------------------
	// 設定・取得関数
	//-----------------------------
	void SetPos(D3DXVECTOR3 pos)			{ m_pos = pos; }					// 位置の設定
	void SetPosOld(D3DXVECTOR3 posOld)		{ m_posOld = posOld; }				// 最後の位置の設定
	void SetSize(D3DXVECTOR3 size)			{ m_size = size; }					// 大きさの設定
	void SetObjType(OBJTYPE objType)		{ m_objType = objType; }			// オブジェクトタイプの設定
	void SetTex(D3DXVECTOR2 tex)			{ m_Tex = tex; }					// テクスチャの大きさ設定
	void SetCol(D3DXCOLOR col)				{ m_col = col; }					// カラーの設定
	void SetModelType(MODTYPE ModTp)		{ m_ModelType = ModTp; }	// モデルタイプ設定

	static int	GetNumObj(int nPriority)	{ return m_nNumAll[nPriority]; }		// オブジェクト数の取得
	D3DXVECTOR3 GetPos(void)				{ return m_pos; }						// 位置の取得
	D3DXVECTOR3 GetPosOld(void)				{ return m_posOld; }
	D3DXVECTOR3 GetSize(void)				{ return m_size; }						// 大きさの取得
	D3DXVECTOR2 GetTex(void)				{ return m_Tex; }						// テクスチャの大きさ取得
	OBJTYPE		GetObjType(void)			{ return m_objType; }					// オブジェクトタイプの取得
	D3DXCOLOR	GetCol(void)				{ return m_col; }						// カラーの取得
	MODTYPE		GetModelType()				{ return m_ModelType; }					// モデルタイプの取得
	bool		GetDeath()					{ return m_bDeath; }					// 死亡判定の取得

	static CScene *GetScene(const int nPriority)	{ return m_pTop[nPriority]; }	// シーン取得
	static CScene *GetSceneNext(CScene *pTop)		{ return pTop->m_pNext; }		// 次シーン取得

protected:
	void Release(); 															// 自身の破棄

private:
	static CScene	*m_pTop[OBJTYPE_MAX];					// 先頭のオブジェクトのポインタ(オブジェクトの数分)
	static CScene	*m_pCur[OBJTYPE_MAX];					// 現在(一番後ろ)のオブジェクトのポインタ(オブジェクトの数分)
	CScene			*m_pPrev;								// 前のオブジェクトへのポインタ
	CScene			*m_pNext;								// 次のオブジェクトへのポインタ

	// ↑を追加することで↓のm_apScene(配列)が必要なくなる
	// static CScene *m_apScene[MAX_POLYGON];				// オブジェクトの格納場所
	static int		m_nNumAll[OBJTYPE_MAX];					// オブジェクトの最大数(オブジェクトの数分)

	int				m_nPriority;							// 優先順位
	bool			m_bDeath;								// 死亡フラグ
	D3DXVECTOR3		m_pos,m_posOld;							// 位置
	D3DXVECTOR3		m_size;									// スケール
	D3DXCOLOR		m_col;									// カラー
	D3DXVECTOR2		m_Tex;									// テクスチャ
	OBJTYPE			m_objType;								// オブジェクトタイプの変数
	MODTYPE			m_ModelType;							// モデルの種類
};

#endif
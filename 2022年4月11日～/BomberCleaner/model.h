//========================================================
// モデルの処理
// 作成者：村元翼
//========================================================
#ifndef _MODEL_H
#define _MODEL_H

//========================================================
// インクルードファイル
//========================================================
#include "main.h"
#include "scene.h"

//========================================================
// マクロ定義
//========================================================
#define MAX_MODEL_VTX		(8)		// モデルの頂点数
#define SURFACE_NUM			(6)		// キューブ体の面の数

//========================================================
// 前方宣言
//========================================================

//========================================================
// クラスの定義
//========================================================
class CModel
{
public:
	//---------------------------------------
	// どこから衝突しているか
	//---------------------------------------
	typedef enum
	{
		FROM_UP = 0,
		FROM_DOWN,
		FROM_RIGHT,
		FROM_LEFT,
		FROM_FRONT,
		FROM_BACK,
		FROM_MAX

	}HIT_FROM;

	//---------------------------------------
	// 衝突判定の種類
	//---------------------------------------
	typedef enum
	{
		TYPE_DOT = 0,
		TYPE_LINE,
		TYPE_SPHERE,
		TYPE_CUBE,
		TYPE_MAX

	}HIT_TYPE;

	//---------------------------------------
	// モデルの各頂点の保存用構造体
	//---------------------------------------
	typedef struct
	{
		D3DXVECTOR3 vtx,vtxWorld, rot;					// 頂点・回転
		D3DXMATRIX mtxWorld;							// ワールドマトリックス
	}ModelVtx;

	CModel();											// コンストラクタ
	~CModel();											// デストラクタ
	void Init(int nXType);								// 初期化
	void Uninit(void);									// 終了
	void Update(void);									// 更新
	void Draw(void);									// 描画

	// 生成
	static CModel *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, int nXType,bool bCollision = true);

	//------------------------------------
	// 当たり判定系
	//------------------------------------
	bool CModel::CalcParticlePlaneCollision(FLOAT r, D3DXVECTOR3 *pPre_pos, D3DXVECTOR3 *pPos, D3DXVECTOR3 *pNormal, D3DXVECTOR3 *pPlane_pos, FLOAT *t, D3DXVECTOR3 *pOut_colli);
	bool SurfaceCollisionSphere(CScene *pScene);															// 面と球の当たり判定
	bool SphereCollisionSphere(float fRadius,CScene *pScene);												// 球と球の当たり判定
	bool DotCollisionCube(CScene *pScene, const HIT_TYPE &hit_type);										// 点と直方体の当たり判定
	bool LineCollisionCube(CScene *pScene, const HIT_TYPE &hit_type);										// 線と直方体の当たり判定
	bool HitFrom(const HIT_FROM &hitFrom, const float *fDot, const float *fDotOld, const float &ObjRad);	// 範囲内に当たっているか(引数：タイプ,内積,1フレーム前の内積)
	D3DXVECTOR3 PushDistanceSide(const D3DXVECTOR3 &moveVec, const D3DXVECTOR3 &vecNor);					// 横判定時の押し出す距離を求める

	//---------------------------------------
	// 設定・取得関数
	//---------------------------------------
	void SetParent(CModel *pModel)				{ m_pParent = pModel; }			// 親のモデルを代入
	void SetPos(D3DXVECTOR3 pos)				{ m_pos = pos; }				// 位置設定
	void SetRot(D3DXVECTOR3 rot)				{ m_rot = rot; }				// 回転設定
	void SetScale(D3DXVECTOR3 scale)			{ m_scale = scale; }			// スケール設定
	void SetLayerPos(D3DXVECTOR3 pos)			{ m_layerpos = pos; }			// レイヤー位置設定
	void SetLayerRot(D3DXVECTOR3 rot)			{ m_layerrot = rot; }			// レイヤー回転設定
	void SetEmissive(D3DCOLORVALUE Emissive)	{ m_SaveEmissive = Emissive; }	// 拡散設定
	void SetDraw(bool bDraw)					{ m_bDraw = bDraw; }			// 描画判定
	void SetNumParent(int Parent)				{ m_nNumParent = Parent; }		// 親番号設定
	void SetVtxScale(float fScale);												// モデル頂点のスケール変更

	D3DXMATRIX	GetMatrix()				{ return m_mtxWorld; }				// ワールドマトリックスの取得
	D3DXVECTOR3 GetPos()				{ return m_pos; }					// 位置取得
	D3DXVECTOR3 GetRot()				{ return m_rot; }					// 回転取得
	D3DXVECTOR3 GetLayerPos()			{ return m_layerpos; }				// レイヤー位置取得
	D3DXVECTOR3 GetLayerRot()			{ return m_layerrot; }				// レイヤー回転取得
	D3DXVECTOR3 GetMinPos()				{ return m_vtxMin; }				// 頂点最小位置取得
	D3DXVECTOR3 GetMaxPos()				{ return m_vtxMax; }				// 頂点最大位置取得
	D3DXVECTOR3 GetSize()				{ return m_size; }					// サイズ取得
	D3DXVECTOR3 GetSizeHalf()			{ return m_size / 2; }				// サイズ半径取得
	D3DXVECTOR3 GetScale()				{ return m_scale; }					// スケール取得
	int			GetNumParent(void)		{ return m_nNumParent; }			// 親番号取得
	ModelVtx	GetVtx(int nNumVtx)		{ return m_vtx[nNumVtx]; }			// 各モデルの頂点情報取得
	D3DXMATRIX	GetMtxWorld(void)		{ return m_mtxWorld; }				// ワールドマトリックス取得
	D3DXMATRIX *GetVtxMtxWorld(void)	{ return &m_vtx[0].mtxWorld; }		// 頂点ワールドマトリックス取得
	bool		GetHitBool()			{ return m_bHit; }					// 当たった情報を取得

private:
	LPD3DXMESH					m_pMesh;					// メッシュ（頂点情報）へのポインタ
	LPD3DXBUFFER				m_pBuffMat;					// マテリアル（材質情報）へのポインタ
	vector<LPDIRECT3DTEXTURE9>	m_pTexture;					// テクスチャ
	DWORD						m_nNumMat;					// マテリアルの数
	D3DXVECTOR3					m_pos,m_layerpos;			// モデルの位置（オフセット）
	D3DXVECTOR3					m_scale, m_size;			// モデルの大きさ、規模(比率)
	D3DXVECTOR3					m_rot, m_layerrot;			// 向き
	ModelVtx					m_vtx[MAX_MODEL_VTX];		// 各頂点の情報格納構造体
	D3DXVECTOR3					m_vtxMin, m_vtxMax;			// モデルの最小値、最大値
	D3DXMATRIX					m_mtxWorld;					// ワールドマトリックス
	D3DXMATERIAL				*m_pMat;					// マテリアルへのポインタ
	D3DXCOLOR					m_SaveEmissive;				// 発光色の保存用変数
	DWORD						m_sizeFVF;					// 頂点フォーマットのサイズ
	BYTE						*m_pVtxBuff;				// 頂点バッファのポインタ
	LPDIRECT3DVERTEXBUFFER9		m_pVtxData;					// 頂点バッファへのポインタの情報

	int							m_nNumVtx;					// 頂点の数
	int							m_nNumParent;				// 現在の親モデルの番号
	CModel						*m_pParent;					// 親モデルのポインタ
	bool						m_bDraw;					// 描画するか
	bool						m_bDoOnce;					// 一回だけ通る
	bool						m_bCollision;				// 当たり判定を行うか
	bool						m_bHit;						// 当たったか
};


#endif

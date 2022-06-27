//===========================================
// モデルを配置するための処理
//===========================================
#ifndef _MODEL_SPAWNER_H
#define _MODEL_SPAWNER_H

//===========================================
// インクルードファイル
//===========================================
#include "main.h"
#include "scene.h"

//===========================================
// 前方宣言
//===========================================
class CModel;

//===========================================
// クラスの定義
//===========================================
class CModel_Spawner : public CScene
{
public:
	CModel_Spawner(OBJTYPE nPriority = OBJTYPE_MODEL);	// コンストラクタ
	~CModel_Spawner();									// デストラクタ

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot);		// 初期化
	void Uninit(void);									// 終了
	void Update(void);									// 更新
	void Draw(void);									// 描画

	// 生成
	static CModel_Spawner *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, int nXType);//rotは角度を入れて

	//-----------------------------------------------
	// 設定・取得処理
	//-----------------------------------------------
	CModel *GetModel(void)			{ return m_pModel; }	// モデル情報の取得
	void SetRot(float fRot)			{ m_rot.y = fRot; }		// Y軸回転情報の設定
	void SetSRot(D3DXVECTOR3 rot)	{ m_rot = rot; }		// 回転情報の設定
	void SetSPos(D3DXVECTOR3 pos)	{ m_pos = pos; }		// 位置情報の設定

private:
	float m_fRot;									// 回転
	D3DXVECTOR3				m_pos;					// モデルの位置（オフセット）
	D3DXVECTOR3				m_rot;					// 向き
	D3DXMATRIX				m_mtxWorld;				// ワールドマトリックス
	D3DXMATERIAL			*m_pMat;				// マテリアルへのポインタ
	CModel					*m_pModel;				// モデルクラス
};
#endif

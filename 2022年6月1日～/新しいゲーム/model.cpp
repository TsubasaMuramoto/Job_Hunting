//=============================================================================
// ポリゴンの処理
// 作成者 : 村元翼
//=============================================================================
#include "model.h"
#include "Scene3D.h"
#include "scene.h"
#include "input.h"
#include "renderer.h"
#include "manager.h"
#include "player.h"
#include "LoadX.h"
#include "shadow.h"
#include <assert.h>

//=============================================================================
// コンストラクタ
//=============================================================================
CModel::CModel()
{
	memset(m_vtx, NULL, sizeof(m_vtx));
	m_pMesh			= nullptr;						// メッシュ（頂点情報）へのポインタ
	m_nNumVtx		= 0;							// 頂点の数
	m_vtxMin		= { 0.0f, 0.0f, 0.0f };			// モデルの最小値
	m_vtxMax		= { 0.0f, 0.0f, 0.0f };			// モデルの最大値
	m_pos			= { 0.0f, 0.0f, 0.0f };			// モデルの位置（オフセット）
	m_rot			= { 0.0f, 0.0f, 0.0f };			// モデルの向き
	m_size			= { 0.0f, 0.0f, 0.0f };			// モデルの大きさ
	m_scale			= { 0.0f, 0.0f, 0.0f };			// モデルの規模
	m_SaveEmissive	= { 0.0f, 0.0f, 0.0f,1.0f };	// 発光色保存
	m_bDraw			= false;						// 描画するか					
	m_bDoOnce		= false;						// 一回のみ通る処理
}

//=============================================================================
// デストラクタ
//=============================================================================
CModel::~CModel()
{

}

//=============================================================================
// インスタンス生成処理
//=============================================================================
CModel *CModel::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, int nXType, bool bCollision)
{
	//インスタンス生成
	CModel *pModel = nullptr;
	if (!pModel)
	{
		pModel = new CModel;
		if (pModel)
		{
			pModel->m_pos = pos;							// 位置
			pModel->m_rot = rot;							// 回転
			pModel->m_scale = scale;						// 規模
			pModel->m_bCollision = bCollision;				// 当たり判定をつけるか
			pModel->Init(nXType);							// 初期化
		}
	}
	return pModel;
}


//=============================================================================
// モデルの初期化処理
//=============================================================================
void CModel::Init(int nXType)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	// デバイスのポインタ

	// Xファイルデータ取得
	CLoadX::XData pXData = CManager::GetInstance()->GetLoadX()->GetXData(nXType);
	m_pBuffMat		= pXData.m_pBuffMat;
	m_pMesh			= pXData.m_pMesh;
	m_nNumMat		= pXData.m_nNumMat;
	m_pMat			= pXData.m_pMat;
	m_pTexture		= pXData.m_pTex;
	m_SaveEmissive	= pXData.m_pMat->MatD3D.Emissive;

	// 頂点数を取得
	m_nNumVtx = m_pMesh->GetNumVertices();
	// 頂点フォーマットのサイズを取得
	m_sizeFVF = D3DXGetFVFVertexSize(m_pMesh->GetFVF());
	// 頂点バッファをロック
	m_pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&m_pVtxBuff);
	//モデルの大きさを測る
	for (int nCnt = 0; nCnt < m_nNumVtx; nCnt++)
	{
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)m_pVtxBuff;	// 頂点座標の代入
		
		// xの最大値の比較
		if (m_vtxMax.x <= vtx.x)
		{
			m_vtxMax.x = vtx.x;
		}
		// xの最小値の比較
		if (m_vtxMin.x >= vtx.x)
		{
			m_vtxMin.x = vtx.x;
		}
		// yの最大値の比較
		if (m_vtxMax.y <= vtx.y)
		{
			m_vtxMax.y = vtx.y;
		}
		// yの最小値の比較
		if (m_vtxMin.y >= vtx.y)
		{
			m_vtxMin.y = vtx.y;
		}
		// zの最大値の比較
		if (m_vtxMax.z <= vtx.z)
		{
			m_vtxMax.z = vtx.z;
		}
		// zの最小値の比較
		if (m_vtxMin.z >= vtx.z)
		{
			m_vtxMin.z = vtx.z;
		}

		// xyzの最大値と最小値を引き、サイズにする
		m_size.x = m_vtxMax.x - m_vtxMin.x;
		m_size.y = m_vtxMax.y - m_vtxMin.y;
		m_size.z = m_vtxMax.z - m_vtxMin.z;

		m_pVtxBuff += m_sizeFVF;	// 頂点フォーマットのサイズ分ポインタを進める
	}

	// 頂点バッファをアンロック
	m_pMesh->UnlockVertexBuffer();

	// 8つの頂点情報の保存
	// 左奥（上面）
	m_vtx[0].vtx.x = m_vtxMin.x;
	m_vtx[0].vtx.y = m_vtxMax.y;
	m_vtx[0].vtx.z = m_vtxMax.z;

	// 右奥（上面）
	m_vtx[1].vtx.x = m_vtxMax.x;
	m_vtx[1].vtx.y = m_vtxMax.y;
	m_vtx[1].vtx.z = m_vtxMax.z;
		
	// 左前（上面）
	m_vtx[2].vtx.x = m_vtxMin.x;
	m_vtx[2].vtx.y = m_vtxMax.y;
	m_vtx[2].vtx.z = m_vtxMin.z;

	// 右前（上面）
	m_vtx[3].vtx.x = m_vtxMax.x;
	m_vtx[3].vtx.y = m_vtxMax.y;
	m_vtx[3].vtx.z = m_vtxMin.z;
		
	// 左奥（下面）
	m_vtx[4].vtx.x = m_vtxMin.x;
	m_vtx[4].vtx.y = m_vtxMin.y;
	m_vtx[4].vtx.z = m_vtxMax.z;
	
	// 右奥（下面）
	m_vtx[5].vtx.x = m_vtxMax.x;
	m_vtx[5].vtx.y = m_vtxMin.y;
	m_vtx[5].vtx.z = m_vtxMax.z;

	// 左前（下面）
	m_vtx[6].vtx.x = m_vtxMin.x;
	m_vtx[6].vtx.y = m_vtxMin.y;
	m_vtx[6].vtx.z = m_vtxMin.z;

	// 右前（下面）
	m_vtx[7].vtx.x = m_vtxMax.x;
	m_vtx[7].vtx.y = m_vtxMin.y;
	m_vtx[7].vtx.z = m_vtxMin.z;

	//　変数初期化
	m_SaveEmissive = { 0.0f,0.0f,0.0f,1.0f };
}

//=============================================================================
// モデルの終了処理
//=============================================================================
void CModel::Uninit(void)
{
 
}

//=============================================================================
// モデルの更新処理
//=============================================================================
void CModel::Update(void)
{
	// オブジェクトとの当たり判定を取るか
	if (m_bCollision)
	{
		m_bHit = false;

		if (m_bDoOnce)
		{
			CScene::OBJTYPE objtype;
			CScene *pScene = nullptr;
			HIT_TYPE hitType;
			for (int nCnt = 0; nCnt < 2; nCnt++)
			{
				// プレイヤーのシーン取得
				switch (nCnt)
				{
				case 0:
					objtype = CScene::OBJTYPE_PLAYER;
					hitType = TYPE_SPHERE;
					break;
				case 1:
					objtype = CScene::OBJTYPE_SHADOW;
					hitType = TYPE_LINE;
					break;
				case 2:
					objtype = CScene::OBJTYPE_MODEL;
					hitType = TYPE_SPHERE;
					break;
				}

				pScene = CScene::GetScene(objtype);
				// シーンがnullになるまで通る
				while (pScene)
				{
					// 次のシーンを取得
					CScene *pSceneNext = CScene::GetSceneNext(pScene);
					
					float fDist;								// プレイヤーと爆弾の距離を取る変数
					D3DXVECTOR3 ObjectPos = pScene->GetPos();	// オブジェクトの位置取得

					// 三平方の定理を使い距離を測る
					fDist = ((ObjectPos.x - m_mtxWorld._41) * (ObjectPos.x - m_mtxWorld._41)) + ((ObjectPos.y - m_mtxWorld._42) * (ObjectPos.y - m_mtxWorld._42)) + ((ObjectPos.z - m_mtxWorld._43) * (ObjectPos.z - m_mtxWorld._43));

					if (fDist < 100000.0f)
					{
						// 当たり判定
						if (LineCollisionCube(pScene, hitType))
						{
							m_bHit = true;	// 一回でも当たればヒット判定とする
						}
					}

					// 次のシーンを現在のシーンにする
					pScene = pSceneNext;
				}
			}
		}

		else
		{
			m_bDoOnce = true;
		}
	}
}

//=============================================================================
// モデルの描画処理
//=============================================================================
void CModel::Draw(void)
{
	//========================================================================
	// モデル本体のワールド座標設定
	//========================================================================
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	// デバイスのポインタ

	// 現在のマテリアルを取得
	D3DXMATRIX mtxRot, mtxTrans, mtxScale;				// 計算用マトリックス
	D3DXMATRIX mtxParent;								// 親のマトリックス
	D3DMATERIAL9 Matdef;

	// 各パーツのワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// サイズ変更
	D3DXMatrixScaling(&mtxScale, m_scale.x, m_scale.y, m_scale.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScale);

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// マテリアルの取得
	pDevice->GetMaterial(&Matdef);

	// 書くパーツの親のマトリックスを設定
	if (m_pParent)
	{
		mtxParent = m_pParent->GetMatrix();
	}

	else
	{
		// 最新のマトリックスを取得
		pDevice->GetTransform(D3DTS_WORLD, &mtxParent);
	}

	// 算出した各パーツのワールドマトリックスと親のマトリックスを掛け合わせる
	D3DXMatrixMultiply
	(
		&m_mtxWorld,
		&m_mtxWorld,
		&mtxParent
	);

	// 各パーツのワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	if (!m_bDraw)
	{
		// マテリアルデータ編のポインタを取得
		m_pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)m_nNumMat; nCntMat++)
		{
			//m_pMat[nCntMat].MatD3D.Ambient = m_SaveEmissive;
			//m_pMat[nCntMat].MatD3D.Diffuse = m_SaveEmissive;
			//m_pMat[nCntMat].MatD3D.Specular = m_SaveEmissive;
			 m_pMat[nCntMat].MatD3D.Emissive = m_SaveEmissive;

			// マテリアルの設定
			pDevice->SetMaterial(&m_pMat[nCntMat].MatD3D);

			// テクスチャの設定
			if (m_pTexture.at(nCntMat))
			{
				pDevice->SetTexture(0, m_pTexture[nCntMat]);
			}
			else
			{
				pDevice->SetTexture(0, NULL);
			}

			// モデル（パーツ）の描画
			m_pMesh->DrawSubset(nCntMat);
		}
		// 保存していたマテリアルを戻す
		pDevice->SetMaterial(&Matdef);
	}

	//========================================================================
	// 各モデルの頂点のワールド座標設定
	//========================================================================
	D3DXMATRIX vtxParent;	// 各モデルの頂点の親マトリックス
	D3DXMATRIX vtxTrans;	// 計算用マトリックス

	// 配列サイズを設定
	for (int nCnt = 0; nCnt < MAX_MODEL_VTX; nCnt++)
	{
		// 各頂点のワールドマトリックスの初期化
		D3DXMatrixIdentity(&m_vtx[nCnt].mtxWorld);

		// 位置を反映
		D3DXMatrixTranslation(&vtxTrans, m_vtx[nCnt].vtx.x, m_vtx[nCnt].vtx.y, m_vtx[nCnt].vtx.z);
		D3DXMatrixMultiply(&m_vtx[nCnt].mtxWorld, &m_vtx[nCnt].mtxWorld, &vtxTrans);

		//*****************************************************************************
		// 親子関係付け処理
		//*****************************************************************************
		// 各頂点の親のマトリックスを設定
		if (m_mtxWorld)
		{
			vtxParent = m_mtxWorld;
		}

		else
		{
			// 最新のマトリックスを取得
			pDevice->GetTransform(D3DTS_WORLD, &vtxParent);
		}

		// 算出した各頂点のワールドマトリックスと親のマトリックスを掛け合わせる
		D3DXMatrixMultiply
		(
			&m_vtx[nCnt].mtxWorld,
			&m_vtx[nCnt].mtxWorld,
			&vtxParent
		);
		//****************************************************************************
		// ここまで

		// 各頂点のワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &m_vtx[nCnt].mtxWorld);

		// 各頂点のワールド座標の保存
		m_vtx[nCnt].vtxWorld.x = m_vtx[nCnt].mtxWorld._41;
		m_vtx[nCnt].vtxWorld.y = m_vtx[nCnt].mtxWorld._42;
		m_vtx[nCnt].vtxWorld.z = m_vtx[nCnt].mtxWorld._43;
	}
}

#define IKD_EPSIRON 0.00001f // 誤差

///////////////////////////////////////////////////
// 平面パーティクル衝突判定・時刻・位置算出関数
// r : パーティクルの半径
// pPre_pos : パーティクルの前の位置
// pPos : パーティクルの次の到達位置
// pNormal : 平面の法線
// pPlane_pos : 平面上の1点
// pOut_t : 衝突時間を格納するFLOAT型へのポインタ
// pOut_colli : パーティクルの衝突位置を格納するD3DXVECTOR型へのポインタ
// 戻り値 : 衝突(true), 非衝突(false)

bool CModel::CalcParticlePlaneCollision(
	FLOAT r,
	D3DXVECTOR3 *pPre_pos, D3DXVECTOR3 *pPos,
	D3DXVECTOR3 *pNormal, D3DXVECTOR3 *pPlane_pos,
	FLOAT *t,
	D3DXVECTOR3 *pOut_colli
)
{
	D3DXVECTOR3 C0 = *pPre_pos - *pPlane_pos;	// 平面上の一点から現在位置へのベクトル
	D3DXVECTOR3 D = *pPos - *pPre_pos;			// 現在位置から予定位置までのベクトル
	D3DXVECTOR3 N;								// 法線
	D3DXVec3Normalize(&N, pNormal);				// 法線を標準化

	// 平面と中心点の距離を算出
	FLOAT Dot_C0 = D3DXVec3Dot(&C0, &N);
	FLOAT dist_plane_to_point = fabs(Dot_C0);

	// 進行方向と法線の関係をチェック
	FLOAT Dot = D3DXVec3Dot(&D, &N);

	// 平面と平行に移動してめり込んでいるスペシャルケース
	if ((IKD_EPSIRON - fabs(Dot) > 0.0f) && (dist_plane_to_point < r)) {
		// 一生抜け出せないので最大時刻を返す
		*t = FLT_MAX;
		// 衝突位置は仕方ないので今の位置を指定
		*pOut_colli = *pPre_pos;
		return true;
	}

	// 交差時間の算出
	*t = (r - Dot_C0) / Dot;

	// 衝突位置の算出
	*pOut_colli = *pPre_pos + (*t) * D;

	// めり込んでいたら衝突として処理終了
	if (dist_plane_to_point < r)
		return true;

	// 壁に対して移動が逆向きなら衝突していない
	if (Dot >= 0)
		return false;

	// 時間が0～1の間にあれば衝突
	if ((0 <= *t) && (*t <= 1))
		return true;

	return false;
}


//=============================================================================
// モデルの当たり判定(球と面)
//=============================================================================
bool CModel::SurfaceCollisionSphere(CScene *pScene)
{
	D3DXVECTOR3 pos = pScene->GetPos();
	D3DXVECTOR3 size = pScene->GetSize();

	D3DXVECTOR3 posSphere;
	posSphere.x = m_mtxWorld._41;
	posSphere.y = m_mtxWorld._42;
	posSphere.z = m_mtxWorld._43;
	float fRadius = m_size.x;

	return false;
}

//=============================================================================
// モデルの当たり判定(球と球)
//=============================================================================
bool CModel::SphereCollisionSphere(float fRadius, CScene *pScene)
{
	D3DXVECTOR3 pos1 = pScene->GetPos();
	D3DXVECTOR3 pos2;
	pos2.x = m_mtxWorld._41;
	pos2.y = m_mtxWorld._42;
	pos2.z = m_mtxWorld._43;

	float fRadius1 = pScene->GetSize().x;
	float fRadius2 = fRadius;

	if (powf(fRadius1 + fRadius2, 2) >= powf(pos1.x - pos2.x, 2) + powf(pos1.y - pos2.y, 2) + powf(pos1.z - pos2.z, 2))
	{
		return true;
	}

	return false;
}

//=============================================================================
// モデルの当たり判定(点と直方体)
//=============================================================================
bool CModel::DotCollisionCube(CScene *pScene, const HIT_TYPE &hit_type)
{
	//------------------------------------------------
	// ポリゴンの法線ベクトルを求める
	//------------------------------------------------
	// 6面の法線ベクトル
	D3DXVECTOR3 normalVec[6];
	// 上面
	D3DXVec3Cross(&normalVec[0], &(m_vtx[1].vtxWorld - m_vtx[0].vtxWorld), &(m_vtx[2].vtxWorld - m_vtx[0].vtxWorld));
	// 下面
	D3DXVec3Cross(&normalVec[1], &(m_vtx[5].vtxWorld - m_vtx[7].vtxWorld), &(m_vtx[6].vtxWorld - m_vtx[7].vtxWorld));
	// 右面
	D3DXVec3Cross(&normalVec[2], &(m_vtx[1].vtxWorld - m_vtx[3].vtxWorld), &(m_vtx[7].vtxWorld - m_vtx[3].vtxWorld));
	// 左面
	D3DXVec3Cross(&normalVec[3], &(m_vtx[2].vtxWorld - m_vtx[0].vtxWorld), &(m_vtx[4].vtxWorld - m_vtx[0].vtxWorld));
	// 前面
	D3DXVec3Cross(&normalVec[4], &(m_vtx[3].vtxWorld - m_vtx[2].vtxWorld), &(m_vtx[6].vtxWorld - m_vtx[2].vtxWorld));
	// 背面
	D3DXVec3Cross(&normalVec[5], &(m_vtx[0].vtxWorld - m_vtx[1].vtxWorld), &(m_vtx[5].vtxWorld - m_vtx[1].vtxWorld));

	// 単位ベクトル化
	for (int nCnt = 0; nCnt < SURFACE_NUM; nCnt++)
	{
		D3DXVec3Normalize(&normalVec[nCnt], &normalVec[nCnt]);
	}

	//------------------------------------------------
	// 頂点の位置から対象の位置までのベクトル
	//------------------------------------------------
	D3DXVECTOR3 pos = pScene->GetPos();	// 現在位置取得
	D3DXVECTOR3 vecPos[SURFACE_NUM];	// 位置と面のベクトル

	// 上面
	vecPos[0] = pos - m_vtx[0].vtxWorld;
	// 下面
	vecPos[1] = pos - m_vtx[7].vtxWorld;
	// 右面
	vecPos[2] = pos - m_vtx[3].vtxWorld;
	// 左面
	vecPos[3] = pos - m_vtx[0].vtxWorld;
	// 前面
	vecPos[4] = pos - m_vtx[2].vtxWorld;
	// 背面
	vecPos[5] = pos - m_vtx[1].vtxWorld;

	// 板ポリゴンの法線ベクトルと、法線とシーンの位置を結ぶベクトルの内積を求める
	float fDot[SURFACE_NUM];

	for (int nCnt = 0; nCnt < SURFACE_NUM; nCnt++)
	{
		fDot[nCnt] = D3DXVec3Dot(&normalVec[nCnt], &vecPos[nCnt]);
	}

	float Radius = 0.0f;

	switch (hit_type)
	{
	case TYPE_DOT:
		break;

	case TYPE_LINE:
		break;

	case TYPE_SPHERE:
		Radius = pScene->GetSize().x / 2;
		break;

	case TYPE_CUBE:
		Radius = pScene->GetSize().x / 2;
		break;

	default:
		break;
	}

	if (fDot[4] <= Radius && fDot[5] <= Radius && fDot[2] <= Radius && fDot[3] <= Radius && fDot[0] <= Radius && fDot[1] <= Radius)
	{
		return true;
	}

	else
	{
		return false;
	}
}

//=============================================================================
// モデルの当たり判定(線と直方体)
//=============================================================================
bool CModel::LineCollisionCube(CScene *pScene,const HIT_TYPE &hit_type)
{
	//------------------------------------------------
	// ポリゴンの法線ベクトルを求める
	//------------------------------------------------
	// 6面の法線ベクトル
	D3DXVECTOR3 normalVec[6];
	// 上面
	D3DXVec3Cross(&normalVec[0], &(m_vtx[1].vtxWorld - m_vtx[0].vtxWorld), &(m_vtx[2].vtxWorld - m_vtx[0].vtxWorld));
	// 下面
	D3DXVec3Cross(&normalVec[1], &(m_vtx[5].vtxWorld - m_vtx[7].vtxWorld), &(m_vtx[6].vtxWorld - m_vtx[7].vtxWorld));
	// 右面
	D3DXVec3Cross(&normalVec[2], &(m_vtx[1].vtxWorld - m_vtx[3].vtxWorld), &(m_vtx[7].vtxWorld - m_vtx[3].vtxWorld));
	// 左面
	D3DXVec3Cross(&normalVec[3], &(m_vtx[2].vtxWorld - m_vtx[0].vtxWorld), &(m_vtx[4].vtxWorld - m_vtx[0].vtxWorld));
	// 前面
	D3DXVec3Cross(&normalVec[4], &(m_vtx[3].vtxWorld - m_vtx[2].vtxWorld), &(m_vtx[6].vtxWorld - m_vtx[2].vtxWorld));
	// 背面
	D3DXVec3Cross(&normalVec[5], &(m_vtx[0].vtxWorld - m_vtx[1].vtxWorld), &(m_vtx[5].vtxWorld - m_vtx[1].vtxWorld));

	for (int nCnt = 0; nCnt < SURFACE_NUM; nCnt++)
	{
		// 単位ベクトル化
		D3DXVec3Normalize(&normalVec[nCnt], &normalVec[nCnt]);
	}

	//------------------------------------------------
	// 頂点の位置から対象の位置までのベクトル
	//------------------------------------------------
	D3DXVECTOR3 pos = pScene->GetPos();							// 現在位置取得
	D3DXVECTOR3 posOld = pScene->GetPosOld();					// 1フレーム前の位置取得
	D3DXVECTOR3 vecPos[SURFACE_NUM], vecPosOld[SURFACE_NUM];	// 位置と面のベクトル
	// 上面
	vecPos[0] = pos - m_vtx[0].vtxWorld, vecPosOld[0] = posOld - m_vtx[0].vtxWorld;
	// 下面
	vecPos[1] = pos - m_vtx[7].vtxWorld, vecPosOld[1] = posOld - m_vtx[7].vtxWorld;
	// 右面
	vecPos[2] = pos - m_vtx[3].vtxWorld, vecPosOld[2] = posOld - m_vtx[3].vtxWorld;
	// 左面
	vecPos[3] = pos - m_vtx[0].vtxWorld, vecPosOld[3] = posOld - m_vtx[0].vtxWorld;
	// 前面
	vecPos[4] = pos - m_vtx[2].vtxWorld, vecPosOld[4] = posOld - m_vtx[2].vtxWorld;
	// 背面
	vecPos[5] = pos - m_vtx[1].vtxWorld, vecPosOld[5] = posOld - m_vtx[1].vtxWorld;

	// 板ポリゴンの法線ベクトルと、法線とシーンの位置を結ぶベクトルの内積を求める
	float fDot[SURFACE_NUM], fDotOld[SURFACE_NUM];

	for (int nCnt = 0; nCnt < SURFACE_NUM; nCnt++)
	{
		fDot[nCnt] = D3DXVec3Dot(&normalVec[nCnt], &vecPos[nCnt]);
		fDotOld[nCnt] = D3DXVec3Dot(&normalVec[nCnt], &vecPosOld[nCnt]);
	}

	float Radius = 0.0f;
	switch (hit_type)
	{
	case TYPE_DOT:
		break;

	case TYPE_LINE:
		break;

	case TYPE_SPHERE:
		Radius = pScene->GetSize().y / 2;
		break;

	case TYPE_CUBE:
		Radius = pScene->GetSize().y / 2;
		break;

	default:
		break;
	}


	// 移動ベクトルを求める
	const D3DXVECTOR3 moveVec = pos - posOld;

	// 上から当たる
	if (HitFrom(FROM_UP, fDot, fDotOld, Radius))
	{
		// 押し出す位置を求める
		// PushPos.y = ((normalVec[0].x * normalVec[0].x) - (normalVec[0].z * normalVec[0].z)) / normalVec[0].y;
		pos.y = m_vtx[0].vtxWorld.y - (1 / normalVec[0].y * (normalVec[0].x * (pos.x - m_vtx[0].vtxWorld.x) + normalVec[0].z * (pos.z - m_vtx[0].vtxWorld.z)));
		pos.y += Radius;

		CScene::OBJTYPE objtype = pScene->GetObjType();
		CPlayer *pPlayer = nullptr;
		CShadow *pShadow = nullptr;
		switch (objtype)
		{
		case CScene::OBJTYPE_PLAYER:
			pPlayer = (CPlayer*)pScene;
			pPlayer->SetGravity(0.0f, false);
			break;

		case CScene::OBJTYPE_SHADOW:
			pShadow = (CShadow*)pScene;
			pShadow->SetHeight(pos.y);
			break;
		}

		pScene->SetPos(pos);	// SetPosをすることで、最初に当たったオブジェクトのみを判定とする
		return true;
	}

	// 下から当たる
	else if (HitFrom(FROM_DOWN, fDot, fDotOld, Radius))
	{
		return true;
	}

	// 右からあたる
	if (HitFrom(FROM_RIGHT, fDot, fDotOld, Radius))
	{
		//対象の押出先を求める
		const D3DXVECTOR3 PushVec = PushDistanceSide(moveVec, normalVec[2]);
		pos += PushVec;
		pScene->SetPos(pos);

		return true;
	}

	// 左から当たる
	else if (HitFrom(FROM_LEFT, fDot, fDotOld, Radius))
	{
		//対象の押出先を求める
		const D3DXVECTOR3 PushVec = PushDistanceSide(moveVec, normalVec[3]);
		pos += PushVec;
		pScene->SetPos(pos);

		return true;
	}

	// 前から当たる
	if (HitFrom(FROM_FRONT, fDot, fDotOld, Radius))
	{
		//対象の押出先を求める
		const D3DXVECTOR3 PushVec = PushDistanceSide(moveVec, normalVec[4]);
		pos += PushVec;
		pScene->SetPos(pos);

		return true;

	}

	// 後ろからあたる
	else if (HitFrom(FROM_BACK, fDot, fDotOld, Radius))
	{
		//対象の押出先を求める
		const D3DXVECTOR3 PushVec = PushDistanceSide(moveVec, normalVec[5]);
		pos += PushVec;
		pScene->SetPos(pos);

		return true;
	}

	return false;
}

//===================================
// 押し戻す距離を求める
//===================================
D3DXVECTOR3 CModel::PushDistanceSide(const D3DXVECTOR3 &moveVec, const D3DXVECTOR3 &vecNor)
{
	// ムーブベクトルと法線の内積を求める
	const float fDotMoveVec = -D3DXVec3Dot(&moveVec, &vecNor);
	// 法線の方向に内積分伸ばしたベクトルを求める
	const D3DXVECTOR3 Push = D3DXVECTOR3(vecNor.x * fDotMoveVec, vecNor.y * fDotMoveVec, vecNor.z * fDotMoveVec);

	return Push;
}

//===================================
// どこから当たっているかの判定
//===================================
bool CModel::HitFrom(const HIT_FROM &hitFrom, const float *fDot, const float *fDotOld,const float &Radius)
{
	switch (hitFrom)
	{
	case FROM_UP:
		return fDot[4] <= Radius && fDot[5] <= Radius && fDot[2] <= Radius && fDot[3] <= Radius && fDot[0] <= Radius && fDotOld[0] > -ALLOWABLE_ERROR + Radius;

	case FROM_DOWN:
		return fDot[4] <= Radius && fDot[5] <= Radius && fDot[2] <= Radius && fDot[3] <= Radius && fDot[1] <= Radius && fDotOld[1] > -ALLOWABLE_ERROR + Radius;

	case FROM_RIGHT:
		return fDot[0] <= Radius && fDot[1] <= Radius && fDot[4] <= Radius && fDot[5] <= Radius && fDot[2] <= Radius && fDotOld[2] > -ALLOWABLE_ERROR + Radius;

	case FROM_LEFT:
		return fDot[0] <= Radius && fDot[1] <= Radius && fDot[4] <= Radius && fDot[5] <= Radius && fDot[3] <= Radius && fDotOld[3] > -ALLOWABLE_ERROR + Radius;

	case FROM_FRONT:
		return fDot[0] <= Radius && fDot[1] <= Radius && fDot[2] <= Radius && fDot[3] <= Radius && fDot[4] <= Radius && fDotOld[4] > -ALLOWABLE_ERROR + Radius;

	case FROM_BACK:
		return fDot[0] <= Radius && fDot[1] <= Radius && fDot[2] <= Radius && fDot[3] <= Radius && fDot[5] <= Radius && fDotOld[5] > -ALLOWABLE_ERROR + Radius;

	default:
		assert(hitFrom <= -1 || hitFrom >= FROM_MAX);
		return false;
	}
}

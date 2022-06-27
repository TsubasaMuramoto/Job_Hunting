//==============================================================================
// フィールドの作成
// Author:村元翼
//==============================================================================
#include "Scene3D.h"
#include "player.h"
#include "model.h"
#include "scene.h"
#include "renderer.h"
#include "manager.h"
#include "field.h"

//=============================================================================
// コンストラクタ
//=============================================================================
CField::CField()
{
	// 各種初期化
	m_pTexture = nullptr;						// テクスチャポインタ
	m_pVtxBuff = nullptr;						// 頂点バッファポインタ
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 位置
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// サイズ
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 回転

	for (int nCnt = 0; nCnt < FIELD_VTX; nCnt++)
	{
		m_vtx[nCnt] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_vtxRot[nCnt] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}

	m_pScene3D = nullptr;
}

//=============================================================================
// デストラクタ
//=============================================================================
CField::~CField()
{

}

//=============================================================================
// インスタンス生成処理
//=============================================================================
CField *CField::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 rot)
{
	// インスタンス生成
	CField *pField = nullptr;
	if (!pField)
	{
		// 動的メモリ確保
		pField = new CField();

		// NULLチェック
		if (pField)
		{
			pField->m_pos = pos;			// 位置
			pField->m_size = size;			// 大きさ
			pField->m_rot = rot;			// 回転

			pField->Init(pField->m_pos, pField->m_size);	// 初期化
		}
	}

	return pField;
}

//=============================================================================
// ポリゴンの初期化処理
//=============================================================================
HRESULT CField::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	// シーン3Dの生成とテクスチャの取得
	m_pScene3D = CScene3D::Create(pos, size, m_rot);
	m_pScene3D->BindTexture(CManager::GetInstance()->GetTexture()->GetTexture("TEX_TYPE_FLOOR"));
	return S_OK;
}

//=============================================================================
// ポリゴンの終了処理
//=============================================================================
void CField::Uninit(void)
{
	m_pScene3D->Uninit();
}

//=============================================================================
// ポリゴンの更新処理
//=============================================================================
void CField::Update(void)
{
	m_pScene3D->Update();

	// プレイヤーのシーン取得
	CScene *pScene = CScene::GetScene(OBJTYPE_PLAYER);

	// シーンがnullになるまで通る
	while (pScene)
	{
		// 次のシーンを取得
		CScene *pSceneNext = CScene::GetSceneNext(pScene);
		CPlayer *pPlayer = (CPlayer*)pScene;

		// 当たり判定
		if (LineCollisionField(pScene))
		{
			// 重力をつけるとトランポリンになる
			//pPlayer->SetGravity(20.0f, true);
		}

		// 次のシーンを現在のシーンにする
		pScene = pSceneNext;
	}
}

//=============================================================================
// ポリゴンの描画処理
//=============================================================================
void CField::Draw(void)
{
	m_pScene3D->Draw();

	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	// デバイスのポインタ
	D3DXMATRIX mtxTrans,mtxRot;															// 位置計算用マトリックス
	D3DXMATRIX mtxParent;																// 親のマトリックス

	for (int nCnt = 0; nCnt < FIELD_VTX; nCnt++)
	{
		D3DXVECTOR3 LocalVtx[FIELD_VTX];							// ローカル座標保存変数
		LocalVtx[nCnt] = m_pScene3D->GetLocalVtx(nCnt);				// ローカル座標取得

		// 各頂点のワールドマトリックスの初期化
		D3DXMatrixIdentity(&m_vtxMtxWorld[nCnt]);

		//// 向きを反映
		//D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
		//D3DXMatrixMultiply(&m_vtxMtxWorld[nCnt], &m_vtxMtxWorld[nCnt], &mtxRot);

		// 位置を反映
		D3DXMatrixTranslation(&mtxTrans, LocalVtx[nCnt].x, LocalVtx[nCnt].y, LocalVtx[nCnt].z);
		D3DXMatrixMultiply(&m_vtxMtxWorld[nCnt], &m_vtxMtxWorld[nCnt], &mtxTrans);

		// 親子関係付け処理
		//*****************************************************************************
		// 各頂点の親のマトリックスを設定
		if (m_pScene3D->GetMatrix() != nullptr)
		{
			mtxParent = m_pScene3D->GetMatrix();
		}

		else
		{
			// 最新のマトリックスを取得
			pDevice->GetTransform(D3DTS_WORLD, &mtxParent);
		}

		// 算出した各頂点のワールドマトリックスと親のマトリックスを掛け合わせる
		D3DXMatrixMultiply(&m_vtxMtxWorld[nCnt], &m_vtxMtxWorld[nCnt], &mtxParent);
		//****************************************************************************
		// ここまで

		// 各頂点のワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &m_vtxMtxWorld[nCnt]);
	}

	// 各頂点のワールド座標の保存
	for (int nCnt = 0; nCnt < FIELD_VTX; nCnt++)
	{
		m_vtx[nCnt].x = m_vtxMtxWorld[nCnt]._41;
		m_vtx[nCnt].y = m_vtxMtxWorld[nCnt]._42;
		m_vtx[nCnt].z = m_vtxMtxWorld[nCnt]._43;
	}
}

//================================================
// ワールドマトリックス設定処理
//================================================
void CField::SetVtxMtxWorld(D3DXVECTOR3 vtxPos, int nCntVtx)
{
	//デバイスのポインタ
	LPDIRECT3DDEVICE9 pDevice;
	//デバイスの取得
	pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// 計算用マトリックス
	D3DXMATRIX mtxRot, mtxTrans;						

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_vtxMtxWorld[nCntVtx]);		

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_vtxRot[nCntVtx].y, m_vtxRot[nCntVtx].x, m_vtxRot[nCntVtx].z);
	D3DXMatrixMultiply(&m_vtxMtxWorld[nCntVtx], &m_vtxMtxWorld[nCntVtx], &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, vtxPos.x, vtxPos.y, vtxPos.z);
	D3DXMatrixMultiply(&m_vtxMtxWorld[nCntVtx], &m_vtxMtxWorld[nCntVtx], &mtxTrans);

	D3DXMATRIX mtxParent;		//親のマトリックス
	mtxParent = m_mtxWorld;

	//算出した各パーツのワールドマトリックスと親のワールドマトリックスを掛け合わせる
	D3DXMatrixMultiply(&m_vtxMtxWorld[nCntVtx], &m_vtxMtxWorld[nCntVtx], &mtxParent);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_vtxMtxWorld[nCntVtx]);
}



//=============================================================================
// ポリゴン(フィールド)と点の当たり判定
//=============================================================================
bool CField::DotCollisionField(CScene *pScene)
{
	// 板ポリゴンの0,1,2,3番目をそれぞれA,B,C,Dとし
	// pSceneの位置をPとする

	// 各頂点とシーンの位置を結ぶベクトル
	D3DXVECTOR3 vecAP = pScene->GetPos() - m_vtx[0];
	D3DXVECTOR3 vecBP = pScene->GetPos() - m_vtx[1];
	D3DXVECTOR3 vecCP = pScene->GetPos() - m_vtx[2];
	D3DXVECTOR3 vecDP = pScene->GetPos() - m_vtx[3];

	// 各頂点のベクトルを計算
	D3DXVECTOR3 vecAB = m_vtx[1] - m_vtx[0];
	D3DXVECTOR3 vecBD = m_vtx[3] - m_vtx[1];
	D3DXVECTOR3 vecDC = m_vtx[2] - m_vtx[3];
	D3DXVECTOR3 vecCA = m_vtx[0] - m_vtx[2];

	// ポリゴンの範囲内にいるかの計算(4つの2D外積結果が0以下なら)
	if (vecAB.x * vecAP.z - vecAP.x * vecAB.z <= 0 &&
		vecBD.x * vecBP.z - vecBP.x * vecBD.z <= 0 &&
		vecDC.x * vecDP.z - vecDP.x * vecDC.z <= 0 &&
		vecCA.x * vecCP.z - vecCP.x * vecCA.z <= 0)
	{
		// 法線ベクトル
		D3DXVECTOR3 normalVec;
		// ポリゴンの法線ベクトルを求める		↓vecAC
		D3DXVec3Cross(&normalVec, &vecAB, &(m_vtx[2] - m_vtx[0]));
		// 単位ベクトル化
		D3DXVec3Normalize(&normalVec, &normalVec);

		// 板ポリゴンの法線ベクトルと、法線とシーンの位置を結ぶベクトルの内積を求める
		float Dot = D3DXVec3Dot(&normalVec,&vecAP);

		// 内積結果のベクトルの方向が0以下だったら
		if (Dot <= 0)
		{
			// 当たった判定を返す
			return true;
		}

		else
		{
			// 当たり判定処理をやめる
			return false;
		}
	}

	else
	{
		// 当たり判定処理をやめる
		return false;
	}
}

//=============================================================================
// ポリゴン(フィールド)と線の当たり判定
//=============================================================================
bool CField::LineCollisionField(CScene *pScene)
{
	const D3DXVECTOR3 vecQP = pScene->GetPos() - pScene->GetPosOld();

	// 各頂点とシーンの最後の位置を結ぶベクトル
	const D3DXVECTOR3 vecAQ = pScene->GetPosOld() - m_vtx[0];
	const D3DXVECTOR3 vecBQ = pScene->GetPosOld() - m_vtx[1];
	const D3DXVECTOR3 vecCQ = pScene->GetPosOld() - m_vtx[2];
	const D3DXVECTOR3 vecDQ = pScene->GetPosOld() - m_vtx[3];

	// 各頂点とシーンの位置を結ぶベクトル
	const D3DXVECTOR3 vecAP = pScene->GetPos() - m_vtx[0];
	const D3DXVECTOR3 vecBP = pScene->GetPos() - m_vtx[1];
	const D3DXVECTOR3 vecCP = pScene->GetPos() - m_vtx[2];
	const D3DXVECTOR3 vecDP = pScene->GetPos() - m_vtx[3];

	// 各頂点のベクトルを計算
	const D3DXVECTOR3 vecAB = m_vtx[1] - m_vtx[0];
	const D3DXVECTOR3 vecBD = m_vtx[3] - m_vtx[1];
	const D3DXVECTOR3 vecDC = m_vtx[2] - m_vtx[3];
	const D3DXVECTOR3 vecCA = m_vtx[0] - m_vtx[2];

	// オブジェクトとポリゴンの2D内積
	float crossXY[FIELD_VTX];		// XY範囲(X方向の壁)
	float crossZY[FIELD_VTX];		// ZY範囲(Z方向の壁)
	float crossXZ[FIELD_VTX];		// XZ範囲(平面)
	
	crossXY[0] = vecAB.x * vecAP.y - vecAP.x * vecAB.y;
	crossXY[1] = vecBD.x * vecBP.y - vecBP.x * vecBD.y;
	crossXY[2] = vecDC.x * vecDP.y - vecDP.x * vecDC.y;
	crossXY[3] = vecCA.x * vecCP.y - vecCP.x * vecCA.y;

	crossZY[0] = vecAB.z * vecAP.y - vecAP.z * vecAB.y;
	crossZY[1] = vecBD.z * vecBP.y - vecBP.z * vecBD.y;
	crossZY[2] = vecDC.z * vecDP.y - vecDP.z * vecDC.y;
	crossZY[3] = vecCA.z * vecCP.y - vecCP.z * vecCA.y;

	crossXZ[0] = vecAB.x * vecAP.z - vecAP.x * vecAB.z;
	crossXZ[1] = vecBD.x * vecBP.z - vecBP.x * vecBD.z;
	crossXZ[2] = vecDC.x * vecDP.z - vecDP.x * vecDC.z;
	crossXZ[3] = vecCA.x * vecCP.z - vecCP.x * vecCA.z;

	// 壁の向きが-90 <= x < 90 の時のみ それ以外はif文の符号が逆になる 
	if (m_rot.y >= -D3DX_PI / 2 && m_rot.y < D3DX_PI / 2)
	{

	}

	else
	{
		for (int nCnt = 0; nCnt < FIELD_VTX; nCnt++)
		{
			crossXY[nCnt] *= -1;
			crossZY[nCnt] *= -1;
		}
	}

	// 法線ベクトル
	D3DXVECTOR3 normalVec;
	// ポリゴンの法線ベクトルを求める
	D3DXVec3Cross(&normalVec, &vecAB, &vecBD);
	// 単位ベクトル化
	D3DXVec3Normalize(&normalVec, &normalVec);

	// 板ポリゴンの法線ベクトルと、法線とシーンの位置を結ぶベクトルの内積を求める
	float Dot = D3DXVec3Dot(&normalVec, &vecAP);
	float DotOld = D3DXVec3Dot(&normalVec, &vecAQ);

	CPlayer *pPlayer = (CPlayer*)pScene;
	const float fPlayerRad = pPlayer->GetSize().x / 2;

	//***************************************************************************************
	// 壁の当たり判定
	//***************************************************************************************
	if (m_size.y > 0)
	{
		if ((crossXY[0] < 0.0f && crossXY[1] < 0.0f && crossXY[2] < 0.0f && crossXY[3] < 0.0f)	||	// X方向の範囲
			(crossZY[0] < 0.0f && crossZY[1] < 0.0f && crossZY[2] < 0.0f && crossZY[3] < 0.0f))		// Z方向の範囲
		{
			if (Dot <= 0.0f && DotOld > -ALLOWABLE_ERROR)
			{
				// 壁ずりベクトルを求め、正規化する
				//D3DXVECTOR3 SlideVec = vecQP - D3DXVec3Dot(&vecQP, &normalVec) * normalVec;
				//D3DXVec3Normalize(&SlideVec, &SlideVec);

				// 板ポリゴンの高さを計算し、代入
				D3DXVECTOR3 &pos = pScene->GetPos();
				D3DXVECTOR3 &posOld = pScene->GetPosOld();

				if (Dot != 0.0f || DotOld != 0.0f)
				{
					// 内分比を求める
					const float &DividRatio = fabsf(DotOld) / (fabsf(DotOld) + fabsf(Dot));
					// 頂点から衝突点に向かうベクトル
					const D3DXVECTOR3 &DividVec = ((1 - DividRatio) * vecAQ) + (DividRatio * vecAP);
					// 内分点を求める
					const D3DXVECTOR3 &DividP = m_vtx[0] + DividVec;
					// プレイヤーがめり込んだ位置から壁の内分点までのベクトルを求める(* PERCENT_EXTRAは壁より少し手前に戻すため )
					const D3DXVECTOR3 &old = normalVec * D3DXVec3Dot(&normalVec, &((DividP - pos) * PERCENT_EXTRA));
					pos += old;
					pScene->SetPos(pos);
				}
				// 当たった判定を返す
				return true;
			}

			else
			{
				return false;
			}
		}

		else
		{
			return false;
		}
	}

	else if (m_size.y <= 0)
	{
		//***************************************************************************************
		// 床の当たり判定
		//***************************************************************************************
		// ポリゴンの範囲内にいるかの計算(4つの2D外積結果が0より下なら)
		if (crossXZ[0] < 0.0f && crossXZ[1] < 0.0f &&
			crossXZ[2] < 0.0f && crossXZ[3] < 0.0f)
		{
			// 2つの内積結果のベクトルの+-が異なると通る処理(排他的論理和)
			//if(Dot * DotOld < 0)
			// 上からあたる
			if ((Dot <= fPlayerRad && DotOld >= -ALLOWABLE_ERROR + fPlayerRad))
			{
				// 板ポリゴンの高さを計算し、代入
				D3DXVECTOR3 &pos = pScene->GetPos();
				pos.y = m_vtx[0].y - (1 / normalVec.y * (normalVec.x * (pos.x - m_vtx[0].x) + normalVec.z * (pos.z - m_vtx[0].z)));
				pos.y += fPlayerRad;
				pScene->SetPos(pos);

				// 重力を0にする
				pPlayer->SetGravity(0.0f, false);

				// 当たった判定を返す
				return true;
			}

			else
			{
				// 当たり判定処理をやめる
				return false;
			}
		}

		else
		{
			// 当たり判定処理をやめる
			return false;
		}
	}

	else
	{
		return false;
	}
}


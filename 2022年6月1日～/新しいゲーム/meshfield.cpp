//==============================================================================
// メッシュフィールドの作成
// Author:村元翼
//==============================================================================
#include "meshfield.h"
#include "renderer.h"
#include "manager.h"
#include "player.h"
#include "shadow.h"

//=============================================================================
// コンストラクタ
//=============================================================================
CMeshField::CMeshField(OBJTYPE nPriority) :CScene(nPriority)
{

}

//=============================================================================
// デストラクタ
//=============================================================================
CMeshField::~CMeshField()
{

}

//---------------------------------------------------------------
// インスタンス生成処理
//---------------------------------------------------------------
CMeshField *CMeshField::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 rot, int nLine, int nVertical)
{
	// インスタンス生成
	CMeshField *pMeshField = nullptr;
	if (!pMeshField)
	{
		pMeshField = new CMeshField();
		if (pMeshField)
		{
			float rot_x = D3DXToRadian(rot.x);
			float rot_y = D3DXToRadian(rot.y);
			float rot_z = D3DXToRadian(rot.z);

			pMeshField->m_rotMS = { rot_x,rot_y,rot_z };	// 回転
			pMeshField->m_nLine = nLine;					// 横ポリゴン数
			pMeshField->m_nVertical = nVertical;			// 縦ポリゴン数
			pMeshField->Init(pos, size);					// 初期化
			pMeshField->BindTexture(CManager::GetInstance()->GetTexture()->GetTexture("TEX_TYPE_FLOOR2"));
		}
	}

	return pMeshField;
}

//=============================================================================
// ポリゴンの初期化処理
//=============================================================================
HRESULT CMeshField::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	// デバイスのポインタ

	// 位置・サイズ
	m_posMS = pos;
	m_sizeMS = size;

	// 位置・サイズ設定処理
	CScene::SetPos(m_posMS);
	CScene::SetSize(m_sizeMS);

	// 頂点情報を設定
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_3D) * (m_nLine + 1) * (m_nVertical + 1),	// 頂点数
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL
	);

	// 頂点インデックスを設定
	pDevice->CreateIndexBuffer
	(
		(sizeof(WORD) * ((m_nVertical - 1) * (m_nLine + 3) + (m_nLine + 1) * (m_nVertical + 1))),
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pVtxIndexBuff,
		NULL
	);

	// 3D頂点バッファのポインタ
	VERTEX_3D *pVtx;

	// 頂点バッファのロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標
	int nNum = 0;
	for (int nVertical = 0; nVertical < m_nVertical + 1; nVertical++)
	{
		for (int nLine = 0; nLine < m_nLine + 1; nLine++, nNum++)
		{
			// フィールド
			if (m_sizeMS.y <= 0)
			{
				pVtx[nNum].pos.x = -m_sizeMS.x / 2.0f + (m_sizeMS.x / m_nLine) * nLine;
				pVtx[nNum].pos.y = 0.0f;
				pVtx[nNum].pos.z = m_sizeMS.z / 2.0f - (m_sizeMS.z / m_nVertical) * nVertical;
			}

			// ウォール
			else if (m_sizeMS.y > 0)
			{
				pVtx[nNum].pos.x = -m_sizeMS.x / 2.0f + (m_sizeMS.x / m_nLine) * nLine;
				pVtx[nNum].pos.y = m_sizeMS.y - (m_sizeMS.y / m_nVertical) * nVertical;
				pVtx[nNum].pos.z = 0.0f;
			}

			// 法線
			pVtx[nNum].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			//カラー
			pVtx[nNum].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			//テクスチャ
			pVtx[nNum].tex = D3DXVECTOR2(0.0f + (1.0f * nLine), 0.0f + (1.0f * nVertical));

			// 配列の最後に頂点の位置データを追加
			m_vtxLocal.emplace_back(pVtx[nNum].pos);
		}
	}

	// 頂点バッファのアンロック
	m_pVtxBuff->Unlock();

	// インデックス情報へのポインタ
	WORD *pIdx;

	// 頂点インデックスバッファのロック
	m_pVtxIndexBuff->Lock(0, 0, (void**)&pIdx, 0);

	// インデックスの割り当て
	for (int nCnt1 = 0; nCnt1 < m_nVertical; nCnt1++)
	{
		for (int nCnt2 = 0; nCnt2 < m_nLine + 1; nCnt2++)
		{
			// 番号割り当て
			pIdx[(nCnt2 * 2) + 0 + (m_nLine + 2) * 2 * nCnt1] = (m_nLine + 1) + nCnt2 + (m_nLine + 1) * nCnt1;
			pIdx[(nCnt2 * 2) + 1 + (m_nLine + 2) * 2 * nCnt1] =					nCnt2 + (m_nLine + 1) * nCnt1;
		}
	}
	for (int nCnt3 = 0; nCnt3 < m_nVertical - 1; nCnt3++)
	{
		// 縮退ポリゴンの番号割り当て
		pIdx[((m_nLine + 1) * 2 + 0) * (nCnt3 + 1) + (2 * nCnt3)] = m_nLine +		  (m_nLine + 1) * nCnt3;
		pIdx[((m_nLine + 1) * 2 + 1) * (nCnt3 + 1) + (1 * nCnt3)] = m_nLine * 2 + 2 + (m_nLine + 1) * nCnt3;
	}

	// インデックスバッファのアンロック
	m_pVtxIndexBuff->Unlock();

	return S_OK;
}

//=============================================================================
// ポリゴンの終了処理
//=============================================================================
void CMeshField::Uninit(void)
{
	// 頂点バッファの破棄
	if (m_pVtxBuff)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}

	// インデックスバッファの破棄
	if (m_pVtxIndexBuff)
	{
		m_pVtxIndexBuff->Release();
		m_pVtxIndexBuff = nullptr;
	}

	// オブジェクトの破棄
	Release();
}

//=============================================================================
// ポリゴンの更新処理
//=============================================================================
void CMeshField::Update(void)
{
	CollisionManager();
}

//=============================================================================
// ポリゴンの描画処理
//=============================================================================
void CMeshField::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice(); // デバイスのポインタ

	D3DXMATRIX mtxRot, mtxTrans, mtxScale;	// 計算用マトリックス
	D3DXMatrixIdentity(&m_mtxWorld);		// ワールドマトリックスの初期化

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rotMS.y, m_rotMS.x, m_rotMS.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_posMS.x, m_posMS.y, m_posMS.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	pDevice->SetFVF(FVF_VERTEX_3D);					// 頂点フォーマットの設定

	pDevice->SetIndices(m_pVtxIndexBuff);			// 頂点インデックスの登録

	pDevice->SetTexture(0, m_pTexture);				// テクスチャの設定

	// ポリゴンの描画
	pDevice->DrawIndexedPrimitive
	(
		D3DPT_TRIANGLESTRIP,
		0,
		0,
		((m_nLine + 1) * (m_nVertical + 1)),					// 頂点の数
		0,														// 開始する頂点のインデックス
		2 * m_nLine * m_nVertical + (m_nVertical * 4) - 4		// 描画するプリミティブ数
		// ↑1フィールドに2プリミティブ	↑縮退ポリゴンの分(Z方向に1フィールド伸ばす分4つ生成するから)
	);

	D3DXMATRIX mtxParent;	// 親のマトリックス

	// 配列サイズを設定
	m_mtxVec.resize((m_nLine + 1) * (m_nVertical + 1));
	m_vtxWorld.resize((m_nLine + 1) * (m_nVertical + 1));

	int nNum = 0;
	for (int nVertical = 0; nVertical < m_nVertical + 1; nVertical++)
	{
		for (int nLine = 0; nLine < m_nLine + 1; nLine++, nNum++)
		{
			// 各頂点のワールドマトリックスの初期化
			D3DXMatrixIdentity(&m_mtxVec[nNum]);

			// 位置を反映
			D3DXMatrixTranslation(&mtxTrans, m_vtxLocal[nNum].x, m_vtxLocal[nNum].y, m_vtxLocal[nNum].z);
			D3DXMatrixMultiply(&m_mtxVec[nNum], &m_mtxVec[nNum], &mtxTrans);

			//*****************************************************************************
			// 親子関係付け処理
			//*****************************************************************************
			// 各頂点の親のマトリックスを設定
			if (m_mtxWorld)
			{
				mtxParent = m_mtxWorld;
			}

			else
			{
				// 最新のマトリックスを取得
				pDevice->GetTransform(D3DTS_WORLD, &mtxParent);
			}

			// 算出した各頂点のワールドマトリックスと親のマトリックスを掛け合わせる
			D3DXMatrixMultiply
			(
				&m_mtxVec[nNum],
				&m_mtxVec[nNum],
				&mtxParent
			);
			//****************************************************************************
			// ここまで

			// 各頂点のワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &m_mtxVec[nNum]);

			// 各頂点のワールド座標の保存
			m_vtxWorld[nNum].x = m_mtxVec[nNum]._41;
			m_vtxWorld[nNum].y = m_mtxVec[nNum]._42;
			m_vtxWorld[nNum].z = m_mtxVec[nNum]._43;
		}
	}
}

//=============================================================================
// 当たり判定の管理
//=============================================================================
void  CMeshField::CollisionManager(void)
{
	// シーン取得
	for (int nCnt = 0; nCnt < 2; nCnt++)
	{
		CScene *pScene = nullptr;
		switch (nCnt)
		{
		case 0:
			pScene = CScene::GetScene(OBJTYPE_PLAYER);
			break;

		case 1:
			pScene = CScene::GetScene(OBJTYPE_SHADOW);
			break;
		}

		// シーンがnullになるまで通る
		CScene *pScenePlayer = CScene::GetScene(OBJTYPE_PLAYER);
		while (pScene && pScenePlayer && !m_vtxWorld.empty())
		{
			// 次のシーンを取得
			CScene *pSceneNext = CScene::GetSceneNext(pScene);

			float fDist;								// プレイヤーと爆弾の距離を取る変数
			D3DXVECTOR3 ObjectPos = pScene->GetPos();	// オブジェクトの位置取得
			D3DXVECTOR3 PlayerPos = pScenePlayer->GetPos();

			// 三平方の定理を使い距離を測る
			fDist = ((ObjectPos.x - PlayerPos.x) * (ObjectPos.x - PlayerPos.x)) + ((ObjectPos.y - PlayerPos.y) * (ObjectPos.y - PlayerPos.y)) + ((ObjectPos.z - PlayerPos.z) * (ObjectPos.z - PlayerPos.z));

			if (fDist < 10000000.0f)
			{
				// 4頂点の番号割り当て
				int nID[4];
				for (int nCnt = 0; nCnt < m_nVertical; nCnt++)
				{
					for (int nCnt2 = 0; nCnt2 < m_nLine; nCnt2++)
					{
						nID[0] = (0 + nCnt2) + (nCnt * (m_nLine + 1));
						nID[1] = (1 + nCnt2) + (nCnt * (m_nLine + 1));
						nID[2] = ((2 + (m_nLine - 1)) + nCnt2) + (nCnt * (m_nLine + 1));
						nID[3] = ((3 + (m_nLine - 1)) + nCnt2) + (nCnt * (m_nLine + 1));

						if (LineCollisionMesh(pScene, &nID[0]))
						{
							break;
						}
					}
				}
			}
				// 次のシーンを現在のシーンにする
				pScene = pSceneNext;
		}
	}
}

//=============================================================================
// ポリゴン(フィールド)と線の当たり判定
//=============================================================================
bool CMeshField::LineCollisionMesh(CScene *pScene,const int *pnVtx)
{
	// 板ポリゴンの0,1,2,3番目をそれぞれA,B,C,Dとし
	// pSceneの位置をPとする
	// pSceneの最後の位置をQとする
	D3DXVECTOR3 pos = pScene->GetPos();
	D3DXVECTOR3 posOld = pScene->GetPosOld();

	// Oldposからposの位置を結ぶベクトル(これが線分になる(進行ベクトル))
	const D3DXVECTOR3 vecQP = pos - posOld;

	// 各頂点の外周を沿うベクトル
	const D3DXVECTOR3 vecAB = m_vtxWorld[pnVtx[1]] - m_vtxWorld[pnVtx[0]];
	const D3DXVECTOR3 vecBD = m_vtxWorld[pnVtx[3]] - m_vtxWorld[pnVtx[1]];
	const D3DXVECTOR3 vecDC = m_vtxWorld[pnVtx[2]] - m_vtxWorld[pnVtx[3]];
	const D3DXVECTOR3 vecCA = m_vtxWorld[pnVtx[0]] - m_vtxWorld[pnVtx[2]];

	// 各頂点とシーンの位置を結ぶベクトル
	const D3DXVECTOR3 vecAP = pos - m_vtxWorld[pnVtx[0]];
	const D3DXVECTOR3 vecBP = pos - m_vtxWorld[pnVtx[1]];
	const D3DXVECTOR3 vecCP = pos - m_vtxWorld[pnVtx[2]];
	const D3DXVECTOR3 vecDP = pos - m_vtxWorld[pnVtx[3]];

	// 各頂点とシーンの最後の位置を結ぶベクトル
	const D3DXVECTOR3 vecAQ = posOld - m_vtxWorld[pnVtx[0]];
	const D3DXVECTOR3 vecBQ = posOld - m_vtxWorld[pnVtx[1]];
	const D3DXVECTOR3 vecCQ = posOld - m_vtxWorld[pnVtx[2]];
	const D3DXVECTOR3 vecDQ = posOld - m_vtxWorld[pnVtx[3]];

	// オブジェクトとポリゴンの2D内積
	float crossXY[MESH_VTX];		// XY範囲(X方向の壁)
	float crossZY[MESH_VTX];		// ZY範囲(Z方向の壁)
	float crossXZ[MESH_VTX];		// XZ範囲(平面)

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

	// プレイヤーのサイズXを半径として取得
	float Radius = pScene->GetSize().y / 2;

	//***************************************************************************************
	// 床の当たり判定
	//***************************************************************************************
	if (m_sizeMS.y <= 0)
	{
		// ポリゴンの範囲内にいるかの計算(4つの2D外積結果が0より下なら)
		if (crossXZ[0] < 0.0f && crossXZ[1] < 0.0f &&
			crossXZ[2] < 0.0f && crossXZ[3] < 0.0f)
		{
			// 法線ベクトル
			D3DXVECTOR3 normalVec;
			// ポリゴンの法線ベクトルを求める
			D3DXVec3Cross(&normalVec, &vecAB, &vecBD);
			// 単位ベクトル化
			D3DXVec3Normalize(&normalVec, &normalVec);

			// 板ポリゴンの法線ベクトルと、法線とシーンの位置を結ぶベクトルの内積を求める
			float Dot = D3DXVec3Dot(&normalVec, &vecAP);
			float DotOld = D3DXVec3Dot(&normalVec, &vecAQ);

			// 上からあたる
			if ((Dot <= Radius && DotOld >= -ALLOWABLE_ERROR + Radius))
			{
				// 板ポリゴンの高さを計算し、代入
				D3DXVECTOR3 &pos = pScene->GetPos();
				pos.y = m_vtxWorld[pnVtx[0]].y -(1 / normalVec.y * (normalVec.x * (pos.x - m_vtxWorld[pnVtx[0]].x) + normalVec.z * (pos.z - m_vtxWorld[pnVtx[0]].z)));
				pos.y += Radius;

				ProcessByObjtype(pScene,pos);

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

	//***************************************************************************************
	// 壁の当たり判定
	//***************************************************************************************
	// 壁の向きが-90 <= x < 90 の時のみ それ以外はif文の符号が逆になる 
	else if (m_sizeMS.y > 0)
	{
		if (m_rotMS.y >= -D3DX_PI / 2 && m_rotMS.y < D3DX_PI / 2)
		{

		}

		else
		{
			for (int nCnt = 0; nCnt < MESH_VTX; nCnt++)
			{
				crossXY[nCnt] *= -1;
				crossZY[nCnt] *= -1;
			}
		}


		if ((crossXY[0] < 0.0f && crossXY[1] < 0.0f && crossXY[2] < 0.0f && crossXY[3] < 0.0f) ||	// X方向の範囲
			(crossZY[0] < 0.0f && crossZY[1] < 0.0f && crossZY[2] < 0.0f && crossZY[3] < 0.0f))		// Z方向の範囲
		{
			// 法線ベクトル
			D3DXVECTOR3 normalVec;
			// ポリゴンの法線ベクトルを求める
			D3DXVec3Cross(&normalVec, &vecAB, &vecBD);
			// 単位ベクトル化
			D3DXVec3Normalize(&normalVec, &normalVec);

			// 板ポリゴンの法線ベクトルと、法線とシーンの位置を結ぶベクトルの内積を求める
			float Dot = D3DXVec3Dot(&normalVec, &vecAP);
			float DotOld = D3DXVec3Dot(&normalVec, &vecAQ);

			if (Dot <= Radius && DotOld > -ALLOWABLE_ERROR)
			{
				// 壁ずりベクトルを求め、正規化する
				//D3DXVECTOR3 SlideVec = vecQP - D3DXVec3Dot(&vecQP, &normalVec) * normalVec;
				//D3DXVec3Normalize(&SlideVec, &SlideVec);

				// 板ポリゴンの高さを計算し、代入
				D3DXVECTOR3 &pos = pScene->GetPos();
				D3DXVECTOR3 &posOld = pScene->GetPosOld();

				// 移動ベクトルを求める
				const D3DXVECTOR3 moveVec = pos - posOld;

				if (Dot != 0.0f || DotOld != 0.0f)
				{
					//// 内分比を求める
					//const float &DividRatio = fabsf(DotOld) / (fabsf(DotOld) + fabsf(Dot));
					//// 頂点から衝突点に向かうベクトル
					//const D3DXVECTOR3 &DividVec = ((1 - DividRatio) * vecAQ) + (DividRatio * vecAP);
					//// 内分点を求める
					//const D3DXVECTOR3 &DividP = m_vtxWorld[pnVtx[0]] + DividVec;
					//// プレイヤーがめり込んだ位置から壁の内分点までのベクトルを求める(* PERCENT_EXTRAは壁より少し手前に戻すため )
					//const D3DXVECTOR3 &old = normalVec * D3DXVec3Dot(&normalVec, &((DividP - pos) * PERCENT_EXTRA));
					//pos += old;
					//pScene->SetPos(pos);

					// 対象の押出先を求める
					// ムーブベクトルと法線の内積を求める
					const float fDotMoveVec = -D3DXVec3Dot(&moveVec, &normalVec);
					// 法線の方向に内積分伸ばしたベクトルを求める
					const D3DXVECTOR3 Push = D3DXVECTOR3(normalVec.x * fDotMoveVec, normalVec.y * fDotMoveVec, normalVec.z * fDotMoveVec);
					pos += Push;
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

	else
	{
		return false;
	}
}

//=============================================================================
// ポリゴン(フィールド)と影の当たり判定
//=============================================================================
bool CMeshField::ShadowCollisionMesh(CScene *pScene, const int *pnVtx)
{
	// 板ポリゴンの0,1,2,3番目をそれぞれA,B,C,Dとし
	// pSceneの位置をPとする
	// pSceneの最後の位置をQとする

	// Oldposからposの位置を結ぶベクトル(これが線分になる(進行ベクトル))
	const D3DXVECTOR3 vecQP = pScene->GetPos() - pScene->GetPosOld();

	// 各頂点の外周を沿うベクトル
	const D3DXVECTOR3 vecAB = m_vtxWorld[pnVtx[1]] - m_vtxWorld[pnVtx[0]];
	const D3DXVECTOR3 vecBD = m_vtxWorld[pnVtx[3]] - m_vtxWorld[pnVtx[1]];
	const D3DXVECTOR3 vecDC = m_vtxWorld[pnVtx[2]] - m_vtxWorld[pnVtx[3]];
	const D3DXVECTOR3 vecCA = m_vtxWorld[pnVtx[0]] - m_vtxWorld[pnVtx[2]];

	// 各頂点とシーンの位置を結ぶベクトル
	const D3DXVECTOR3 vecAP = pScene->GetPos() - m_vtxWorld[pnVtx[0]];
	const D3DXVECTOR3 vecBP = pScene->GetPos() - m_vtxWorld[pnVtx[1]];
	const D3DXVECTOR3 vecCP = pScene->GetPos() - m_vtxWorld[pnVtx[2]];
	const D3DXVECTOR3 vecDP = pScene->GetPos() - m_vtxWorld[pnVtx[3]];

	// 各頂点とシーンの最後の位置を結ぶベクトル
	const D3DXVECTOR3 vecAQ = pScene->GetPosOld() - m_vtxWorld[pnVtx[0]];
	const D3DXVECTOR3 vecBQ = pScene->GetPosOld() - m_vtxWorld[pnVtx[1]];
	const D3DXVECTOR3 vecCQ = pScene->GetPosOld() - m_vtxWorld[pnVtx[2]];
	const D3DXVECTOR3 vecDQ = pScene->GetPosOld() - m_vtxWorld[pnVtx[3]];

	// オブジェクトとポリゴンの2D内積
	float crossXZ[MESH_VTX];		// XZ範囲(平面)

	crossXZ[0] = vecAB.x * vecAP.z - vecAP.x * vecAB.z;
	crossXZ[1] = vecBD.x * vecBP.z - vecBP.x * vecBD.z;
	crossXZ[2] = vecDC.x * vecDP.z - vecDP.x * vecDC.z;
	crossXZ[3] = vecCA.x * vecCP.z - vecCP.x * vecCA.z;

	// プレイヤーのサイズXを半径として取得
	CPlayer *pPlayer = (CPlayer*)pScene;
	float Radius = pPlayer->GetSize().x / 2;

	//***************************************************************************************
	// 床の当たり判定
	//***************************************************************************************
		// ポリゴンの範囲内にいるかの計算(4つの2D外積結果が0より下なら)
	if (crossXZ[0] < 0.0f && crossXZ[1] < 0.0f &&
		crossXZ[2] < 0.0f && crossXZ[3] < 0.0f)
	{
		//// 法線ベクトル
		//D3DXVECTOR3 normalVec;
		//// ポリゴンの法線ベクトルを求める
		//D3DXVec3Cross(&normalVec, &vecAB, &vecBD);
		//// 単位ベクトル化
		//D3DXVec3Normalize(&normalVec, &normalVec);

		//// 板ポリゴンの法線ベクトルと、法線とシーンの位置を結ぶベクトルの内積を求める
		//float Dot = D3DXVec3Dot(&normalVec, &vecAP);
		//float DotOld = D3DXVec3Dot(&normalVec, &vecAQ);

		//// 2つの内積結果のベクトルの+-が異なると通る処理(排他的論理和)
		////if(Dot * DotOld < 0)
		//// 上からあたる
		//if (Dot <= Radius && DotOld >= -ALLOWABLE_ERROR)
		//{
		//	// 板ポリゴンの高さを計算し、代入
		//	D3DXVECTOR3 &pos = pScene->GetPos();
		//	pos.y = m_vtxWorld[pnVtx[0]].y - (1 / normalVec.y * (normalVec.x * (pos.x - m_vtxWorld[pnVtx[0]].x) + normalVec.z * (pos.z - m_vtxWorld[pnVtx[0]].z)));
			return true;
		//}
	}

	return false;
}

//------------------------------------------------------------
// メッシュフィールドのオブジェクトタイプ別処理
//------------------------------------------------------------
void CMeshField::ProcessByObjtype(CScene *pScene,D3DXVECTOR3 &pos)
{
	OBJTYPE objtype = pScene->GetObjType();
	CPlayer *pPlayer = nullptr;
	CShadow *pShadow = nullptr;
	switch(objtype)
	{
	case OBJTYPE_PLAYER:	// プレイヤー当たり判定
		pPlayer = (CPlayer*)pScene;
		pPlayer->SetGravity(0.0f, false);	// 重力を0にする

		break;
		
	case OBJTYPE_SHADOW:	// 影当たり判定
		pShadow = (CShadow*)pScene;
		pShadow->SetHeight(m_posMS.y);
		break;
	}
	pScene->SetPos(pos);
}
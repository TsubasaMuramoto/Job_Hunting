//=============================================================================
// renderer.cppの作成
// Author:村元翼
//=============================================================================
#include "renderer.h"
#include "scene.h"
#include "manager.h"
#include "camera.h"
#include "stage.h"
#include "player.h"
#include "DirectInput.h"

//=============================================================================
// コンストラクタ
//=============================================================================
CRenderer::CRenderer()
{

}

//=============================================================================
// デストラクタ
//=============================================================================
CRenderer::~CRenderer()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CRenderer::Init(HWND hWnd, bool bWindow)
{
	D3DPRESENT_PARAMETERS d3dpp;
	D3DDISPLAYMODE d3ddm;

	// Direct3Dオブジェクトの作成
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	if (!m_pD3D)
	{
		return E_FAIL;
	}

	// 現在のディスプレイモードを取得
	if (FAILED(m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	// デバイスのプレゼンテーションパラメータの設定
	ZeroMemory(&d3dpp, sizeof(d3dpp));							// ワークをゼロクリア
	d3dpp.BackBufferCount = 1;									// バックバッファの数
	d3dpp.BackBufferWidth = SCREEN_WIDTH;						// ゲーム画面サイズ(幅)
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;						// ゲーム画面サイズ(高さ)
	d3dpp.BackBufferFormat = d3ddm.Format;						// カラーモードの指定
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;					// 映像信号に同期してフリップする
	d3dpp.EnableAutoDepthStencil = TRUE;						// デプスバッファ（Ｚバッファ）とステンシルバッファを作成
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;					// デプスバッファとして16bitを使う
	d3dpp.Windowed = bWindow;									// ウィンドウモード
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;	// リフレッシュレート
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;	// インターバル

	// デバイスの生成
	// ディスプレイアダプタを表すためのデバイスを作成
	// 描画と頂点処理をハードウェアで行なう
	if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp, &m_pD3DDevice)))
	{
		// 上記の設定が失敗したら
		// 描画をハードウェアで行い、頂点処理はCPUで行なう
		if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp, &m_pD3DDevice)))
		{
			// 上記の設定が失敗したら
			// 描画と頂点処理をCPUで行なう
			if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF, hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp, &m_pD3DDevice)))
			{
				// 生成失敗
				return E_FAIL;
			}
		}
	}

	// レンダーステートの設定
	m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);				// カリングを行わない
	m_pD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);						// Zバッファを使用
	m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);				// αブレンドを行う(透明度を可能にするかの設定)
	m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// αソースカラーの指定
	m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// αデスティネーションカラーの指定

	// サンプラーステートの設定
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);	// テクスチャアドレッシング方法(U値)を設定
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);	// テクスチャアドレッシング方法(V値)を設定
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);	// テクスチャ縮小フィルタモードを設定
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	// テクスチャ拡大フィルタモードを設定

	// テクスチャステージステートの設定
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);

#ifdef _DEBUG
	// デバッグ情報表示用フォントの生成
	D3DXCreateFont(m_pD3DDevice, 20, 10, 0, 0, TRUE, SHIFTJIS_CHARSET,
	OUT_STRING_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Terminal", &m_pFont);
#endif

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CRenderer::Uninit(void)
{
	//---------------------------------
	// 全ての終了処理
	//---------------------------------
	//CScene::ReleaseAll();

#ifdef _DEBUG
	// デバッグ情報表示用フォントの破棄
	if (m_pFont)
	{
		m_pFont->Release();
		m_pFont = nullptr;
	}
#endif

	// デバイスの破棄
	if (m_pD3DDevice)
	{
		m_pD3DDevice->Release();
		m_pD3DDevice = nullptr;
	}

	// Direct3Dオブジェクトの破棄
	if (m_pD3D)
	{
		m_pD3D->Release();
		m_pD3D = nullptr;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void CRenderer::Update(void)
{
	//---------------------------------
	// 全ての更新処理
	//---------------------------------
	CScene::UpdateAll();
}

//=============================================================================
// 描画処理
//=============================================================================
void CRenderer::Draw(void)
{
	// バックバッファ＆Ｚバッファのクリア
	m_pD3DDevice->Clear
	(
		0,
		NULL,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DCOLOR_RGBA(0, 255, 255, 0), 1.0f, 0
	);

	// Direct3Dによる描画の開始
	if (SUCCEEDED(m_pD3DDevice->BeginScene()))
	{
		//---------------------------------
		// 全ての描画処理
		//---------------------------------
		CScene::DrawAll();

#ifdef _DEBUG
		// FPS表示
		DrawFPS();

		if (CManager::Getmode() == CManager::MODE::GAME)
		{
			DrawTXT();
		}
#endif
		// Direct3Dによる描画の終了
		m_pD3DDevice->EndScene();
	}

	// バックバッファとフロントバッファの入れ替え
	m_pD3DDevice->Present(NULL, NULL, NULL, NULL);

}
//=============================================================================
// Zテストの処理
//=============================================================================
void CRenderer::SetZTest(bool bUse, LPDIRECT3DDEVICE9 device)
{
	if (bUse)
	{
		device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	}

	else
	{
		device->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	}
}

//=============================================================================
// アルファテストの処理
//=============================================================================
void CRenderer::SetAlphaTest(bool bUse, LPDIRECT3DDEVICE9 device)
{
	if (bUse)
	{
		device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
		device->SetRenderState(D3DRS_ALPHAREF, 0x00);
	}

	else
	{
		device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
		device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
		device->SetRenderState(D3DRS_ALPHAREF, 0x00);
	}
}

//=============================================================================
// 加算合成(透過版)の処理
//=============================================================================
void CRenderer::SetAddSynthesis(bool bUse, LPDIRECT3DDEVICE9 device)
{
	if (bUse)
	{
		// αブレンディングを加算合成に設定
		device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	}
	else
	{
		// αブレンディングを通常合成に設定
		device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	}
}

//=============================================================================
// 減算合成の処理
//=============================================================================
void CRenderer::SetSubSynthesis(bool bUse, LPDIRECT3DDEVICE9 device)
{
	if (bUse)
	{
		//減算合成の設定
		device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
		device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	}
	else
	{
		//通常合成に戻す
		device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	}
}

#ifdef _DEBUG
//=============================================================================
// FPS表示
//=============================================================================
void CRenderer::DrawFPS(void)
{
	//mainからFPSを取得
	int nCntFPS = GetFPS();

	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	char str[32];

	wsprintf(str, "FPS:%d\n", nCntFPS);

	// テキスト描画
	m_pFont->DrawText(NULL, str, -1, &rect, DT_RIGHT, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));
}

//=============================================================================
// テキスト表示
//=============================================================================
void CRenderer::DrawTXT(void)
{
	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	char aStr[1024];

	CCamera *pCamera = CManager::GetInstance()->GetCamera(0);
	CPlayer *pPlayer = (CPlayer*)CScene::GetScene(CScene::OBJTYPE_PLAYER);

	//sprintfは<stdio.h>が必要&wsprintfと違ってfloatを表示できる
	int nNum = sprintf(&aStr[0], "[カメラの視点       :(%.1f ：%.1f ：%.1f)]\n", pCamera->GetPosV().x, pCamera->GetPosV().y, pCamera->GetPosV().z);

	//使われた文字数の次のaStrのアドレスを渡す
	nNum += sprintf(&aStr[nNum], "[カメラの注視点     :(%.1f ：%.1f : %.1f)]\n", pCamera->GetPosR().x, pCamera->GetPosR().y,pCamera->GetPosR().z);
	nNum += sprintf(&aStr[nNum], "[カメラの向き       :(%.2f : %.2f : %.2f)]\n", pCamera->GetRot().x, pCamera->GetRot().y, pCamera->GetRot().z);
	nNum += sprintf(&aStr[nNum], "[視点と注視点の距離 :(%.1f)]\n\n", pCamera->GetLong());
	nNum += sprintf(&aStr[nNum], "*** カメラ移動 ***\n(移動 : W / A / S / D)\n\n");
	nNum += sprintf(&aStr[nNum], "*** 視点操作 ***\n(移動 : Y / N)(旋回 : Z / C)\n\n");
	nNum += sprintf(&aStr[nNum], "*** 注視点操作 ***\n(移動 : T / B)(旋回 : Q / E)\n\n");
	nNum += sprintf(&aStr[nNum], "*** リセット ***\nR\n\n");
	nNum += sprintf(&aStr[nNum], "***  上視点  ***\n5\n\n");

	if (pPlayer)
	{
		nNum += sprintf(&aStr[nNum], "[プレイヤーの位置     :(%.1f, %.1f, %.1f)]\n", pPlayer->GetPos().x, pPlayer->GetPos().y, pPlayer->GetPos().z);
		nNum += sprintf(&aStr[nNum], "[プレイヤーの回転     :(%.2f, %.2f, %.2f)]\n", pPlayer->GetRot().x, pPlayer->GetRot().y, pPlayer->GetRot().z);
		nNum += sprintf(&aStr[nNum], "[プレイヤーの速度     :(%.1f, %.1f, %.1f)]\n", pPlayer->GetSpeed().x, pPlayer->GetSpeed().y, pPlayer->GetSpeed().z);
		nNum += sprintf(&aStr[nNum], "[ジャンプしているか   :(%s)]\n\n", pPlayer->GetJump() ? "true" : "false");
	}

	nNum += sprintf(&aStr[nNum], "[エフェクト数   :(%d)]\n", CScene::GetNumObj(CScene::OBJTYPE_EFFECT));
	nNum += sprintf(&aStr[nNum], "[ビルボード数   :(%d)]\n", CScene::GetNumObj(CScene::OBJTYPE_BILLBOARD));
	nNum += sprintf(&aStr[nNum], "[モデル数   :(%d)]\n\n", CScene::GetNumObj(CScene::OBJTYPE_MODEL));
	nNum += sprintf(&aStr[nNum], "[DInput左スティック入力情報   :(%s)]\n", CDInput::GetStateL());
	nNum += sprintf(&aStr[nNum], "[DInput右スティック入力情報   :(%s)]\n\n", CDInput::GetStateR());

	nNum += sprintf(&aStr[nNum], "[BボタンかKキーかFキーで爆弾を出す]\n\n");


	m_pFont->DrawText(NULL, aStr, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));
}

#endif
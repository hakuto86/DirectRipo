//#include "main.h"
//#include "pause.h"
//#include "fade.h"
//#include "input.h"
//
////グローバル変数
//LPDIRECT3DTEXTURE9 g_apTexturePause[3] = {};  //テクスチャのポインタ
//LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPause = NULL;  //頂点バッファへのポインタ
//PAUSE_MENU g_pauseMenu;
//
////==================================================================
////ポーズの初期化の処理
////==================================================================
//void InitPause(void)
//{
//	LPDIRECT3DDEVICE9 pDevice; //デバイスポインタ
//
//	pDevice = GetDevice();     //デバイスの取得
//
//	//テクスチャの読み込み
//	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\title.png", &g_pTexturePause);
//
//	//頂点バッファの生成
//	pDevice->CreateVertexBuffer(
//		sizeof(VERTEX_2D) * 4,
//		D3DUSAGE_WRITEONLY,
//		FVF_VERTEX_2D,
//		D3DPOOL_MANAGED,
//		&g_pVtxBuffPause,
//		NULL);
//	VERTEX_2D * pVtx;
//
//	//頂点バッファをロックし、頂点情報へのポインタを取得
//	g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);
//
//	//頂点座標の設定
//	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
//	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
//	pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
//	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);
//
//	//rhwの設定
//	pVtx[0].rhw = 1.0f;
//	pVtx[1].rhw = 1.0f;
//	pVtx[2].rhw = 1.0f;
//	pVtx[3].rhw = 1.0f;
//
//	//頂点カラーの設定
//	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
//	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
//	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
//	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
//
//	//テクスチャ座標の設定
//	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
//	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
//	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
//	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
//
//	//頂点バッファをアンロックする
//	g_pVtxBuffPause->Unlock();
//}
//
////==================================================================
////ポーズの終了処理
////==================================================================
//void UninitPause(void)
//{
//	//テクスチャの破棄
//	if (g_pTexturePause != NULL)
//	{
//		g_pTexturePause->Release();
//		g_pTexturePause = NULL;
//	}
//
//	//頂点バッファの破棄
//	if (g_pVtxBuffPause != NULL)
//	{
//		g_pVtxBuffPause->Release();
//		g_pVtxBuffPause = NULL;
//	}
//}
//
////==================================================================
////ポーズの更新処理
////==================================================================
//void UpdateTitle(void)
//{
//	if (GetKeyboardTrigger(DIK_RETURN) == true)
//	{//決定キー(ENTERキー)が押された
//	 //モード設定(ゲーム画面に移行)
//		SetFade(MODE_GAME);
//	}
//}
//
////==================================================================
////ポーズの描画処理
////==================================================================
//void DrawTitle(void)
//{
//	LPDIRECT3DDEVICE9 pDevice; //デバイスポインタ
//
//	pDevice = GetDevice();	    //デバイスの取得
//
//								//頂点バッファをデータストリームに設定
//	pDevice->SetStreamSource(0, g_pVtxBuffTitle, 0, sizeof(VERTEX_2D));
//
//	//頂点のフォーマットの設定
//	pDevice->SetFVF(FVF_VERTEX_2D);
//
//	//テクスチャの設定
//	pDevice->SetTexture(0, g_pTextureTitle);
//
//	//ポリゴンの描画
//	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
//}
#include "main.h"
#include "bg.h"

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureBG[NUM_BG] = {};  //テクスチャのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBG = NULL;  //頂点バッファへのポインタ
D3DXVECTOR3 g_posBg;  //位置
float g_aTexV[NUM_BG];
//==================================================================
//背景の初期化の処理
//==================================================================
void InitBg(void)
{
	int nCntBG;

	LPDIRECT3DDEVICE9 pDevice; //デバイスポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\umi.jpeg", &g_pTextureBG[0]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\umi.jpeg", &g_pTextureBG[1]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\umi.jpeg", &g_pTextureBG[2]);

	//テクスチャ座標の開始位置(V値)の初期化
	for (nCntBG = 0; nCntBG < NUM_BG; nCntBG++)
	{
		g_aTexV[nCntBG] = 0.0f;
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * NUM_BG,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBG,
		NULL);
	VERTEX_2D * pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBG->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBG = 0; nCntBG < NUM_BG; nCntBG++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラーの設定
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, g_aTexV[nCntBG]);
		pVtx[1].tex = D3DXVECTOR2(1.0f, g_aTexV[nCntBG]);
		pVtx[2].tex = D3DXVECTOR2(0.0f, g_aTexV[nCntBG] + 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, g_aTexV[nCntBG] + 1.0f);

		pVtx += 4;   //頂点データのポインタを4つ分進める
	}
	//頂点バッファをアンロックする
	g_pVtxBuffBG->Unlock();
}

//==================================================================
//背景の終了処理
//==================================================================
void UninitBg(void)
{
	//テクスチャの破棄
	for (int nCnt = 0; nCnt < 3; nCnt++)
	{
		if (g_pTextureBG[nCnt] != NULL)
		{
			g_pTextureBG[nCnt]->Release();
			g_pTextureBG[nCnt] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxBuffBG != NULL)
	{
		g_pVtxBuffBG->Release();
		g_pVtxBuffBG = NULL;
	}
}

//==================================================================
//背景の更新処理
//==================================================================
void UpdateBg(void)
{
	int nCntBG;

	VERTEX_2D * pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBG->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBG = 0; nCntBG < NUM_BG; nCntBG++)
	{
		//テクスチャ座標の開始位置(V値)の更新
		g_aTexV[0] -= 0.005f*(nCntBG + 1);

		//テクスチャ座標の更新
		pVtx[0].tex = D3DXVECTOR2(0.0f, g_aTexV[nCntBG]);
		pVtx[1].tex = D3DXVECTOR2(1.0f, g_aTexV[nCntBG]);
		pVtx[2].tex = D3DXVECTOR2(0.0f, g_aTexV[nCntBG] + 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, g_aTexV[nCntBG] + 1.0f);

		pVtx += 4;   //頂点データのポインタを4つ分進める

	}
		//頂点バッファをアンロックする
	    g_pVtxBuffBG->Unlock();
}

//==================================================================
//背景の描画処理
//==================================================================
void DrawBg(void)
{
	int nCntBG;

	LPDIRECT3DDEVICE9 pDevice; //デバイスポインタ

	pDevice = GetDevice();	    //デバイスの取得
	
	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffBG, 0, sizeof(VERTEX_2D));

	//頂点のフォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	for (nCntBG = 0; nCntBG < NUM_BG; nCntBG++)
	{
		pDevice->SetTexture(0, g_pTextureBG[nCntBG]);
		
		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntBG * 4, 3);
	}
}
#include "main.h"
#include "fade.h"
#include "title.h"
#include "input.h"

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureTitle[NUM_Title] = {};  //テクスチャのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTitle = NULL;  //頂点バッファへのポインタ
float g_aTexV1[NUM_Title];
Title g_nTitle[NUM_Title];
int nCntT;
bool bFlash;

//==================================================================
//タイトル画面の初期化の処理
//==================================================================
void InitTitle(void)
{
	int nCntTitle;
	int nCntT = 0;
	bFlash = false;

	LPDIRECT3DDEVICE9 pDevice; //デバイスポインタ
		   
	pDevice = GetDevice();     //デバイスの取得

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\title.png", &g_pTextureTitle[0]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\-PRESS ENTER-.png", &g_pTextureTitle[1]);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * NUM_Title,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTitle,
		NULL);
	VERTEX_2D * pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntTitle = 0; nCntTitle < NUM_Title; nCntTitle++)
	{
		g_nTitle[nCntTitle].bUse = true;

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
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;   //頂点データのポインタを4つ分進める
	}
	//頂点バッファをアンロックする
	g_pVtxBuffTitle->Unlock();
}

//============================================================0======
//タイトル画面の終了処理
//==================================================================
void UninitTitle(void)
{
	//テクスチャの破棄
	for (int nCnt = 0; nCnt < 2; nCnt++)
	{
		if (g_pTextureTitle[nCnt] != NULL)
		{
			g_pTextureTitle[nCnt]->Release();
			g_pTextureTitle[nCnt] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxBuffTitle != NULL)
	{
		g_pVtxBuffTitle->Release();
		g_pVtxBuffTitle = NULL;
	}

}

//==================================================================
//タイトル画面の更新処理
//==================================================================
void UpdateTitle(void)
{
	nCntT++;

	if (GetKeyboardTrigger(DIK_RETURN) == true && GetFade() == FADE_NONE)
	{//決定キー(ENTERキー)が押された
		bFlash = true;
		//モード設定(ゲーム画面に移行)
		SetFade(MODE_GAME);
	}

	if (nCntT >= 26 && bFlash == false)
	{
		g_nTitle[1].bUse = g_nTitle[1].bUse ? false : true;
		nCntT = 0;
	}
	else if (nCntT >= 6 && bFlash == true)
	{
		g_nTitle[1].bUse = g_nTitle[1].bUse ? false : true;
		nCntT = 0;
	}
}

//==================================================================
//タイトル画面の描画処理
//==================================================================
void DrawTitle(void)
{
	int nCntTitle;

	LPDIRECT3DDEVICE9 pDevice; //デバイスポインタ

	pDevice = GetDevice();	    //デバイスの取得

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffTitle, 0, sizeof(VERTEX_2D));

	//頂点のフォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	for (nCntTitle = 0; nCntTitle < NUM_Title; nCntTitle++)
	{
		if (g_nTitle[nCntTitle].bUse == true)
		{
			pDevice->SetTexture(0, g_pTextureTitle[nCntTitle]);
			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntTitle * 4, 4);
		}
	}
}
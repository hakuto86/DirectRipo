#include "main.h"
#include "hp_mark.h"

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureHpMark = NULL;  //テクスチャのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffHpMark = NULL;  //頂点バッファへのポインタ
D3DXVECTOR3 g_posHpMark;   //スコアの位置

//==================================================================
//スコアの初期化の処理
//==================================================================
void InitHpMark(void)
{
	LPDIRECT3DDEVICE9 pDevice; //デバイスポインタ

	pDevice = GetDevice();	   //デバイスの取得

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\Life.png", &g_pTextureHpMark);

	g_posHpMark = D3DXVECTOR3(1100, 50, 0.0f);  //位置を初期化する

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffHpMark,
		NULL);
	VERTEX_2D * pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffHpMark->Lock(0, 0, (void**)&pVtx, 0);

		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(150.0f, 20.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(200.0f, 20.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(150.0f, 40.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(200.0f, 40.0f, 0.0f);

		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラーの設定
		pVtx[0].col = D3DCOLOR_RGBA(255, 100, 100, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 100, 100, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 100, 100, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 100, 100, 255);

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロックする
	g_pVtxBuffHpMark->Unlock();
}

//==================================================================
//スコアの終了処理
//==================================================================
void UninitHpMark(void)
{
	//テクスチャの破棄
	if (g_pTextureHpMark != NULL)
	{
		g_pTextureHpMark->Release();
		g_pTextureHpMark = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffHpMark != NULL)
	{
		g_pVtxBuffHpMark->Release();
		g_pVtxBuffHpMark = NULL;
	}
}

//==================================================================
//スコアの更新処理
//==================================================================
void UpdateHpMark(void)
{
	VERTEX_2D * pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffHpMark->Lock(0, 0, (void**)&pVtx, 0);

		//テクスチャ座標の設定
		pVtx[0].pos = D3DXVECTOR3(350.0f , 20.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(390.0f , 20.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(350.0f , 70.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(390.0f , 70.0f, 0.0f);

	//頂点バッファをアンロックする
	g_pVtxBuffHpMark->Unlock();
}

//==================================================================
//スコアの描画処理
//==================================================================
void DrawHpMark(void)
{
	LPDIRECT3DDEVICE9 pDevice; //デバイスポインタ

	pDevice = GetDevice();	    //デバイスの取得

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffHpMark, 0, sizeof(VERTEX_2D));

	//頂点のフォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureHpMark);

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}
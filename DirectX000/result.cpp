#include "main.h"
#include "result.h"
#include "input.h"
#include "fade.h"
#include "resultscore.h"

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureResult = {};  //テクスチャのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffResult = NULL;  //頂点バッファへのポインタ

//==================================================================
//リザルト画面の初期化の処理
//==================================================================
void InitResult(void)
{
	LPDIRECT3DDEVICE9 pDevice; //デバイスポインタ

	pDevice = GetDevice();     //デバイスの取得

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\result.png", &g_pTextureResult);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffResult,
		NULL);
	VERTEX_2D * pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffResult->Lock(0, 0, (void**)&pVtx, 0);

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

	//頂点バッファをアンロックする
	g_pVtxBuffResult->Unlock();

	//スコアの初期化処理
	InitResultScore();
}

//==================================================================
//リザルト画面の終了処理
//==================================================================
void UninitResult(void)
{
	//テクスチャの破棄
	if (g_pTextureResult != NULL)
	{
		g_pTextureResult->Release();
		g_pTextureResult = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffResult != NULL)
	{
		g_pVtxBuffResult->Release();
		g_pVtxBuffResult = NULL;
	}
	//スコアの初期化処理
	UninitResultScore();
}

//==================================================================
//リザルト画面の更新処理
//==================================================================
void UpdateResult(void)
{
	if (GetKeyboardTrigger(DIK_RETURN) == true && GetFade() == FADE_NONE)
	{//決定キー(ENTERキー)が押された
	 //モード設定(ゲーム画面に移行)
		SetFade(MODE_TITLE);
	}
	UpdateResultScore();
}

//==================================================================
//リザルト画面の描画処理
//==================================================================
void DrawResult(void)
{
	LPDIRECT3DDEVICE9 pDevice; //デバイスポインタ

	pDevice = GetDevice();	    //デバイスの取得

								//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffResult, 0, sizeof(VERTEX_2D));

	//頂点のフォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureResult);

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	DrawResultScore();
}
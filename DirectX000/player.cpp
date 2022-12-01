#include "main.h"
#include "player.h"
#include "input.h"
#include "bullet.h"
#include "explosion.h"
#include "game.h"
#include "enemy.h"
#include "hp.h"

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTexturePlayer = NULL;  //テクスチャのポインタ
//VERTEX_2D g_aVertex[4];  //頂点情報を格納
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPlayer = NULL;  //頂点バッファへのポインタ
int g_nCounterAnimPlayer;  //アニメーションカウンター
int g_nPatternAnimPlayer;  //アニメーションパターンNo.
float g_fLengthPlayer;    //対角線の長さ
float g_fAnglePlayer;     //対角線の角度
Player g_player;

Player *GetPlayer(void)
{
	return &g_player;
}

//==================================================================
//プレイヤー初期化の処理
//==================================================================
void InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice; //デバイスポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	g_nCounterAnimPlayer = 0;  //カウンターを初期化する
	g_nPatternAnimPlayer = 0;  //パターンNo.を初期化する
	g_player.pos = D3DXVECTOR3(500.0f,500.0f,0.0f); //プレイヤーの初期位置
	g_player.move = D3DXVECTOR3(0.0f,0.0f,0.0f);   //プレイヤーの初期移動量
	g_player.nLife = 4;                            //プレイヤーの体力
	g_player.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  //向きを初期化する(Z値を使用)
	g_fLengthPlayer = sqrtf(PLAYER_SIZE_WIDTH * PLAYER_SIZE_WIDTH + PLAYER_SIZE_HEIGHT * PLAYER_SIZE_HEIGHT)* 0.5f;  //対角線の長さを算出する
	g_fAnglePlayer = atan2f(140.0f, 200.0f);  //対角線の角度を算出する
	g_player.state = PLAYERSTATE_NORMAL;
	g_player.nCounterState = 0;
	g_player.bDisp = true;
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\player.png", &g_pTexturePlayer);
	
	//頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPlayer,
		NULL);
	VERTEX_2D * pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(-PLAYER_SIZE_WIDTH, -PLAYER_SIZE_HEIGHT, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(+PLAYER_SIZE_WIDTH, -PLAYER_SIZE_HEIGHT, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-PLAYER_SIZE_WIDTH, +PLAYER_SIZE_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(+PLAYER_SIZE_WIDTH, +PLAYER_SIZE_HEIGHT, 0.0f);

	////回転の処理
	//pVtx[0].pos.x = g_posPlayer.x + sinf(0.0f + (D3DX_PI - g_fAnglePlayer)) * g_fLengthPlayer;
	//pVtx[0].pos.y = g_posPlayer.y + sinf(0.0f + (D3DX_PI - g_fAnglePlayer)) * g_fLengthPlayer;
	//pVtx[0].pos.z = 0.0f;

	//pVtx[1].pos.x = g_posPlayer.x + sinf(0.0f + (D3DX_PI + g_fAnglePlayer)) * g_fLengthPlayer;
	//pVtx[1].pos.y = g_posPlayer.y + sinf(0.0f + (D3DX_PI - g_fAnglePlayer)) * g_fLengthPlayer;
	//pVtx[1].pos.z = 0.0f;

	//pVtx[2].pos.x = g_posPlayer.x + sinf(0.0f + (D3DX_PI - g_fAnglePlayer)) * g_fLengthPlayer;
	//pVtx[2].pos.y = g_posPlayer.y + sinf(0.0f + (D3DX_PI + g_fAnglePlayer)) * g_fLengthPlayer;
	//pVtx[2].pos.z = 0.0f;

	//pVtx[3].pos.x = g_posPlayer.x + sinf(0.0f + (D3DX_PI + g_fAnglePlayer)) * g_fLengthPlayer;
	//pVtx[3].pos.y = g_posPlayer.y + sinf(0.0f + (D3DX_PI + g_fAnglePlayer)) * g_fLengthPlayer;
	//pVtx[3].pos.z = 0.0f;

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
	pVtx[0].tex = D3DXVECTOR2(((float)(g_nPatternAnimPlayer % PATTERN_WIDTH)) / PATTERN_WIDTH,
		(float)(g_nPatternAnimPlayer / PATTERN_WIDTH) / PATTERN_HEIGHT);
	pVtx[1].tex = D3DXVECTOR2(((float)(g_nPatternAnimPlayer % PATTERN_WIDTH) + 1) / PATTERN_WIDTH,
		(float)(g_nPatternAnimPlayer / PATTERN_WIDTH) / PATTERN_HEIGHT);
	pVtx[2].tex = D3DXVECTOR2((float)(g_nPatternAnimPlayer % PATTERN_WIDTH) / PATTERN_WIDTH,
		((float)(g_nPatternAnimPlayer / PATTERN_WIDTH) + 1) / PATTERN_HEIGHT);
	pVtx[3].tex = D3DXVECTOR2(((float)(g_nPatternAnimPlayer % PATTERN_WIDTH) + 1) / PATTERN_WIDTH,
		((float)(g_nPatternAnimPlayer / PATTERN_WIDTH) + 1) / PATTERN_HEIGHT);

	//頂点バッファをアンロックする
	g_pVtxBuffPlayer->Unlock();

	AddHp(g_player.nLife);
}

//==================================================================
//プレイヤーの終了処理
//==================================================================
void UninitPlayer(void)
{
	//テクスチャの破棄
	if (g_pTexturePlayer != NULL)
	{
		g_pTexturePlayer->Release();
		g_pTexturePlayer = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffPlayer != NULL)
	{
		g_pVtxBuffPlayer->Release();
		g_pVtxBuffPlayer = NULL;
	}
}

#if 1
//==================================================================
//プレイヤーの更新処理
//==================================================================
void UpdatePlayer(void)
{
	//頂点バッファの生成
	VERTEX_2D *pVtx;

	if (GetKeyboardPress(DIK_A) == true)
	{//Aキーが押された

		if (GetKeyboardPress(DIK_W) == true)
		{//Wキーが押された
			g_player.move.x += sinf(-D3DX_PI * 0.75f) * MOVE;
			g_player.move.y += cosf(-D3DX_PI * 0.75f) * MOVE;
		}
		else if (GetKeyboardPress(DIK_S) == true)
		{//Sキーが押された
			g_player.move.x += sinf(-D3DX_PI * 0.25f) * MOVE;
			g_player.move.y += cosf(-D3DX_PI * 0.25f) * MOVE;
		}
		else
		{
			g_player.move.x -= MOVE;
		}
	}

	else if (GetKeyboardPress(DIK_D) == true)
	{//Dキーが押された
		if (GetKeyboardPress(DIK_W) == true)
		{//Wキーが押された
			g_player.move.x += sinf( D3DX_PI * 0.75f) * MOVE;
			g_player.move.y += cosf(-D3DX_PI * 0.75f) * MOVE;
		}
		else if (GetKeyboardPress(DIK_S) == true)
		{//Sキーが押された
			g_player.move.x += sinf( D3DX_PI * 0.25f) * MOVE;
			g_player.move.y += cosf(-D3DX_PI * 0.25f) * MOVE;
		}
		else
		{
			g_player.move.x += MOVE;
		}
	}

	else if (GetKeyboardPress(DIK_W) == true)
	{//Wキーが押された
		g_player.move.y -= MOVE;
	}
	
	else if (GetKeyboardPress(DIK_S) == true)
	{//Sキーが押された
		g_player.move.y += MOVE;
	}	 

	//画面外に出た
	if (g_player.pos.x >= SCREEN_WIDTH - 100)
	{
		g_player.pos.x = SCREEN_WIDTH - 100;
	}

	if (g_player.pos.y >= SCREEN_HEIGHT -30)
	{
		g_player.pos.y = SCREEN_HEIGHT -30;
	}

	if (g_player.pos.x <= 0 + 150)
	{
		g_player.pos.x = 0 +150;
	}

	if (g_player.pos.y <= 0 +65)
	{
		g_player.pos.y = 0 +65;
	}

	//位置を更新
	g_player.pos.x +=g_player.move.x;
	g_player.pos.y +=g_player.move.y;

	//移動量を更新(減衰させる)
	g_player.move.x += (0.0f - g_player.move.x) * 0.1f;
	g_player.move.y += (0.0f - g_player.move.y) * 0.1f;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos.x = g_player.pos.x - PLAYER_SIZE_WIDTH;
	pVtx[0].pos.y = g_player.pos.y - PLAYER_SIZE_HEIGHT;
	pVtx[1].pos.x = g_player.pos.x + PLAYER_SIZE_WIDTH;
	pVtx[1].pos.y = g_player.pos.y - PLAYER_SIZE_HEIGHT;
	pVtx[2].pos.x = g_player.pos.x - PLAYER_SIZE_WIDTH;
	pVtx[2].pos.y = g_player.pos.y + PLAYER_SIZE_HEIGHT;
	pVtx[3].pos.x = g_player.pos.x + PLAYER_SIZE_WIDTH;
	pVtx[3].pos.y = g_player.pos.y + PLAYER_SIZE_HEIGHT;

	g_nCounterAnimPlayer++; //カウンターを加算

	if ((g_nCounterAnimPlayer % 10) == 0)
	{//一定時間経過
		//パターンNo.を更新する
		g_nPatternAnimPlayer = (g_nPatternAnimPlayer + 1) % 10;

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(((float)(g_nPatternAnimPlayer % PATTERN_WIDTH))/ PATTERN_WIDTH,
			(float)(g_nPatternAnimPlayer / PATTERN_WIDTH)/ PATTERN_HEIGHT);
		pVtx[1].tex = D3DXVECTOR2(((float)(g_nPatternAnimPlayer % PATTERN_WIDTH)+1)/ PATTERN_WIDTH,
			(float)(g_nPatternAnimPlayer / PATTERN_WIDTH)/ PATTERN_HEIGHT);
		pVtx[2].tex = D3DXVECTOR2((float)(g_nPatternAnimPlayer % PATTERN_WIDTH)/ PATTERN_WIDTH,
			((float)(g_nPatternAnimPlayer / PATTERN_WIDTH)+1)/ PATTERN_HEIGHT);
		pVtx[3].tex = D3DXVECTOR2(((float)(g_nPatternAnimPlayer % PATTERN_WIDTH)+1)/ PATTERN_WIDTH,
			((float)(g_nPatternAnimPlayer / PATTERN_WIDTH)+1)/ PATTERN_HEIGHT);

		//頂点バッファをアンロックする
		g_pVtxBuffPlayer->Unlock();
	}

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

	switch (g_player.state)
	{
	case PLAYERSTATE_NORMAL:
		if (GetKeyboardTrigger(DIK_SPACE) == true)
		{//SPACEキーが押された
		 //弾の設定
			SetBullet(D3DXVECTOR3(g_player.pos.x + 180.0f, g_player.pos.y - 20.0f, g_player.pos.z), D3DXVECTOR3(BULLET_LIGHT, 0.0f, 0.0f), 100, BULLETTYPE_PLAYER);
		}
		break;

	case PLAYERSTATE_DAMAGE:

		g_player.nCounterState--;

		if (g_player.nCounterState <= 0)
		{
			g_player.state = PLAYERSTATE_NORMAL;

			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}
		if (g_player.nCounterState % 5 == 0)
		{
			g_player.bDisp = g_player.bDisp ? false : true;
			
			if (g_player.nCounterState <= 0)
			{
				g_player.bDisp = true;
			}
		}
		break;
	}
	//頂点バッファをアンロックする
	g_pVtxBuffPlayer->Unlock();
}
#endif

//==================================================================
//プレイヤーの描画処理
//==================================================================
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice; //デバイスポインタ

	pDevice = GetDevice();	    //デバイスの取得
	
	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffPlayer, 0, sizeof(VERTEX_2D));

	//頂点のフォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	

	//プレイヤーの描画
	if (g_player.bDisp == true)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_pTexturePlayer);

		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	}
}

//==================================================================
//プレイヤーのヒット処理
//==================================================================
void HitPlayer(int nDamage)
{
	if (g_player.state == PLAYERSTATE_NORMAL && g_player.nCounterState == 0)
	{
		g_player.nLife -= nDamage;
		AddHp(-1);
	}

	if (g_player.nLife <= 0)
	{		
		//爆発の設定
		SetExplosion(g_player.pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		g_player.state = PLAYERSTATE_DEATH;

		//ゲームの状態の設定
		SetGameState(GAMESTATE_END, 60);
		g_player.bDisp = false;
	}

	else
	{
		VERTEX_2D * pVtx;

		//頂点バッファをロックし、頂点情報へのポインタを取得
		g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

		//頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

		//頂点バッファをアンロックする
		g_pVtxBuffPlayer->Unlock();

		if (g_player.state == PLAYERSTATE_NORMAL && g_player.nCounterState == 0)
		{
			g_player.state = PLAYERSTATE_DAMAGE;
			g_player.nCounterState = 100;
		}
	}
}
#include "game.h"
#include "player.h"
#include "bg.h"
#include "bullet.h"
#include "explosion.h"
#include "enemy.h"
#include "input.h"
#include "score.h"
#include "resultscore.h"
#include "fade.h"
#include "Effect.h"
#include "enemy_bullet.h"
#include "hp.h"
#include "hp_mark.h"

//グローバル変数
GAMESTATE g_gameState = GAMESTATE_NONE;   //ゲームの状態
int g_nCounterGameState = 0;              //状態管理カウンター
/*bool g_bPause = false; */                   //ポーズ状態のON/OFF

//==================================================================
//ゲーム画面の初期化処理
//==================================================================
void InitGame(void)
{
	//背景の初期化処理
	InitBg();
	//スコアの初期化処理
	InitScore();
	//体力の初期化処理
	InitHp();
	//体力の初期化処理
	InitHpMark();
	//プレイヤーの初期化処理
	InitPlayer();
	//弾の初期化処理
	InitBullet();
	//爆発の初期化処理
	InitExplosion();
	//エフェクトの初期化処理
	InitEffect();
	//敵の初期化処理
	InitEnemy();
	//敵の弾の初期化処理
	InitEnemyBullet();

	g_gameState = GAMESTATE_NORMAL;   //通常状態に設定
	g_nCounterGameState = 0;
}

//==================================================================
//ゲーム画面の終了処理
//==================================================================
void UninitGame(void)
{
	//背景の終了処理
	UninitBg();
	//スコアの終了処理
	UninitScore();
	//体力の終了処理
	UninitHp();
	//体力の終了処理
	UninitHpMark();
	//プレイヤーの終了処理
	UninitPlayer();
	//弾の終了処理
	UninitBullet();
	//爆発の終了処理
	UninitExplosion();
	//エフェクトの終了処理
	UninitEffect();
	//敵の終了処理
	UninitEnemy();
	//敵の弾の終了処理
	UninitEnemyBullet();

	SetResultScore(*GetScore());

	// ランキングソート
	RankingResultScore();
}

//==================================================================
//ゲーム画面の更新処理
//==================================================================
void UpdateGame(void)
{
	//背景の更新処理
	UpdateBg();
	//スコアの更新処理
	UpdateScore();
	//体力の更新処理
	UpdateHp();
	//体力の更新処理
	UpdateHpMark();
	//プレイヤーの更新処理
	UpdatePlayer();
	//弾の更新処理
	UpdateBullet();
	//爆発の更新処理
	UpdateExplosion();
	//エフェクトの更新処理
	UpdateEffect();
	//敵の更新処理
	UpdateEnemy();
	//敵の弾の更新処理
	UpdateEnemyBullet();
	SetScore();

	switch (g_gameState)
	{
	case GAMESTATE_NORMAL:  //通常状態
		break;

	case GAMESTATE_END:     //終了状態
		g_nCounterGameState--;
		if (g_nCounterGameState <= 0)
		{
			g_gameState = GAMESTATE_NONE;  //何もしていない状態に設定

			//モード設定(リザルト画面に移行)
			SetFade(MODE_RESULT);
			RankingResultScore();
		}
		break;
	case GAMESTATE_CLEAR:     //終了状態
		g_nCounterGameState--;
		if (g_nCounterGameState <= 0)
		{
			g_gameState = GAMESTATE_NONE;  //何もしていない状態に設定

										   //モード設定(リザルト画面に移行)
			SetFade(MODE_CLEAR);
			RankingResultScore();
		}
		break;
	}
}

//==================================================================
//ゲーム画面の描画処理
//==================================================================
void DrawGame(void)
{
	//背景の描画処理
	DrawBg();
	//スコアの描画処理
	DrawScore();
	//体力の描画処理
	DrawHp();
	//体力の描画処理
	DrawHpMark();
	//プレイヤーの描画処理
	DrawPlayer();
	//弾の描画処理
	DrawBullet();
	//爆発の描画処理
	DrawExplosion();
	//背景の描画処理
	DrawEffect();
	//敵の描画処理
	DrawEnemy();
	//敵の弾の描画処理
	DrawEnemyBullet();
}

//==================================================================
//ゲームの状態の設定
//==================================================================
void SetGameState(GAMESTATE state, int nCounter)
{
	g_gameState = state;
	g_nCounterGameState = nCounter;
}

//==================================================================
//ゲームの状態の取得
//==================================================================
GAMESTATE GetGameState(void)
{
	return g_gameState;
}
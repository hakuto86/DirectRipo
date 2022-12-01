#ifndef _PLAYER_H_
#define _PLAYER_H_

//マクロ定義
#define PATTERN_WIDTH (1)
#define PATTERN_HEIGHT (3)
#define MOVE (0.8f)  //移動量
#define BULLET_UP (-10.0f)  //移動量
#define BULLET_DIA (10.0f)  //移動量
#define BULLET_LIGHT (10.0f)  //移動量
#define BULLET_LEFT (-10.0f)  //移動量
#define PLAYER_SIZE_WIDTH (130.0f)  //プレイヤーの横のサイズ
#define PLAYER_SIZE_HEIGHT (80.0f)//プレイヤーの縦のサイズ

//プレイヤーの状態
typedef enum
{
	PLAYERSTATE_APPEAR = 0,		//出現状態(点滅)
	PLAYERSTATE_NORMAL,			//通常状態
	PLAYERSTATE_DAMAGE,			//ダメージ状態
	PLAYERSTATE_WAIT,			//出現待ち状態(表示なし)
	PLAYERSTATE_DEATH,			//死亡状態
	PLAYESTATE_MAX
}PLAYERSTATE;

//プレイヤー構造体の定義
typedef struct
{
	D3DXVECTOR3 pos;   //位置
	D3DXVECTOR3 rot;   //向き
	D3DXVECTOR3 move;  //移動量
	int nLife;         //体力
	PLAYERSTATE state; //プレイヤーの状態
	int nCounterState; //状態管理カウンター
	bool bDisp;        //表示するかしないか
	//bool bUse;         //使用されているかどうか
}Player;

//プロトタイプ宣言
void InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
Player *GetPlayer(void);
void HitPlayer(int nDamage);

#endif


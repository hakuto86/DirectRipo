#ifndef _BULLET_H_
#define _BULLET_H_
#include "main.h"

//マクロ定義
#define BULLET_SIZE_WIDTH (80.0f)  //弾の横のサイズ
#define BULLET_SIZE_HEIGHT (60.0f)//弾の縦のサイズ

//弾の種類
typedef enum
{
	BULLETTYPE_PLAYER = 0,  //プレイヤーの弾
	BULLETTYPE_MAX,
}BULLETTYPE;

//プロトタイプ宣言
void InitBullet(void);
void UninitBullet(void);
void UpdateBullet(void);
void DrawBullet(void);
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move, int nLife, BULLETTYPE type);



#endif
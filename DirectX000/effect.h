#ifndef _EFFECT_H_
#define _EFFECT_H_
#include "main.h"

//マクロ定義
#define EFFECT_SIZE_WIDTH (30.0f)  //弾の横のサイズ
#define EFFECT_SIZE_HEIGHT (20.0f)//弾の縦のサイズ

//プロトタイプ宣言
void InitEffect(void);
void UninitEffect(void);
void UpdateEffect(void);
void DrawEffect(void);
void SetEffect(D3DXVECTOR3 pos, D3DXCOLOR col,float fRadius,int nLife);

#endif

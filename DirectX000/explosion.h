#ifndef _EXPLOSION_H_
#define _EXPLOSION_H_
#include "main.h"

//マクロ定義
#define PATTERN_EXPLOSION_WIDTH (8)
#define PATTERN_EXPLOSION_HEIGHT (1)

//プロトタイプ宣言
void InitExplosion(void);
void UninitExplosion(void);
void UpdateExplosion(void);
void DrawExplosion(void);
void SetExplosion(D3DXVECTOR3 pos, D3DXCOLOR col);

#endif

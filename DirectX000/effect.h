#ifndef _EFFECT_H_
#define _EFFECT_H_
#include "main.h"

//�}�N����`
#define EFFECT_SIZE_WIDTH (30.0f)  //�e�̉��̃T�C�Y
#define EFFECT_SIZE_HEIGHT (20.0f)//�e�̏c�̃T�C�Y

//�v���g�^�C�v�錾
void InitEffect(void);
void UninitEffect(void);
void UpdateEffect(void);
void DrawEffect(void);
void SetEffect(D3DXVECTOR3 pos, D3DXCOLOR col,float fRadius,int nLife);

#endif

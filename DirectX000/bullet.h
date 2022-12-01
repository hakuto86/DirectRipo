#ifndef _BULLET_H_
#define _BULLET_H_
#include "main.h"

//�}�N����`
#define BULLET_SIZE_WIDTH (80.0f)  //�e�̉��̃T�C�Y
#define BULLET_SIZE_HEIGHT (60.0f)//�e�̏c�̃T�C�Y

//�e�̎��
typedef enum
{
	BULLETTYPE_PLAYER = 0,  //�v���C���[�̒e
	BULLETTYPE_MAX,
}BULLETTYPE;

//�v���g�^�C�v�錾
void InitBullet(void);
void UninitBullet(void);
void UpdateBullet(void);
void DrawBullet(void);
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move, int nLife, BULLETTYPE type);



#endif
#ifndef _E_BULLET_H_
#define _E_BULLET_H_
#include "main.h"

//�}�N����`
#define E_BULLET_SIZE_WIDTH (40.0f)  //�e�̉��̃T�C�Y
#define E_BULLET_SIZE_HEIGHT (30.0f)//�e�̏c�̃T�C�Y

//�e�̎��
typedef enum
{
	ENEMYBULLETTYPE_STRAIGHT = 0,  //�^����������
	ENEMYBULLETTYPE_ROCKON,  //�_���Č���
	ENEMYBULLETTYPE_HOMING,  //�z�[�~���O�e
	ENEMYBULLETTYPE_BOSS,
	ENEMYBULLETTYPE_MAX,
}ENEMYBULLETTYPE;

//�v���g�^�C�v�錾
void InitEnemyBullet(void);
void UninitEnemyBullet(void);
void UpdateEnemyBullet(void);
void DrawEnemyBullet(void);
void SetEnemyBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move, int nLife, ENEMYBULLETTYPE type,int sType);

#endif
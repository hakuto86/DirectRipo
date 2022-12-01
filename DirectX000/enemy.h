#ifndef _ENEMY_H_
#define _ENEMY_H_
#include "main.h"
#include <stdio.h>
#include <stdlib.h>

//�}�N����`
#define MAX_ENEMY (128)  //�G�̍ő吔

//�G�̏��
typedef enum
{
	ENEMYSTATE_NORMAL = 0,
	ENEMYSTATE_DAMAGE,
	ENEMYSTATE_MAX
}ENEMYSTATE;

//�G�\���̂̒�`
typedef struct
{
	D3DXVECTOR3 pos;    //�ʒu
	D3DXVECTOR3 move;   //�ړ���
	int nType;          //���
	int nScore;         //�X�R�A
	int nSpeed;         //���x
	int m_pattern;      //�ړ��p�^�[��
	int nLife;      	//�̗�
	int nDamage;      	//�e�̃_���[�W
	int B_Type;         //�e�̎��
	int B_pattern;      //�e�̃p�^�[��
	int A_Time;         //�o������
	bool nStop;          //��~����
	bool Reversal;       //�߂鎞��
	int nCntStop;       //�J�E���g
	ENEMYSTATE state;   //���
	int nCounterState;  //��ԊǗ��J�E���^�[
	bool bUse;          //�g�p���Ă��邩�ǂ���
}Enemy;

//�}�N����`
#define ENEMY_SIZE_WIDTH (60.0f)  //�G�̉��̃T�C�Y
#define ENEMY_SIZE_HEIGHT (60.0f)//�G�̏c�̃T�C�Y

//�v���g�^�C�v�錾
void InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void LoadEnemy(void);
void DrawEnemy(void);
void SetEnemy(D3DXVECTOR3 pos, int nCntEnemy);
Enemy *GetEnemy(void);
void HitEnemyPlayer(Enemy *pEnemy);
void HitEnemy(int nCntEnemy, int nDamage);
int GetNumEnemy(void);

#endif
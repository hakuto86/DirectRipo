#ifndef _SCORE_H_
#define _SCORE_H_
#include "main.h"

//�}�N����`
#define SCORE_SIZE_WIDTH (30.0f)
#define SCORE_SIZE_HEIGHT (80.0f)
#define NUM_PLACE  (8)  //�X�R�A�̌���
#define NUM_RANKING  (5)  //�X�R�A�̕ۑ���

//�v���g�^�C�v�錾
void InitScore(void);
void UninitScore(void);
void UpdateScore(void);
void DrawScore(void);
void SetScore(void);
void AddScore(int nValue);
int *GetScore(void);

#endif
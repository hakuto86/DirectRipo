#ifndef _RESULTSCORE_H_
#define _RESULTSCORE_H_
#include "main.h"

//�}�N����`
#define RESULTSCORE_SIZE_WIDTH (30.0f)
#define RESULTSCORE_SIZE_HEIGHT (80.0f)

//�v���g�^�C�v�錾
void InitResultScore(void);
void UninitResultScore(void);
void UpdateResultScore(void);
void DrawResultScore(void);
void SetResultScore(int nScore);
//void AddResultScore(int nValue);
void RankingResultScore(void);

#endif
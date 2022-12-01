#include "main.h"
#include "score.h"
#include "enemy.h"
#include "resultscore.h"

//�O���[�o���ϐ�
LPDIRECT3DTEXTURE9 g_pTextureResultScore = NULL;  //�e�N�X�`���̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffResultScore = NULL;  //���_�o�b�t�@�ւ̃|�C���^
D3DXVECTOR3 g_posResultScore;   //�X�R�A�̈ʒu
int g_nResultScore;  //�X�R�A�̒l
int g_aRankingScore[NUM_RANKING] = { 100,200,300,400,500 };  //�����L���O

//==================================================================
//�X�R�A�̏������̏���
//==================================================================
void InitResultScore(void)
{
	LPDIRECT3DDEVICE9 pDevice; //�f�o�C�X�|�C���^

	pDevice = GetDevice();	   //�f�o�C�X�̎擾

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\number000.png", &g_pTextureResultScore);

	g_posResultScore = D3DXVECTOR3(1100, 50, 0.0f);  //�ʒu������������
	g_nResultScore = 0;  //�l������������

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * NUM_PLACE * NUM_RANKING,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffResultScore,
		NULL);
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffResultScore->Lock(0, 0, (void**)&pVtx, 0);

	int nCntPlace;
	int nCntRnking;

	for (nCntRnking = 0; nCntRnking < NUM_RANKING; nCntRnking++)
	{
		for (nCntPlace = 0; nCntPlace < NUM_PLACE; nCntPlace++, pVtx += 4)
		{
			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(400.0f, 520.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(500.0f, 520.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(400.0f, 540.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(500.0f, 540.0f, 0.0f);

			//rhw�̐ݒ�
			pVtx[0].rhw = 1.0f;
			pVtx[1].rhw = 1.0f;
			pVtx[2].rhw = 1.0f;
			pVtx[3].rhw = 1.0f;

			//���_�J���[�̐ݒ�
			pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

			//�e�N�X�`�����W�̐ݒ�
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);
		}

	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffResultScore->Unlock();
}

//==================================================================
//�X�R�A�̏I������
//==================================================================
void UninitResultScore(void)
{
	int nCntRnking;

	//�e�N�X�`���̔j��
	for (nCntRnking = 0; nCntRnking < NUM_RANKING; nCntRnking++)
	{
		if (g_pTextureResultScore != NULL)
		{
			g_pTextureResultScore->Release();
			g_pTextureResultScore = NULL;
		}
	}
	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffResultScore != NULL)
	{
		g_pVtxBuffResultScore->Release();
		g_pVtxBuffResultScore = NULL;
	}
}

//==================================================================
//�X�R�A�̍X�V����
//==================================================================
void UpdateResultScore(void)
{
	int nCntPlace;
	int nCntRnking;

	int aTexU[NUM_PLACE];

	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffResultScore->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntRnking = 0; nCntRnking < NUM_RANKING; nCntRnking++)
	{
		aTexU[0] = g_aRankingScore[nCntRnking] % 100000000 / 10000000;
		aTexU[1] = g_aRankingScore[nCntRnking] % 10000000 / 1000000;
		aTexU[2] = g_aRankingScore[nCntRnking] % 1000000 / 100000;
		aTexU[3] = g_aRankingScore[nCntRnking] % 100000 / 10000;
		aTexU[4] = g_aRankingScore[nCntRnking] % 10000 / 1000;
		aTexU[5] = g_aRankingScore[nCntRnking] % 1000 / 100;
		aTexU[6] = g_aRankingScore[nCntRnking] % 100 / 10;
		aTexU[7] = g_aRankingScore[nCntRnking] % 10 / 1;

		for (nCntPlace = 0; nCntPlace < NUM_PLACE; nCntPlace++, pVtx += 4)
		{
			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(450.0f + (nCntPlace * 40), 420.0f + (nCntRnking * 50), 0.0f);
			pVtx[1].pos = D3DXVECTOR3(495.0f + (nCntPlace * 40), 420.0f + (nCntRnking * 50), 0.0f);
			pVtx[2].pos = D3DXVECTOR3(450.0f + (nCntPlace * 40), 470.0f + (nCntRnking * 50), 0.0f);
			pVtx[3].pos = D3DXVECTOR3(495.0f + (nCntPlace * 40), 470.0f + (nCntRnking * 50), 0.0f);

			//�e�N�X�`�����W�̐ݒ�
			pVtx[0].tex = D3DXVECTOR2(aTexU[nCntPlace] * 0.1f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(aTexU[nCntPlace] * 0.1f + 0.1f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(aTexU[nCntPlace] * 0.1f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(aTexU[nCntPlace] * 0.1f + 0.1f, 1.0f);
		}
	}
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffResultScore->Unlock();
}

//==================================================================
//�X�R�A�̕`�揈��
//==================================================================
void DrawResultScore(void)
{
	LPDIRECT3DDEVICE9 pDevice; //�f�o�C�X�|�C���^
	int nCntPlace;
	int nCntRnking;
	int nPoligon = 0;

	pDevice = GetDevice();	    //�f�o�C�X�̎擾

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffResultScore, 0, sizeof(VERTEX_2D));

	//���_�̃t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntRnking = 0; nCntRnking < NUM_RANKING; nCntRnking++)
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureResultScore);

		for (nCntPlace = 0; nCntPlace < NUM_PLACE; nCntPlace++, nPoligon++)
		{
			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nPoligon * 4, 2);
		}
	}
}

//==================================================================
//�X�R�A�̐ݒ菈��
//==================================================================
void SetResultScore(int nScore)
{
	g_nResultScore = nScore;
}

////==================================================================
////�X�R�A�̉��Z����
////==================================================================
//void AddResultScore(int nValue)
//{
//	int aTexU[NUM_PLACE];
//	int nCntPlace;;
//	VERTEX_2D * pVtx;
//
//	g_nResultScore += nValue;
//
//	aTexU[0] = g_nResultScore % 100000000 / 10000000;
//	aTexU[1] = g_nResultScore % 10000000 / 1000000;
//	aTexU[2] = g_nResultScore % 1000000 / 100000;
//	aTexU[3] = g_nResultScore % 100000 / 10000;
//	aTexU[4] = g_nResultScore % 10000 / 1000;
//	aTexU[5] = g_nResultScore % 1000 / 100;
//	aTexU[6] = g_nResultScore % 100 / 10;
//	aTexU[7] = g_nResultScore % 10 / 1;
//
//	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
//	g_pVtxBuffResultScore->Lock(0, 0, (void**)&pVtx, 0);
//
//	for (nCntPlace = 0; nCntPlace < NUM_PLACE; nCntPlace++, pVtx += 4)
//	{
//		//�e�N�X�`�����W�̐ݒ�
//		pVtx[0].tex = D3DXVECTOR2(aTexU[nCntPlace] * 0.1f, 0.0f);
//		pVtx[1].tex = D3DXVECTOR2(aTexU[nCntPlace] * 0.1f + 0.1f, 0.0f);
//		pVtx[2].tex = D3DXVECTOR2(aTexU[nCntPlace] * 0.1f, 1.0f);
//		pVtx[3].tex = D3DXVECTOR2(aTexU[nCntPlace] * 0.1f + 0.1f, 1.0f);
//	}
//	//���_�o�b�t�@���A�����b�N����
//	g_pVtxBuffResultScore->Unlock();
//}

//==================================================================
//�����L���O�̏���
//==================================================================
void RankingResultScore(void)
{
	int nCount;
	int nCount2;
	int nTemp;

	//�~���̏���
	for (nCount2 = 0; nCount2 < NUM_RANKING - 1; nCount2++)
	{
		for (nCount = (nCount2 + 1); nCount < NUM_RANKING; nCount++)
		{
			if (g_aRankingScore[nCount2] < g_aRankingScore[nCount])
			{
				nTemp = g_aRankingScore[nCount2];                 //���f�[�^���i�[
				g_aRankingScore[nCount2] = g_aRankingScore[nCount];//���f�[�^�ɎQ�Ƃ��Ă���f�[�^����
				g_aRankingScore[nCount] = nTemp;                  //�i�[�����f�[�^���Q�ƃf�[�^�ɑ��
			}
		}
	}

	if (g_nResultScore >= g_aRankingScore[NUM_RANKING - 1])
	{
		g_aRankingScore[NUM_RANKING - 1] = g_nResultScore;
	}

	//�~���̏���
	for (nCount2 = 0; nCount2 < NUM_RANKING - 1; nCount2++)
	{
		for (nCount = (nCount2 + 1); nCount < NUM_RANKING; nCount++)
		{
			if (g_aRankingScore[nCount2] < g_aRankingScore[nCount])
			{
				nTemp = g_aRankingScore[nCount2];                 //���f�[�^���i�[
				g_aRankingScore[nCount2] = g_aRankingScore[nCount];//���f�[�^�ɎQ�Ƃ��Ă���f�[�^����
				g_aRankingScore[nCount] = nTemp;                  //�i�[�����f�[�^���Q�ƃf�[�^�ɑ��
			}
		}
	}
}
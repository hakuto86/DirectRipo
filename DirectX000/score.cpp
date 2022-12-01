#include "main.h"
#include "score.h"
#include "enemy.h"

//�O���[�o���ϐ�
LPDIRECT3DTEXTURE9 g_pTextureScore = NULL;  //�e�N�X�`���̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffScore = NULL;  //���_�o�b�t�@�ւ̃|�C���^
D3DXVECTOR3 g_posScore;   //�X�R�A�̈ʒu
int g_nScore;  //�X�R�A�̒l

//==================================================================
//�X�R�A�̏������̏���
//==================================================================
void InitScore(void)
{
	LPDIRECT3DDEVICE9 pDevice; //�f�o�C�X�|�C���^

    pDevice = GetDevice();	   //�f�o�C�X�̎擾
	
	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\number000.png", &g_pTextureScore);

	g_posScore = D3DXVECTOR3(1100, 50, 0.0f);  //�ʒu������������
	g_nScore = 0;  //�l������������

    //���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * NUM_PLACE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffScore,
		NULL);
	VERTEX_2D * pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);

	int nCntScore;

	for (nCntScore = 0; nCntScore < NUM_PLACE; nCntScore++, pVtx += 4)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(0.0f, 20.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(100.0f, 20.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, 40.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(100.0f, 40.0f, 0.0f);

		//rhw�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//���_�J���[�̐ݒ�
		pVtx[0].col = D3DCOLOR_RGBA(255, 200, 100, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 200, 100, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 200, 100, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 200, 100, 255);

		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffScore->Unlock();
}

//==================================================================
//�X�R�A�̏I������
//==================================================================
void UninitScore(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureScore != NULL)
	{
		g_pTextureScore->Release();
		g_pTextureScore = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffScore != NULL)
	{
		g_pVtxBuffScore->Release();
		g_pVtxBuffScore = NULL;
	}
}

//==================================================================
//�X�R�A�̍X�V����
//==================================================================
void UpdateScore(void)
{
	int nCntScore;

	VERTEX_2D * pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntScore = 0; nCntScore < NUM_PLACE; nCntScore++, pVtx += 4)
	{
		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(0.0f + (nCntScore * 40), 20.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(45.0f + (nCntScore * 40), 20.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f + (nCntScore * 40), 70.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(45.0f + (nCntScore * 40), 70.0f, 0.0f);
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffScore->Unlock();
}

//==================================================================
//�X�R�A�̕`�揈��
//==================================================================
void DrawScore(void)
{
	LPDIRECT3DDEVICE9 pDevice; //�f�o�C�X�|�C���^
	int nCntScore;

	pDevice = GetDevice();	    //�f�o�C�X�̎擾

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffScore, 0, sizeof(VERTEX_2D));

	//���_�̃t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureScore);

	for (nCntScore = 0; nCntScore < NUM_PLACE; nCntScore++)
	{
		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntScore * 4, 2);
	}
}

//==================================================================
//�X�R�A�̐ݒ菈��
//==================================================================
void SetScore(void)
{
	int aTexU[NUM_PLACE];
	int nCntScore;
	VERTEX_2D * pVtx;

	aTexU[0] = g_nScore % 100000000 / 10000000;
	aTexU[1] = g_nScore % 10000000 / 1000000;
	aTexU[2] = g_nScore % 1000000 / 100000;
	aTexU[3] = g_nScore % 100000 / 10000;
	aTexU[4] = g_nScore % 10000 / 1000;
	aTexU[5] = g_nScore % 1000 / 100;
	aTexU[6] = g_nScore % 100 / 10;
	aTexU[7] = g_nScore % 10 / 1;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntScore = 0; nCntScore < NUM_PLACE; nCntScore++, pVtx += 4)
	{
		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(aTexU[nCntScore] * 0.1f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(aTexU[nCntScore] * 0.1f + 0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(aTexU[nCntScore] * 0.1f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(aTexU[nCntScore] * 0.1f + 0.1f, 1.0f);
	}
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffScore->Unlock();
}

//==================================================================
//�X�R�A�̉��Z����
//==================================================================
void AddScore(int nValue)
{
	int aTexU[NUM_PLACE];
	int nCntScore;

	VERTEX_2D * pVtx;

	g_nScore += nValue;

	aTexU[0] = g_nScore % 100000000 / 10000000;
	aTexU[1] = g_nScore % 10000000 / 1000000;
	aTexU[2] = g_nScore % 1000000 / 100000;
	aTexU[3] = g_nScore % 100000 / 10000;
	aTexU[4] = g_nScore % 10000 / 1000;
	aTexU[5] = g_nScore % 1000 / 100;
	aTexU[6] = g_nScore % 100 / 10;
	aTexU[7] = g_nScore % 10 / 1;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntScore = 0; nCntScore < NUM_PLACE; nCntScore++, pVtx += 4)
	{
		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(aTexU[nCntScore] * 0.1f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(aTexU[nCntScore] * 0.1f + 0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(aTexU[nCntScore] * 0.1f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(aTexU[nCntScore] * 0.1f + 0.1f, 1.0f);
	}
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffScore->Unlock();
}

//==================================================================
//�X�R�A�̎擾
//==================================================================
int *GetScore(void)
{
	return &g_nScore;
}
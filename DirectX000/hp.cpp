#include "main.h"
#include "hp.h"
#include "enemy.h"
#include "player.h"

//�O���[�o���ϐ�
LPDIRECT3DTEXTURE9 g_pTextureHp = NULL;  //�e�N�X�`���̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffHp = NULL;  //���_�o�b�t�@�ւ̃|�C���^
D3DXVECTOR3 g_posHp;   //�X�R�A�̈ʒu
int g_nHp;  //�X�R�A�̒l

//==================================================================
//�X�R�A�̏������̏���
//==================================================================
void InitHp(void)
{
	LPDIRECT3DDEVICE9 pDevice; //�f�o�C�X�|�C���^

	pDevice = GetDevice();	   //�f�o�C�X�̎擾

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\number000.png", &g_pTextureHp);

	g_posHp = D3DXVECTOR3(1100, 50, 0.0f);  //�ʒu������������
	g_nHp = 0;  //�l������������

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * NUM_HP,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffHp,
		NULL);
	VERTEX_2D * pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffHp->Lock(0, 0, (void**)&pVtx, 0);

	int nCntScore;

	for (nCntScore = 0; nCntScore < NUM_HP; nCntScore++, pVtx += 4)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(150.0f, 20.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(200.0f, 20.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(150.0f, 40.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(200.0f, 40.0f, 0.0f);

		//rhw�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//���_�J���[�̐ݒ�
		pVtx[0].col = D3DCOLOR_RGBA(255, 100, 100, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 100, 100, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 100, 100, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 100, 100, 255);

		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffHp->Unlock();
}

//==================================================================
//�X�R�A�̏I������
//==================================================================
void UninitHp(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureHp != NULL)
	{
		g_pTextureHp->Release();
		g_pTextureHp = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffHp != NULL)
	{
		g_pVtxBuffHp->Release();
		g_pVtxBuffHp = NULL;
	}
}

//==================================================================
//�X�R�A�̍X�V����
//==================================================================
void UpdateHp(void)
{
	int nCntHp;

	VERTEX_2D * pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffHp->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntHp = 0; nCntHp < NUM_HP; nCntHp++, pVtx += 4)
	{
		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(400.0f + (nCntHp * 40), 20.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(445.0f + (nCntHp * 40), 20.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(400.0f + (nCntHp * 40), 70.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(445.0f + (nCntHp * 40), 70.0f, 0.0f);
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffHp->Unlock();
}

//==================================================================
//�X�R�A�̕`�揈��
//==================================================================
void DrawHp(void)
{
	LPDIRECT3DDEVICE9 pDevice; //�f�o�C�X�|�C���^
	int nCntHp;

	pDevice = GetDevice();	    //�f�o�C�X�̎擾

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffHp, 0, sizeof(VERTEX_2D));

	//���_�̃t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureHp);

	for (nCntHp = 0; nCntHp < NUM_HP; nCntHp++)
	{
		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntHp * 4, 2);
	}
}

//==================================================================
//�X�R�A�̐ݒ菈��
//==================================================================
void SetHp(void)
{
	int aTexU[NUM_HP];
	int nCntHp;
	VERTEX_2D * pVtx;

	aTexU[0] = g_nHp % 100 / 10;
	aTexU[1] = g_nHp % 10 / 1;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffHp->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntHp = 0; nCntHp < NUM_HP; nCntHp++, pVtx += 4)
	{
		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(aTexU[nCntHp] * 0.1f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(aTexU[nCntHp] * 0.1f + 0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(aTexU[nCntHp] * 0.1f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(aTexU[nCntHp] * 0.1f + 0.1f, 1.0f);
	}
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffHp->Unlock();
}

//==================================================================
//�X�R�A�̉��Z����
//==================================================================
void AddHp(int nValue)
{
	int aTexU[NUM_HP];
	int nCntHp;

	VERTEX_2D * pVtx;

	g_nHp += nValue;

	aTexU[0] = g_nHp % 100 / 10;
	aTexU[1] = g_nHp % 10 / 1;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffHp->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntHp = 0; nCntHp < NUM_HP; nCntHp++, pVtx += 4)
	{
		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(aTexU[nCntHp] * 0.1f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(aTexU[nCntHp] * 0.1f + 0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(aTexU[nCntHp] * 0.1f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(aTexU[nCntHp] * 0.1f + 0.1f, 1.0f);
	}
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffHp->Unlock();
}
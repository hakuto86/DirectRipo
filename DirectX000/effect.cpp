#include "main.h"
#include "Effect.h"
#include "Bullet.h"
#include "explosion.h"
#include "enemy.h"

//�}�N����`
#define MAX_Effect (4096)  //�G�t�F�N�g�̍ő吔

//�G�t�F�N�g�\���̂̒�`
typedef struct
{
	D3DXVECTOR3 pos;  //�ʒu
	D3DXCOLOR col;    //�ړ���
	float fRadius;    //���a(�傫��)
	int nLife;        //����(�\������)
	bool bUse;        //�g�p���Ă��邩�ǂ���
} Effect;

//�O���[�o���ϐ�
LPDIRECT3DTEXTURE9 g_pTextureEffect = NULL;  //�e�N�X�`���̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEffect = NULL;  //���_�o�b�t�@�ւ̃|�C���^
Effect g_aEffect[MAX_Effect];

//==================================================================
//�G�t�F�N�g�̏������̏���
//==================================================================
void InitEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice; //�f�o�C�X�|�C���^
	int nCntEffect;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\bubble.jpg", &g_pTextureEffect);

	//�e�̏��̏�����
	for (nCntEffect = 0; nCntEffect < MAX_Effect; nCntEffect++)
	{
		g_aEffect[nCntEffect].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEffect[nCntEffect].col = D3DXCOLOR(0.0f, 0.0f, 0.0f,0.0f);
		g_aEffect[nCntEffect].nLife = 100;
		g_aEffect[nCntEffect].bUse = false;  //�g�p���Ă��Ȃ���Ԃɂ���
	}

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * MAX_Effect,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffEffect,
		NULL);

	VERTEX_2D * pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntEffect = 0; nCntEffect < MAX_Effect; nCntEffect++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(-EFFECT_SIZE_WIDTH, -EFFECT_SIZE_HEIGHT, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(+EFFECT_SIZE_WIDTH, -EFFECT_SIZE_HEIGHT, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-EFFECT_SIZE_WIDTH, +EFFECT_SIZE_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(+EFFECT_SIZE_WIDTH, +EFFECT_SIZE_HEIGHT, 0.0f);

		//rhw�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;  //���_�f�[�^�̃|�C���^��4���i�߂�
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffEffect->Unlock();
}

//==================================================================
//�G�t�F�N�g�̏I������
//==================================================================
void UninitEffect(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureEffect != NULL)
	{
		g_pTextureEffect->Release();
		g_pTextureEffect = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffEffect != NULL)
	{
		g_pVtxBuffEffect->Release();
		g_pVtxBuffEffect = NULL;
	}
}

//==================================================================
//�G�t�F�N�g�̍X�V����
//==================================================================
void UpdateEffect(void)
{
	int nCntEffect;

	VERTEX_2D * pVtx;
	LPDIRECT3DDEVICE9 pDevice; //�f�o�C�X�|�C���^
		  
	pDevice = GetDevice(); //�f�o�C�X�̎擾

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntEffect = 0; nCntEffect < MAX_Effect; nCntEffect++, pVtx += 4)
	{
		if (g_aEffect[nCntEffect].bUse == true)
		{
			//���_�J���[�̐ݒ�
			pVtx[0].col = g_aEffect[nCntEffect].col;
			pVtx[1].col = g_aEffect[nCntEffect].col;
			pVtx[2].col = g_aEffect[nCntEffect].col;
			pVtx[3].col = g_aEffect[nCntEffect].col;

			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(-g_aEffect[nCntEffect].fRadius + g_aEffect[nCntEffect].pos.x, -g_aEffect[nCntEffect].fRadius + g_aEffect[nCntEffect].pos.y, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(+g_aEffect[nCntEffect].fRadius + g_aEffect[nCntEffect].pos.x, -g_aEffect[nCntEffect].fRadius + g_aEffect[nCntEffect].pos.y, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-g_aEffect[nCntEffect].fRadius + g_aEffect[nCntEffect].pos.x, +g_aEffect[nCntEffect].fRadius + g_aEffect[nCntEffect].pos.y, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(+g_aEffect[nCntEffect].fRadius + g_aEffect[nCntEffect].pos.x, +g_aEffect[nCntEffect].fRadius + g_aEffect[nCntEffect].pos.y, 0.0f);

			//��ʊO�ɏo��
			if (g_aEffect[nCntEffect].pos.x >= SCREEN_WIDTH || g_aEffect[nCntEffect].pos.y >= SCREEN_HEIGHT || g_aEffect[nCntEffect].pos.x <= 0 || g_aEffect[nCntEffect].pos.y <= 0)
			{
				g_aEffect[nCntEffect].bUse = false;  //�g�p���Ă��Ȃ���Ԃɂ���
			}

			g_aEffect[nCntEffect].nLife--;

			//�������s����
			if (g_aEffect[nCntEffect].nLife == 0)
			{
				g_aEffect[nCntEffect].bUse = false;  //�g�p���Ă��Ȃ���Ԃɂ���
			}
		}
	}
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffEffect->Unlock();
}

//==================================================================
//�G�t�F�N�g�̕`�揈��
//==================================================================
void DrawEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice; //�f�o�C�X�|�C���^
	int nCntEffect;

	pDevice = GetDevice();	    //�f�o�C�X�̎擾

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffEffect, 0, sizeof(VERTEX_2D));

	//���_�̃t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureEffect);

	//���u�����f�B���O�����Z�����ɐݒ�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	for (nCntEffect = 0; nCntEffect < MAX_Effect; nCntEffect++)
	{
		if (g_aEffect[nCntEffect].bUse == true)
		{
			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntEffect * 4, 2);
		}
	}
	//���u�����f�B���O�����Z�����ɐݒ�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//==================================================================
//�G�t�F�N�g�̐ݒ菈��
//==================================================================
void SetEffect(D3DXVECTOR3 pos, D3DXCOLOR col, float fRadius, int nLife)
{
	int nCntEffect;
	VERTEX_2D * pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntEffect = 0; nCntEffect < MAX_Effect; nCntEffect++, pVtx+=4)
	{
		if (g_aEffect[nCntEffect].bUse == false)
		{//�e���g�p����Ă��Ȃ�

			g_aEffect[nCntEffect].pos = pos;

			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(-fRadius + g_aEffect[nCntEffect].pos.x, -fRadius + g_aEffect[nCntEffect].pos.y, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(+fRadius + g_aEffect[nCntEffect].pos.x, -fRadius + g_aEffect[nCntEffect].pos.y, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-fRadius + g_aEffect[nCntEffect].pos.x, +fRadius + g_aEffect[nCntEffect].pos.y, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(+fRadius + g_aEffect[nCntEffect].pos.x, +fRadius + g_aEffect[nCntEffect].pos.y, 0.0f);
			
			g_aEffect[nCntEffect].col = col;

			//���_�J���[�̐ݒ�
			pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

			g_aEffect[nCntEffect].fRadius = fRadius;
			g_aEffect[nCntEffect].nLife = nLife;
			g_aEffect[nCntEffect].bUse = true;

			break;
		}
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffEffect->Unlock();
}
#include "main.h"
#include "explosion.h"

//�}�N����`
#define MAX_EXPLOSION (128)  //�����̍ő吔

//�����\���̂̒�`
typedef struct
{
	D3DXVECTOR3 pos;  //�ʒu
	D3DXCOLOR col;    //�F
	int nCounterAnim; //�A�j���[�V�����J�E���^�[
	int nPatternAnim; //�A�j���[�V�����p�^�[��No.
	bool bUse;        //�g�p���Ă��邩�ǂ���
}Bullet;

//�O���[�o���ϐ�
LPDIRECT3DTEXTURE9 g_pTextureExplosion = NULL;  //�e�N�X�`���̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffExplosion = NULL;  //���_�o�b�t�@�ւ̃|�C���^
Bullet g_aExplosion[MAX_EXPLOSION];

//==================================================================
//�����̏������̏���
//==================================================================
void InitExplosion(void)
{
	LPDIRECT3DDEVICE9 pDevice; //�f�o�C�X�|�C���^
	int nCntExplosion;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\explosion000.png", &g_pTextureExplosion);

	//�����̏��̏�����
	for (nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		g_aExplosion[nCntExplosion].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aExplosion[nCntExplosion].col = D3DXCOLOR(0.0f, 0.0f, 0.0f ,0.0f);
		g_aExplosion[nCntExplosion].bUse = false;  //�g�p���Ă��Ȃ���Ԃɂ���
	}

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * MAX_EXPLOSION,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffExplosion,
		NULL);
	VERTEX_2D * pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(-70.0f, -100.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(+70.0f, -100.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-70.0f, +100.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(+70.0f, +100.0f, 0.0f);

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
		pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);

		pVtx += 4;  //���_�f�[�^�̃|�C���^��4���i�߂�
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffExplosion->Unlock();
}

//==================================================================
//�����̏I������
//==================================================================
void UninitExplosion(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureExplosion != NULL)
	{
		g_pTextureExplosion->Release();
		g_pTextureExplosion = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffExplosion != NULL)
	{
		g_pVtxBuffExplosion->Release();
		g_pVtxBuffExplosion = NULL;
	}
}

//==================================================================
//�����̍X�V����
//==================================================================
void UpdateExplosion(void)
{
	int nCntExplosion;

	VERTEX_2D * pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++, pVtx += 4)
	{
		if (g_aExplosion[nCntExplosion].bUse == true)
		{
			g_aExplosion[nCntExplosion].nCounterAnim++; //�J�E���^�[�����Z

			if ((g_aExplosion[nCntExplosion].nCounterAnim % 5) == 0)
			{//��莞�Ԍo��
			 //�p�^�[��No.���X�V����
				g_aExplosion[nCntExplosion].nPatternAnim++;

				pVtx[0].tex = D3DXVECTOR2(((float)(g_aExplosion[nCntExplosion].nPatternAnim % PATTERN_EXPLOSION_WIDTH)) / PATTERN_EXPLOSION_WIDTH,
					(float)(g_aExplosion[nCntExplosion].nPatternAnim / PATTERN_EXPLOSION_WIDTH) / PATTERN_EXPLOSION_HEIGHT);
				pVtx[1].tex = D3DXVECTOR2(((float)(g_aExplosion[nCntExplosion].nPatternAnim % PATTERN_EXPLOSION_WIDTH) + 1) / PATTERN_EXPLOSION_WIDTH,
					(float)(g_aExplosion[nCntExplosion].nPatternAnim / PATTERN_EXPLOSION_WIDTH) / PATTERN_EXPLOSION_HEIGHT);
				pVtx[2].tex = D3DXVECTOR2((float)(g_aExplosion[nCntExplosion].nPatternAnim % PATTERN_EXPLOSION_WIDTH) / PATTERN_EXPLOSION_WIDTH,
					((float)(g_aExplosion[nCntExplosion].nPatternAnim / PATTERN_EXPLOSION_WIDTH) + 1) / PATTERN_EXPLOSION_HEIGHT);
				pVtx[3].tex = D3DXVECTOR2(((float)(g_aExplosion[nCntExplosion].nPatternAnim % PATTERN_EXPLOSION_WIDTH) + 1) / PATTERN_EXPLOSION_WIDTH,
					((float)(g_aExplosion[nCntExplosion].nPatternAnim / PATTERN_EXPLOSION_WIDTH) + 1) / PATTERN_EXPLOSION_HEIGHT);

					if (g_aExplosion[nCntExplosion].nPatternAnim == 8)
					{
   						g_aExplosion[nCntExplosion].bUse = false;  //�g�p���Ă��Ȃ���Ԃɂ���
					}

					//���_���W�̍X�V
					pVtx[0].pos = D3DXVECTOR3(-70.0f + g_aExplosion[nCntExplosion].pos.x, -100.0f + g_aExplosion[nCntExplosion].pos.y, 0.0f);
					pVtx[1].pos = D3DXVECTOR3(+70.0f + g_aExplosion[nCntExplosion].pos.x, -100.0f + g_aExplosion[nCntExplosion].pos.y, 0.0f);
					pVtx[2].pos = D3DXVECTOR3(-70.0f + g_aExplosion[nCntExplosion].pos.x, +100.0f + g_aExplosion[nCntExplosion].pos.y, 0.0f);
					pVtx[3].pos = D3DXVECTOR3(+70.0f + g_aExplosion[nCntExplosion].pos.x, +100.0f + g_aExplosion[nCntExplosion].pos.y, 0.0f);
			}
		}
	}
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffExplosion->Unlock();
}

//==================================================================
//�����̕`�揈��
//==================================================================
void DrawExplosion(void)
{
	LPDIRECT3DDEVICE9 pDevice; //�f�o�C�X�|�C���^
	int nCntBullet;

	pDevice = GetDevice();	    //�f�o�C�X�̎擾

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffExplosion, 0, sizeof(VERTEX_2D));

	//���_�̃t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureExplosion);

	for (nCntBullet = 0; nCntBullet < MAX_EXPLOSION; nCntBullet++)
	{
		if (g_aExplosion[nCntBullet].bUse == true)
		{
			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntBullet * 4, 2);
		}
	}
}

//==================================================================
//�����̐ݒ菈��
//==================================================================
void SetExplosion(D3DXVECTOR3 pos, D3DXCOLOR col)
{
	int nCntExplosion;

	//���_�o�b�t�@�̐���
	VERTEX_2D * pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++, pVtx += 4)
	{
		if (g_aExplosion[nCntExplosion].bUse == false)
		{//�e���g�p����Ă��Ȃ�

			g_aExplosion[nCntExplosion].pos = pos;

			g_aExplosion[nCntExplosion].nCounterAnim = 0;  //�A�j���[�V�����J�E���^�[�̏�����
			g_aExplosion[nCntExplosion].nPatternAnim = 0;  //�A�j���[�V�����p�^�[��No.�̏�����

			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(-70.0f + g_aExplosion[nCntExplosion].pos.x, -100.0f + g_aExplosion[nCntExplosion].pos.y, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(+70.0f + g_aExplosion[nCntExplosion].pos.x, -100.0f + g_aExplosion[nCntExplosion].pos.y, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-70.0f + g_aExplosion[nCntExplosion].pos.x, +100.0f + g_aExplosion[nCntExplosion].pos.y, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(+70.0f + g_aExplosion[nCntExplosion].pos.x, +100.0f + g_aExplosion[nCntExplosion].pos.y, 0.0f);

			//�����̐F�̐ݒ�
			pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

			g_aExplosion[nCntExplosion].bUse = true;  //�g�p���Ă����Ԃɂ���

			break;
		}
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffExplosion->Unlock();
}
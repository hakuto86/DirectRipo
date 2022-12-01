#include "main.h"
#include "bg.h"

//�O���[�o���ϐ�
LPDIRECT3DTEXTURE9 g_pTextureBG[NUM_BG] = {};  //�e�N�X�`���̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBG = NULL;  //���_�o�b�t�@�ւ̃|�C���^
D3DXVECTOR3 g_posBg;  //�ʒu
float g_aTexV[NUM_BG];
//==================================================================
//�w�i�̏������̏���
//==================================================================
void InitBg(void)
{
	int nCntBG;

	LPDIRECT3DDEVICE9 pDevice; //�f�o�C�X�|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\umi.jpeg", &g_pTextureBG[0]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\umi.jpeg", &g_pTextureBG[1]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\umi.jpeg", &g_pTextureBG[2]);

	//�e�N�X�`�����W�̊J�n�ʒu(V�l)�̏�����
	for (nCntBG = 0; nCntBG < NUM_BG; nCntBG++)
	{
		g_aTexV[nCntBG] = 0.0f;
	}

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * NUM_BG,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBG,
		NULL);
	VERTEX_2D * pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffBG->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBG = 0; nCntBG < NUM_BG; nCntBG++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

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
		pVtx[0].tex = D3DXVECTOR2(0.0f, g_aTexV[nCntBG]);
		pVtx[1].tex = D3DXVECTOR2(1.0f, g_aTexV[nCntBG]);
		pVtx[2].tex = D3DXVECTOR2(0.0f, g_aTexV[nCntBG] + 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, g_aTexV[nCntBG] + 1.0f);

		pVtx += 4;   //���_�f�[�^�̃|�C���^��4���i�߂�
	}
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffBG->Unlock();
}

//==================================================================
//�w�i�̏I������
//==================================================================
void UninitBg(void)
{
	//�e�N�X�`���̔j��
	for (int nCnt = 0; nCnt < 3; nCnt++)
	{
		if (g_pTextureBG[nCnt] != NULL)
		{
			g_pTextureBG[nCnt]->Release();
			g_pTextureBG[nCnt] = NULL;
		}
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffBG != NULL)
	{
		g_pVtxBuffBG->Release();
		g_pVtxBuffBG = NULL;
	}
}

//==================================================================
//�w�i�̍X�V����
//==================================================================
void UpdateBg(void)
{
	int nCntBG;

	VERTEX_2D * pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffBG->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBG = 0; nCntBG < NUM_BG; nCntBG++)
	{
		//�e�N�X�`�����W�̊J�n�ʒu(V�l)�̍X�V
		g_aTexV[0] -= 0.005f*(nCntBG + 1);

		//�e�N�X�`�����W�̍X�V
		pVtx[0].tex = D3DXVECTOR2(0.0f, g_aTexV[nCntBG]);
		pVtx[1].tex = D3DXVECTOR2(1.0f, g_aTexV[nCntBG]);
		pVtx[2].tex = D3DXVECTOR2(0.0f, g_aTexV[nCntBG] + 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, g_aTexV[nCntBG] + 1.0f);

		pVtx += 4;   //���_�f�[�^�̃|�C���^��4���i�߂�

	}
		//���_�o�b�t�@���A�����b�N����
	    g_pVtxBuffBG->Unlock();
}

//==================================================================
//�w�i�̕`�揈��
//==================================================================
void DrawBg(void)
{
	int nCntBG;

	LPDIRECT3DDEVICE9 pDevice; //�f�o�C�X�|�C���^

	pDevice = GetDevice();	    //�f�o�C�X�̎擾
	
	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffBG, 0, sizeof(VERTEX_2D));

	//���_�̃t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	for (nCntBG = 0; nCntBG < NUM_BG; nCntBG++)
	{
		pDevice->SetTexture(0, g_pTextureBG[nCntBG]);
		
		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntBG * 4, 3);
	}
}
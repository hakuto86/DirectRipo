#include "main.h"
#include "hp_mark.h"

//�O���[�o���ϐ�
LPDIRECT3DTEXTURE9 g_pTextureHpMark = NULL;  //�e�N�X�`���̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffHpMark = NULL;  //���_�o�b�t�@�ւ̃|�C���^
D3DXVECTOR3 g_posHpMark;   //�X�R�A�̈ʒu

//==================================================================
//�X�R�A�̏������̏���
//==================================================================
void InitHpMark(void)
{
	LPDIRECT3DDEVICE9 pDevice; //�f�o�C�X�|�C���^

	pDevice = GetDevice();	   //�f�o�C�X�̎擾

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\Life.png", &g_pTextureHpMark);

	g_posHpMark = D3DXVECTOR3(1100, 50, 0.0f);  //�ʒu������������

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffHpMark,
		NULL);
	VERTEX_2D * pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffHpMark->Lock(0, 0, (void**)&pVtx, 0);

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
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffHpMark->Unlock();
}

//==================================================================
//�X�R�A�̏I������
//==================================================================
void UninitHpMark(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureHpMark != NULL)
	{
		g_pTextureHpMark->Release();
		g_pTextureHpMark = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffHpMark != NULL)
	{
		g_pVtxBuffHpMark->Release();
		g_pVtxBuffHpMark = NULL;
	}
}

//==================================================================
//�X�R�A�̍X�V����
//==================================================================
void UpdateHpMark(void)
{
	VERTEX_2D * pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffHpMark->Lock(0, 0, (void**)&pVtx, 0);

		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(350.0f , 20.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(390.0f , 20.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(350.0f , 70.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(390.0f , 70.0f, 0.0f);

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffHpMark->Unlock();
}

//==================================================================
//�X�R�A�̕`�揈��
//==================================================================
void DrawHpMark(void)
{
	LPDIRECT3DDEVICE9 pDevice; //�f�o�C�X�|�C���^

	pDevice = GetDevice();	    //�f�o�C�X�̎擾

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffHpMark, 0, sizeof(VERTEX_2D));

	//���_�̃t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureHpMark);

	//�|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}
#include  "input.h"

//�}�N����`
#define NUM_KEY_MAX (256) //�L�[�̍ő吔

//�O���[�o���ϐ�
LPDIRECTINPUT8 g_pInput = NULL;  //DirectInput�I�u�W�F�N�g�ւ̃|�C���^
LPDIRECTINPUTDEVICE8 g_pDev_Keyboard = NULL;  //���̓f�o�C�X(�L�[�{�[�h)�ւ̃|�C���^
BYTE g_aKeyState[NUM_KEY_MAX];  //�L�[�{�[�h�̃v���X���
BYTE g_aKeyStateTrigger[NUM_KEY_MAX];  //�L�[�{�[�h�̃g���K�[���

//==================================================================
//�L�[�{�[�h�̏���������
//==================================================================
HRESULT InitKeyboard(HINSTANCE hInstance, HWND hWnd)
{
	//DirectInput�I�u�W�F�N�g�̐���
	if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&g_pInput, NULL)))
	{
		return E_FAIL;
	}

	//���̓f�o�C�X(�L�[�{�[�h)�̐���
	if (FAILED(g_pInput->CreateDevice(GUID_SysKeyboard, &g_pDev_Keyboard, NULL)))
	{
		return E_FAIL;
	}

	//�f�[�^�t�H�[�}�b�g��ݒ�
	if (FAILED(g_pDev_Keyboard->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}

	//�������[�h��ݒ�
	if (FAILED(g_pDev_Keyboard -> SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	//�L�[�{�[�h�ւ̃A�N�Z�X�����l��
	g_pDev_Keyboard->Acquire();
	return S_OK;
}

//==================================================================
//�L�[�{�[�h�̏I������
//==================================================================
void UninitKeyboard(void)
{
	//���̓f�o�C�X(�L�[�{�[�h)�̔j��
	if (g_pDev_Keyboard != NULL)
	{
		g_pDev_Keyboard->Unacquire();  //�L�[�{�[�h�ւ̃A�N�Z�X����j��
		g_pDev_Keyboard->Release();
		g_pDev_Keyboard = NULL;
	}

	//DirectInput�I�u�W�F�N�g�̔j��
	if (g_pInput != NULL)
	{
		g_pInput->Release();
		g_pInput = NULL;
	}
}

//==================================================================
//�L�[�{�[�h�̍X�V����
//==================================================================
void UpdateKeyboard(void)
{
	BYTE aKeyState[NUM_KEY_MAX];  //�L�[�{�[�h�̓��͏��
	int nCntKey;
	
	//���̓f�o�C�X����f�[�^���擾
	if (SUCCEEDED(g_pDev_Keyboard->GetDeviceState(sizeof(aKeyState), &aKeyState[0])))
	{
		for (nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
		{
			g_aKeyStateTrigger[nCntKey] = (g_aKeyState[nCntKey] ^ aKeyState[nCntKey]) & aKeyState[nCntKey];//�L�[�{�[�h�̃g���K�[����ۑ�
			g_aKeyState[nCntKey] = aKeyState[nCntKey];  //�L�[�{�[�h�̃v���X����ۑ�
		}
	}
	else
	{
		g_pDev_Keyboard->Acquire();  //�L�[�{�[�h�ւ̃A�N�Z�X�����l��
	}
}

//==================================================================
//�L�[�{�[�h�v���X�̏����擾
//==================================================================
bool GetKeyboardPress(int nKey)
{
	return(g_aKeyState[nKey] & 0x80) ? true : false;
}

//==================================================================
//�L�[�{�[�h�g���K�[�̏����擾
//==================================================================
bool GetKeyboardTrigger(int nKey)
{
	return(g_aKeyStateTrigger[nKey] & 0x80) ? true : false;
}
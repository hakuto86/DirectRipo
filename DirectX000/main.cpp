#include "main.h"
#include "input.h"
#include "title.h"
#include "game.h"
#include "result.h"
#include "clear.h"
#include "fade.h"
#include "sound.h"

//�}�N����`
#define CLASS_NAME "windowsClass"        //�E�C���h�E�N���X�̖��O
#define WINDOW_NAME "Fish Hunting"       //�E�C���h�E�̖��O
#define SCREEN_WIDTH (1280)              //�E�C���h�E�̕�
#define SCREEN_HEIGHT (720)              //�E�C���h�E�̍���

//�v���g�^�C�v�錾
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM IParam);
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
void Uninit(void);
void Update(void);
void Draw(void);
void DrawFPS(void);

//�O���[�o���ϐ��錾
LPDIRECT3D9 g_pD3D = NULL;              //Direct3D�I�u�W�F�N�g�ւ̃|�C���^
LPDIRECT3DDEVICE9  g_pD3DDevice = NULL; //Direct3D�f�o�C�X�ւ̃|�C���^
LPD3DXFONT g_pFont = NULL;  //�t�H���g�ւ̃|�C���^
int g_nCountFPS = 0;  //FPS�J�E���^
bool g_bDispDebug = false;  //�f�o�b�O�\����ON/OFF
MODE g_mode = MODE_TITLE;  //���݂̃��[�h
bool g_bPause = false;  //�|�[�Y��Ԃ�ON/OFF

//=======================================================================
//���C���֐�
//=======================================================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hInstancePrev, LPSTR lpCmdLine, int nCmdShow)
{
	DWORD dwCurrentTime;   //���ݎ���
	DWORD dwExecLastTime;  //���ݎ���
	DWORD dwFrameCount;    //�t���[���J�E���g
	DWORD dwFPSLastTime;           //�Ō��FPS���v����������

	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),            //WNDCLASSEX�̃������T�C�Y
		CS_CLASSDC,                    //�E�C���h�E�̃X�^�C��
		WindowProc,                    //�E�C���h�E�̃v���V�[�W��
		0,                             //0�ɂ���(�ʏ�͎g�p���Ȃ�)
		0,                             //0�ɂ���(�ʏ�͎g�p���Ȃ�)
		hInstance,                     //�C���X�^���X�n���h��
		LoadIcon(NULL,IDI_APPLICATION),//�^�X�N�o�[�̃A�C�R��
		LoadCursor(NULL,IDC_ARROW),    //�}�E�X�J�[�\��
		(HBRUSH)(COLOR_WINDOW + 1),    //�N���C�A���g�̈�̔w�i�F
		NULL,                          //���j���[�o�[
		CLASS_NAME,                    //�E�C���h�E�N���X�̖��O
		LoadIcon(NULL,IDI_APPLICATION) //�t�@�C���̃A�C�R��
	};

	HWND hWnd;
	MSG msg;

	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };   //��ʃT�C�Y�̍\����

	//�E�C���h�E�N���X�̓o�^
	RegisterClassEx(&wcex);

	//�N���C�A���g�̈���w��̃T�C�Y�ɒ���
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	//�E�C���h�E�𐶐�
	hWnd = CreateWindowEx(
		0,
		CLASS_NAME,
		WINDOW_NAME,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		SCREEN_WIDTH,
		SCREEN_HEIGHT,
		NULL,
		NULL,
		hInstance,
		NULL);

	//����������
	if (FAILED(Init(hInstance, hWnd, (TRUE))))
	{//���������������s�����ꍇ
		return-1;
	}
	//�E�C���h�E�̕\��
	ShowWindow(hWnd, nCmdShow);

	UpdateWindow(hWnd);

	//����\��ݒ�
	timeBeginPeriod(1);
	dwCurrentTime = 0;  //����������
	dwExecLastTime = timeGetTime();  //���ݎ������擾
	dwFrameCount = 0;
	dwFPSLastTime = timeGetTime();

	//���b�Z�[�W���[�v
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)
		{//Windows�̏���
			if (msg.message == WM_QUIT)
			{//WM_QUIT���b�Z�[�W���󂯎�����烁�b�Z�[�W���[�v�𔲂���
				break;
			}
			else
			{
				//���b�Z�[�W�̐ݒ�
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			//DirectX�̏���
			if ((dwCurrentTime - dwFPSLastTime) >= 500)
			{//0.5�b�o��
				//FPS���v��
				g_nCountFPS = (dwFrameCount * 1000) / (dwCurrentTime - dwFPSLastTime);
				dwFPSLastTime = dwCurrentTime;  //FPS�𑪒肵��������ۑ�
				dwFrameCount = 0;  //�t���[���J�E���g���N���A
			}

			dwCurrentTime = timeGetTime();  //���ݎ������擾

			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
			{//60����1�b�o��
				dwExecLastTime = dwCurrentTime;  //�����J�n�̎�����ۑ�
				dwFrameCount++;  //�t���[���J�E���g�����Z

				//�X�V����				 
				Update();

				//�`�揈��
				Draw();
			}
		}
	}

	//�I������
	Uninit();

	//�E�C���h�E�N���X�̓o�^������
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	return(int)msg.wParam;
}

//�E�C���h�E�v���V�[�W��
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM IParam)
{
	int nID;
	
	switch (uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:
			nID = MessageBox(hWnd, "�I�����܂����H", "�I�����b�Z�[�W", MB_YESNO);
			if (nID == IDYES)
			{
				//�E�C���h�E��j������
				DestroyWindow(hWnd);
			}
			break;

		case WM_CLOSE:
			nID = MessageBox(hWnd, "�I�����܂����H", "�I�����b�Z�[�W", MB_YESNO);
			if (nID == IDYES)
			{
				//�E�C���h�E��j������
				DestroyWindow(hWnd);
			}
			else
			{
				return 0;  //0��Ԃ��Ȃ��ƏI�����Ă��܂�
			}
			break;
		}
	}
	return DefWindowProc(hWnd, uMsg, wParam, IParam);
}

//==================================================================
//����������
//==================================================================
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	D3DDISPLAYMODE d3ddm;        //�f�B�X�v���C���[�h
	D3DPRESENT_PARAMETERS d3dpp; //�v���[���e�[�V�����p�����[�^

	//Direct3D�I�u�W�F�N�g�̐���
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (g_pD3D == NULL)
	{
		return E_FAIL;
	}
	//���݂̃f�B�X�v���C���[�h���擾
	if (FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT,&d3ddm)))
	{
		return E_FAIL;
	}
	//�f�o�C�X�̃v���[���e�[�V�����p�����[�^�̐ݒ�
	ZeroMemory(&d3dpp, sizeof(d3dpp));                         //�p�����[�^�̃[���N���A
	d3dpp.BackBufferWidth = SCREEN_WIDTH;                      //��ʃT�C�Y(��)
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;                    //��ʃT�C�Y(����)
	d3dpp.BackBufferFormat = d3ddm.Format;                     //�o�b�N�o�b�t�@�̌`��
	d3dpp.BackBufferCount = 1;                                 //�o�b�N�o�b�t�@�̐�
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;				   //�_�u���o�b�t�@�̐؂�ւ�(�f���M���ɓ���)
	d3dpp.EnableAutoDepthStencil = TRUE;					   //�f�v�X�o�b�t�@�ƃX�e���V���o�b�t�@���쐬
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;				   //�f�v�X�o�b�t�@�Ƃ���16bit���g��
	d3dpp.Windowed = bWindow;								   //�E�C���h�E���[�h
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;//���t���b�V�����[�g
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;  //�C���^�[�o��

	//Direct3D�f�o�C�X�̐���
	if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &g_pD3DDevice)))
	{
		if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &g_pD3DDevice)))
		{
			if (FAILED(g_pD3D->CreateDevice(D3DDEVTYPE_REF, D3DDEVTYPE_HAL, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &g_pD3DDevice)))
			{
				return E_FAIL;
			}
		}
	}

	//�����_�[�X�e�[�g�̐ݒ�
	g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//�T���v���[�X�e�[�g�̐ݒ�
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);

	//�e�N�X�`���X�e�[�W�X�e�[�g�̐ݒ�
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	//�f�o�b�N�\���p�t�H���g�̐���
	D3DXCreateFont(g_pD3DDevice,18, 0, 0, 0, FALSE, SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Terminal", &g_pFont);

	//�L�[�{�[�h�̏���������
	if (FAILED(InitKeyboard(hInstance, hWnd)))
	{
		return E_FAIL;
	}

	InitSound(hWnd);
	PlaySound(SOUND_LABEL_BGM000);

	//�t�F�[�h�̐ݒ�
	InitFade(g_mode);

	return S_OK;
}

//==================================================================
//�I������
//==================================================================
void Uninit(void)
{
	//�^�C�g����ʂ̏I������
	UninitTitle();

	//�Q�[����ʂ̏I������
	UninitGame();

	//���U���g��ʂ̏I������
	UninitResult();

	//���U���g��ʂ̏I������
	UninitClear();

	//�t�F�[�h�̏I������
	UninitFade();

	//����\��߂�
	timeEndPeriod(1);

	//�L�[�{�[�h�̏I������
	UninitKeyboard();

	//Direct3D�f�o�C�X�̔j��
	if (g_pD3DDevice != NULL)
	{
		g_pD3DDevice->Release();
		g_pD3DDevice = NULL;
	}

	//Direct3D�I�u�W�F�N�g�̔j��
	if (g_pD3D != NULL)
	{
		g_pD3D->Release();
		g_pD3D = NULL;
	}

	//�f�o�b�N�\���p�t�H���g�̔j��
	if (g_pFont != NULL)
	{
		g_pFont->Release();
		g_pFont = NULL;
	}

	StopSound();
	UninitSound();
}

//==================================================================
//�X�V����
//==================================================================
void Update(void)
{
	UpdateKeyboard();

	if (GetKeyboardTrigger(DIK_P) == true)
	{//P�L�[�������ꂽ
		g_bPause ^= 1;
	}
	if (g_bPause == false)
	{
		switch (g_mode)
		{
		case MODE_TITLE:   //�^�C�g�����
			UpdateTitle();
			break;

		case MODE_GAME:    //�Q�[�����
			UpdateGame();
			break;

		case MODE_RESULT:  //���U���g���
			UpdateResult();
			break;

		case MODE_CLEAR:  //���U���g���
			UpdateClear();
			break;
		}
		//�t�F�[�h�̍X�V����
		UpdateFade();
	}
}

//==================================================================
//�`�揈��
//==================================================================
void Draw(void)
{
	//��ʃN���A(�o�b�N�o�b�t�@&Z�o�b�t�@�̃N���A)
	g_pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	//�`��J�n
	if (SUCCEEDED(g_pD3DDevice->BeginScene()))
	{//�`��J�n�����������ꍇ
		//FPS�̕\��
		DrawFPS();
		
		switch (g_mode)
		{
		case MODE_TITLE:   //�^�C�g�����
			DrawTitle();
			break;

		case MODE_GAME:    //�Q�[�����
			DrawGame();
			break;

		case MODE_RESULT:  //���U���g���
			DrawResult();
			break;

		case MODE_CLEAR:  //���U���g���
			DrawClear();
			break;
		}

		//�t�F�[�h�̕`�揈��
		DrawFade();

#ifdef _DEBUG
		//FPS�̕\��
		DrawFPS();
#endif
		//�`��I��
		g_pD3DDevice->EndScene();
	}

	//�o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�̓���ւ�
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//==================================================================
//FPS�`�揈��
//==================================================================
void DrawFPS(void)
{
	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	char aStr[256];

	//������ɑ��
	wsprintf(&aStr[0], "FPS:%d\n", g_nCountFPS);

	//�e�L�X�g�̕`��
	g_pFont->DrawTextA(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));

}
//==================================================================
//���[�h�̐ݒ�
//==================================================================
void SetMode(MODE mode)
{
	//���݂̉��(���[�h)�̏I������
	switch (g_mode)
	{
	case MODE_TITLE:   //�^�C�g�����
		UninitTitle();
		break;

	case MODE_GAME:    //�Q�[�����
		UninitGame();
		break;

	case MODE_RESULT:  //���U���g���
		UninitResult();
		break;

	case MODE_CLEAR:  //���U���g���
		UninitClear();
		break;
	}

	//�V�������(���[�h)�̏���������
	switch (mode)
	{
	case MODE_TITLE:   //�^�C�g�����
		InitTitle();
		break;

	case MODE_GAME:    //�Q�[�����
		InitGame();
		break;

	case MODE_RESULT:  //���U���g���
		InitResult();
		break;

	case MODE_CLEAR:  //���U���g���
		InitClear();
		break;
	}
	g_mode = mode;  //���݂̉��(���[�h)��؂�ւ���
}

//==================================================================
//���[�h�̎擾
//==================================================================
MODE GetMode(void)
{
	return g_mode;
}

//==================================================================
//�f�o�C�X�̎擾
//==================================================================
LPDIRECT3DDEVICE9 GetDevice(void)
{
	return g_pD3DDevice;
}
// Hanafuda.cpp : �A�v���P�[�V�����̃G���g�� �|�C���g���`���܂��B
//

#include "stdafx.h"
#include "Hanafuda.h"
#include <mmsystem.h>

#pragma comment(lib,"winmm.lib")

#define MAX_LOADSTRING 100

#define MUTEX_NAME TEXT("Hanafuda Mutex")

// �O���[�o���ϐ�:
HINSTANCE hInst;								// ���݂̃C���^�[�t�F�C�X
TCHAR szTitle[MAX_LOADSTRING];					// �^�C�g�� �o�[�̃e�L�X�g
TCHAR szWindowClass[MAX_LOADSTRING];			// ���C�� �E�B���h�E �N���X��

// ���̃R�[�h ���W���[���Ɋ܂܂��֐��̐錾��]�����܂�:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);


int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{

	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: �����ɃR�[�h��}�����Ă��������B
	HANDLE hMutex;

	//���d�N���h�~
	hMutex = CreateMutex(NULL,TRUE,MUTEX_NAME);
	if(GetLastError() == ERROR_ALREADY_EXISTS){
		ReleaseMutex(hMutex);
		CloseHandle(hMutex);
		return FALSE;
	}

	//GDI+
	GdiplusStartupInput gdiSI;
    ULONG_PTR gdiToken;

	MSG msg;
	HACCEL hAccelTable;

	GdiplusStartup(&gdiToken, &gdiSI, NULL);

	// �O���[�o������������������Ă��܂��B
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_HANAFUDA, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// �A�v���P�[�V�����̏����������s���܂�:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_HANAFUDA));

	// ���C�� ���b�Z�[�W ���[�v:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}


	//GDI+�I��
	GdiplusShutdown(gdiToken);

	//Mutex�̉��
	ReleaseMutex(hMutex);
	CloseHandle(hMutex);

	return (int) msg.wParam;
}



//
//  �֐�: MyRegisterClass()
//
//  �ړI: �E�B���h�E �N���X��o�^���܂��B
//
//  �R�����g:
//
//    ���̊֐�����юg�����́A'RegisterClassEx' �֐����ǉ����ꂽ
//    Windows 95 ���O�� Win32 �V�X�e���ƌ݊�������ꍇ�ɂ̂ݕK�v�ł��B
//    �A�v���P�[�V�������A�֘A�t����ꂽ
//    �������`���̏������A�C�R�����擾�ł���悤�ɂ���ɂ́A
//    ���̊֐����Ăяo���Ă��������B
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_HANAFUDA));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	//wcex.hbrBackground	= (HBRUSH)CreateSolidBrush(RGB(0, 0xff, 0xff)); 
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_HANAFUDA);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   �֐�: InitInstance(HINSTANCE, int)
//
//   �ړI: �C���X�^���X �n���h����ۑ����āA���C�� �E�B���h�E���쐬���܂��B
//
//   �R�����g:
//
//        ���̊֐��ŁA�O���[�o���ϐ��ŃC���X�^���X �n���h����ۑ����A
//        ���C�� �v���O���� �E�B���h�E���쐬����ѕ\�����܂��B
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	Mediator* md = Mediator::GetInstance();
	HWND hWnd;

	hInst = hInstance; // �O���[�o���ϐ��ɃC���X�^���X�������i�[���܂��B

   int width = md->WND_WIDTH
//  + GetSystemMetrics(SM_CXFIXEDFRAME) // �^�C�g���o�[���Œ�T�C�Y�E�B���h�E�̃t���[���̕�
//  + GetSystemMetrics(SM_CXFRAME)//�E�B���h�E�̘g�̕�
  + GetSystemMetrics(SM_CXEDGE)        // ���̉��̕�(����)
  + GetSystemMetrics(SM_CXBORDER);      // ���̉��̕�(�񗧑�)
   int height = md->WND_HEIGHT
//  + GetSystemMetrics(SM_CYFIXEDFRAME)  // �^�C�g���o�[���Œ�T�C�Y�E�B���h�E�̃t���[���̍���
//  + GetSystemMetrics(SM_CYFRAME)//�E�B���h�E�̘g�̍���
  + GetSystemMetrics(SM_CYEDGE)        // �c�̉��̍���(����)
  + GetSystemMetrics(SM_CYBORDER)      // �c�̉��̍���(�񗧑�)
  + GetSystemMetrics(SM_CYCAPTION)	// �^�C�g���o�[�̍���
  + GetSystemMetrics(SM_CYMENU);   //���j���[�o�[�̍���
   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME ^ WS_MAXIMIZEBOX,
	   CW_USEDEFAULT, 0, width, height, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  �֐�: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  �ړI:  ���C�� �E�B���h�E�̃��b�Z�[�W���������܂��B
//
//  WM_COMMAND	- �A�v���P�[�V���� ���j���[�̏���
//  WM_PAINT	- ���C�� �E�B���h�E�̕`��
//  WM_DESTROY	- ���~���b�Z�[�W��\�����Ė߂�
//
//

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	Mediator* md = Mediator::GetInstance();
	int wmId, wmEvent;
	static BOOL bHook = FALSE;
/*	PAINTSTRUCT ps;
	HDC hdc;*/

	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// �I�����ꂽ���j���[�̉��:
		switch (wmId)
		{
		case IDM_NEWGAME://�V�����Q�[��
			md->Reset();
			break;
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_CREATE:
		//������
		md->Init(hWnd);
		break;
	case WM_MOUSEMOVE:
		md->MouseMove(LOWORD(lParam),HIWORD(lParam));
		break;
	case WM_LBUTTONUP:
		md->MouseClick(LOWORD(lParam),HIWORD(lParam));
		break;
	case WM_PAINT:
//		hdc = BeginPaint(hWnd, &ps);
		// TODO: �`��R�[�h�������ɒǉ����Ă�������...
		md->Paint();
//		EndPaint(hWnd, &ps);
		break;
	case WM_SOUND://���ʉ�
		md->Sound(wParam);
		break;
	case WM_SETCURSOR://�J�[�\���ύX
		if(md->GetROID()) SetCursor(LoadCursor(NULL,IDC_HAND));
		else SetCursor(LoadCursor(NULL,IDC_ARROW));
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// �o�[�W�������{�b�N�X�̃��b�Z�[�W �n���h���ł��B
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

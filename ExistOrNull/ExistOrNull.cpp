// ExistOrNull.cpp : ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
#include "ExistOrNull.h"
#include <ctime>
#include <cstdlib>
#include "Stage.h"
#include "User.h"
#include "TILE.h"

#define MAX_LOADSTRING 100






/* ���� ���� ���� ������Ϸ� �̵�
// ���� ����:
*/
HINSTANCE hInst;                                // ���� �ν��Ͻ��Դϴ�.
WCHAR szTitle[MAX_LOADSTRING];                  // ���� ǥ���� �ؽ�Ʈ�Դϴ�.
WCHAR szWindowClass[MAX_LOADSTRING];            // �⺻ â Ŭ���� �̸��Դϴ�.

// �� �ڵ� ��⿡ ��� �ִ� �Լ��� ������ �����Դϴ�.
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: ���⿡ �ڵ带 �Է��մϴ�.

    // ���� ���ڿ��� �ʱ�ȭ�մϴ�.
	
	//����
	srand(time(NULL));

    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_EXISTORNULL, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);


    // ���� ���α׷� �ʱ�ȭ�� �����մϴ�.
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_EXISTORNULL));
    MSG msg;

    // �⺻ �޽��� �����Դϴ�.
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEX wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_EXISTORNULL));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCEW(IDC_EXISTORNULL);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // �ν��Ͻ� �ڵ��� ���� ������ �����մϴ�.

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW, 
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static Stage *mainStage = new Stage(hWnd, hInst);
	//static 
	//static HBITMAP tileResource[3][3][3];
	
	static RECT set, endSet;

    switch (message)
    {
	case WM_CREATE: {

		RECT tmpRect;
		SetRect(&tmpRect, 0, 0, USER_GUI_WIDTH*2 + (TILE_SIZE * TILE_WIDTH_AMOUNT), TILE_SIZE*TILE_HEIGHT_AMOUNT + SYSTEM_GUI_HEIGHT);

		AdjustWindowRect(&tmpRect, WS_OVERLAPPEDWINDOW, TRUE); // ���� �ֱ����ؼ� �޴��κ� TRUE �� ����
		int rectWidth = tmpRect.right - tmpRect.left + 1;
		int rectHeight = tmpRect.bottom - tmpRect.top + 1;
		SetWindowPos(hWnd, NULL, 0, 0, rectWidth, rectHeight , SWP_NOMOVE | SWP_NOZORDER);
		

		// ���� �������� Ÿ�� �����κ�
		RECT tileSizeRect;
		for (int i = 0; i < TILE_WIDTH_AMOUNT; i++) {
			for (int j = 0; j < TILE_HEIGHT_AMOUNT; j++) {

				tileSizeRect.left = USER_GUI_WIDTH + i*TILE_SIZE;
				tileSizeRect.right = USER_GUI_WIDTH + (i + 1)*TILE_SIZE;

				tileSizeRect.top = j*TILE_SIZE;
				tileSizeRect.bottom = (j + 1)*TILE_SIZE;

				//�̹� setTileRect���� ����������.
				mainStage->setTileRect(i, j, tileSizeRect);
			}
		}

		RECT forGUIRect;
		// �������� �Ѱ� RECT ����
		forGUIRect = { USER_GUI_WIDTH,0,USER_GUI_WIDTH + TILE_SIZE * TILE_WIDTH_AMOUNT, TILE_SIZE * TILE_HEIGHT_AMOUNT };
		mainStage->setStageGUI(forGUIRect);
		// 0������ UI RECT
		forGUIRect = { 0,0, USER_GUI_WIDTH, TILE_SIZE*TILE_HEIGHT_AMOUNT };
		mainStage->setLeftGUI(forGUIRect);

		// 1������ UI RECT
		forGUIRect = { USER_GUI_WIDTH + TILE_SIZE * TILE_WIDTH_AMOUNT ,0, USER_GUI_WIDTH* 2 + TILE_SIZE * TILE_WIDTH_AMOUNT, TILE_SIZE*TILE_HEIGHT_AMOUNT };
		mainStage->setRightGUI(forGUIRect);

		// �ý��� UI RECT ����
		forGUIRect = { USER_GUI_WIDTH, TILE_SIZE * TILE_HEIGHT_AMOUNT, USER_GUI_WIDTH + TILE_SIZE * TILE_WIDTH_AMOUNT, TILE_SIZE * TILE_HEIGHT_AMOUNT + SYSTEM_GUI_HEIGHT};
		mainStage->setSystemGUI(forGUIRect);

		forGUIRect = { forGUIRect.left, forGUIRect.bottom - 20, forGUIRect.right, forGUIRect.bottom };
		mainStage->setProgressGUI(forGUIRect);

		// ���̵� UI RECT ����
		forGUIRect = { 0, TILE_SIZE*TILE_HEIGHT_AMOUNT, USER_GUI_WIDTH, TILE_SIZE*TILE_HEIGHT_AMOUNT + SYSTEM_GUI_HEIGHT};
		mainStage->setGuideGUI(forGUIRect);

		// ��ư ���ҽ� ���ݱ����� PtInRect�� ����
		forGUIRect = { USER_GUI_WIDTH + TILE_SIZE*TILE_WIDTH_AMOUNT, TILE_SIZE*TILE_HEIGHT_AMOUNT, USER_GUI_WIDTH * 2 + TILE_SIZE*TILE_WIDTH_AMOUNT, TILE_SIZE*TILE_HEIGHT_AMOUNT + SYSTEM_GUI_HEIGHT };
		mainStage->setSetRect(forGUIRect);
		

		/* ��/ �� �����մϴ�.
		// �չ�ư
		set = { USER_GUI_WIDTH + TILE_SIZE*TILE_WIDTH_AMOUNT, TILE_SIZE*TILE_HEIGHT_AMOUNT, (USER_GUI_WIDTH / 2) * 3 + TILE_SIZE*TILE_WIDTH_AMOUNT, TILE_SIZE*TILE_HEIGHT_AMOUNT + SYSTEM_GUI_HEIGHT };
		// ���ư
		endSet = { (USER_GUI_WIDTH / 2) * 3 + TILE_SIZE*TILE_WIDTH_AMOUNT, TILE_SIZE*TILE_HEIGHT_AMOUNT, USER_GUI_WIDTH *2 + TILE_SIZE*TILE_WIDTH_AMOUNT, TILE_SIZE*TILE_HEIGHT_AMOUNT + SYSTEM_GUI_HEIGHT };
		*/
		break;
	}

    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // �޴� ������ ���� �м��մϴ�.
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
	case WM_KEYDOWN: {
		mainStage->keyEvent(wParam);
	}
	case WM_LBUTTONDOWN: {
		mainStage->mouseEvent(lParam);
	}
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: ���⿡ hdc�� ����ϴ� �׸��� �ڵ带 �߰��մϴ�.

			mainStage->DrawStage(hdc);
			mainStage->DrawGUI(hdc);

            EndPaint(hWnd, &ps);
        }
        break;
	case WM_TIMER: {
		switch (wParam) {
		case 0: {
			// 0�� Ÿ�̸Ӵ� ���� ���������� ���� await����.
			KillTimer(hWnd, 0);
			
			mainStage->setIsStart(1);
			
			InvalidateRect(hWnd, nullptr, true);
			break;
		}
		case 1: {
			// 1�� Ÿ�̸Ӵ� system�޽��� ��Ʈ��
			KillTimer(hWnd, 1);
			mainStage->resetSystemMsg();
			break;
		}
		case 2: {
			// 2�� Ÿ�̸Ӵ� ���� �ð�����
			mainStage->decreaseRemainTime(500);
			if (mainStage->getRemainTime() <= 0) {
				mainStage->resetRemainTime();
				mainStage->increaseUserTurn();
			}
			//InvalidateRect(hWnd, &mainStage->getProgressGUI(), TRUE);

			break;
		}
		}
		// ���⼭ break�Ⱦ��� �ٷ� WM_DESTROY����.. �����ض�� ����
		break;
	}
    case WM_DESTROY:
        PostQuitMessage(0);




        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// ���� ��ȭ ������ �޽��� ó�����Դϴ�.
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

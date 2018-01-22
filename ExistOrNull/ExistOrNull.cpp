// ExistOrNull.cpp : 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "ExistOrNull.h"
#include <ctime>
#include <cstdlib>
#include "Stage.h"
#include "User.h"
#include "TILE.h"

#define MAX_LOADSTRING 100






/* 전역 변수 선언 헤더파일로 이동
// 전역 변수:
*/
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

// 이 코드 모듈에 들어 있는 함수의 정방향 선언입니다.
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

    // TODO: 여기에 코드를 입력합니다.

    // 전역 문자열을 초기화합니다.
	
	//랜덤
	srand(time(NULL));

    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_EXISTORNULL, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);


    // 응용 프로그램 초기화를 수행합니다.
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_EXISTORNULL));
    MSG msg;

    // 기본 메시지 루프입니다.
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
   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

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

		AdjustWindowRect(&tmpRect, WS_OVERLAPPEDWINDOW, TRUE); // 설명 넣기위해서 메뉴부분 TRUE 로 만듬
		int rectWidth = tmpRect.right - tmpRect.left + 1;
		int rectHeight = tmpRect.bottom - tmpRect.top + 1;
		SetWindowPos(hWnd, NULL, 0, 0, rectWidth, rectHeight , SWP_NOMOVE | SWP_NOZORDER);
		

		// 메인 스테이지 타일 설정부분
		RECT tileSizeRect;
		for (int i = 0; i < TILE_WIDTH_AMOUNT; i++) {
			for (int j = 0; j < TILE_HEIGHT_AMOUNT; j++) {

				tileSizeRect.left = USER_GUI_WIDTH + i*TILE_SIZE;
				tileSizeRect.right = USER_GUI_WIDTH + (i + 1)*TILE_SIZE;

				tileSizeRect.top = j*TILE_SIZE;
				tileSizeRect.bottom = (j + 1)*TILE_SIZE;

				//이미 setTileRect에서 뒤집혀있음.
				mainStage->setTileRect(i, j, tileSizeRect);
			}
		}

		RECT forGUIRect;
		// 스테이지 총괄 RECT 설정
		forGUIRect = { USER_GUI_WIDTH,0,USER_GUI_WIDTH + TILE_SIZE * TILE_WIDTH_AMOUNT, TILE_SIZE * TILE_HEIGHT_AMOUNT };
		mainStage->setStageGUI(forGUIRect);
		// 0번유저 UI RECT
		forGUIRect = { 0,0, USER_GUI_WIDTH, TILE_SIZE*TILE_HEIGHT_AMOUNT };
		mainStage->setLeftGUI(forGUIRect);

		// 1번유저 UI RECT
		forGUIRect = { USER_GUI_WIDTH + TILE_SIZE * TILE_WIDTH_AMOUNT ,0, USER_GUI_WIDTH* 2 + TILE_SIZE * TILE_WIDTH_AMOUNT, TILE_SIZE*TILE_HEIGHT_AMOUNT };
		mainStage->setRightGUI(forGUIRect);

		// 시스템 UI RECT 설정
		forGUIRect = { USER_GUI_WIDTH, TILE_SIZE * TILE_HEIGHT_AMOUNT, USER_GUI_WIDTH + TILE_SIZE * TILE_WIDTH_AMOUNT, TILE_SIZE * TILE_HEIGHT_AMOUNT + SYSTEM_GUI_HEIGHT};
		mainStage->setSystemGUI(forGUIRect);

		forGUIRect = { forGUIRect.left, forGUIRect.bottom - 20, forGUIRect.right, forGUIRect.bottom };
		mainStage->setProgressGUI(forGUIRect);

		// 가이드 UI RECT 설정
		forGUIRect = { 0, TILE_SIZE*TILE_HEIGHT_AMOUNT, USER_GUI_WIDTH, TILE_SIZE*TILE_HEIGHT_AMOUNT + SYSTEM_GUI_HEIGHT};
		mainStage->setGuideGUI(forGUIRect);

		// 버튼 리소스 사용금금지라 PtInRect로 구현
		forGUIRect = { USER_GUI_WIDTH + TILE_SIZE*TILE_WIDTH_AMOUNT, TILE_SIZE*TILE_HEIGHT_AMOUNT, USER_GUI_WIDTH * 2 + TILE_SIZE*TILE_WIDTH_AMOUNT, TILE_SIZE*TILE_HEIGHT_AMOUNT + SYSTEM_GUI_HEIGHT };
		mainStage->setSetRect(forGUIRect);
		

		/* 결/ 합 통합합니다.
		// 합버튼
		set = { USER_GUI_WIDTH + TILE_SIZE*TILE_WIDTH_AMOUNT, TILE_SIZE*TILE_HEIGHT_AMOUNT, (USER_GUI_WIDTH / 2) * 3 + TILE_SIZE*TILE_WIDTH_AMOUNT, TILE_SIZE*TILE_HEIGHT_AMOUNT + SYSTEM_GUI_HEIGHT };
		// 결버튼
		endSet = { (USER_GUI_WIDTH / 2) * 3 + TILE_SIZE*TILE_WIDTH_AMOUNT, TILE_SIZE*TILE_HEIGHT_AMOUNT, USER_GUI_WIDTH *2 + TILE_SIZE*TILE_WIDTH_AMOUNT, TILE_SIZE*TILE_HEIGHT_AMOUNT + SYSTEM_GUI_HEIGHT };
		*/
		break;
	}

    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 메뉴 선택을 구문 분석합니다.
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
            // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다.

			mainStage->DrawStage(hdc);
			mainStage->DrawGUI(hdc);

            EndPaint(hWnd, &ps);
        }
        break;
	case WM_TIMER: {
		switch (wParam) {
		case 0: {
			// 0번 타이머는 다음 스테이지를 위한 await상태.
			KillTimer(hWnd, 0);
			
			mainStage->setIsStart(1);
			
			InvalidateRect(hWnd, nullptr, true);
			break;
		}
		case 1: {
			// 1번 타이머는 system메시지 컨트롤
			KillTimer(hWnd, 1);
			mainStage->resetSystemMsg();
			break;
		}
		case 2: {
			// 2번 타이머는 유저 시간제한
			mainStage->decreaseRemainTime(500);
			if (mainStage->getRemainTime() <= 0) {
				mainStage->resetRemainTime();
				mainStage->increaseUserTurn();
			}
			//InvalidateRect(hWnd, &mainStage->getProgressGUI(), TRUE);

			break;
		}
		}
		// 여기서 break안쓰면 바로 WM_DESTROY간다.. 조심해라ㅓ 제발
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

// 정보 대화 상자의 메시지 처리기입니다.
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

#pragma once

#include "stdafx.h"
//#include "ExistOrNull.h"
#include "User.h"
#include "TILE.h"
#include "Answer.h"

// for GUI
#define TILE_SIZE 150

#define SCORE_BOARD_HEIGHT 75
#define USER_GUI_WIDTH 200
#define SYSTEM_GUI_HEIGHT 100

#define MAX_ANSWERED_LIST 30
#define ANSWER_CHAIN 3

#define DEFAULT_REMAIN_TIME 5000

class Stage {
private:

	int currentStage;

	TILE currentStageTile[TILE_WIDTH_AMOUNT][TILE_HEIGHT_AMOUNT]; // 변함
	int currentStageTileIntList[9];									// 변함
	HBITMAP tileResource[3][3][3];						// 안변함
	RECT tileRect[TILE_WIDTH_AMOUNT][TILE_HEIGHT_AMOUNT];	// 안변함
	HBITMAP tileResourceBack;	// 안변함

	int isStart;
	HWND hWnd;
	HINSTANCE hInst;

	int checkArray[3];

	Answer answeredList[MAX_ANSWERED_LIST];

	RECT leftGUI, rightGUI;
	RECT stageGUI;    // 3x3 의 타일을 가리키는 영역
	
	RECT systemGUI;   // 하단 영역 
	RECT progressGUI;
	TCHAR systemMsg[128];

	RECT guideGUI;
	TCHAR *guideMsg[64];

	RECT setRect;
	TCHAR *setUnsetMsg[20];

	int selectList[3];
	int selectTileList[3]; // tile x,y pos from (3x3 to 1D)
	int selectCount;
	
	int answerCombineCount; // 컴퓨터가 계산으로 찾아낸 정답량
	int answeredCount;		// 유저가 답한 정답량

	int remainTime;
	int missCount;

	// 9C3
	//int correctArray[84]; // deprecated.
	//int answerArray[84];  // deprecated.



	int userTurn;

	User player[2]; // 조절가능하도록

public:
	Stage(HWND hwnd, HINSTANCE hInst);

	void setIsStart(int val);

	TILE getTile(int indexW, int indexH);
	int getTileColorBG(int indexW, int indexH);
	int getTileBaseShape(int indexW, int indexH);
	int getTileColorShape(int indexW, int indexH);
	bool getTileIsSelected(int indexW, int indexH);

	void increaseUserTurn();
	int getUserTurn();

	void setTileResource(int indexBG, int indexShape, int indexColor, HBITMAP bit);
	HBITMAP getTileResource(int indexBG, int indexShape, int indexColor);

	void resetStage(HDC);

	void setTileRect(int, int, RECT);
	RECT getTileRect(int indexW, int indexH);

	void setLeftGUI(RECT rt);
	void setRightGUI(RECT rt);
	void setStageGUI(RECT rt);
	void setSystemGUI(RECT rt);
	void setProgressGUI(RECT rt);
	RECT getSystemGUI();
	RECT getProgressGUI();
	void setGuideGUI(RECT rt);
	void setSetRect(RECT rt);

	//void DrawBitmap(HDC hdc, int x, int y, HBITMAP tmpBitmap, int arrayOption[]);
	void DrawBitmap(HDC hdc, int x, int y, int w, int h, int cx, int cy,  HBITMAP tmpBitmap, int option);
	void DrawStage(HDC hdc);
	void DrawStage(HDC hdc, RECT rect, int indexW, int indexH);
	void DrawGUI(HDC hdc);

	HBITMAP getMatchedTileToResource(int indexW, int indexH);
	void keyEvent(int wParam); 
	void mouseEvent(int lParam);
	void eventHandler1(int w, int h);
	void eventHandlerAfterSetUnSet();
	void eventHandlerOverTheMaxError();

	bool isValidCombination(int arr[], bool userQuery);
	void checkAllCombination(int arr[], int index, int n, int r, int target);
	void setUnset();
	void initAllTile(int turn);
	void allTileUnselect();

	void resetSystemMsg();
	void printText(HDC hdc, RECT &rect, TCHAR *str);

	void resetRemainTime();
	void decreaseRemainTime(int n);
	int getRemainTime();
};
#include "stdafx.h"
#include "ExistOrNull.h"
#include "Stage.h"

void Stage::setIsStart(int val) {
	this->isStart = val;
}
TILE Stage::getTile(int indexW, int indexH) {
	return currentStageTile[indexH][indexW];
}
int Stage::getTileColorBG(int indexW, int indexH) {
	return currentStageTile[indexH][indexW].getColorBG();

}
int Stage::getTileBaseShape(int indexW, int indexH) {
	return currentStageTile[indexH][indexW].getBaseShape();
}
int Stage::getTileColorShape(int indexW, int indexH) {
	return currentStageTile[indexH][indexW].getColorShape();

}
bool Stage::getTileIsSelected(int indexW, int indexH) {
	return currentStageTile[indexH][indexW].getIsSelected();
}
void Stage::setTileResource(int indexBG, int indexShape, int indexColor, HBITMAP bit) {
	tileResource[indexBG][indexShape][indexColor] = bit;
}
HBITMAP Stage::getTileResource(int indexBG, int indexShape, int indexColor) {
	return tileResource[indexBG][indexShape][indexColor];
}
void Stage::increaseUserTurn() {
	this->userTurn = (this->getUserTurn() + 1) % 2;
}
int Stage::getUserTurn() {
	return this->userTurn;
}
void Stage::resetRemainTime() {
	this->remainTime = DEFAULT_REMAIN_TIME;
}
void Stage::decreaseRemainTime(int n) {
	this->remainTime -= n;
}
int Stage::getRemainTime() {
	return this->remainTime;
}

Stage::Stage(HWND hWnd, HINSTANCE hInst) {
	this->hWnd = hWnd;
	this->hInst = hInst;
	for (int i = 0; i < 3; i++) { // bg
		for (int j = 0; j < 3; j++) { // shape 
			for (int k = 0; k < 3; k++) { // color
										  //setTileResource(i, j, k, LoadBitmap(hInst, MAKEINTRESOURCE(1000 + (i * 100) + (j * 10) + (k))));
				this->tileResource[i][j][k] = LoadBitmap(hInst, MAKEINTRESOURCE(1000 + (i * 100) + (j * 10) + (k)));
			}
		}
	}

	//tileResourceBack = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_TILE_BACK));
	isStart = false;
	tileResourceBack = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_TILE_BACK));
	currentStage = 0;

	answerCombineCount = 0;
	answeredCount = 0;
	userTurn = 0;
	*setUnsetMsg = TEXT("결/합 체크");
	missCount = 0;
}

void Stage::initAllTile(int turn) {  // 추후 파라미터로 이긴사람 먼저 할 수 있도록.

	// //////////////////////////////// //
	// 게임 최초 실행시 (stage==0) 초기화 //
	// //////////////////////////////// //
	if (currentStage == 0) {
		memset(player, 0, sizeof(User) * 2); // 2명이므로 2개만큼 초기화
	}

	// -1은 빈값.
	currentStage++;

	userTurn = turn;
	answerCombineCount = 0; // 
	answeredCount = 0;		// 사용자"들"이 맞춘 갯수


	memset(selectTileList, -1, sizeof(selectTileList));
	memset(selectList, -1, sizeof(selectList));
	selectCount = 0;
	missCount = 0;

	memset(answeredList, -1, sizeof(Answer)*MAX_ANSWERED_LIST);

	memset(currentStageTile, -1, sizeof(TILE)*TILE_WIDTH_AMOUNT*TILE_HEIGHT_AMOUNT);
	// isSelected는 false로 초기화해야하기때문에 69번째줄 참고
	memset(currentStageTileIntList, -1, sizeof(currentStageTileIntList));

	this->resetRemainTime();

	for (int j = 0; j < TILE_HEIGHT_AMOUNT; j++) {
		for (int i = 0; i < TILE_WIDTH_AMOUNT; i++) {
			currentStageTile[j][i].toggleSelect(); // 255 -> 1
			currentStageTile[j][i].toggleSelect(); // 1 -> 0

			currentStageTile[j][i].initializing();


			bool notDuplicate = false;

			while (!notDuplicate) {
				for (int a = 0; a < TILE_WIDTH_AMOUNT* TILE_HEIGHT_AMOUNT; a++) {

					/*
					// -1이 나왔다는건 그동안 중복된 값을 못찾았단 것임.
					if (currentStageTileIntList[a] == -1) {
						notDuplicate = true;
						break;
					}
					*/

					//현재 생성된 값이 중복된다면

					if (currentStageTileIntList[a] == currentStageTile[j][i].getColorBG() * 9 + currentStageTile[j][i].getBaseShape() * 3 + currentStageTile[j][i].getColorShape()) {
						currentStageTile[j][i].initializing();
						break;
					}
					if (a == TILE_WIDTH_AMOUNT * TILE_HEIGHT_AMOUNT - 1) {
						notDuplicate = true;
					}
				}

			}

			// 생성리스트에 넣음
			currentStageTileIntList[j * 3 + i] = currentStageTile[j][i].getColorBG() * 9 + currentStageTile[j][i].getBaseShape() * 3 + currentStageTile[j][i].getColorShape();
		}
	}
	// 생성된 스테이지에서 전수조사로 답을 찾아냄
	int tempArray[9];
	checkAllCombination(tempArray, 0, TILE_WIDTH_AMOUNT*TILE_HEIGHT_AMOUNT, 3, 0);
	resetSystemMsg();
}

void Stage::checkAllCombination(int arr[], int index, int n, int r, int target) {

	if (r == 0) {
		memset(checkArray, 0, sizeof(checkArray));
		for (int i = 0; i < index; i++) {
			checkArray[i] = arr[i];
		}
		if (isValidCombination(checkArray, false) == true) {
			answerCombineCount += 1;
			TCHAR resultString[20];
			wsprintf(resultString, TEXT("치트: %d, %d, %d"), checkArray[0]+1, checkArray[1]+1, checkArray[2]+1);


			MessageBox(hWnd, resultString, nullptr, 0);
		}
	}
	else if (target == n) {
		return;
	}
	else {
		arr[index] = target;
		checkAllCombination(arr, index + 1, n, r - 1, target + 1);
		checkAllCombination(arr, index, n, r, target + 1);
	}
}

bool Stage::isValidCombination(int arr[], bool userQuery) {
	

	int totalBG = 0, totalShape = 0, totalColor = 0;

	
	for (int i = 0; i < 3; i++) {

		// 값으로 주어졌을때
		if (userQuery == true) {
			int temp1, temp2, temp3;

			temp1 = arr[i] / 9;   // 2
			totalBG += temp1;

			temp2 = (arr[i] - temp1 * 9) / 3;  // (22 - 18)/3 = 4/3 = 1
			totalShape += temp2;

			temp3 = (arr[i] - (temp1 * 9) - (temp2 * 3)) / 3;
			totalColor += temp3;
		}
		// 타일로 주어졌을때
		else {
			int w = 0, h = 0;

			w = arr[i] % 3;
			h = arr[i] / 3;
			totalBG += currentStageTile[h][w].getColorBG();
			totalShape += currentStageTile[h][w].getBaseShape();
			totalColor += currentStageTile[h][w].getColorShape();
		}

	}
	return totalBG % 3 == 0 && totalShape % 3 == 0 && totalColor % 3 == 0;
	//이제 전부다 2차원 좌표로 분리된 상태.

}
void Stage::setUnset() {
	
	// missCount는 플레이어 오답또는 중복합 체크 수이고 이 값이 6이되면 자동으로 다음 스테이지로 넘어간다.
	if (this->selectCount == 0) {
		if (this->answeredCount == this->answerCombineCount) {
			if (this->currentStage+1 > 3) {
				wsprintf(this->systemMsg, TEXT("게임종료 플레이어 %s의 승리!"), player[0].getScore() > player[1].getScore() ? TEXT("1") : TEXT("2"));
				this->isStart = 0;
				//InvalidateRect(hWnd, &this->systemGUI, TRUE);
				InvalidateRect(hWnd, nullptr, TRUE);
				//has next round?
				return;
			}
			wsprintf(this->systemMsg, TEXT("[플레이어 %d] 결 정답! 스테이지가 종료됐습니다."), getUserTurn()+ 1);
			//this->systemMsg = TEXT("결: 플레이어 %s의 정답! 다음스테이지로 이동합니다.");
			this->isStart = 2; // await상태
			this->player[userTurn].matchUnSet();

			this->initAllTile(userTurn);
			SetTimer(hWnd, 0, 3000, nullptr);

			
		}
		else {
			wsprintf(this->systemMsg, TEXT("[플레이어 %d] 결 오답!"), getUserTurn() + 1);
			//this->systemMsg = TEXT("결: 오답입니다.");
			this->player[userTurn].missUnSet();

			missCount++;
		}

	}
	else if (this->selectCount == 3) {
		if (this->isValidCombination(this->selectList, true) == true) {

			for (int i = 0; i < MAX_ANSWERED_LIST; i++) {
				int checkHasElementCounter = 0;
				for (int a = 0; a < ANSWER_CHAIN; a++) {
					for (int b = 0; b < ANSWER_CHAIN; b++) {

						if (this->answeredList[i].pickArray[a] == this->selectTileList[b]) {
							checkHasElementCounter++;
						}

					}
				}
				if (checkHasElementCounter >= 3) {
					this->allTileUnselect();
					wsprintf(this->systemMsg, TEXT("[플레이어 %d] 중복된 합입니다!"), getUserTurn() + 1);
					//this->systemMsg = TEXT("합: 중복된 답입니다.");
					InvalidateRect(hWnd, &this->systemGUI, TRUE);
					InvalidateRect(hWnd, &this->stageGUI, TRUE);
					missCount++;
					return;
				}

			}
			// 중복되지 않은 정답일경우.

			for (int i = 0; i < ANSWER_CHAIN; i++) {
				this->answeredList[answeredCount].pickArray[i] = this->selectTileList[i];
			}
			this->answeredList[answeredCount].userId = userTurn;

			this->answeredCount++;
			wsprintf(this->systemMsg, TEXT("[플레이어 %d] 합 정답!!"), getUserTurn() + 1);
			this->player[userTurn].matchSet();
			//this->systemMsg = TEXT("합: 정답!!");
			missCount = 0;
		}
		else {
			wsprintf(this->systemMsg, TEXT("[플레이어 %d] 합 오답!"), getUserTurn() + 1);
			this->player[userTurn].missSet();
			missCount++;
			//this->systemMsg = TEXT("합: 오답!");
		}

	}
	else {
		wsprintf(this->systemMsg, TEXT("[플레이어 %d] 3개의 타일을 선택하세요"), getUserTurn() + 1);
		//this->systemMsg = TEXT("3개의 타일을 선택하세요.");
		this->allTileUnselect();
		return;
	}

	this->allTileUnselect();
	increaseUserTurn();
}

void Stage::DrawBitmap(HDC hdc, int x, int y, int w, int h, int cx, int cy, HBITMAP tmpBitmap, int option){ // int arrayOption[]) {

	HDC MemDC;
	HBITMAP OldBitmap;
	//int bx, by;

	BITMAP bit;

	MemDC = CreateCompatibleDC(hdc);
	OldBitmap = (HBITMAP)SelectObject(MemDC, tmpBitmap);

	// 기본값이면 이미지의 전체크기를 구해옴
	if (w == 0 && h == 0) {
		GetObject(tmpBitmap, sizeof(BITMAP), &bit);
		w = bit.bmWidth;
		h = bit.bmHeight;
	}

	if (option == 0){
		BitBlt(hdc, x, y, w, h, MemDC, cx, cy, SRCCOPY);
	}
	else {

		StretchBlt(hdc, x+10, y+10, w -20, h-20, MemDC, cx, cy, w, h, option);

		HPEN ellipseOldPen, ellipsePen;
		ellipsePen = CreatePen(PS_NULL, 0, 0);
		ellipseOldPen = (HPEN)SelectObject(hdc, ellipsePen);
		
		HBRUSH ellipseOldBrush, ellipseBrush;
		ellipseBrush = CreateSolidBrush(RGB(200, 40, 0));
		ellipseOldBrush= (HBRUSH)SelectObject(hdc, ellipseBrush);
		
		Ellipse(hdc, x+5, y+5, x +25, y + 25);

		SelectObject(hdc, ellipseOldBrush);
		DeleteObject(ellipseBrush);		

	}

	SelectObject(MemDC, OldBitmap);
	DeleteDC(MemDC);

}

void Stage::DrawStage(HDC hdc) {
	if (isStart == 1) {

		for (int j = 0; j < TILE_HEIGHT_AMOUNT; j++) { // 세로
			TCHAR index[3];
			for (int i = 0; i < TILE_WIDTH_AMOUNT; i++) { // 가로 인데 j=1이랑 i=0이면 1,0.
				DrawBitmap(hdc, tileRect[j][i].left, tileRect[j][i].top, 0,0,0,0, getMatchedTileToResource(i, j), currentStageTile[j][i].getIsSelected() != true ? 0 : NOTSRCCOPY );

				RECT smallRect = { tileRect[j][i].left + 10, tileRect[j][i].top + 10, tileRect[j][i].right - 10,tileRect[j][i].bottom - 10 };
				wsprintf(index, TEXT("%d"), i + (j * 3) + 1);
				DrawText(hdc, index, -1, &smallRect, DT_RIGHT);
			}
		}

	}
	else {
		for (int j = 0; j < TILE_HEIGHT_AMOUNT; j++) {
			for (int i = 0; i < TILE_WIDTH_AMOUNT; i++) {
				DrawBitmap(hdc, tileRect[j][i].left, tileRect[j][i].top, 0, 0, 0, 0, tileResourceBack, 0);
				
			}
		}
	}
	//리소스가 정상적으로 들어왔는지 확인
	//DrawBitmap(hdc, 0, 0, 0, 0, 0, 0, LoadBitmap(hInst, MAKEINTRESOURCE(IDB_TILE_122)), 0);

	// RECT영역 체크
	//Rectangle(hdc, leftGUI.left, leftGUI.top, leftGUI.right, leftGUI.bottom);
	//Rectangle(hdc, rightGUI.left, rightGUI.top, rightGUI.right, rightGUI.bottom);
	//Rectangle(hdc, stageGUI.left, stageGUI.top, stageGUI.right, stageGUI.bottom);
	//Rectangle(hdc, systemGUI.left, systemGUI.top, systemGUI.right, systemGUI.bottom);
	//Rectangle(hdc, guideGUI.left, guideGUI.top, guideGUI.right, guideGUI.bottom);

	// 텍스트로 RECT영역 확인
	/* 
	DrawText(hdc, TEXT("1번 GUI 영역"), -1, &this->leftGUI, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
	DrawText(hdc, TEXT("2번 GUI 영역"), -1, &this->rightGUI, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
	DrawText(hdc, TEXT("시스템 메시지 영역"), -1, &this->systemGUI, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
	DrawText(hdc, TEXT("가이드 텍스트 영역"), -1, &this->guideGUI, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
	*/
}

/* deprecated 
void Stage::DrawScreen(HDC hdc, RECT rect, int indexW, int indexH) {

	if (isStart == true) {
		int arrayOption[1] = {NOTSRCCOPY};
		DrawBitmap(hdc, rect.left, rect.top, getMatchedTileToResource(indexW, indexH), arrayOption);
	}
	else {
		DrawBitmap(hdc, rect.left, rect.top, tileResourceBack, nullptr);
	}
}
*/

HBITMAP Stage::getMatchedTileToResource(int indexW, int indexH) {

	return this->tileResource[currentStageTile[indexH][indexW].getColorBG()][currentStageTile[indexH][indexW].getBaseShape()][currentStageTile[indexH][indexW].getColorShape()];
	// 추후 단일비트맵으로 변경하자.
}

void Stage::setTileRect(int indexW, int indexH, RECT rt) {
	tileRect[indexH][indexW] = rt;
}

RECT Stage::getTileRect(int indexW, int indexH) {
	return tileRect[indexH][indexW];
}

void Stage::setLeftGUI(RECT rt) {
	this->leftGUI = rt;
}
void Stage::setRightGUI(RECT rt) {
	this->rightGUI = rt;
}
void Stage::setStageGUI(RECT rt) {
	this->stageGUI = rt;
}
void Stage::setSystemGUI(RECT rt) {
	this->systemGUI = rt;
}
RECT Stage::getSystemGUI() {
	return this->systemGUI;
}
void Stage::setProgressGUI(RECT rt) {
	this->systemGUI = rt;
}
RECT Stage::getSystemGUI() {
	return this->systemGUI;
}

void Stage::setGuideGUI(RECT rt) {
	this->guideGUI = rt;
}
void Stage::setSetRect(RECT rt) {
	this->setRect = rt;
}

void Stage::resetStage(HDC hdc) {
	for (int j = 0; j < TILE_HEIGHT_AMOUNT; j++) {
		for (int i = 0; i < TILE_WIDTH_AMOUNT; i++) {
			//DrawBitmap(hdc, tileRect[j][i].left, tileRect[j][i].top, 0, 0, 0, 0, tileResourceBack, 0);
			DrawBitmap(hdc, tileRect[j][i].left, tileRect[j][i].top, 0, 0, 0, 0, tileResourceBack, 0);
		}
	}

}
//시스템메시지 출력방식
void test1(TCHAR *) {

}
void test2() {
	test1(TEXT("테스트"));
}

void Stage::allTileUnselect() {
	for (int j = 0; j < TILE_HEIGHT_AMOUNT; j++) {
		for (int i = 0; i < TILE_WIDTH_AMOUNT; i++) {
			
			if (currentStageTile[j][i].getIsSelected() == true) {
				currentStageTile[j][i].toggleSelect();
			}
			
		}
	}
	this->selectCount = 0;
}

void Stage::keyEvent(int wParam) {

	if (isStart >= 2) {

		return;
	}
	else if (isStart == 1) {


		switch (wParam) {
		case VK_RETURN: {
			// 결인지 합인지
			this->setUnset();
			this->eventHandlerAfterSetUnSet();
			this->resetRemainTime();
			break;
		}
		case VK_ESCAPE: {
			this->isStart = false;
			break;

		}
		case VK_NUMPAD1:
		case 0x31: {

		}
		case VK_NUMPAD2: 
		case 0x32 :{

		}
		case VK_NUMPAD3:
		case 0x33: {

		}
		case VK_NUMPAD4:
		case 0x34: {

		}
		case VK_NUMPAD5:
		case 0x35: {
			
		}
		case VK_NUMPAD6:
		case 0x36: {
			
		}
		case VK_NUMPAD7:
		case 0x37: {
			
		}
		case VK_NUMPAD8:
		case 0x38: {
			
		}
		case VK_NUMPAD9:
		case 0x39: {
			// 숫자패드면 0x60부터, 일반키면 0x30부터.
			int w = wParam - 0x60 - 1 < 0 ? (wParam - 0x30 - 1) % 3 : (wParam - 0x60 - 1) % 3;
			int h = wParam - 0x60 - 1 < 0 ? (wParam - 0x30 - 1) / 3 : (wParam - 0x60 - 1) / 3;
			this->eventHandler1(w, h);

			break;

		}
		}


	}
	else { // 시작전일때
		switch (wParam) {
		case VK_RETURN: {
			this->currentStage = 0;

			initAllTile(rand()%2); // 랜덤 시작

			isStart = true;
			SetTimer(hWnd, 2, 500, nullptr);
			break;
		}
		}

	}
	InvalidateRect(hWnd, &this->systemGUI, TRUE);
	InvalidateRect(hWnd, &this->guideGUI, TRUE);
	InvalidateRect(hWnd, &this->setRect, TRUE);
	InvalidateRect(hWnd, &this->stageGUI, TRUE);
	
	InvalidateRect(hWnd, &this->leftGUI, TRUE);
	InvalidateRect(hWnd, &this->rightGUI, TRUE);
	
}

void Stage::mouseEvent(int lParam) {
	if (isStart >= 2 || isStart == 0) {
		//시작을 안했거나 await상태일때.
		return;
	}
	else if (isStart == 1) {
		POINT xy = { LOWORD(lParam) , HIWORD(lParam) };
		int i, j;

		if (PtInRect(&stageGUI, xy) == true) {

			i = (xy.x - USER_GUI_WIDTH) / TILE_SIZE;
			j = (xy.y) / TILE_SIZE;
			this->eventHandler1(i, j);

			//InvalidateRect(hWnd, &this->stageGUI, TRUE);
		}
		else if (PtInRect(&setRect, xy) == true) {
			this->setUnset();
			this->eventHandlerAfterSetUnSet();

		}

		InvalidateRect(hWnd, &this->systemGUI, TRUE);
		InvalidateRect(hWnd, &this->guideGUI, TRUE);
		InvalidateRect(hWnd, &this->setRect, TRUE);
		InvalidateRect(hWnd, &this->stageGUI, TRUE);

		InvalidateRect(hWnd, &this->leftGUI, TRUE);
		InvalidateRect(hWnd, &this->rightGUI, TRUE);
	}
	
}


// 이벤트핸들러1: 결합전에 3개를 선택하는 이벤트를 처리
void Stage::eventHandler1(int w, int h) {
	if (currentStageTile[h][w].getIsSelected() == false) {

		if (selectCount >= 3) {
			wsprintf(this->systemMsg, TEXT("3개를 초과해서 고를 수 없습니다"));
			//this->systemMsg = TEXT("3개를 초과해서 고를 수 없습니다");

			return;
		}

		selectList[selectCount] = currentStageTile[h][w].getColorBG() * 9 + currentStageTile[h][w].getBaseShape() * 3 + currentStageTile[h][w].getColorShape();
		selectTileList[selectCount] = w + (h * 3);

		selectCount++;
	}
	else {
		for (int a = 0; a < sizeof(selectList) / sizeof(int); a++) {
			// select를 했으면 selectList에 들어가있으므로 이 리스트를 전수조사해서 찾으면
			if (selectList[a] == currentStageTile[h][w].getColorBG() * 9 + currentStageTile[h][w].getBaseShape() * 3 + currentStageTile[h][w].getColorShape()) {

				//미리 -1로 대입해줘야함
				selectList[a] = -1;
				selectTileList[a] = -1;

				for (int b = a; b < sizeof(selectList) / sizeof(int) - 1; b++) {
					//왜냐면 이 구문이 안들어갈 수도 있기때문에. (마지막 인덱스일 경우)
					selectList[b] = selectList[b + 1];
					selectList[b + 1] = -1;

					selectTileList[a] = selectTileList[a + 1];
					selectTileList[a + 1] = -1;
				}

				break;
			}
		}
		selectCount--;
	}
	currentStageTile[h][w].toggleSelect();
}

void Stage::DrawGUI(HDC hdc) {
	printText(hdc, this->systemGUI, this->systemMsg);
	//printText(hdc, this->leftGUI, *leftMsg);
	//printText(hdc, this->rightGUI, *rightMsg);
	printText(hdc, this->guideGUI, *guideMsg);
	
	TCHAR answeredToStr[10];

	if (isStart == 1) {
		printText(hdc, this->setRect, *setUnsetMsg);
		wsprintf(answeredToStr, TEXT("%d점"), player[0].getScore());
		TextOut(hdc, 100, TILE_SIZE *(TILE_HEIGHT_AMOUNT - 1), answeredToStr, lstrlen(answeredToStr));
		wsprintf(answeredToStr, TEXT("%d점"), player[1].getScore());
		TextOut(hdc, 100 + USER_GUI_WIDTH + TILE_SIZE*TILE_WIDTH_AMOUNT, TILE_SIZE *(TILE_HEIGHT_AMOUNT - 1), answeredToStr, lstrlen(answeredToStr));
		
		// progressGUI 새로 만들 것.
		Rectangle(hdc, systemGUI.left, systemGUI.bottom - 20, systemGUI.left + (systemGUI.right - systemGUI.left) * (remainTime / DEFAULT_REMAIN_TIME), systemGUI.bottom);

		int leftUserCount = 0;
		int rightUserCount = 0;
		for (int i = 0; i < answeredCount; i++) {
			wsprintf(answeredToStr, TEXT("%d %d %d"), answeredList[i].pickArray[0] + 1, answeredList[i].pickArray[1] + 1, answeredList[i].pickArray[2] + 1);
			if (answeredList[i].userId == 0) {
				TextOut(hdc, 75, 50 * (leftUserCount + 1), answeredToStr, lstrlen(answeredToStr));
				leftUserCount++;
			}
			else {
				TextOut(hdc, 75 + USER_GUI_WIDTH + TILE_SIZE*TILE_WIDTH_AMOUNT, 50 * (rightUserCount + 1), answeredToStr, lstrlen(answeredToStr));
				rightUserCount++;
			}
		}
	}
	

}
void Stage::printText(HDC hdc, RECT &rect, TCHAR *str) {

	DrawText(hdc, str, -1, &rect, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
	SetTimer(hWnd, 1, 2000, nullptr);
	//InvalidateRect(hWnd, &rect, TRUE);

}
void Stage::resetSystemMsg() {
	if (this->isStart == 1) {
		wsprintf(this->systemMsg, TEXT("%s"), this->getUserTurn() == 0 ? TEXT("<< 차례") : TEXT("차례 >>"));
		//this->systemMsg = TEXT("");
		InvalidateRect(hWnd, &this->systemGUI, TRUE);
	}
	else if (this->isStart == 2) {
		// currentStage-1인 이유는 initAlltile에서 ++ 됐으므로 현재 스테이지는 1개 추가됐기 때문임.
		wsprintf(this->systemMsg, TEXT("[스테이지%d 종료] 스코어 %d:%d"), currentStage-1, player[0].getScore(), player[1].getScore());
		//this->systemMsg = TEXT("");
		InvalidateRect(hWnd, &this->systemGUI, TRUE);
	}
	else {
		wsprintf(this->systemMsg, TEXT(""));
		//this->systemMsg = TEXT("");
		InvalidateRect(hWnd, &this->systemGUI, TRUE);
	}
}
void Stage::eventHandlerAfterSetUnSet() {

	if (missCount >= 6) {
		Stage::eventHandlerOverTheMaxError();
	}

}
void Stage::eventHandlerOverTheMaxError() {
	if (this->currentStage+1 > 3) {
		wsprintf(this->systemMsg, TEXT("게임종료 플레이어 %s의 승리!"), player[0].getScore() > player[1].getScore() ? TEXT("1") : TEXT("2"));
		//InvalidateRect(hWnd, &this->systemGUI, TRUE);
		this->isStart = 0;
		InvalidateRect(hWnd, nullptr, TRUE);

		//has next round?  // deprecated
		return; 
	}
	wsprintf(this->systemMsg, TEXT("플레이어들의 오답이 %d회 입니다. 스테이지가 종료됩니다."), missCount );
	InvalidateRect(hWnd, &this->systemGUI, TRUE);
	this->isStart = 2; // await상태
	
	this->initAllTile(userTurn);
	//this->player[userTurn].matchUnSet();

	SetTimer(hWnd, 0, 3000, nullptr);
}
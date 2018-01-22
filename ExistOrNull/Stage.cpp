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
	*setUnsetMsg = TEXT("��/�� üũ");
	missCount = 0;
}

void Stage::initAllTile(int turn) {  // ���� �Ķ���ͷ� �̱��� ���� �� �� �ֵ���.

	// //////////////////////////////// //
	// ���� ���� ����� (stage==0) �ʱ�ȭ //
	// //////////////////////////////// //
	if (currentStage == 0) {
		memset(player, 0, sizeof(User) * 2); // 2���̹Ƿ� 2����ŭ �ʱ�ȭ
	}

	// -1�� ��.
	currentStage++;

	userTurn = turn;
	answerCombineCount = 0; // 
	answeredCount = 0;		// �����"��"�� ���� ����


	memset(selectTileList, -1, sizeof(selectTileList));
	memset(selectList, -1, sizeof(selectList));
	selectCount = 0;
	missCount = 0;

	memset(answeredList, -1, sizeof(Answer)*MAX_ANSWERED_LIST);

	memset(currentStageTile, -1, sizeof(TILE)*TILE_WIDTH_AMOUNT*TILE_HEIGHT_AMOUNT);
	// isSelected�� false�� �ʱ�ȭ�ؾ��ϱ⶧���� 69��°�� ����
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
					// -1�� ���Դٴ°� �׵��� �ߺ��� ���� ��ã�Ҵ� ����.
					if (currentStageTileIntList[a] == -1) {
						notDuplicate = true;
						break;
					}
					*/

					//���� ������ ���� �ߺ��ȴٸ�

					if (currentStageTileIntList[a] == currentStageTile[j][i].getColorBG() * 9 + currentStageTile[j][i].getBaseShape() * 3 + currentStageTile[j][i].getColorShape()) {
						currentStageTile[j][i].initializing();
						break;
					}
					if (a == TILE_WIDTH_AMOUNT * TILE_HEIGHT_AMOUNT - 1) {
						notDuplicate = true;
					}
				}

			}

			// ��������Ʈ�� ����
			currentStageTileIntList[j * 3 + i] = currentStageTile[j][i].getColorBG() * 9 + currentStageTile[j][i].getBaseShape() * 3 + currentStageTile[j][i].getColorShape();
		}
	}
	// ������ ������������ ��������� ���� ã�Ƴ�
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
			wsprintf(resultString, TEXT("ġƮ: %d, %d, %d"), checkArray[0]+1, checkArray[1]+1, checkArray[2]+1);


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

		// ������ �־�������
		if (userQuery == true) {
			int temp1, temp2, temp3;

			temp1 = arr[i] / 9;   // 2
			totalBG += temp1;

			temp2 = (arr[i] - temp1 * 9) / 3;  // (22 - 18)/3 = 4/3 = 1
			totalShape += temp2;

			temp3 = (arr[i] - (temp1 * 9) - (temp2 * 3)) / 3;
			totalColor += temp3;
		}
		// Ÿ�Ϸ� �־�������
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
	//���� ���δ� 2���� ��ǥ�� �и��� ����.

}
void Stage::setUnset() {
	
	// missCount�� �÷��̾� ����Ǵ� �ߺ��� üũ ���̰� �� ���� 6�̵Ǹ� �ڵ����� ���� ���������� �Ѿ��.
	if (this->selectCount == 0) {
		if (this->answeredCount == this->answerCombineCount) {
			if (this->currentStage+1 > 3) {
				wsprintf(this->systemMsg, TEXT("�������� �÷��̾� %s�� �¸�!"), player[0].getScore() > player[1].getScore() ? TEXT("1") : TEXT("2"));
				this->isStart = 0;
				//InvalidateRect(hWnd, &this->systemGUI, TRUE);
				InvalidateRect(hWnd, nullptr, TRUE);
				//has next round?
				return;
			}
			wsprintf(this->systemMsg, TEXT("[�÷��̾� %d] �� ����! ���������� ����ƽ��ϴ�."), getUserTurn()+ 1);
			//this->systemMsg = TEXT("��: �÷��̾� %s�� ����! �������������� �̵��մϴ�.");
			this->isStart = 2; // await����
			this->player[userTurn].matchUnSet();

			this->initAllTile(userTurn);
			SetTimer(hWnd, 0, 3000, nullptr);

			
		}
		else {
			wsprintf(this->systemMsg, TEXT("[�÷��̾� %d] �� ����!"), getUserTurn() + 1);
			//this->systemMsg = TEXT("��: �����Դϴ�.");
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
					wsprintf(this->systemMsg, TEXT("[�÷��̾� %d] �ߺ��� ���Դϴ�!"), getUserTurn() + 1);
					//this->systemMsg = TEXT("��: �ߺ��� ���Դϴ�.");
					InvalidateRect(hWnd, &this->systemGUI, TRUE);
					InvalidateRect(hWnd, &this->stageGUI, TRUE);
					missCount++;
					return;
				}

			}
			// �ߺ����� ���� �����ϰ��.

			for (int i = 0; i < ANSWER_CHAIN; i++) {
				this->answeredList[answeredCount].pickArray[i] = this->selectTileList[i];
			}
			this->answeredList[answeredCount].userId = userTurn;

			this->answeredCount++;
			wsprintf(this->systemMsg, TEXT("[�÷��̾� %d] �� ����!!"), getUserTurn() + 1);
			this->player[userTurn].matchSet();
			//this->systemMsg = TEXT("��: ����!!");
			missCount = 0;
		}
		else {
			wsprintf(this->systemMsg, TEXT("[�÷��̾� %d] �� ����!"), getUserTurn() + 1);
			this->player[userTurn].missSet();
			missCount++;
			//this->systemMsg = TEXT("��: ����!");
		}

	}
	else {
		wsprintf(this->systemMsg, TEXT("[�÷��̾� %d] 3���� Ÿ���� �����ϼ���"), getUserTurn() + 1);
		//this->systemMsg = TEXT("3���� Ÿ���� �����ϼ���.");
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

	// �⺻���̸� �̹����� ��üũ�⸦ ���ؿ�
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

		for (int j = 0; j < TILE_HEIGHT_AMOUNT; j++) { // ����
			TCHAR index[3];
			for (int i = 0; i < TILE_WIDTH_AMOUNT; i++) { // ���� �ε� j=1�̶� i=0�̸� 1,0.
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
	//���ҽ��� ���������� ���Դ��� Ȯ��
	//DrawBitmap(hdc, 0, 0, 0, 0, 0, 0, LoadBitmap(hInst, MAKEINTRESOURCE(IDB_TILE_122)), 0);

	// RECT���� üũ
	//Rectangle(hdc, leftGUI.left, leftGUI.top, leftGUI.right, leftGUI.bottom);
	//Rectangle(hdc, rightGUI.left, rightGUI.top, rightGUI.right, rightGUI.bottom);
	//Rectangle(hdc, stageGUI.left, stageGUI.top, stageGUI.right, stageGUI.bottom);
	//Rectangle(hdc, systemGUI.left, systemGUI.top, systemGUI.right, systemGUI.bottom);
	//Rectangle(hdc, guideGUI.left, guideGUI.top, guideGUI.right, guideGUI.bottom);

	// �ؽ�Ʈ�� RECT���� Ȯ��
	/* 
	DrawText(hdc, TEXT("1�� GUI ����"), -1, &this->leftGUI, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
	DrawText(hdc, TEXT("2�� GUI ����"), -1, &this->rightGUI, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
	DrawText(hdc, TEXT("�ý��� �޽��� ����"), -1, &this->systemGUI, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
	DrawText(hdc, TEXT("���̵� �ؽ�Ʈ ����"), -1, &this->guideGUI, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
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
	// ���� ���Ϻ�Ʈ������ ��������.
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
//�ý��۸޽��� ��¹��
void test1(TCHAR *) {

}
void test2() {
	test1(TEXT("�׽�Ʈ"));
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
			// ������ ������
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
			// �����е�� 0x60����, �Ϲ�Ű�� 0x30����.
			int w = wParam - 0x60 - 1 < 0 ? (wParam - 0x30 - 1) % 3 : (wParam - 0x60 - 1) % 3;
			int h = wParam - 0x60 - 1 < 0 ? (wParam - 0x30 - 1) / 3 : (wParam - 0x60 - 1) / 3;
			this->eventHandler1(w, h);

			break;

		}
		}


	}
	else { // �������϶�
		switch (wParam) {
		case VK_RETURN: {
			this->currentStage = 0;

			initAllTile(rand()%2); // ���� ����

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
		//������ ���߰ų� await�����϶�.
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


// �̺�Ʈ�ڵ鷯1: �������� 3���� �����ϴ� �̺�Ʈ�� ó��
void Stage::eventHandler1(int w, int h) {
	if (currentStageTile[h][w].getIsSelected() == false) {

		if (selectCount >= 3) {
			wsprintf(this->systemMsg, TEXT("3���� �ʰ��ؼ� �� �� �����ϴ�"));
			//this->systemMsg = TEXT("3���� �ʰ��ؼ� �� �� �����ϴ�");

			return;
		}

		selectList[selectCount] = currentStageTile[h][w].getColorBG() * 9 + currentStageTile[h][w].getBaseShape() * 3 + currentStageTile[h][w].getColorShape();
		selectTileList[selectCount] = w + (h * 3);

		selectCount++;
	}
	else {
		for (int a = 0; a < sizeof(selectList) / sizeof(int); a++) {
			// select�� ������ selectList�� �������Ƿ� �� ����Ʈ�� ���������ؼ� ã����
			if (selectList[a] == currentStageTile[h][w].getColorBG() * 9 + currentStageTile[h][w].getBaseShape() * 3 + currentStageTile[h][w].getColorShape()) {

				//�̸� -1�� �����������
				selectList[a] = -1;
				selectTileList[a] = -1;

				for (int b = a; b < sizeof(selectList) / sizeof(int) - 1; b++) {
					//�ֳĸ� �� ������ �ȵ� ���� �ֱ⶧����. (������ �ε����� ���)
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
		wsprintf(answeredToStr, TEXT("%d��"), player[0].getScore());
		TextOut(hdc, 100, TILE_SIZE *(TILE_HEIGHT_AMOUNT - 1), answeredToStr, lstrlen(answeredToStr));
		wsprintf(answeredToStr, TEXT("%d��"), player[1].getScore());
		TextOut(hdc, 100 + USER_GUI_WIDTH + TILE_SIZE*TILE_WIDTH_AMOUNT, TILE_SIZE *(TILE_HEIGHT_AMOUNT - 1), answeredToStr, lstrlen(answeredToStr));
		
		// progressGUI ���� ���� ��.
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
		wsprintf(this->systemMsg, TEXT("%s"), this->getUserTurn() == 0 ? TEXT("<< ����") : TEXT("���� >>"));
		//this->systemMsg = TEXT("");
		InvalidateRect(hWnd, &this->systemGUI, TRUE);
	}
	else if (this->isStart == 2) {
		// currentStage-1�� ������ initAlltile���� ++ �����Ƿ� ���� ���������� 1�� �߰��Ʊ� ������.
		wsprintf(this->systemMsg, TEXT("[��������%d ����] ���ھ� %d:%d"), currentStage-1, player[0].getScore(), player[1].getScore());
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
		wsprintf(this->systemMsg, TEXT("�������� �÷��̾� %s�� �¸�!"), player[0].getScore() > player[1].getScore() ? TEXT("1") : TEXT("2"));
		//InvalidateRect(hWnd, &this->systemGUI, TRUE);
		this->isStart = 0;
		InvalidateRect(hWnd, nullptr, TRUE);

		//has next round?  // deprecated
		return; 
	}
	wsprintf(this->systemMsg, TEXT("�÷��̾���� ������ %dȸ �Դϴ�. ���������� ����˴ϴ�."), missCount );
	InvalidateRect(hWnd, &this->systemGUI, TRUE);
	this->isStart = 2; // await����
	
	this->initAllTile(userTurn);
	//this->player[userTurn].matchUnSet();

	SetTimer(hWnd, 0, 3000, nullptr);
}
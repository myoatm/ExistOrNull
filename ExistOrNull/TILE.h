#pragma once
#include <ctime>
#include <cstdlib>


#define TILE_WIDTH_AMOUNT 3
#define TILE_HEIGHT_AMOUNT 3

struct TILE {
private:
	int colorBG; // 0 = Black , 1 = DarkGray, 2 = White
	int baseShape;  // 0 = Triangle, 1 = Square , 2 = Circle
	int colorShape; // 0 = DarkCyan, 1 = DarkMagenta, 2 = DarkYellow
	bool isSelected;
public:
	TILE();
	void setRandColorBG();
	void setRandColorShape();
	void setRandBaseShape();
	void toggleSelect();
	void initializing();
	int getColorBG();
	int getColorShape();
	int getBaseShape();
	bool getIsSelected();

};
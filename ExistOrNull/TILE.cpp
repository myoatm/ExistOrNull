#pragma once
#include "stdafx.h"
#include "TILE.h"


TILE::TILE() {

	//initializing();
	isSelected = false;
}


void TILE::setRandColorBG() {
	this->colorBG = rand() % 3;
}
void TILE::setRandColorShape() {
	this->colorShape = rand() % 3;
}
void TILE::setRandBaseShape() {
	this->baseShape = rand() % 3;  // random number. range of 0~2
}
void TILE::toggleSelect() {
	if (isSelected == true) {
		isSelected = false;
		return;
	}
	isSelected = true;
}

void TILE::initializing() {
	setRandBaseShape();
	setRandColorShape();
	setRandColorBG();
}
int TILE::getColorBG() {
	return colorBG;
}
int TILE::getColorShape() {
	return colorShape;
}
int TILE::getBaseShape() {
	return baseShape;
}
bool TILE::getIsSelected() {
	return isSelected;
}
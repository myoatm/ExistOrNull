#pragma once
#include "stdafx.h"
#include "User.h"


User::User() {
	this->score = 0;
}

User::~User() {

}

int User::getScore() {
	return this->score;
}


void User::matchSet() { // �ռ���
	this->score += 1;
}
void User::matchUnSet() { // �� ����
	this->score += 3;
}
void User::missSet() { // �� ����
	this->score += 0;
}
void User::missUnSet() { // �� ����
	this->score -= 1;
}
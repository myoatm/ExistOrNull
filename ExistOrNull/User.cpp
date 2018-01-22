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


void User::matchSet() { // 합성공
	this->score += 1;
}
void User::matchUnSet() { // 결 성공
	this->score += 3;
}
void User::missSet() { // 합 실패
	this->score += 0;
}
void User::missUnSet() { // 결 실패
	this->score -= 1;
}
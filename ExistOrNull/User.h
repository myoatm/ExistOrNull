#pragma once
#include "stdafx.h"

class User {
private:
	int score;

public:
	User();
	~User();
	int getScore();

	void matchSet();
	void matchUnSet();
	void missSet();
	void missUnSet();

};
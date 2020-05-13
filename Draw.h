#pragma once
//#include <Windows.h>
#include <iostream>

using namespace std;

class Draw {
private:
	int* N = new int;
	char Mass[20][20];
public:
	void draw();
	void setN(int a);
	int* getN();
	void setup();
};

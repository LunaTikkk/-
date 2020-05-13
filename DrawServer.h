#pragma once
#include <iostream>
#include <Windows.h>

using namespace std;

class DrawServer {
private:
	int* N = new int;
	char Mass[20][20];
public:
	int* getN();
	char* getMass(int i);
	void draw();
	void setup();
};

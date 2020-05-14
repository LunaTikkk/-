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
	void setN(int a);
	char* getMass(int i);
	void changeMass(int* x, int* y, char* a);
	void draw();
	void setup();
};

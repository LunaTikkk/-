#pragma once
#include <iostream>
#include <Windows.h>

using namespace std;

class DrawServer {
private:
	int* N = new int;
	char Mass[20][20];
public:
	DrawServer();			//Конструктор без параметров
	~DrawServer();			//Деструктор
	int* getN();			//Получение адреса размерности карты
	void changeMass(int* x, int* y, char* a);	//Изменение карты для отрисовки
	void draw();			//Отрисовка карты
	void setup();			//Установка начальных значений карты
};

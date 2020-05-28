#pragma once
#include <iostream>

//using namespace sf;
using namespace std;

class Draw {
private:
	int* N = new int;
	char Mass[20][20];
	//Texture texture;
	//Sprite sprite;
public:
	Draw();				//Конструктор без параметров
	~Draw();			//Деструктор
	void draw();		//Отрисока карты
	void setN(int a);	//Установка параметра размерности карты
	int* getN();		//Метод get, для получение размерности карты
	void changeMass(int* x, int* y, char* a);	//Изменение массива после ходов
	void setup();		//Установка начальных параметров карты
};

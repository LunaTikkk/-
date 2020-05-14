#include <iostream>
#include<cstdlib>
#include <ctime>
#include "ADDRServer.h"
#include "DrawServer.h"

ADDRServer server;
DrawServer draw;

enum STATE { FRIEND = 0, FRIENDCASTLE, ENEMY, ENEMYCASTLE, EMPTY, BORDER };
STATE arr[20][20];
int var, cnt;
int* x = new int [4];
int* y = new int [4];
char msg[5];
char* ch1 = new char;
char* ch2 = new char;
int* enemy_x = new int;
int* enemy_y = new int;
char* enemy_c = new char;
bool F = false;

void start(); //Согласие на установку соединения с клиентом
void logicmap(); //Логическая состоялвяющая карты
void intelligance(); //Действия ИИ
void logic(); //Логика игры
void read(); //Считывание данных от клиента

int main(int argc, char* argv[]) {
	setlocale(LC_ALL, "ru");
	bool flag1 = true;

	while (server.connection()) {}

	start();
	draw.setup();
	logicmap();
	draw.draw();

	while (1) {
		read();
		Sleep(500);
		intelligance();
		logic();
		Sleep(500);
		draw.draw();
	}

	system("pause");
	return 0;
}

void start() {
	char array[] = "Server ready!\n";
	send(server.getConnect(), array, sizeof(array), NULL);
	Sleep(500);
	server.receivingint(draw.getN());
	//draw.setN(4);
}

void logicmap() {
	for (int i = 0; i < *draw.getN() + 2; i++) {
		for (int j = 0; j < *draw.getN() + 2; j++) {
			if (i == 0 || j == 0 || j == *draw.getN() + 1 || i == *draw.getN() + 1)
				arr[i][j] = BORDER;
			else if (i == 1 && j == 1)
				arr[i][j] = FRIEND;
			else if (i == *draw.getN() && j == *draw.getN())
				arr[i][j] = ENEMY;
			else
				arr[i][j] = EMPTY;
		}
	}
	for (int i = 0; i < *draw.getN(); i++) {
		x[i] = 0;
		y[i] = 0;
	}
}

void intelligance() {
	for (int i = 0; i < *draw.getN() + 2; i++) {
		for (int j = 0; j < *draw.getN() + 2; j++) {
			if (arr[i][j] == EMPTY && (arr[i - 1][j] == FRIEND || arr[i][j - 1] == FRIEND || arr[i + 1][j] == FRIEND || arr[i][j + 1] == FRIEND)) {
				x[i] = i;
				y[i] = j;
			}

			else if (arr[i][j] == EMPTY && (arr[i - 1][j] == FRIENDCASTLE || arr[i][j - 1] == FRIENDCASTLE || arr[i + 1][j] == FRIENDCASTLE || arr[i][j + 1] == FRIENDCASTLE)) {
				x[i] = i;
				y[i] = j;
			}

			else if (arr[i][j] == ENEMY && (arr[i - 1][j] == FRIEND || arr[i][j - 1] == FRIEND || arr[i + 1][j] == FRIEND || arr[i][j + 1] == FRIEND)) {
				x[i] = i;
				y[i] = j;
			}

			else if (arr[i][j] == ENEMY && (arr[i - 1][j] == FRIENDCASTLE || arr[i][j - 1] == FRIENDCASTLE || arr[i + 1][j] == FRIENDCASTLE || arr[i][j + 1] == FRIENDCASTLE)) {
				x[i] = i;
				y[i] = j;
			}
		}
	}

	bool flag = true;
	while (flag) {
		var = rand() % 4;
		if (x[var] != 0 && y[var] != 0)
			flag = 0;
	}
}

void logic() {
	int* x1 = &x[var];
	int* y1 = &y[var];
	char ch11[] = "x";
	char ch21[] = "Q";
	*ch1 = 'x';
	*ch2 = 'Q';
	if (arr[x[var]][y[var]] == EMPTY && (arr[x[var] - 1][y[var]] == FRIEND || arr[x[var]][y[var] - 1] == FRIEND || arr[x[var] + 1][y[var]] == FRIEND || arr[x[var]][y[var] + 1] == FRIEND)) {
		arr[x[var]][y[var]] = FRIEND;
		draw.changeMass(x1, y1, ch1);
		server.sendingint(x1);
		Sleep(300);
		server.sendingint(y1);
		Sleep(300);
		send(server.getConnect(), ch11, sizeof(ch11), NULL);
		//cout << "Отправка символа:" << ch11 << endl;
		//system("pause");
		Sleep(300);
	}

	else if (arr[x[var]][y[var]] == EMPTY && (arr[x[var] - 1][y[var]] == FRIENDCASTLE || arr[x[var]][y[var] - 1] == FRIENDCASTLE || arr[x[var] + 1][y[var]] == FRIENDCASTLE || arr[x[var]][y[var] + 1] == FRIENDCASTLE)) {
		arr[x[var]][y[var]] = FRIEND;
		draw.changeMass(x1, y1, ch1);
		server.sendingint(x1);
		Sleep(300);
		server.sendingint(y1);
		Sleep(300);
		send(server.getConnect(), ch11, sizeof(ch11), NULL);
		Sleep(300);
	}

	else if (arr[x[var]][y[var]] == ENEMY && (arr[x[var] - 1][y[var]] == FRIEND || arr[x[var]][y[var] - 1] == FRIEND || arr[x[var] + 1][y[var]] == FRIEND || arr[x[var]][y[var] + 1] == FRIEND)) {
		arr[x[var]][y[var]] = FRIENDCASTLE;
		draw.changeMass(x1, y1, ch2);
		server.sendingint(x1);
		Sleep(300);
		server.sendingint(y1);
		Sleep(300);
		send(server.getConnect(), ch21, sizeof(ch21), NULL);
		Sleep(300);
	}

	else if (arr[x[var]][y[var]] == ENEMY && (arr[x[var] - 1][y[var]] == FRIENDCASTLE || arr[x[var]][y[var] - 1] == FRIENDCASTLE || arr[x[var] + 1][y[var]] == FRIENDCASTLE || arr[x[var]][y[var] + 1] == FRIENDCASTLE)) {
		arr[x[var]][y[var]] = FRIENDCASTLE;
		draw.changeMass(x1, y1, ch2);
		server.sendingint(x1);
		Sleep(300);
		server.sendingint(y1);
		Sleep(300);
		send(server.getConnect(), ch21, sizeof(ch21), NULL);
		Sleep(300);
	}

	for (int i = 0; i < *draw.getN(); i++) {
		x[i] = 0;
		y[i] = 0;
	}
	cnt = 0;
}

void read() {
	server.receivingint(enemy_x);
	//cout << "Считало Enemy_x = " << *enemy_x << endl;
	server.receivingint(enemy_y);
	//cout << "Считало Enemy_y = " << *enemy_y << endl;
	recv(server.getConnect(), msg, sizeof(msg), NULL);

	if (arr[*enemy_x][*enemy_y] == EMPTY)
		arr[*enemy_x][*enemy_y] = ENEMY;
	else if (arr[*enemy_x][*enemy_y] == FRIEND)
		arr[*enemy_x][*enemy_y] = ENEMYCASTLE;
	draw.changeMass(enemy_x, enemy_y, msg);
	//cout << "Считало Enemy_c = " << msg << endl;

	Sleep(500);
	//system("pause");
}

#include <cstdlib>
#include <ctime>
#include "ADDRServer.h"
#include "DrawServer.h"

ADDRServer* server;
DrawServer* draw;

enum STATE { FRIEND = 0, FRIENDCASTLE, ENEMY, ENEMYCASTLE, EMPTY, BORDER };

STATE arr[20][20];			//Массив логической составляющей карты (Состояния клеток)

int var;					//Переменная для хранения определенной вариации хода ИИ
int* x;						//Массив для хранения вариация хода ИИ по оси X
int* y;						//Массив для хранения вариаций хода ИИ по оси Y
char msg[5];				//Переменная для считывания символа от клиента
char* ch1 = new char;		//Переменная для добавления фишки на карту (обычная фишка)
char* ch2 = new char;		//Переменная для добавления фишки на карту (фишка-крепость)
int* enemy_x = new int;		//Указатель для хранения хода клиента по оси X
int* enemy_y = new int;		//Указатель для хранения хода клиента по оси Y
char* enemy_c = new char;	//Указатель для хранения символа, который отправил клиент

bool F_over = true;

void start();				//Согласие на установку соединения с клиентом
void logicmap();			//Логическая состоялвяющая карты
void intelligance();		//Логика ИИ
void logic();				//Логика игры
void read();				//Считывание данных от клиента
int cheking();
int cheking_empty_friend();
int cheking_empty_enemy();

int main(int argc, char* argv[]) {
	setlocale(LC_ALL, "ru");

	server = new ADDRServer;
	draw = new DrawServer;

	while (server->connection()) {} //Ожидание подключене клиента
	start();			//Стартовые значения и принятие соединения
	draw->setup();		//Установка начальной карты
	logicmap();			//Установка логической карты
	draw->draw();		//Отрисовка карты

	while (F_over) {
		//Ход игрока
		for (int i = 0; i < 3; i++) {
			if (cheking()) {
				//Считывание хода клиента
				read();
				Sleep(500);		//Задержка после считывания
				draw->draw();	//Отрисовка карты
			}
			else
				break;
		}

		//Ход ИИ
		if (F_over) {
			for (int i = 0; i < 3; i++) {
				if (cheking()) {
					intelligance();	//Ход ИИ
					logic();		//Логическая обработка хода ИИ
					Sleep(500);		//Задержка
					draw->draw();	//Отрисовка карты
				}
				else
					break;
			}
		}
	}


	/*
	Sleep(500);
	char dis[] = "Disconnect";
	send(server->getConnect(), dis, sizeof(dis), NULL);
	cout << "Sending: " << dis << endl;
	system("pause");
	*/

	x = nullptr;
	y = nullptr;
	ch1 = nullptr;
	ch2 = nullptr;
	enemy_x = nullptr;
	enemy_y = nullptr;
	enemy_c = nullptr;

	delete x;
	delete y;
	delete ch1;
	delete ch2;
	delete enemy_x;
	delete enemy_y;
	delete enemy_c;

	delete server;
	delete draw;

	system("pause");
	return 0;
}

void start() {
	char array[] = "Server ready!\n";
	send(server->getConnect(), array, sizeof(array), NULL);	//Отправка сообщения сервера о готовности
	Sleep(500);
	server->receivingint(draw->getN());						//Принятие данных сервером о размерности карты

	x = new int[*draw->getN() * 3];
	y = new int[*draw->getN() * 3];
}

void logicmap() {
	//Создание логической части карты
	for (int i = 0; i < *draw->getN() + 2; i++) {
		for (int j = 0; j < *draw->getN() + 2; j++) {
			if (i == 0 || j == 0 || j == *draw->getN() + 1 || i == *draw->getN() + 1)
				arr[i][j] = BORDER;
			else if (i == 1 && j == 1)
				arr[i][j] = FRIEND;
			else if (i == *draw->getN() && j == *draw->getN())
				arr[i][j] = ENEMY;
			else
				arr[i][j] = EMPTY;
		}
	}

	//Обнуление массива вариаций ходов
	for (int i = 0; i < *draw->getN(); i++) {
		x[i] = 0;
		y[i] = 0;
	}
}

void intelligance() {
	//Поиск возможного хода ИИ
	srand(time(NULL));

	for (int i = 0; i < *draw->getN() * 3; i++) {
		x[i] = 0;
		y[i] = 0;
	}

	int count = 0;
	for (int i = 0; i < *draw->getN() + 2; i++) {
		for (int j = 0; j < *draw->getN() + 2; j++) {

			if (arr[i][j] == ENEMY && (arr[i - 1][j] == FRIEND || arr[i][j - 1] == FRIEND || arr[i + 1][j] == FRIEND || arr[i][j + 1] == FRIEND)) {
				x[count] = i;
				y[count] = j;
				count++;
			}

			else if (arr[i][j] == ENEMY && (arr[i - 1][j] == FRIENDCASTLE || arr[i][j - 1] == FRIENDCASTLE || arr[i + 1][j] == FRIENDCASTLE || arr[i][j + 1] == FRIENDCASTLE)) {
				x[count] = i;
				y[count] = j;
				count++;
			}

			else if (arr[i][j] == EMPTY && (arr[i - 1][j] == FRIEND || arr[i][j - 1] == FRIEND || arr[i + 1][j] == FRIEND || arr[i][j + 1] == FRIEND)) {
				x[count] = i;
				y[count] = j;
				count++;
			}

			else if (arr[i][j] == EMPTY && (arr[i - 1][j] == FRIENDCASTLE || arr[i][j - 1] == FRIENDCASTLE || arr[i + 1][j] == FRIENDCASTLE || arr[i][j + 1] == FRIENDCASTLE)) {
				x[count] = i;
				y[count] = j;
				count++;
			}
		}
	}

	bool flag = true;
	while (flag) {
		var = rand() % (*draw->getN() * 3);
		if (x[var] != 0 && y[var] != 0)
			flag = false;
	}
}

void logic() {
	int* x1 = &x[var];		//Запись в указатель вариации хода ИИ по оси X
	int* y1 = &y[var];		//Запись в указатель вариации хода ИИ по оси Y
	char ch11[] = "x";		//Символ для отправки клиенту
	char ch21[] = "X";		//Символ для отправки клиенту
	*ch1 = 'x';				//Указатель для отрисовки карты
	*ch2 = 'X';				//Указатель для отрисовки карты

	if (arr[x[var]][y[var]] == EMPTY && (arr[x[var] - 1][y[var]] == FRIEND || arr[x[var]][y[var] - 1] == FRIEND || arr[x[var] + 1][y[var]] == FRIEND || arr[x[var]][y[var] + 1] == FRIEND)) {

		arr[x[var]][y[var]] = FRIEND;
		draw->changeMass(x1, y1, ch1);

		server->sendingint(x1);
		Sleep(300);

		server->sendingint(y1);
		Sleep(300);

		send(server->getConnect(), ch11, sizeof(ch11), NULL);
		Sleep(300);
	}

	else if (arr[x[var]][y[var]] == EMPTY && (arr[x[var] - 1][y[var]] == FRIENDCASTLE || arr[x[var]][y[var] - 1] == FRIENDCASTLE || arr[x[var] + 1][y[var]] == FRIENDCASTLE || arr[x[var]][y[var] + 1] == FRIENDCASTLE)) {
		arr[x[var]][y[var]] = FRIEND;
		draw->changeMass(x1, y1, ch1);
		server->sendingint(x1);
		Sleep(300);
		server->sendingint(y1);
		Sleep(300);
		send(server->getConnect(), ch11, sizeof(ch11), NULL);
		Sleep(300);
	}

	else if (arr[x[var]][y[var]] == ENEMY && (arr[x[var] - 1][y[var]] == FRIEND || arr[x[var]][y[var] - 1] == FRIEND || arr[x[var] + 1][y[var]] == FRIEND || arr[x[var]][y[var] + 1] == FRIEND)) {
		arr[x[var]][y[var]] = FRIENDCASTLE;
		draw->changeMass(x1, y1, ch2);
		server->sendingint(x1);
		Sleep(300);
		server->sendingint(y1);
		Sleep(300);
		send(server->getConnect(), ch21, sizeof(ch21), NULL);
		Sleep(300);
	}

	else if (arr[x[var]][y[var]] == ENEMY && (arr[x[var] - 1][y[var]] == FRIENDCASTLE || arr[x[var]][y[var] - 1] == FRIENDCASTLE || arr[x[var] + 1][y[var]] == FRIENDCASTLE || arr[x[var]][y[var] + 1] == FRIENDCASTLE)) {
		arr[x[var]][y[var]] = FRIENDCASTLE;
		draw->changeMass(x1, y1, ch2);
		server->sendingint(x1);
		Sleep(300);
		server->sendingint(y1);
		Sleep(300);
		send(server->getConnect(), ch21, sizeof(ch21), NULL);
		Sleep(300);
	}

	for (int i = 0; i < 4; i++) {
		x[i] = 0;
		y[i] = 0;
	}
	x1 = nullptr;
	y1 = nullptr;

	delete x1;
	delete y1;
}

void read() {
	server->receivingint(enemy_x);							//Считывание сервером координаты фишки по оси X
	server->receivingint(enemy_y);							//Считывание сервером координаты фишки по оси Y

	recv(server->getConnect(), msg, sizeof(msg), NULL);		//Считывание сервером символа, который будет использован для отрисовки карты

	if (arr[*enemy_x][*enemy_y] == EMPTY)
		arr[*enemy_x][*enemy_y] = ENEMY;
	else if (arr[*enemy_x][*enemy_y] == FRIEND)
		arr[*enemy_x][*enemy_y] = ENEMYCASTLE;

	draw->changeMass(enemy_x, enemy_y, msg);				//Изменение карты после хода клиента

	Sleep(500);
}

int cheking() {
	//Проверка на ничью

	if (!cheking_empty_friend() && !cheking_empty_enemy()) {
		cout << "Ничья...\n";
		F_over = false;
		return 0;
	}

	//Проверка на победу

	if (!cheking_empty_enemy()) {
		cout << "Победа!!!:)\n";
		F_over = false;
		return 0;
	}
	/*
	if (Flag == true) {
		cout << "Победа!!!\n";
		return 0;
	}
	*/

	//Проверка на поражение

	if (!cheking_empty_friend()) {
		cout << "Порожение!!!:(\n";
		F_over = false;
		return 0;
	}

	return 1;
}

int cheking_empty_friend() {
	//Проверка на невозможность хода
	bool Flag = true;
	int count = 0;

	for (int i = 0; i < *draw->getN() * 3; i++) {
		x[i] = 0;
		y[i] = 0;
	}

	for (int i = 0; i < *draw->getN() + 2; i++) {
		for (int j = 0; j < *draw->getN() + 2; j++) {

			if (arr[i][j] == ENEMY && (arr[i - 1][j] == FRIEND || arr[i][j - 1] == FRIEND || arr[i + 1][j] == FRIEND || arr[i][j + 1] == FRIEND)) {
				x[count] = i;
				y[count] = j;
				count++;
			}

			else if (arr[i][j] == ENEMY && (arr[i - 1][j] == FRIENDCASTLE || arr[i][j - 1] == FRIENDCASTLE || arr[i + 1][j] == FRIENDCASTLE || arr[i][j + 1] == FRIENDCASTLE)) {
				x[count] = i;
				y[count] = j;
				count++;
			}

			else if (arr[i][j] == EMPTY && (arr[i - 1][j] == FRIEND || arr[i][j - 1] == FRIEND || arr[i + 1][j] == FRIEND || arr[i][j + 1] == FRIEND)) {
				x[count] = i;
				y[count] = j;
				count++;
			}

			else if (arr[i][j] == EMPTY && (arr[i - 1][j] == FRIENDCASTLE || arr[i][j - 1] == FRIENDCASTLE || arr[i + 1][j] == FRIENDCASTLE || arr[i][j + 1] == FRIENDCASTLE)) {
				x[count] = i;
				y[count] = j;
				count++;
			}
		}
	}

	for (int i = 0; i < *draw->getN() * 3; i++) {
		if (x[i] == 0 && y[i] == 0)
			Flag = true;
		else {
			Flag = false;
			break;
		}
	}

	if (Flag == true) {
		F_over = false;
		return 0;
	}
	return 1;
}

int cheking_empty_enemy() {
	//Проверка на невозможность хода
	bool Flag = true;
	int count = 0;

	for (int i = 0; i < *draw->getN() * 3; i++) {
		x[i] = 0;
		y[i] = 0;
	}

	for (int i = 0; i < *draw->getN() + 2; i++) {
		for (int j = 0; j < *draw->getN() + 2; j++) {

			if (arr[i][j] == FRIEND && (arr[i - 1][j] == ENEMY || arr[i][j - 1] == ENEMY || arr[i + 1][j] == ENEMY || arr[i][j + 1] == ENEMY)) {
				x[count] = i;
				y[count] = j;
				count++;
			}

			else if (arr[i][j] == FRIEND && (arr[i - 1][j] == ENEMYCASTLE || arr[i][j - 1] == ENEMYCASTLE || arr[i + 1][j] == ENEMYCASTLE || arr[i][j + 1] == ENEMYCASTLE)) {
				x[count] = i;
				y[count] = j;
				count++;
			}

			else if (arr[i][j] == EMPTY && (arr[i - 1][j] == ENEMY || arr[i][j - 1] == ENEMY || arr[i + 1][j] == ENEMY || arr[i][j + 1] == ENEMY)) {
				x[count] = i;
				y[count] = j;
				count++;
			}

			else if (arr[i][j] == EMPTY && (arr[i - 1][j] == ENEMYCASTLE || arr[i][j - 1] == ENEMYCASTLE || arr[i + 1][j] == ENEMYCASTLE || arr[i][j + 1] == ENEMYCASTLE)) {
				x[count] = i;
				y[count] = j;
				count++;
			}
		}
	}

	for (int i = 0; i < *draw->getN() * 3; i++) {
		if (x[i] == 0 && y[i] == 0)
			Flag = true;
		else {
			Flag = false;
			break;
		}
	}

	if (Flag == true) {
		F_over = false;
		return 0;
	}
	return 1;
}

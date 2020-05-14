#include "Draw.h"
#include "ADDR.h"
ADDR client;
Draw draw;
int *x = new int, *y = new int, cnt, x_dop, y_dop;
char* ch1 = new char;
char* ch2 = new char;
char msg[5];
int* enemy_x = new int;
int* enemy_y = new int;
char* enemy_c = new char;
bool F = false;

//Состояния клеток
enum STATE{FRIEND = 0, FRIENDCASTLE, ENEMY, ENEMYCASTLE, EMPTY, BORDER};
STATE arr[20][20];

void start(); //Функция задания начальных параметров для установки соединения и игры
void logicmap(); //Логическая часть карты (подобие бэк-енда)
void input(); //Ввод пользователем
void logic(); //Логическая обработка введенных данных, а также логика игры
void read(); //Считывание данных от сервера

int main(int argc, char* argv[]) {
	setlocale(LC_ALL, "ru");
	while(client.connection()){}

	start();
	draw.setup();
	logicmap();

	while (1) {
		draw.draw();
		input();
		logic();
		Sleep(500);
		if (F) {
			read();
			F = false;
		}
	}

	system("pause");
	return 0;
}

void start() {
	int n;
	char array[20];
	cout << "Приветсвую в игре Вирусы\nПравила игры:\n1. Для выбора клетки будет произведен ход, необходимо ввести ее координаты."
		"\n2. Вирусы ставятся только в ближайшей клетке (по вертикали и по горизонтали)"
		", по диагоняли клетки занимать НЕЛЬЗЯ.\n3. Ходы идут поочердно, игра происходит против ИИ,"
		"в роли которой выступает сервер, к которому произошло подключение.\n4. Побеждает тот, кто захватит все поле,"
		" в случае невозможности это сделать, объявляется ничья\n------\nУдачи, игрок!!!\n";
	cout << "\n------------\nВ программе еще не реализован графический интерфейс и условия победы или проигрыша,"
		"а также отсоединения клиента или сервера. Все это я доделаю в билжайшее время, это первичынй вариант для проверки\n"
		"------------\n";
	cout << "Ожидание ответа от сервера...\n";
	recv(client.getConnect(), array, sizeof(array), NULL);
	Sleep(500);
	cout << array << endl;
	system("pause");
	system("cls");
	cout << "Для выбора размерности поля NxN, введите число (больше 3-х и менее 16-и):\n";
	while (1) {
		cin >> n;
		if (n > 3 && n <16)
			break;
		else
			cout << "Некорректный ввод, введите снова!!!\n";
	}
	draw.setN(n);
	client.sendingint(n);
	cnt = 0;
}

void logicmap() {
	for (int i = 0; i < *draw.getN() + 2; i++) {
		for (int j = 0; j < *draw.getN() + 2; j++) {
			if (i == 0 || j == 0 || j == *draw.getN() + 1 || i == *draw.getN() + 1)
				arr[i][j] = BORDER;
			else if (i == 1 && j == 1)
				arr[i][j] = ENEMY;
			else if (i == *draw.getN() && j == *draw.getN())
				arr[i][j] = FRIEND;
			else
				arr[i][j] = EMPTY;
		}
	}
}

void input() {
	cout << "Выберите координату x:\n";
	cin >> x_dop;
	*x = x_dop;
	cout << "Выберите координату y:\n";
	cin >> y_dop;
	*y = y_dop;
}

void logic() {
	cnt = 0;
	char ch11[] = "o";
	char ch21[] = "W";
	*ch1 = 'o';
	*ch2 = 'W';

	if (arr[*x][*y] == EMPTY && (arr[*x - 1][*y] == FRIEND || arr[*x][*y - 1] == FRIEND || arr[*x + 1][*y] == FRIEND || arr[*x][*y + 1] == FRIEND)) {
		arr[*x][*y] = FRIEND;
		draw.changeMass(x, y, ch1);
		client.sendingint(*x);
		Sleep(300);
		client.sendingint(*y);
		Sleep(300);
		send(client.getConnect(), ch11, sizeof(ch11), NULL); //Отправка сообщения клиенту
		//cout << "Отправка символа:" << ch11 << endl;
		//system("pause");
		Sleep(300);
		F = true;
	}

	else if (arr[*x][*y] == EMPTY && (arr[*x - 1][*y] == FRIENDCASTLE || arr[*x][*y - 1] == FRIENDCASTLE || arr[*x + 1][*y] == FRIENDCASTLE || arr[*x][*y + 1] == FRIENDCASTLE)) {
		arr[*x][*y] = FRIEND;
		draw.changeMass(x, y, ch1);
		client.sendingint(*x);
		Sleep(300);
		client.sendingint(*y);
		Sleep(300);
		send(client.getConnect(), ch11, sizeof(ch11), NULL);
		Sleep(300);
		F = true;
	}

	else if (arr[*x][*y] == BORDER ) {
		cout << "Нельзя заходить за границу...\nПоходите снова!!!\n";
		system("pause");
	}

	else if (arr[*x][*y] == FRIEND) {
		cout << "Нельзя вставать на свою клетку...\nПоходите снова!!!\n";
		system("pause");
	}

	else if (arr[*x][*y] == FRIENDCASTLE) {
		cout << "Нельзя вставать на свою клетку...\nПоходите снова!!!\n";
		system("pause");
	}

	else if (arr[*x][*y] == ENEMY && (arr[*x - 1][*y] == FRIEND || arr[*x][*y - 1] == FRIEND || arr[*x + 1][*y] == FRIEND || arr[*x][*y + 1] == FRIEND)) {
		arr[*x][*y] = FRIENDCASTLE;
		draw.changeMass(x, y, ch2);
		client.sendingint(*x);
		Sleep(300);
		client.sendingint(*y);
		Sleep(300);
		send(client.getConnect(), ch21, sizeof(ch21), NULL);
		Sleep(300);
		F = true;
	}

	else if (arr[*x][*y] == ENEMY && (arr[*x - 1][*y] == FRIENDCASTLE || arr[*x][*y - 1] == FRIENDCASTLE || arr[*x + 1][*y] == FRIENDCASTLE || arr[*x][*y + 1] == FRIENDCASTLE)) {
		arr[*x][*y] = FRIENDCASTLE;
		draw.changeMass(x, y, ch2);
		client.sendingint(*x);
		Sleep(300);
		client.sendingint(*y);
		Sleep(300);
		send(client.getConnect(), ch21, sizeof(ch21), NULL);
		Sleep(300);
		F = true;
	}

	else if (arr[*x][*y] == ENEMYCASTLE) {
		cout << "Нельзя заражать вражескую крепость...\nПоходите снова!!!\n";
		system("pause");
	}
	else {
		cout << "Союзные вирусы далеко...\nПоходите снова!!!\n";
		system("pause");
	}
}

void read() {
		client.receivingint(enemy_x);
		//cout << "Считало Enemy_x = " << *enemy_x << endl;
		client.receivingint(enemy_y);
		//cout << "Считало Enemy_y = " << *enemy_y << endl;
		recv(client.getConnect(), msg, sizeof(msg), NULL);

		if(arr[*enemy_x][*enemy_y] == EMPTY)
			arr[*enemy_x][*enemy_y] = ENEMY;
		else if (arr[*enemy_x][*enemy_y] == FRIEND)
			arr[*enemy_x][*enemy_y] = ENEMYCASTLE;

		draw.changeMass(enemy_x, enemy_y, msg);
		Sleep(500);
		//cout << "Считало Enemy_c = " << msg << endl;
		//system("pause");
}

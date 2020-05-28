#include <SFML/Graphics.hpp>
#include "ADDR.h"
#include "Draw.h"

Draw* draw;

int* x;			//Переменная для хранения хода клиента по оси X
int* y;			//Переменная для хранения хода клиента по оси Y
int var_x[4];
int var_y[4];
char* ch1 = new char;			//Переменная для добавления фишки на карту (обычная фишка)
char* ch2 = new char;			//Переменная для добавления фишки на карту (фишка-крепость)
char msg[5];							//Переменная для принятия сообщения от клиента
int* enemy_x = new int;		//Переменная для хранения хода ИИ по оси X
int* enemy_y = new int;		//Переменная для хранения хода ИИ по оси Y
char* enemy_c = new char;	//Переменная для хранения символа после хода ИИ
bool F_input = true;			//Флаг для проверки правильности ввода клиента, если false, то считываем данные от сервера
bool F_over = true;				//Условие выхода из игры

//bool F_click = true;

enum STATE { FRIEND = 0, FRIENDCASTLE, ENEMY, ENEMYCASTLE, EMPTY, BORDER };

STATE arr[20][20];	//Массив логической составляющей карты (Состояния клеток)

void start();				//Функция задания начальных параметров для установки соединения и игры
void logicmap();		//Логическая часть карты (подобие бэк-енда)
void input();				//Ввод пользователем
void logic();				//Логическая обработка введенных данных, а также логика игры
void read();				//Считывание данных от сервера
int cheking();
int cheking_empty_friend();
int cheking_empty_enemy();

int main(int argc, char* argv[]) {
	setlocale(LC_ALL, "ru");

	//ADDR::getAddr();
	//client = new ADDR;
	draw = new Draw;

	while (ADDR::getAddr().connection()) {}

	start();
	draw->setup();
	logicmap();

	sf::Texture texture;
	sf::Sprite sprite;
	sf::RenderWindow window(sf::VideoMode(96 * (*draw->getN() + 2), 96 * (*draw->getN() + 2)), "Game Viruses");

	texture.loadFromFile("1.png");
	sprite.setTexture(texture);

	while (window.isOpen() && F_over) {

		sf::Vector2i pixelPos = sf::Mouse::getPosition(window);

		sf::Event event;
		while (window.pollEvent(event))
		{
			// Пользователь нажал на «крестик» и хочет закрыть окно?
			if (event.type == sf::Event::Closed)
				// Тогда закрываем его
				window.close();
			//Проверк на нажатие клавиши мыши (ЛКМ)
			/*if (event.type == sf::Event::MouseButtonPressed) {
				if (event.key.code == sf::Mouse::Left) {
					cout << pixelPos.x << endl;
					system("pause");
					*x = pixelPos.x / 96 + 1;
					*y = pixelPos.y / 96 + 1;
					F_click = false;
				}
			}*/
		}

		//Ход игрока
		for (int i = 0; i < 3; i++) {
			if (cheking()) {
				//while (F_click) {
				while (F_input) {
					//cout << "Отправка данных серверу\n";
					draw->draw();

					for (int i = 0; i < *draw->getN() + 2; i++) {
						for (int j = 0; j < *draw->getN() + 2; j++) {
							if (arr[i][j] == BORDER)
								sprite.setTextureRect(sf::IntRect(1090, 674, 96, 96));//получили нужный нам прямоугольник
							else if (arr[i][j] == EMPTY)
								sprite.setTextureRect(sf::IntRect(1, 1, 96, 96));
							else if (arr[i][j] == ENEMY)
								sprite.setTextureRect(sf::IntRect(1145, 400, 96, 96));
							else if (arr[i][j] == FRIEND)
								sprite.setTextureRect(sf::IntRect(1152, 218, 96, 96));
							else if (arr[i][j] == FRIENDCASTLE)
								sprite.setTextureRect(sf::IntRect(1151, 130, 96, 96));
							else if (arr[i][j] == ENEMYCASTLE)
								sprite.setTextureRect(sf::IntRect(1146, 305, 96, 96));

							sprite.setPosition(j * 96, i * 96);
							window.draw(sprite);
						}
					}
					// Отрисовка окна
					window.display();

					input();
					//while (F_click) {
					//}
					//F_click = true;

					logic();
				}
				//F_click = true;
			}
			else
				break;

			Sleep(500);
			F_input = true;
		}

		//Ход ИИ
		for (int i = 0; i < 3; i++) {
			if (cheking()) {
				draw->draw();

				for (int i = 0; i < *draw->getN() + 2; i++) {
					for (int j = 0; j < *draw->getN() + 2; j++) {
						if (arr[i][j] == BORDER)
							sprite.setTextureRect(sf::IntRect(1090, 674, 96, 96));//получили нужный нам прямоугольник (спрайт)
						else if (arr[i][j] == EMPTY)
							sprite.setTextureRect(sf::IntRect(1, 1, 96, 96));
						else if (arr[i][j] == ENEMY)
							sprite.setTextureRect(sf::IntRect(1145, 400, 96, 96));
						else if (arr[i][j] == FRIEND)
							sprite.setTextureRect(sf::IntRect(1152, 218, 96, 96));
						else if (arr[i][j] == FRIENDCASTLE)
							sprite.setTextureRect(sf::IntRect(1151, 130, 96, 96));
						else if (arr[i][j] == ENEMYCASTLE)
							sprite.setTextureRect(sf::IntRect(1146, 305, 96, 96));

						sprite.setPosition(j * 96, i * 96);
						window.draw(sprite);
					}
				}
				// Отрисовка окна
				window.display();

				read();
			}
			else
				break;
			Sleep(500);
		}
	}

	//Отправка о завершении соединения
	/*
	Sleep(500);
	char dis[] = "Disconnect";
	send(client->getConnect(), dis, sizeof(dis), NULL);
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

	delete[] x;
	delete[] y;
	delete ch1;
	delete ch2;
	delete enemy_x;
	delete enemy_y;
	delete enemy_c;

	//delete client;
	delete draw;

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

	recv(ADDR::getAddr().getConnect(), array, sizeof(array), NULL);
	Sleep(500);

	cout << array << endl;
	system("pause");
	system("cls");

	cout << "Для выбора размерности поля NxN, введите число (больше 3-х и менее 16-и):\n";

	while (1) {

		while (!(cin >> n)) {
			cin.clear();
			cin.ignore(32767, '\n');
			cout << "Некорректный ввод!!! Повторите\n";
		}

		if (n > 3 && n < 16)
			break;
		else
			cout << "Некорректный ввод, введите снова!!!\n";
	}

	draw->setN(n);
	ADDR::getAddr().sendingint(n);

	x = new int[*draw->getN() * 3];
	y = new int[*draw->getN() * 3];
}

void logicmap() {
	for (int i = 0; i < *draw->getN() + 2; i++) {
		for (int j = 0; j < *draw->getN() + 2; j++) {
			if (i == 0 || j == 0 || j == *draw->getN() + 1 || i == *draw->getN() + 1)
				arr[i][j] = BORDER;
			else if (i == 1 && j == 1)
				arr[i][j] = ENEMY;
			else if (i == *draw->getN() && j == *draw->getN())
				arr[i][j] = FRIEND;
			else
				arr[i][j] = EMPTY;
		}
	}
}

void input() {
	cout << "Выберите координату x:\n";

	while (!(cin >> *x)) {
		cin.clear();
		cin.ignore(32767, '\n');
		cout << "Некорректный ввод!!! Повторите\n";
	}

	cout << "Выберите координату y:\n";

	while (!(cin >> *y)) {
		cin.clear();
		cin.ignore(32767, '\n');
		cout << "Некорректный ввод!!! Повторите\n";
	}
}

void logic() {
	char ch11[] = "o";
	char ch21[] = "O";
	*ch1 = 'o';
	*ch2 = 'O';

	if (arr[*x][*y] == EMPTY && (arr[*x - 1][*y] == FRIEND || arr[*x][*y - 1] == FRIEND || arr[*x + 1][*y] == FRIEND || arr[*x][*y + 1] == FRIEND)) {
		arr[*x][*y] = FRIEND;
		draw->changeMass(x, y, ch1);
		ADDR::getAddr().sendingint(*x);
		Sleep(300);
		ADDR::getAddr().sendingint(*y);
		Sleep(300);
		send(ADDR::getAddr().getConnect(), ch11, sizeof(ch11), NULL);
		Sleep(300);
		F_input = false;
	}

	else if (arr[*x][*y] == EMPTY && (arr[*x - 1][*y] == FRIENDCASTLE || arr[*x][*y - 1] == FRIENDCASTLE || arr[*x + 1][*y] == FRIENDCASTLE || arr[*x][*y + 1] == FRIENDCASTLE)) {
		arr[*x][*y] = FRIEND;
		draw->changeMass(x, y, ch1);
		ADDR::getAddr().sendingint(*x);
		Sleep(300);
		ADDR::getAddr().sendingint(*y);
		Sleep(300);
		send(ADDR::getAddr().getConnect(), ch11, sizeof(ch11), NULL);
		Sleep(300);
		F_input = false;
	}

	else if (arr[*x][*y] == BORDER) {
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
		draw->changeMass(x, y, ch2);
		ADDR::getAddr().sendingint(*x);
		Sleep(300);
		ADDR::getAddr().sendingint(*y);
		Sleep(300);
		send(ADDR::getAddr().getConnect(), ch21, sizeof(ch21), NULL);
		Sleep(300);
		F_input = false;
	}

	else if (arr[*x][*y] == ENEMY && (arr[*x - 1][*y] == FRIENDCASTLE || arr[*x][*y - 1] == FRIENDCASTLE || arr[*x + 1][*y] == FRIENDCASTLE || arr[*x][*y + 1] == FRIENDCASTLE)) {
		arr[*x][*y] = FRIENDCASTLE;
		draw->changeMass(x, y, ch2);
		ADDR::getAddr().sendingint(*x);
		Sleep(300);
		ADDR::getAddr().sendingint(*y);
		Sleep(300);
		send(ADDR::getAddr().getConnect(), ch21, sizeof(ch21), NULL);
		Sleep(300);
		F_input = false;
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
	ADDR::getAddr().receivingint(enemy_x);
	ADDR::getAddr().receivingint(enemy_y);
	recv(ADDR::getAddr().getConnect(), msg, sizeof(msg), NULL);

	if (arr[*enemy_x][*enemy_y] == EMPTY)
		arr[*enemy_x][*enemy_y] = ENEMY;
	else if (arr[*enemy_x][*enemy_y] == FRIEND)
		arr[*enemy_x][*enemy_y] = ENEMYCASTLE;

	draw->changeMass(enemy_x, enemy_y, msg);
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
	bool Flag = true;

	for (int i = 1; (i <= *draw->getN()) && Flag == true; i++) {
		for (int j = 1; (j <= *draw->getN()) && Flag == true; j++) {
			if (arr[i][j] == FRIEND || arr[i][j] == FRIENDCASTLE)
				F_over = false;
			else {
				F_over = true;
				Flag = false;
			}
		}
	}

	if (Flag == true) {
		cout << "Победа!!!\n";
		return 0;
	}

	if (!cheking_empty_enemy()) {
		cout << "Победа!!!:)\n";
		F_over = false;
		return 0;
	}

	//Проверка на поражение
	Flag = true;

	for (int i = 1; (i <= *draw->getN()) && Flag == true; i++) {
		for (int j = 1; (j <= *draw->getN()) && Flag == true; j++) {
			if (arr[i][j] == ENEMY || arr[i][j] == ENEMYCASTLE)
				F_over = false;
			else {
				F_over = true;
				Flag = false;
			}
		}
	}

	if (Flag == true) {
		cout << "Поражение:(\n";
		return 0;
	}

	if (!cheking_empty_friend()) {
		cout << "Победа!!!:)\n";
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

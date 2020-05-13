#include <iostream>
#include "ADDRServer.h"
#include "DrawServer.h"

ADDRServer server;
DrawServer draw;

void start();

int main(int argc, char* argv[]) {
	setlocale(LC_ALL, "ru");

	while (server.connection()) {}

	start();
	draw.setup();

	while (1) {
		draw.draw();
		for (int i = 0; i < *draw.getN() + 2; i++) {
			server.sending(draw.getMass(i));
		}
		//input();
		//logic();
		Sleep(1000);
	}

	system("pause");
	//delete server;
	return 0;
}

void start() {
	while (server.receivingint(draw.getN())){
		cout << "Ожидание клиента...\n";
	}
}

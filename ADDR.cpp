#include "ADDR.h"

ADDR::ADDR() {
	//Проверка на загрузку библиотеки
	if (WSAStartup(DLLVersion, &wsadata) != 0) {
		cout << "Error 0...\nExit\n";
		exit(1);
	}

	addr.sin_addr.s_addr = inet_addr(IP); //Хранение IP-адреса (в моем случае это мой IP-адрес, я и являюсь сервером)
	addr.sin_port = htons(PORT); //Порт для обеспечения передачи/приема данных этой программы
	addr.sin_family = AF_INET; //Указание семейства протоколов (IP)

	newConnection = socket(AF_INET, SOCK_STREAM, NULL);
}

ADDR::~ADDR() {
}

//ADDR* ADDR::p_addr = 0;

int ADDR::connection() {
	//Присоединение к серверу
	if (connect(newConnection, (SOCKADDR*)&addr, sizeofaddr) != 0) {
		cout << "Error 1...\nConnecting again!\n";
		return(1);
	}
	else {
		cout << "Connected to Server!!!\n";
		return 0;
	}
}

void ADDR::receivingint(int* a) {
	char str[10];
	recv(newConnection, str, sizeof(str), NULL);
	*a = atoi(str);
}

void ADDR::sendingint(int a) {
	char buf[5];
	itoa(a, buf, 10);
	//cout << "------\nSending n = " << buf << " = " << a << endl;
	//system("pause");
	send(newConnection, buf, sizeof(buf), NULL);
}

SOCKET ADDR::getConnect() {
	return newConnection;
}

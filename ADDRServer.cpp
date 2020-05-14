#include "ADDRServer.h"

ADDRServer::ADDRServer() {
	//Проверка на загрузку библиотеки
	if (WSAStartup(DLLVersion, &wsadata) != 0) {
		cout << "Error 0...\nExit\n";
		exit(1);
	}

	addr.sin_addr.s_addr = inet_addr(IP); //Хранение IP-адреса (в моем случае это мой IP-адрес, я и являюсь сервером)
	addr.sin_port = htons(PORT); //Порт для обеспечения передачи/приема данных этой программы
	addr.sin_family = AF_INET; //Указание семейства протоколов (IP)

	sListen = socket(AF_INET, SOCK_STREAM, NULL);
	bind(sListen, (SOCKADDR*)&addr, sizeofaddr); //Привязка адреса к сокету
	listen(sListen, SOMAXCONN); //Прослушивание порта в ожидании подключение соединения со стороны клиента

}

int ADDRServer::connection() {
	newConnection = accept(sListen, (SOCKADDR*)&addr, &sizeofaddr); //Вовзращает указатель на новый сокет для общения с клиентом

	if (newConnection == 0) {
		cout << "Error 2...\n";
		return 1;
	}
	else {
		cout << "Client connected!!!\n";
		system("pause");
		return 0;
	}
}

void ADDRServer::sending(char* str) {
	char A[20];
	strcpy_s(A, str);
	system("pause");
	send(newConnection, A, sizeof(A), NULL); //Отправка сообщения клиенту
}

int ADDRServer::receivingint(int* a) {
	char str[10];
	if (recv(newConnection, str, sizeof(str), NULL)) {
		*a = atoi(str);
		//cout << "------\nReceiving n = " << *a << " = " << str << endl;
		//system("pause");
		return 0;
	}
	else
		return 1;
}

int ADDRServer::receiving(char* a) {
	if (recv(newConnection, a, sizeof(a), NULL)) {
		//cout << "------\nReceiving n = " << *a << " = " << *a << endl;
		//system("pause");
		return 0;
	}
	else
		return 1;
}

SOCKET ADDRServer::getConnect() {
	return newConnection;
}

void ADDRServer::sendingint(int* a) {
	char buf[5];
	itoa(*a, buf, 10);
	//cout << "------\nSending n = " << buf << " = " << a << endl;
	send(newConnection, buf, sizeof(buf), NULL);
}

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

int ADDR::connection() {
	//Присоединение к серверу
	if (connect(newConnection, (SOCKADDR*)&addr, sizeofaddr) != 0) {
		cout << "Error 1...\nConnecting again!\n";
		return(1);
	}
	else {
		cout << "Connected to Server!!!\n";
		system("pause");
		return 0;
	}

	//recv(connection, msg, sizeof(msg), NULL);
	//send(connection, msg1, sizeof(msg1), NULL);
}

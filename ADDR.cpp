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
}

char* ADDR::receiving() {
	char arr[7];
	recv(newConnection, arr, sizeof(arr), NULL);
	cout <<"Arr = "<< arr << endl;
	return arr;
}

void ADDR::sendingint(int a) {
	char buf[5];
	itoa(a, buf, 10);
	cout << "------Sending n = " << buf << " = " << a << endl;
	system("pause");
	send(newConnection, buf, sizeof(buf), NULL);
}

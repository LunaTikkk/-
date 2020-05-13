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

	sListen = socket(AF_INET, SOCK_STREAM, NULL);
	bind(sListen, (SOCKADDR*)&addr, sizeofaddr); //Привязка адреса к сокету
	listen(sListen, SOMAXCONN); //Прослушивание порта в ожидании подключение соединения со стороны клиента

}

int ADDR::connection() {
	newConnection = accept(sListen, (SOCKADDR*)&addr, &sizeofaddr); //Вовзращает указатель на новый сокет для общения с клиентом

	//char msg[256] = "Hello Client!!! I'am server!\n";
	if (newConnection == 0) {
		cout << "Error 2...\n";
		return 1;
	}
	else {
		cout << "Client connected!!!\n";
		system("pause");
		return 0;
	}
	//recv(newConnection, msg, sizeof(msg), NULL);
	//send(newConnection, msg, sizeof(msg), NULL); //Отправка сообщения клиенту
}

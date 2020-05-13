#pragma once
#pragma comment(lib, "ws2_32.lib") //Доступ к сетневым функциям
#include <winsock2.h> //Библиотека для работы с сетью
#include <iostream>

#pragma warning (disable: 4996) //Убираение ошибки C4996, связанную с inet_addr

#define IP "192.168.0.5"
#define PORT 7777

using namespace std;

class ADDR {
private:
	//WSAstartup
	WSAData wsadata; //Структура WSAData
	WORD DLLVersion = MAKEWORD(2, 1); //Запрашиваемая версия библиотеки

	SOCKADDR_IN addr; //Структура для хранения адреса
	int sizeofaddr = sizeof(addr); //Переменная для хранения размера addr

	SOCKET sListen; //Создание сокета для прослушивания порта, на который будут приходить данные
	SOCKET newConnection; //Сокет для удержания соединения с клиентом
public:
	ADDR();
	int connection();
};

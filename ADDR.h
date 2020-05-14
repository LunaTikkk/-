#pragma once
#pragma comment(lib, "ws2_32.lib") //Доступ к сетевым функциям
#include <winsock2.h> //Библиотека для работы с сетью
#include <iostream>

#pragma warning (disable: 4996) //Игнорирование ошибки C4996, связанную с inet_addr

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
	SOCKET newConnection; //Создание сокета для соединения с сервером
public:
	ADDR();
	~ADDR();
	int connection();
	int receiving(char *a);
	int receivingint(int* a);
	void sendingint(int a);
	void sending(char a);
	SOCKET getConnect();
};

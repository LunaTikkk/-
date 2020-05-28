#pragma once
#pragma comment(lib, "ws2_32.lib")		//Доступ к сетевым функциям
#include <winsock2.h>					//Библиотека для работы с сетью
#include <iostream>

#pragma warning (disable: 4996)			//Игнорирование ошибки C4996, связанную с inet_addr

#define IP "192.168.0.5"				//IP-адрес сервера
#define PORT 7777						//Порт для приема данных

using namespace std;

class ADDRServer {
private:
	//WSAstartup
	WSAData wsadata;					//Структура WSAData
	WORD DLLVersion = MAKEWORD(2, 1);	//Запрашиваемая версия библиотеки

	SOCKADDR_IN addr;					//Структура для хранения адреса
	int sizeofaddr = sizeof(addr);		//Переменная для хранения размера addr

	SOCKET sListen;						//Создание сокета для прослушивания порта, на который будут приходить данные
	SOCKET newConnection;				//Сокет для удержания соединения с клиентом
public:
	ADDRServer();						//Конструктор без параметров
	~ADDRServer();						//Деструктор
	int connection();					//Установка соединения
	int receivingint(int* a);			//Считывание целого числа от клиента
	SOCKET getConnect();				//Метод get для получения сокета
	void sendingint(int* a);			//Отправка целого числа клиенту
};

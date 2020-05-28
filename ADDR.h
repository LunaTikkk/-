#pragma once
#pragma comment(lib, "ws2_32.lib")	//Доступ к сетевым функциям
#include <winsock2.h>				//Библиотека для работы с сетью
#include <iostream>

#pragma warning (disable: 4996)		//Игнорирование ошибки C4996, связанную с inet_addr

#define IP "192.168.0.5"			//IP-адрес клиента
#define PORT 7777					//Номер порта для отправки/принятия пакетов

using namespace std;

class ADDR {
private:
	//WSAstartup
	WSAData wsadata;					//Структура WSAData
	WORD DLLVersion = MAKEWORD(2, 1);	//Запрашиваемая версия библиотеки

	SOCKADDR_IN addr;					//Структура для хранения адреса
	int sizeofaddr = sizeof(addr);		//Переменная для хранения размера addr
	SOCKET newConnection;				//Создание сокета для соединения с сервером

	//static ADDR* p_addr;

	ADDR();						//Конструктор без параметров
	ADDR(const ADDR&);
	ADDR& operator=(ADDR&);
public:
	~ADDR();					//Деструктор
	int connection();			//Установка соединения
	void receivingint(int* a);	//Считывание числа от сервера
	void sendingint(int a);		//Отправка числа серверу
	SOCKET getConnect();		//Метод get, для получения сокета

	static ADDR& getAddr() {
		static ADDR p_addr;
		return p_addr;
	}
};

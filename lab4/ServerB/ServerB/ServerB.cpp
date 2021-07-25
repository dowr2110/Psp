// ServerB.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
#include <iostream>
#include <string>
#include <time.h>
#include "Winsock2.h"
#include <stdio.h>
#include <tchar.h>
#include <SDKDDKVer.h>
using namespace std;

#pragma warning(disable:4996)
#pragma comment(lib, "WS2_32.lib")

SOCKET sS;

string  GetErrorMsgText(int code)
{
	switch (code)
	{
	case WSAEINTR: return "Работа функции прервана ";
	case WSAEACCES: return "Разрешение отвергнуто";
	case WSAEFAULT:	return "Ошибочный адрес";
	case WSAEINVAL:	return "Ошибка в аргументе";
	case WSAEMFILE: return "Слишком много файлов открыто";
	case WSAEWOULDBLOCK: return "Ресурс временно недоступен";
	case WSAEINPROGRESS: return "Операция в процессе развития";
	case WSAEALREADY: return "Операция уже выполняется";
	case WSAENOTSOCK: return "Сокет задан неправильно";
	case WSAEDESTADDRREQ: return "Требуется адрес расположения";
	case WSAEMSGSIZE: return "Сообщение слишком длинное";
	case WSAEPROTOTYPE: return "Неправильный тип протокола для сокета";
	case WSAENOPROTOOPT: return "Ошибка в опции протокола";
	case WSAEPROTONOSUPPORT: return "Протокол не поддерживается";
	case WSAESOCKTNOSUPPORT: return "Тип сокета не поддерживается";
	case WSAEOPNOTSUPP:	return "Операция не поддерживается";
	case WSAEPFNOSUPPORT: return "Тип протоколов не поддерживается";
	case WSAEAFNOSUPPORT: return "Тип адресов не поддерживается протоколом";
	case WSAEADDRINUSE:	return "Адрес уже используется";
	case WSAEADDRNOTAVAIL: return "Запрошенный адрес не может быть использован";
	case WSAENETDOWN: return "Сеть отключена";
	case WSAENETUNREACH: return "Сеть не достижима";
	case WSAENETRESET: return "Сеть разорвала соединение";
	case WSAECONNABORTED: return "Программный отказ связи";
	case WSAECONNRESET:	return "Связь восстановлена";
	case WSAENOBUFS: return "Не хватает памяти для буферов";
	case WSAEISCONN: return "Сокет уже подключен";
	case WSAENOTCONN: return "Сокет не подключен";
	case WSAESHUTDOWN: return "Нельзя выполнить send : сокет завершил работу";
	case WSAETIMEDOUT: return "Закончился отведенный интервал времени";
	case WSAECONNREFUSED: return "Соединение отклонено";
	case WSAEHOSTDOWN: return "Хост в неработоспособном состоянии";
	case WSAEHOSTUNREACH: return "Нет маршрута для хоста";
	case WSAEPROCLIM: return "Слишком много процессов";
	case WSASYSNOTREADY: return "Сеть не доступна";
	case WSAVERNOTSUPPORTED: return "Данная версия недоступна";
	case WSANOTINITIALISED:	return "Не выполнена инициализация WS2_32.DLL";
	case WSAEDISCON: return "Выполняется отключение";
	case WSATYPE_NOT_FOUND: return "Класс не найден";
	case WSAHOST_NOT_FOUND:	return "Хост не найден";
	case WSATRY_AGAIN: return "Неавторизированный хост не найден";
	case WSANO_RECOVERY: return "Неопределенная  ошибка";
	case WSANO_DATA: return "Нет записи запрошенного типа";
	case WSA_INVALID_HANDLE: return "Указанный дескриптор события  с ошибкой";
	case WSA_INVALID_PARAMETER: return "Один или более параметров с ошибкой";
	case WSA_IO_INCOMPLETE:	return "Объект ввода - вывода не в сигнальном состоянии";
	case WSA_IO_PENDING: return "Операция завершится позже";
	case WSA_NOT_ENOUGH_MEMORY: return "Не достаточно памяти";
	case WSA_OPERATION_ABORTED: return "Операция отвергнута";
	case WSASYSCALLFAILURE: return "Аварийное завершение системного вызова";
	default: return "**ERROR**";
	};
};

string  SetErrorMsgText(string msgText, int code)
{
	return  msgText + GetErrorMsgText(code);
};


bool GetRequestFromClient(char *name, short port, sockaddr* from, int* flen)
{

	memset(from, 0, sizeof(flen));

	if ((sS = socket(AF_INET, SOCK_DGRAM, NULL)) == INVALID_SOCKET)//для сетвеого протокола ipv4, датаграмнный сокет
	{
		throw  SetErrorMsgText("socket:", WSAGetLastError());
	}

	SOCKADDR_IN serv;
	serv.sin_family = AF_INET;
	serv.sin_port = htons(port);
	serv.sin_addr.s_addr = INADDR_ANY;

	if (bind(sS, (LPSOCKADDR)&serv, sizeof(serv)) == SOCKET_ERROR)
	{
		throw  SetErrorMsgText("bind:", WSAGetLastError());
	}

	char nameServer[50];
	if ((recvfrom(sS, nameServer, sizeof(nameServer), NULL, from, flen)) == SOCKET_ERROR)
	{
		if (WSAGetLastError() == WSAETIMEDOUT)
		{
			return false;
		}
		else
		{
			throw  SetErrorMsgText("recv:", WSAGetLastError());
		}
	}

	if (!strcmp(nameServer, name))
	{
		cout << endl << "Позывной сервера совпадает.";
		return true;
	}
	else
	{
		cout << endl << "Позывной сервера не совпадает.";
		return false;
	}
}
bool PutAnswerToClient(char* name, sockaddr* to, int* lto)
{
	if ((sendto(sS, name, strlen(name) + 1, NULL, to, *lto)) == SOCKET_ERROR)
	{
		throw  SetErrorMsgText("send:", WSAGetLastError());
	}

	return true;
}

int main()
{
	setlocale(LC_CTYPE, "rus");

	WSADATA wsaData;

	try
	{
		if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0)
		{
			throw  SetErrorMsgText("Startup:", WSAGetLastError());
		}

		char name[] = "Hello";
		char soob[] = "**Eror name**";
		for (;;)
		{
			SOCKADDR_IN clnt;
			int lc = sizeof(clnt);

			if (GetRequestFromClient(name, 2000, (sockaddr*)&clnt, &lc))
			{
				cout << endl << PutAnswerToClient(name, (sockaddr*)&clnt, &lc);
			}
			else
			{
				cout << endl << PutAnswerToClient(soob, (sockaddr*)&clnt, &lc);
			}

			SOCKADDR_IN* addr = (SOCKADDR_IN*)&clnt;

			cout << endl << "Порт клиента: " << addr->sin_port;
			cout << endl << "IP-адрес клиента: " << inet_ntoa(addr->sin_addr);
			cout << endl;

			if (closesocket(sS) == SOCKET_ERROR)
			{
				throw  SetErrorMsgText("closesocket:", WSAGetLastError());
			}
		}

		if (WSACleanup() == SOCKET_ERROR)
		{
			throw  SetErrorMsgText("Cleanup:", WSAGetLastError());
		}
	}
	catch (string errorMsgText)
	{
		cout << endl << errorMsgText;
	}
}

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.

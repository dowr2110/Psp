// ServerB.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <tchar.h>
#include <iostream>
#include <string>
#include <iostream>
#include "Winsock2.h"
#include <string>
#include <time.h>
#include<cstdlib>
// заголовок WS2_32.dll
#pragma comment(lib, "WS2_32.lib") // экспорт WS2_32.dll

using namespace std;

string GetErrorMsgText(int code)
{
	string msgText;
	switch (code)
	{
	case WSAEINTR: msgText = "Работа функции прервана"; break;
	case WSAEACCES: msgText = "Разрешение отвергнуто"; break;
	case WSAEFAULT: msgText = "Разрешение отвергнуто"; break;
	case WSAEINVAL: msgText = "Ошибка в аргументе"; break;
	case WSAEMFILE: msgText = "Слишком много файлов открыто"; break;
	case WSAEWOULDBLOCK: msgText = "Ресурс временно недоступен"; break;
	case WSAEINPROGRESS: msgText = "Операция в процессе развития"; break;
	case WSAEALREADY: msgText = "Операция уже выполняется"; break;
	case WSAENOTSOCK: msgText = "Сокет задан неправильно"; break;
	case WSAEDESTADDRREQ: msgText = "Требуется адрес расположения"; break;
	case WSAEMSGSIZE: msgText = "Сообщение слишком длинное"; break;
	case WSAEPROTOTYPE: msgText = "Неправильный тип протокола для сокета"; break;
	case WSAENOPROTOOPT: msgText = "Ошибка в опции протокола"; break;
	case WSAEPROTONOSUPPORT: msgText = "Протокол не поддерживается"; break;
	case WSAESOCKTNOSUPPORT: msgText = "Тип сокета не поддерживается"; break;
	case WSAEOPNOTSUPP: msgText = "Операция не поддерживается"; break;
	case WSAEPFNOSUPPORT: msgText = "Тип протоколов не поддерживается"; break;
	case WSAEAFNOSUPPORT: msgText = "Тип адресов не поддерживается протоколом"; break;
	case WSAEADDRINUSE: msgText = "Адрес уже используется"; break;
	case WSAEADDRNOTAVAIL: msgText = "Запрошенный адрес не может быть использован"; break;
	case WSAENETDOWN: msgText = "Сеть отключена"; break;
	case WSAENETUNREACH: msgText = "Сеть не достижима"; break;
	case WSAENETRESET: msgText = "Сеть разорвала соединение"; break;
	case WSAECONNABORTED: msgText = "Программный отказ связи"; break;
	case WSAECONNRESET: msgText = "Связь восстановлена"; break;
	case WSAENOBUFS: msgText = "Не хватает памяти для буферов"; break;
	case WSAEISCONN: msgText = "Сокет уже подключен"; break;
	case WSAENOTCONN: msgText = "Сокет не подключен"; break;
	case WSAESHUTDOWN: msgText = "Нельзя выполнить send : сокет завершил работу"; break;
	case WSAETIMEDOUT: msgText = "Закончился отведенный интервал  времени"; break;
	case WSAECONNREFUSED: msgText = "Соединение отклонено"; break;
	case WSAEHOSTDOWN: msgText = "Хост в неработоспособном состоянии"; break;
	case WSAEHOSTUNREACH: msgText = "Нет маршрута для хоста"; break;
	case WSAEPROCLIM: msgText = "Слишком много процессов"; break;
	case WSASYSNOTREADY: msgText = "Сеть не доступна "; break;
	case WSAVERNOTSUPPORTED: msgText = "Данная версия недоступна"; break;
	case WSANOTINITIALISED: msgText = "Не выполнена инициализация WS2_32.DLL"; break;
	case WSAEDISCON: msgText = "Выполняется отключение"; break;
	case WSATYPE_NOT_FOUND: msgText = "Класс не найден"; break;
	case WSAHOST_NOT_FOUND: msgText = "Хост не найден"; break;
	case WSATRY_AGAIN: msgText = "Неавторизированный хост не найден"; break;
	case WSANO_RECOVERY: msgText = "Неопределенная ошибка"; break;
	case WSANO_DATA: msgText = "Нет записи запрошенного типа"; break;
	case WSA_INVALID_HANDLE: msgText = "Указанный дескриптор события с ошибкой"; break;
	case WSA_INVALID_PARAMETER: msgText = "Один или более параметров с ошибкой"; break;
	case WSA_IO_INCOMPLETE: msgText = "Объект ввода - вывода не в сигнальном состоянии"; break;
	case WSA_IO_PENDING: msgText = "Операция завершится позже"; break;
	case WSA_NOT_ENOUGH_MEMORY: msgText = "Не достаточно памяти"; break;
	case WSA_OPERATION_ABORTED: msgText = "Операция отвергнута"; break;
	case WSASYSCALLFAILURE: msgText = "Аварийное завершение системного вызова"; break;
	default: msgText = "***ERROR***"; break;
		break;
	}
	return msgText;
}

string SetErrorMsgText(string msgText, int code)
{
	return msgText + GetErrorMsgText(code);
}


SOCKET sS;

bool GetRequestFromClient(char* name, short port, sockaddr* from, int* flen)
{

	cout << "Wait message..." << endl;
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

		if (recvfrom(sS, nameServer, sizeof(nameServer), NULL, from, flen) == SOCKET_ERROR)
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
		if (strcmp(nameServer, name) == 0)
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
	if (sendto(sS, name, strlen(name) + 1, NULL, to, *lto) == SOCKET_ERROR)
	{
		throw SetErrorMsgText("sendto: ", WSAGetLastError());
	}
	return true;
}


int main()
{
	setlocale(LC_CTYPE, "rus");

	WSADATA wsaData;
	SOCKET sockServer;
	hostent* server;
	hostent* client;

	
	try
	{
		if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0)
		{
			throw SetErrorMsgText("Startup: ", WSAGetLastError());
		}

		char name[] = "hello";
		char soob[] = "Access";
		char hostname[32];
		cout << "Поиск сервера..." << endl;
		
		for (;;)
		{

			SOCKADDR_IN clnt;
			memset(&clnt, 0, sizeof(clnt));
			int lc = sizeof(clnt);
			gethostname(hostname, sizeof(hostname));
			cout << " My Server name: " << hostname << " ----start" << endl;
			
				if (GetRequestFromClient(name, 2000, (sockaddr*)&clnt, &lc))
				{
					cout << "Succes" << endl;
					cout << "Client: " << gethostbyaddr((char*)&clnt.sin_addr, sizeof(clnt.sin_addr), AF_INET)->h_name << endl;
					cout << "ClientIP: " << inet_ntoa(clnt.sin_addr) << endl;//sonky strocka barlanmadyk
					PutAnswerToClient(soob, (sockaddr*)&clnt, &lc);
				}
				else
				{
					cout << "Error" << endl;
				}
				//SOCKADDR_IN* addr = (SOCKADDR_IN*)&clnt;

				//cout << endl << "Порт клиента: " << addr->sin_port;
				//cout << endl << "IP-адрес клиента: " << inet_ntoa(addr->sin_addr);
				cout << endl<<endl;
			
			if (closesocket(sS) == SOCKET_ERROR)
			{
				throw SetErrorMsgText("closesocket: ", WSAGetLastError());
			}
		}
		if (WSACleanup() == SOCKET_ERROR)
		{
			throw SetErrorMsgText("Cleanup: ", WSAGetLastError());
		}
	}
	catch (string errorMsgText)
	{
		cout << "WSAGetLastError: " << errorMsgText << endl;
	}
	system("pause");
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

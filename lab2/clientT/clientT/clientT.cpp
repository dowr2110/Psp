// clientT.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include "Winsock2.h"
#include <string>
#pragma warning(disable:4996)
// заголовок WS2_32.dll
#pragma comment(lib, "WS2_32.lib") // экспорт WS2_32.dll
using namespace std;

#include "Winsock2.h"
#pragma comment(lib, "WS2_32.lib")


//...................................................................

string GetErrorMsgText(int code) // cформировать текст ошибки
{
	string msgText;
	switch (code) // проверка кода возврата 1`
	{
	case WSAEINTR:
		msgText = "Работа функции прервана";
		break;
	case WSAEACCES:
		msgText = "Разрешение отвергнуто";
		break;
	case WSAEFAULT:
		msgText = "Ошибочный адрес";
		break;
	case WSAEINVAL:
		msgText = "Ошибка в аргументе";
		break;
	case WSAEMFILE:
		msgText = "Открыто слишком много файлов";
		break;
	case WSAEWOULDBLOCK:
		msgText = "Ресурс временно недоступен";
		break;
	case WSAEINPROGRESS:
		msgText = "Операция в процессе развития";
		break;
	case WSAEALREADY:
		msgText = "Операция уже выполняется";
		break;
	case WSAENOTSOCK:
		msgText = "Сокет задан неправильно";
		break;
	case WSAEDESTADDRREQ:
		msgText = "Требуется адрес расположения";
		break;
	case WSAEMSGSIZE:
		msgText = "Сообщение слишком длинное";
		break;
	case WSAEPROTOTYPE:
		msgText = "Неправильный тип протокола для сокета";
		break;
	case WSAENOPROTOOPT:
		msgText = "Ошибка в опции протокола";
		break;
	case WSAEPROTONOSUPPORT:
		msgText = "Протокол не поддерживается";
		break;
	case WSAESOCKTNOSUPPORT:
		msgText = "Тип сокета не поддерживается";
		break;
	case WSAEOPNOTSUPP:
		msgText = "Операция не поддерживается";
		break;
	case WSAEPFNOSUPPORT:
		msgText = "Тип протоколов не поддерживается";
		break;
	case WSAEAFNOSUPPORT:
		msgText = "Тип адресов не поддерживается протоколом";
		break;
	case WSAEADDRINUSE:
		msgText = "Адрес уже используется";
		break;
	case WSAEADDRNOTAVAIL:
		msgText = "Запрошенный адрес не может быть использован";
		break;
	case WSAENETDOWN:
		msgText = "Сеть отключена";
		break;
	case WSAENETUNREACH:
		msgText = "Сеть не достижима";
		break;
	case WSAENETRESET:
		msgText = "Сеть разорвала соединение";
		break;
	case WSAECONNABORTED:
		msgText = "Программный отказ связи";
		break;
	case WSAECONNRESET:
		msgText = "Связь не восстановлена";
		break;
	case WSAENOBUFS:
		msgText = "Не хватает памяти для буферов";
		break;
	case WSAEISCONN:
		msgText = "Сокет уже подключен";
		break;
	case WSAENOTCONN:
		msgText = "Сокет не подключен";
		break;
	case WSAESHUTDOWN:
		msgText = "Нельзя выполнить send: сокет завершил работу";
		break;
	case WSAETIMEDOUT:
		msgText = "Закончился отведенный интервал времени";
		break;
	case WSAECONNREFUSED:
		msgText = "Соединение отклонено";
		break;
	case WSAEHOSTDOWN:
		msgText = "Хост в неработоспособном состоянии";
		break;
	case WSAEHOSTUNREACH:
		msgText = "Нет маршрута для хоста";
		break;
	case WSAEPROCLIM:
		msgText = "Слишком много процессов";
		break;
	case WSASYSNOTREADY:
		msgText = "Сеть не доступна";
		break;
	case WSAVERNOTSUPPORTED:
		msgText = "Данная версия недоступна";
		break;
	case WSANOTINITIALISED:
		msgText = "Не выполнена инициализация WS2_32.dll";
		break;
	case WSAEDISCON:
		msgText = "Выполняется отключение";
		break;
	case WSATYPE_NOT_FOUND:
		msgText = "Класс не найден";
		break;
	case WSAHOST_NOT_FOUND:
		msgText = "Хост не найден";
		break;
	case WSATRY_AGAIN:
		msgText = "Неавторизованный хост не найден";
		break;
	case WSANO_RECOVERY:
		msgText = "Неопределенная ошибка";
		break;
	case WSANO_DATA:
		msgText = "Нет записи запрошенного типа";
		break;
	case WSA_INVALID_HANDLE:
		msgText = "Указанный дескриптор события с ошибкой";
		break;
	case WSA_INVALID_PARAMETER:
		msgText = "Один или более параметров с ошибкой";
		break;
	case WSA_IO_INCOMPLETE:
		msgText = "Объект ввода-вывода не в сигнальном состоянии";
		break;
	case WSA_IO_PENDING:
		msgText = "Операция завершится позже";
		break;
	case WSA_NOT_ENOUGH_MEMORY:
		msgText = "Не достаточно памяти";
		break;
	case WSA_OPERATION_ABORTED:
		msgText = "Операция отвергнута";
		break;
	case WSASYSCALLFAILURE:
		msgText = "Аварийное завершение системного вызова";
		break;
	default:
		break;
	}
	return msgText;
};
string SetErrorMsgText(string msgText, int code)
{
	return msgText + GetErrorMsgText(code);
};

using namespace std;
int main()
{
	SOCKET sS; // дескриптор сокета
	// дескриптор сокета
	WSADATA wsaData;

	char ibuf[50], //буфер ввода
		obuf[50] = "Hello from client!"; //буфер вывода
	int libuf = 0, //количество принятых байт
		lobuf = 0; //количество отправленных байь

	try
	{
		if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0)
			throw SetErrorMsgText("Startup:", WSAGetLastError());
		if ((sS = socket(AF_INET, SOCK_STREAM, NULL)) == INVALID_SOCKET)
			throw SetErrorMsgText("socket:", WSAGetLastError());

		SOCKET cC; // серверный сокет
		if ((cC = socket(AF_INET, SOCK_STREAM, NULL)) == INVALID_SOCKET)
			throw SetErrorMsgText("socket:", WSAGetLastError());

		SOCKADDR_IN serv; // параметры сокета сервера
		serv.sin_family = AF_INET; // используется IP-адресация
		serv.sin_port = htons(2000); // TCP-порт 2000
		serv.sin_addr.s_addr = inet_addr("127.0.0.1"); // адрес сервера
		//Канал связи
		if ((connect(cC, (sockaddr*)&serv, sizeof(serv))) == SOCKET_ERROR)
			throw SetErrorMsgText("connect:", WSAGetLastError());

		//int libuf = 0,
		//	lobuf = 0;
		//char ibuf[30],
		//	obuf[30] = "Hello from client ";
		int count_from_Hello = 100;
		for (int i = 0; i < count_from_Hello; i++) {
			char num[4];
			_itoa(i, num, 10);
			strcat(obuf, num);
			if ((lobuf = send(cC, obuf, strlen(obuf) + 1, NULL)) == SOCKET_ERROR)//функция пересылает заданное  количество  байт данных по каналу определенного сокета  
				throw  SetErrorMsgText("send:", WSAGetLastError());
			cout << obuf << endl;
			if ((recv(cC, obuf, strlen(obuf) + 1, NULL)) == SOCKET_ERROR)//функция принимает заданное  количество байт данных по каналу определенного сокета  
				throw  SetErrorMsgText("recv:", WSAGetLastError());
			char numtwo[4];
			char Message[30] = "Hello from client ";
			_itoa(i + 1, numtwo, 10);
			strcat(Message, numtwo);
			if ((lobuf = send(cC, Message, strlen(Message) + 1, NULL)) == SOCKET_ERROR)
				throw  SetErrorMsgText("send:", WSAGetLastError());
			cout << " " << endl;
			cout << Message << endl;
		}
		if ((lobuf = send(cC, "", strlen("") + 1, NULL)) == SOCKET_ERROR)
			throw  SetErrorMsgText("send:", WSAGetLastError());




		if (closesocket(sS) == SOCKET_ERROR)
			throw SetErrorMsgText("closesocket:", WSAGetLastError());
		if (WSACleanup() == SOCKET_ERROR)
			throw SetErrorMsgText("Cleanup:", WSAGetLastError());
	}
	catch (string errorMsgText)
	{
		cout << endl << errorMsgText;
	}
	return 0; 
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

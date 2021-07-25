// ClientMS.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <string>
#include <time.h>
#include <Windows.h>
using namespace std;

 string SetMaislotError( string msgText, int code)
{
	char intStr[20];
	_itoa_s(code, intStr, 10);
	return msgText + " " +  string(intStr);
};

int main()
{
	setlocale(LC_ALL, "");
	HANDLE hM;
	DWORD wb;
	try
	{
		if ((hM = CreateFile(TEXT("\\\\.\\mailslot\\Box"), GENERIC_WRITE,
			FILE_SHARE_READ, NULL, OPEN_EXISTING, NULL, NULL))
			== INVALID_HANDLE_VALUE)
			throw (GetLastError() == 2) ?  string("Mail server not found!") : SetMaislotError("CreateFileError: ", GetLastError());
		clock_t start = clock(),stop;
		for (int i = 0; i < 1000; i++)
		{
			char wbuf[30];
			_itoa_s(i, wbuf, 10);
			if (!WriteFile(hM, wbuf, sizeof(wbuf), &wb, NULL))
				throw SetMaislotError("ReadFileError: ", GetLastError());
			
			if (i == 999)
			{
				stop = clock() - start;
			}
		}
		cout << endl << "Total time for send: " << stop;
		CloseHandle(hM);
	}
	catch ( string messageError)
	{
		 cout << endl << messageError;
	}

	 cout <<  endl;
	system("pause");
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

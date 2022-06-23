#include <windows.h>
#include <iostream>
#include "array.h"


DWORD WINAPI worker(LPVOID v)
{
	Data* data = (Data*)v;
	int min = INT_MAX;
	for (int i = 0; i < data->size; i++)
	{
		if (data->array[i] < min)
			min = data->array[i];
	}
	std::cout << "Минимальный элемент массива = " << min << endl;
	return 0;
}

DWORD WINAPI fibonacci(LPVOID v)
{
	int num1 = 1;
	int num2 = 2;

    while (num2 < INT_MAX) {
		cout << num2 << " ";

		int tmp = num2;
		num2 += num1;
		num1 = tmp;

		Sleep(200);
	}

	return 0;
}

int main()
{
	setlocale(LC_ALL, "Russian");

	std::cout << "Введите размер массива:\n";
	int size;
	std::cin >> size;

	cout << "Введите элементы:\n";
	int* array = new int[size];
	for (int i = 0; i < size; i++)
	{
		std::cin >> array[i];
	}

	std::cout << "Введите задержку:\n";
	int time;
	std::cin >> time;

	HANDLE taskThread;
	DWORD IDThread1;

	taskThread = CreateThread(NULL, 0, worker, (void*)new Data(size, array), 0, &IDThread1);

	SuspendThread(taskThread);     
	ResumeThread(taskThread);
	WaitForSingleObject(taskThread, INFINITE);  
	CloseHandle(taskThread);
	//Доп задание
	HANDLE thread;
	DWORD IDThread2;

	thread = CreateThread(NULL, 0, fibonacci, NULL, 0, &IDThread2);

	WaitForSingleObject(thread, 9000);
	SuspendThread(thread);
	CloseHandle(thread);
	delete array;

	system("pause");

	return 0;
}

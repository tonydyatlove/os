#include <windows.h>
#include <iostream>
#include <conio.h>
#include <list>

HANDLE event1;
HANDLE event2;
HANDLE event3;
HANDLE event4;
HANDLE parentEvent1;
HANDLE parentEvent2;
HANDLE parentEndEvent;
HANDLE semaphore;
bool flag1 = true;
bool flag2 = true;
bool flag3 = true;
volatile int messageParentEventCount = 0;
volatile int messageWriterEndCount = 0;

struct ProcessInformation {
	STARTUPINFO info; //Используется для настройки свойств процесса, например расположения окон и заголовок.
	PROCESS_INFORMATION processInformation;//Структура PROCESS_INFORMATION с информацией о процессе. Будет заполнена Windows.

	ProcessInformation() {
		ZeroMemory(&info, sizeof(STARTUPINFO)); //заполняет блок памяти нулями. указатель на начало блока памяти. размер в байтах
		info.cb = sizeof(STARTUPINFO);
	}
};

void sendMessage(int type) {
	switch (type) {
	case 1:
		SetEvent(event1);
		break;
	case 2:
		SetEvent(event2);
		break;
	case 3:
		SetEvent(event3);
		break;
	case 4:
		SetEvent(event4);
	}
}

DWORD WINAPI waitEvent1(LPVOID v) { //указатель на константу любого вида
	while (flag1) {
		WaitForSingleObject(parentEvent1, INFINITE); //останавливает работу программы, пока объект не окажется в сигнальном положении
			std::cout << "Получено сообщение 1 от процесса Parent\n";
		messageParentEventCount++;
	}
	return 0;
}

DWORD WINAPI waitEvent2(LPVOID v) {
	while (flag2) {
		WaitForSingleObject(parentEvent2, INFINITE);
			std::cout << "Получено сообщение 2 от процесса Parent\n";
		messageParentEventCount++;
	}
	return 0;
}

DWORD WINAPI waitParentEndEvent(LPVOID v) {
	while (flag3) {
		WaitForSingleObject(parentEndEvent, INFINITE);
		std::cout << "Процесс Parent завершил работу.\n";
		messageWriterEndCount++;
	}
	return 0;
}

int main()
{
	setlocale(LC_ALL, "Russian");

	event1 = CreateEvent(NULL, FALSE, FALSE, "Event1"); //атрибут защиты; если фолс, то авто-сброс и таким событием можно возобновить работу одного ждущего потока; начальное состояние; имя объекта 
	event2 = CreateEvent(NULL, FALSE, FALSE, "Event2");
	event3 = CreateEvent(NULL, FALSE, FALSE, "Event3");
	event4 = CreateEvent(NULL, FALSE, FALSE, "Event4");
	parentEvent1 = CreateEvent(NULL, FALSE, FALSE, "ParentEvent1");
	parentEvent2 = CreateEvent(NULL, FALSE, FALSE, "ParentEvent2");
	parentEndEvent = CreateEvent(NULL, FALSE, FALSE, "ParentEndEvent");
	semaphore = CreateSemaphore(NULL, 3, 3, "Semaphore");// атрибут доступа; инициализированное начальное состояние счетчика; максимальное количество обращений; имя объекта

	std::cout << "Введите кол-во процессов \"Parent\" и \"Os lab 4\":\n";
	int processCount;
		std::cin >> processCount;
	system("cls");

	ProcessInformation** processInformation;
	processInformation = new ProcessInformation * [processCount];

	for (int i = 0; i < processCount * 2; i += 2) {
		processInformation[i] = new ProcessInformation();
		if (!CreateProcess(NULL, dir1, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &processInformation[i]->info, &processInformation[i]->processInformation)) {
				std::cout << "Os lab 4 not created\n"; //createprocess создает новый процесс и его первичный (главный) поток
		}
		
		TCHAR dir[200] = "D:\\Os lab 4\\Debug\\Parent.exe";
		processInformation[i + 1] = new ProcessInformation();
		if (!CreateProcess(NULL, dir, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &processInformation[i + 1]->info, &processInformation[i + 1]->processInformation)) {
				std::cout << "Parent not created\n";
		}
	}

	HANDLE parentThread1;
	DWORD IDThread1;

	HANDLE parentThread2;
	DWORD IDThread2;

	HANDLE parentEndThread;
	DWORD IDThread3;

	parentThread1 = CreateThread(NULL, 0, waitEvent1, (void*)parentEvent1, 0, &IDThread1);// дескриптор защиты
	// начальный размер стека
  // функция потока
		// указатель на переменную, которая передаётся в поток
		// опции создания (если 0, то поток запускается сразу)
		// идентификатор потока
	parentThread2 = CreateThread(NULL, 0, waitEvent2, (void*)parentEvent2, 0, &IDThread2);
	parentEndThread = CreateThread(NULL, 0, waitParentEndEvent, (void*)parentEndEvent, 0, &IDThread3);

	for (int i = 0; i < processCount * 2; i++) {
		std::cout << "Введите сообщение (1-4):\n";
		int message;
		std::cin >> message;

		sendMessage(message);
	}

	while ((messageParentEventCount < processCount) || (messageWriterEndCount < processCount));

	TerminateThread(parentThread1, 0); //завершение работы потока (0 - код потока(успешно))
	TerminateThread(parentThread2, 0);
	TerminateThread(parentEndThread, 0);

	CloseHandle(parentThread1);
	CloseHandle(parentThread2);
	CloseHandle(parentEndThread);

	system("pause");

	return 0;
}

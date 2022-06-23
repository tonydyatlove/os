#include <windows.h>
#include <iostream>
#include <conio.h>

volatile bool flag = true;

void sendMessage(int type, HANDLE parentEvent1, HANDLE parentEvent2) {
	switch (type) {
	case 1:
		SetEvent(parentEvent1); //устанавливаем событие в сигнальное состояние
		break;
	case 2:
		SetEvent(parentEvent2);
	}
}

void getMessage(int num) {
	switch (num) {
	case 1:
		std::cout << "Получено сообщение 1 в процессе Parent\n";
		break;
	case 2:
		std::cout << "Получено сообщение 2 в процессе Parent\n";
		break;
	case 3:
		std::cout << "Получено сообщение 3 в процессе Parent\n";
		break;
	case 4:
		std::cout << "Получено сообщение 4 в процессе Parent\n";
		break;

	}
}

int main(int argc, char* argv[])
{
	setlocale(LC_ALL, "Russian");

	std::cout << "Parent process\n";

	HANDLE* handleArray = new HANDLE[7];
	handleArray[0] = OpenEvent(EVENT_ALL_ACCESS, FALSE, "Event1"); //открываем событие(всевозможные флаги доступа)
	handleArray[1] = OpenEvent(EVENT_ALL_ACCESS, FALSE, "Event2");
	handleArray[2] = OpenEvent(EVENT_ALL_ACCESS, FALSE, "Event3");
	handleArray[3] = OpenEvent(EVENT_ALL_ACCESS, FALSE, "Event4");
	HANDLE parentEvent1 = OpenEvent(EVENT_ALL_ACCESS, FALSE, "ParentEvent1");
	HANDLE parentEvent2 = OpenEvent(EVENT_ALL_ACCESS, FALSE, "ParentEvent2");
	HANDLE parentEndEvent = OpenEvent(EVENT_ALL_ACCESS, FALSE, "ParentEndEvent");
	HANDLE semaphore = OpenSemaphore(EVENT_ALL_ACCESS, FALSE, "Semaphore");

	std::cout << "Не активен\n";
	WaitForSingleObject(semaphore, INFINITE);
	system("cls");
	std::cout << "Parent process\n";
	std::cout << "Активен\n";

	int pos = WaitForMultipleObjects(4, handleArray, FALSE, INFINITE); //(размер массива; указатель на массив; если фолс, то ждёт пока любой объект перейдёт в сигнальное сост)

	getMessage(pos + 1);

	std::cout << "Введите сообщение (1-2):\n";
	int message;
	std::cin >> message;

	sendMessage(message, parentEvent1, parentEvent2);

	SetEvent(parentEndEvent);

	ReleaseSemaphore(semaphore, 1, NULL);//увеличение счётчика семафора (идентификатор, значение инкремента, // адрес переменной для записи 
                   // предыдущего значения счетчика семафора)

	system("pause");
	return 1;
}

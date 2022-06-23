#include <windows.h>
#include <iostream>

int main() {
	setlocale(LC_ALL, "rus");
	std::cout << "Server process\n";

	std::cout << "Введите размер массива:\n";
	int size;
	std::cin >> size;

	short* arr = new short[size];
	std::cout << "Введите " << size << " элементов типа short.\n";
	for (int i = 0; i < size; i++)
		std::cin >> arr[i];


	SECURITY_ATTRIBUTES sa; // атрибуты защиты
	SECURITY_DESCRIPTOR sd; // дескриптор защиты
	HANDLE hNamedPipe;
	DWORD dwBytesRead; // для числа прочитанных байтов
	DWORD dwBytesWrite; // для числа записанных байтов

	// инициализация атрибутов защиты
	sa.nLength = sizeof(sa);
	sa.bInheritHandle = FALSE; // дескриптор канала ненаследуемый

	// инициализируем дескриптор защиты
	InitializeSecurityDescriptor(&sd, SECURITY_DESCRIPTOR_REVISION);

	// устанавливаем атрибуты защиты, разрешая доступ всем пользователям
	SetSecurityDescriptorDacl(&sd, TRUE, NULL, FALSE);
	sa.lpSecurityDescriptor = &sd;

	// создаем именованный канал для чтения
	hNamedPipe = CreateNamedPipe("\\\\.\\pipe\\demo_pipe", PIPE_ACCESS_DUPLEX, PIPE_TYPE_MESSAGE | PIPE_WAIT, 1, 0, 0, INFINITE, &sa);

	// проверяем на успешное создание
	if (hNamedPipe == INVALID_HANDLE_VALUE)
	{
		std::cout << "Invalid handle value.\n";
		system("pause");
		return 0;
	}

	// ждем, пока клиент свяжется с каналом
	std::cout << "The server is waiting for connection." << endl;
	if (!ConnectNamedPipe(hNamedPipe, (LPOVERLAPPED)NULL)) {
		std::cout << "Connection failed.\n";
		system("pause");
		return 0;
	}

	

	if (!WriteFile(hNamedPipe, &size, sizeof(size), &dwBytesWrite, (LPOVERLAPPED)NULL)) {
		// ошибка записи
		std::cout << "Write size failed.\n";
		system("pause");
		CloseHandle(hNamedPipe);
		return 0;
	}

	for (int i = 0; i < size; i++) {
		short num = arr[i];

		if (!WriteFile(hNamedPipe, &num, sizeof(num), &dwBytesWrite, (LPOVERLAPPED)NULL)) {
			// ошибка записи
			std::cout << "Write number failed.\n";
			system("pause");
			CloseHandle(hNamedPipe);
			return 0;
		}
	}

	int rSize;

	if (!ReadFile(hNamedPipe, &rSize, sizeof(rSize), &dwBytesRead, (LPOVERLAPPED)NULL)) {
		std::cout << "Read size failed\n";
		system("pause");
		CloseHandle(hNamedPipe);
		return GetLastError();
	}

	std::cout << "Переданный массив:\n";
	for (int i = 0; i < size; i++)
		std::cout << arr[i] << " ";
	std::cout << endl;

	std::cout << "Полученный массив:\n";
	for (int i = 0; i < rSize; i++)
	{
		short data;
		if (!ReadFile(hNamedPipe, &data, sizeof(data), &dwBytesRead, (LPOVERLAPPED)NULL)) {
			std::cout << "Read number failed.\n";
			system("pause");
			CloseHandle(hNamedPipe);
			return GetLastError();
		}
		std::cout << data << " ";
	}
	std::cout << endl;

	// закрываем дескриптор канала
	CloseHandle(hNamedPipe);

	system("pause");

	return 0;
}

#include <windows.h>
#include <iostream>


int main() {
	setlocale(LC_ALL, "rus");
	std::cout << "Part process\n";


	char lpszOutMessage[] = "How do you do server?";

	HANDLE hNamedPipe;
	char machineName[80];
	char pipeName[80];
	DWORD dwBytesWritten; // для числа записанных байтов
	DWORD dwBytesRead; // для числа прочитанных байтов

	// вводим имя машины в сети, на которой работает сервер
	std::cout << "Enter a name of the server machine (. - default): ";
	std::cin >> machineName;

	// подставляем имя машины в имя канала
	wsprintfA(pipeName, "\\\\%s\\pipe\\demo_pipe", machineName);

	// связываемся с именованным каналом
	hNamedPipe = CreateFile("\\\\.\\pipe\\demo_pipe", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, (LPSECURITY_ATTRIBUTES)NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, (HANDLE)NULL);

	// проверяем связь с каналом
	if (hNamedPipe == INVALID_HANDLE_VALUE)
	{
		std::cout << "Invalid handle value.\n";
		system("pause");
		CloseHandle(hNamedPipe);
		return 0;
	}

	int N, M;
	std::cout << "Введите числа N и M (N<M)\n";
	std::cin >> N;
	std::cin >> M;

	int size;

	if (!ReadFile(hNamedPipe, &size, sizeof(size), &dwBytesRead, (LPOVERLAPPED)NULL)) {
		std::cout << "Read size failed\n";
		system("pause");
		CloseHandle(hNamedPipe);
		return GetLastError();
	}

	int rSize = 0;
	short* arr = new short[size];
	std::cout << "Полученный массив: \n";
	for (int i = 0; i < size; i++)
	{
		short data;

		if (!ReadFile(hNamedPipe, &data, sizeof(data), &dwBytesRead, (LPOVERLAPPED)NULL)) {
			std::cout << "Read number failed.\n";
			system("pause");
			CloseHandle(hNamedPipe);
			return GetLastError();
		}

		std::cout << data << " ";
		if ((data >= N) && (data <= M)) {
			arr[rSize++] = data;
		}
	}
	std::cout << endl;

	if (!WriteFile(hNamedPipe, &rSize, sizeof(rSize), &dwBytesWritten, (LPOVERLAPPED)NULL)) {
		// ошибка записи
		std::cout << "Write size failed.\n";
		system("pause");
		CloseHandle(hNamedPipe);
		return 0;
	}

	for (int i = 0; i < rSize; i++) {
		short num = arr[i];

		if (!WriteFile(hNamedPipe, &num, sizeof(num), &dwBytesWritten, (LPOVERLAPPED)NULL)) {
			std::cout << "Write number failed.\n";
			system("pause");
			return GetLastError();
		}
	}

	std::cout << "Переданный массив, состоящий из чисел, которые попали в отрезок [N,M]:\n";
	for (int i = 0; i < rSize; i++) {
		std::cout << arr[i] << " ";
	}
	std::cout << endl;

	// закрываем дескриптор канала
	CloseHandle(hNamedPipe);

	system("pause");

	return 0;
}

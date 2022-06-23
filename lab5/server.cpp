#include <stdio.h>
#include <windows.h>
#include <iostream>
#include <conio.h>
#include <sstream>
#include <wchar.h>

int main() {
	setlocale(LC_ALL, "rus");
		std::cout << "Server process\n";

	HANDLE writePipe;
	HANDLE readPipe;
	HANDLE hEnableRead;
	TCHAR lpszEnableRead[] = "EnableRead";
	hEnableRead = CreateEvent(NULL, FALSE, FALSE, lpszEnableRead);

	if (!CreatePipe(&readPipe, &writePipe, NULL, 0)) {
			std::cout << "Create pipe failed.\n";
		system("pause");
		return GetLastError();
	}

	if (!CreatePipe(&readPipe, &writePipe, NULL, 0)) {
			std::cout << "Create pipe failed.\n";
		system("pause");
		return GetLastError();
	}
	// делаем наследуемый дубликат дескриптора hWritePipe
	HANDLE duplicateWritePipe;

	if (!DuplicateHandle(GetCurrentProcess(), writePipe, GetCurrentProcess(), &duplicateWritePipe, 0, TRUE, DUPLICATE_SAME_ACCESS)) {
			std::cout << "Duplicate handle failed.\n";
		system("pause");
		return GetLastError();
	}

	//делаем наследуемый дубликат дескриптора hReadPipe
	HANDLE duplicateReadPipe;

	if (!DuplicateHandle(GetCurrentProcess(), readPipe, GetCurrentProcess(), &duplicateReadPipe, 0, TRUE, DUPLICATE_SAME_ACCESS)) {
			std::cout << "Duplicate handle failed.\n";
		system("pause");
		return GetLastError();
	}




	TCHAR* comLine = new TCHAR[100]{};
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	//устанавливаем атрибуты нового процесса
	ZeroMemory(&si, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);

	std::string str = "Part.exe " + std::to_string((int)duplicateWritePipe)
		+ " " + std::to_string((int)duplicateReadPipe);
	//printf(comLine, "Part.exe %d %d", (int)duplicateWritePipe, (int)duplicateReadPipe);

		std::cout << "Введите размер массива:\n";
	int size;
		std::cin >> size;

	short* arr = new short[size];
		std::cout << "Введите " << size << " элементов типа short.\n";
	for (int i = 0; i < size; i++)
			std::cin >> arr[i];

	TCHAR name[] = "Part.exe";
	strcpy(comLine, str.c_str());
	if (!CreateProcess(name, comLine, NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi)) {
			std::cout << "Create process failed.\n";
		system("pause");
		return GetLastError();
	}

	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);

	DWORD dwBytesWritten;
	if (!WriteFile(writePipe, &size, sizeof(size), &dwBytesWritten, NULL)) {
			std::cout << "Write to file failed.\n";
		system("pause");
		return GetLastError();
	}

	for (int i = 0; i < size; i++) {
		short num = arr[i];

		if (!WriteFile(writePipe, &num, sizeof(num), &dwBytesWritten, NULL)) {
				std::cout << "Write to file failed.\n";
			system("pause");
			return GetLastError();
		}
	}


	CloseHandle(writePipe);
	cout << "Переданный массив:\n";
	for (int i = 0; i < size; i++)
			std::cout << arr[i] << " ";
		std::cout << endl;


	int rSize;

	hEnableRead = OpenEvent(EVENT_ALL_ACCESS, FALSE, lpszEnableRead);
	//ждём разрешения на чтение
	WaitForSingleObject(hEnableRead, INFINITE);
	DWORD dwBytesRead;
	if (!ReadFile(readPipe, &rSize, sizeof(rSize), &dwBytesRead, NULL)) {
			std::cout << "Read from the pipe failed.\n";
		system("pause");
		return GetLastError();
	}




	std::cout << "Полученный массив: \n";
	for (int i = 0; i < rSize; i++) {
		short data;
		if (!ReadFile(readPipe, &data, sizeof(data), &dwBytesRead, NULL)) {
			std::cout << "Read from the pipe failed.\n";
			system("pause");
			return GetLastError();
		}
			std::cout << data << " ";
	}
	std::cout << endl;

	CloseHandle(readPipe);
	CloseHandle(duplicateWritePipe);
	CloseHandle(duplicateReadPipe);
	CloseHandle(hEnableRead);
	system("pause");
	return 0;
}

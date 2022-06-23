#include <windows.h>
#include <conio.h>
#include <string>
#include <iostream>
#include <stdlib.h>

int main()
{
	int size;
	std::cout << "Enter size" << std::endl;
	std::cin >> size;

	char* arr = new char[size + 1];
	for (int i = 0; i < size; i++) {
		std::cin >> arr[i];
	}

	STARTUPINFO si;
	PROCESS_INFORMATION piApp;
	ZeroMemory(&si, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);
	si.dwFlags = STARTF_USEFILLATTRIBUTE;
	si.dwFillAttribute = FOREGROUND_RED;

	if (!CreateProcess(appName, arr, NULL, NULL, FALSE,
		CREATE_NEW_CONSOLE, NULL, NULL, &si, &piApp))
	{
		_cputs("The new process is not created.\n");
		_cputs("Check a name of the process.\n");
		_cputs("Press any key to finish.\n");
		_getch();

		return 0;
	}
	_cputs("The new process is created.\n");

	WaitForSingleObject(piApp.hProcess, INFINITE);
	CloseHandle(piApp.hThread);
	CloseHandle(piApp.hProcess);

	return 0;
}

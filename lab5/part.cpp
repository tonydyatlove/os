#include <windows.h>
#include <iostream>
#include <conio.h>

int main(int argc, char* argv[]) {
	setlocale(LC_ALL, "rus");
	std::cout << "Part process\n";

	HANDLE writePipe;
	HANDLE readPipe;
	HANDLE hEnableRead;
	TCHAR lpszEnableRead[] = "EnableRead";
	hEnableRead = OpenEvent(EVENT_ALL_ACCESS, FALSE, lpszEnableRead);

	int N, M;
	cout << "Введите числа N и M (N<M)\n";
	cin >> N;
	cin >> M;


	readPipe = (HANDLE)atoi(argv[2]);
	writePipe = (HANDLE)atoi(argv[1]);
	int rSize;
	DWORD dwBytesRead;

	if (!ReadFile(readPipe, &rSize, sizeof(rSize), &dwBytesRead, NULL)) {
		std::cout << "Read from the pipe failed.\n";
		system("pause");
		return GetLastError();
	}
	short* arr = new short[rSize];
	std::cout << "Полученный массив: \n";
	for (int i = 0; i < rSize; i++)
	{

		if (!ReadFile(readPipe, &arr[i], sizeof(arr[i]), &dwBytesRead, NULL)) {
			std::cout << "Read from the pipe failed.\n";
			system("pause");
			return GetLastError();
		}
		std::cout << arr[i] << " ";
	}
	std::cout << endl;

	short* arr2 = new short[rSize];
	int rSize2 = 0;
	for (int i = 0; i < rSize; i++) {
		if ((arr[i] >= N) && (arr[i] <= M)) {
			arr2[rSize2++] = arr[i];
		}
	}



	DWORD dwBytesWritten;
	if (!WriteFile(writePipe, &rSize2, sizeof(rSize2), &dwBytesWritten, NULL)) {
		std::cout << "Write to file failed.\n";
		system("pause");
		return GetLastError();
	}

	for (int i = 0; i < rSize2; i++) {
		short num2 = arr2[i];

		if (!WriteFile(writePipe, &num2, sizeof(num2), &dwBytesWritten, NULL)) {
			std::cout << "Write to file failed.\n";
			system("pause");
			return GetLastError();
		}
	}
	// сигнальное состояние
	SetEvent(hEnableRead);
	CloseHandle(writePipe);



	std::cout << "Переданный массив, состоящий из чисел, которые попали в отрезок [N,M]:\n";
	for (int i = 0; i < rSize2; i++) {
		std::cout << arr2[i] << " ";
	}
	std::cout << endl;

	system("pause");
	return 0;
}

#include <windows.h>
#include <iostream>
#include <limits>

CRITICAL_SECTION cs;
HANDLE hOutEvent;

bool AreDoublesEqual(double a, double b) {
	return abs(a - b) < 10e-9;
}

void clasterize(double* arr, int size, int timeout) {
	double* tempArr = new double[size];

	int left = 0;
	int right = size - 1;
	for (int i = 0; i < size; i++) {
		bool flag = false;
		for (int j = 0; j < size; j++) {
			if (AreDoublesEqual(arr[i], tempArr[j])) {
				flag = true;
			}
		}

		if (flag) {
			continue;
		}

		int duplCount = 1;
		for (int j = i + 1; j < size; j++) {
			if (AreDoublesEqual(arr[i], arr[j])) {
				tempArr[left] = arr[i];
				tempArr[left + duplCount] = arr[j];

				duplCount++;
			}
		}

		if (duplCount == 1) {
			tempArr[right] = arr[i];
			right--;
		}
		else {
			left += duplCount;
		}

		Sleep(timeout);
	}

	for (int i = 0; i < size; i++) {
		arr[i] = tempArr[i];
	}
}

void MultElement(LPVOID arr)
{

	EnterCriticalSection(&cs);
	std::cout << endl << endl << "Thread \'MultElement\' is created" << endl << endl;

	int size = ((double*)arr)[0];
	size++;
	double* newArr = (double*)arr;
	newArr++;

	for (int i = 0; i < size; i++) {
		cout << newArr[i] << " ";
	}

	double mult = 1;
	for (int i = 0; i < size; i++) {
		mult *= newArr[i];
	}
	std::cout << endl << "Multiplication: " << mult << endl;

	LeaveCriticalSection(&cs);

	SetEvent(hOutEvent);
}

void work(LPVOID arr)
{
	EnterCriticalSection(&cs);

	std::cout << endl << endl << "Thread \'Work\' is created" << endl;
	std::cout << "Enter time between elements: ";

	unsigned int time;
	std::cin >> time;
	std::cout << endl;

	int size = ((double*)arr)[0] + 0.1;
	size++;
	double* newArr = (double*)arr;
	newArr++;

	for (int i = 0; i < size; i++) {
		std::cout << newArr[i] << " ";
	}
	std::cout << endl;

	clasterize(newArr, size, time);

	for (int i = 0; i < size; i++) {
		std::cout << newArr[i] << " ";
	}

	LeaveCriticalSection(&cs);
}

int main()
{
	int size;

	std::cout << "Enter size: ";
	std::cin >> size;
	std::cout << "Enter array: ";
	double* arr = new double[size + 1];
	arr[0] = size - 0.5;
	for (int i = 1; i < size + 1; i++) {
		std::cin >> arr[i];
	}

	std::cout << endl << "Size: " << size << endl;
	std::cout.precision(3);
	for (int i = 1; i < size + 1; i++) {
		std::cout << arr[i] << " ";
	}
	std::cout << endl;

	InitializeCriticalSection(&cs);

	HANDLE hThread1;
	DWORD dwThread1;

	HANDLE hThread2;
	DWORD dwThread2;

	hThread1 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)work, (void*)arr, 0, &dwThread1);
	if (hThread1 == NULL)
		return GetLastError();

	hOutEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	if (hOutEvent == NULL)
		return GetLastError();

	hThread2 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)MultElement, (void*)arr, 0, &dwThread2);
	if (hThread2 == NULL)
		return GetLastError();



	if (WaitForSingleObject(hOutEvent, INFINITE) != WAIT_OBJECT_0)
	{
		std::cout << "Wait for single object for MultElement is failed." << endl;
		std::cout << "Press any key to exit." << endl;
	}

	if (WaitForSingleObject(hThread1, INFINITE) != WAIT_OBJECT_0)
	{
		std::cout << "Wait for single object for wokk is failed." << endl;
		std::cout << "Press any key to exit." << endl;
	}

	if (WaitForSingleObject(hThread2, INFINITE) != WAIT_OBJECT_0)
	{
		std::cout << "Wait for single object for wokk is failed." << endl;
		std::cout << "Press any key to exit." << endl;
	}


	DeleteCriticalSection(&cs);
	CloseHandle(hThread1);
	CloseHandle(hThread2);


	return 0;
}




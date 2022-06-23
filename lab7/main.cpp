#include <iostream>
#include <ctime>
#include "MonitorQueue.cpp"

HANDLE start;

CRITICAL_SECTION cs1;
CRITICAL_SECTION cs2;
HANDLE empty_q;
HANDLE full;

struct DataForProducer
{
	MonitorQueue* queue;
	unsigned int number;
	unsigned int count;
};

struct DataForConsumer
{
	MonitorQueue* queue;
	unsigned int count;
};

DWORD WINAPI producer(LPVOID par)
{
	DataForProducer* data = (DataForProducer*)par;
	srand((unsigned int)time(0));
	for (int i = 0; i < data->count; i++)
	{
		WaitForSingleObject(empty_q, INFINITE);

		EnterCriticalSection(&cs1);


		unsigned int k = rand() % 100;
		data->queue->push(k);
		std::cout << "Произведен элемент (продукт) " << k << "\n";
		LeaveCriticalSection(&cs1);
		while (!ReleaseSemaphore(full, 1, NULL));
	}
	return 0;
}

DWORD WINAPI consumer(LPVOID par)
{
	DataForConsumer* data = (DataForConsumer*)par;
	for (int i = 0; i < data->count; i++)
	{
		WaitForSingleObject(full, INFINITE);

		EnterCriticalSection(&cs2);


		unsigned int number = data->queue->pop();
		std::cout << "\tУпотреблен элемент (продукт) " << number << std::endl;
		LeaveCriticalSection(&cs2);
		while (!ReleaseSemaphore(empty_q, 1, NULL));
	}
	return 0;
}

int main()
{
	setlocale(LC_ALL, "rus");
	std::cout << "Введите размер очереди:\n";
	int queueSize;
	std::cin >> queueSize;

	empty_q = CreateSemaphore(NULL, queueSize, queueSize, NULL); //(....; начальное значение счётчика; макс знач счётчика)
	full = CreateSemaphore(NULL, 0, queueSize - 1, NULL);
	InitializeCriticalSection(&cs1);
	InitializeCriticalSection(&cs2);
	start = CreateEvent(NULL, TRUE, FALSE, NULL);

	MonitorQueue* queue = new MonitorQueue(queueSize);

	std::cout << "Введите количество потоков Producer :\n";
	int prodThreadCount;
	std::cin >> prodThreadCount;

	DataForProducer* workWithProductCountProducer = new DataForProducer[prodThreadCount];
	for (int i = 0; i < prodThreadCount; i++)
	{
		std::cout << "Введите количество продуктов, которые должен произвести поток producer №" << i + 1 << endl;
		int count;
		std::cin >> count;
		workWithProductCountProducer[i].count = count;
		workWithProductCountProducer[i].number = i + 1;
		workWithProductCountProducer[i].queue = *&queue;
	}


	std::cout << "Введите количество потоков Consumer:\n";
	int consThreadCount;
	std::cin >> consThreadCount;

	DataForConsumer* workWithProductCountConsumer = new DataForConsumer[consThreadCount];
	for (int i = 0; i < consThreadCount; i++)
	{
		std::cout << "Введите количество продуктов, которые должен произвести поток Consumer №" << i + 1 << "\n";
		int count;
		std::cin >> count;
		workWithProductCountConsumer[i].count = count;
		workWithProductCountConsumer[i].queue = *&queue;
	}
	HANDLE* prodThreadHandls = new HANDLE[prodThreadCount];
	DWORD* prodThreadID = new DWORD[prodThreadCount];

	for (int i = 0; i < prodThreadCount; i++)
	{
		prodThreadHandls[i] = CreateThread(NULL, 0, producer, (void*)&workWithProductCountProducer[i], 0, &prodThreadID[i]);
	}


	HANDLE* consThreadHandls = new HANDLE[consThreadCount];
	DWORD* consThreadID = new DWORD[consThreadCount];

	for (int i = 0; i < consThreadCount; i++)
	{
		consThreadHandls[i] = CreateThread(NULL, 0, consumer, (void*)&workWithProductCountConsumer[i], 0, &consThreadID[i]);
	}

	HANDLE* handlesArray = new HANDLE[prodThreadCount + consThreadCount];

	for (int i = 0; i < prodThreadCount; i++)
	{
		handlesArray[i] = prodThreadHandls[i];
	}

	for (int i = 0; i < consThreadCount; i++)
	{
		handlesArray[i + prodThreadCount] = consThreadHandls[i];
	}

	ResetEvent(start);

	WaitForMultipleObjects(prodThreadCount + consThreadCount, handlesArray, TRUE, INFINITE);
	//вывод очереди
	queue->print();

	system("pause");
	return 0;
}

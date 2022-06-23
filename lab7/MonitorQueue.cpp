#include <Windows.h>
#include <iostream>

class MonitorQueue
{
private:
	int* queue;
	int size;
	//первый элемент
	unsigned int front;
	//последний элемент
	unsigned int rear;


public:
	MonitorQueue(unsigned int size)
	{
		this->rear = -1;
		this->front = -1;
		this->size = size;
		this->queue = new int[size];
	}

	~MonitorQueue()
	{

		delete[]queue;
	}

	bool isFull() {
		if (front == 0 && rear == size - 1)
			return true;
		if (front == rear + 1)
			return true;
		return false;
	}

	bool isEmpty() {
		if (front == -1) return true;
		else return false;
	}

	void push(int element)
	{

		if (front == -1) front = 0;
		rear = (rear + 1) % size;
		queue[rear] = element;
		//std::cout << "Произведен элемент (продукт) " << element << "\"\n";



	}

	int pop()
	{

		int element;
		int temp;

		element = queue[front];
		if (front == rear) {
			front = -1;
			rear = -1;
		}
		else {
			front = (front + 1) % size;
		}
		temp = element;
		//std::cout << "\tУпотреблен элемент (продукт) " << element << std::endl;

		return temp;
	}

	void print() {
		/* Функция для отображения статуса круговой очереди */
		int i;

		if (isEmpty()) {
			std::cout << std::endl << "Пустая очередь" << std::endl;
		}
		else
		{
			std::cout << std::endl << "Элементы -> ";
			for (i = front; i != rear; i = (i + 1) % size)
				std::cout << queue[i] << ", ";
			std::cout << queue[i];
			std::cout << std::endl;
		}

	}

};

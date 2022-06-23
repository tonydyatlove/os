struct Data
{
	int* array;
	int size;

	Data(int s, int* a)
	{
		size = s;
		array = new int[size];
		for (int i = 0; i < size; i++)
		{
			array[i] = a[i];
		}
	}

	~Data()
	{
		delete array;
	}
};

#include<iostream>
#include<time.h>

using namespace std;

int main()
{
	clock_t startTime, endTime;
	startTime = clock();
	for (int i = 0; i < 1000000; i++)
	{
		i++;
	}
	endTime = clock();
	cout << "Totle Time : " << (double)(endTime - startTime) / CLOCKS_PER_SEC << "s" << endl;
	system("pause");
	return 0;
}
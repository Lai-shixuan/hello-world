#include <iostream>
using namespace std;

#include<thrust/reduce.h>
#include<thrust/sequence.h>
#include<thrust/host_vector.h>
#include<thrust/device_vector.h>

int main()
{
	const int N = 50000;

	//任务1：创建数组
	thrust::device_vector<int>a(N);

	//任务2：填充数组
	thrust::sequence(a.begin(), a.end(),0);

	//任务3：并行计算数组各元素之和
	int sumA = thrust::reduce(a.begin(), a.end(), 0);

	//任务4：串行计算0到N-1之和
	int sumCheck = 0;
	for (int i = 0; i < N; i++) sumCheck += i;

	//任务5：检查结果正确性
	if (sumA == sumCheck) cout << "Test Succeeded!" << endl;
	else {
		cerr << "Test FAILED" << endl;
		return(1);
	}

	return 0;
}
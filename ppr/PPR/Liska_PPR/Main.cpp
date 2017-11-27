#include <amp.h>
#include <iostream>


void AddElementsWithRestrictedFunction(concurrency::index<1> idx, concurrency::array_view<int, 1> sum, concurrency::array_view<int, 1> a, concurrency::array_view<int, 1> b) restrict(amp)
{
	sum[idx] = a[idx] + b[idx];
}

void AddArraysWithFunction() {

	int aCPP[] = { 1, 2, 3, 4, 5 };
	int bCPP[] = { 6, 7, 8, 9, 10 };
	int sumCPP[5];

	concurrency::array_view<int, 1> a(5, aCPP);

	concurrency::array_view<int, 1> b(5, bCPP);

	concurrency::array_view<int, 1> sum(5, sumCPP);

	sum.discard_data();


	concurrency::parallel_for_each(
		sum.extent,
		[=](concurrency::index<1> idx) restrict(amp)
	{
		AddElementsWithRestrictedFunction(idx, sum, a, b);

	});


	for (int i = 0; i <5; i++) {
		std::cout << sum[i] << "\n";
	}
}

int main()
{
	AddArraysWithFunction();
	return 0;
}

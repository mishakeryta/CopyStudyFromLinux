#include <pthread.h>
#include <iostream>
#include <vector>
#include <array>
#include <ctime>
#include <cmath>

#define EPS 0.00000001

using namespace std;

long double function11(long double val)
{

	long double result = 1,
		denominator = 3,
		numerator = 1,
		powOfStep = 1,
		
		step = -1;
	do {
		powOfStep = numerator / denominator;
		numerator = denominator - 1;
		denominator += 3;
		step *= -powOfStep *val;
		result += step;
	} while (fabs(step) >= EPS);
	return result;
}
long double function5(long double val)
{
	long double result = 0,prevResult =0,step = val;
	unsigned denominator = 1;
	do
	{
		prevResult = result;
		result += step;
		++denominator;
		step *= -val / denominator;
	} while (abs(result - prevResult) > EPS);
	return result;
}
struct InputTabulate
{
	InputTabulate(long double begin, long double end, long double step, vector<array<long double, 2>>&output, int index);
	long double begin;
	long double end;
	long double step;
	vector<array<long double, 2>>& output;
	unsigned index;
};
InputTabulate::InputTabulate(long double begin, long double end, long double step, vector<array<long double, 2>>&output, int index) :
	begin(begin),
	end(end),
	step(step),
	output(output),
	index(index)
{}
//
void* tabulate(void* inAgs)
{
	InputTabulate& input = *((InputTabulate*)inAgs);
	long double current = input.begin;
	unsigned i = input.index;
	while (current < input.end)
	{
		input.output[i][0] = current;
		input.output[i][1] = function11(current);
		current += input.step;
		++i;
	}
	return 0;
}
vector<array<long double, 2>> tabulateMulti(long double begin, long  double end, long double step, unsigned numberOfThreads)
{
	unsigned numberOfPoints = (end - begin) / step + 1;
	vector<InputTabulate> myInputThreads;
	unsigned pointPerThread = (numberOfPoints - 1) / numberOfThreads;
	vector<array<long double, 2>> output(numberOfPoints);
	long double stepPerThread = step * pointPerThread;
	unsigned outputIndex = 0;

	for (unsigned i = 0; i < numberOfThreads - 1; ++i)
	{
		myInputThreads.emplace_back(begin, begin + stepPerThread, step, output, outputIndex);
		begin += stepPerThread;
		outputIndex += pointPerThread;
	}

	myInputThreads.emplace_back
	(begin, begin + stepPerThread + ((numberOfPoints - 1) % numberOfThreads)*step + step, step, output, outputIndex);

	vector<pthread_t> handles;
	for (auto& myInputThread : myInputThreads)
    {
		pthread_t new_thread;
		pthread_create(&new_thread,nullptr,tabulate,(void*)&myInputThread);
		handles.emplace_back(new_thread);
    }
	for (auto handle : handles)
		pthread_join(handle,nullptr);
	

	return move(output);
}

void PrintTableOfTabulation(const vector<array<long double, 2>>& tabulationResult)
{
	cout << "x     (x+1)^(1/3)\n";
	for (unsigned i = 0; i < tabulationResult.size(); ++i)
		cout << tabulationResult[i][0] << ' ' << tabulationResult[i][1] << '\n';
}
bool IsEqual(const vector<array<long double, 2>>& tabulationResult1, const vector<array<long double, 2>>& tabulationResult2)
{
	if (tabulationResult1.size() != tabulationResult2.size()) return false;
	for (size_t i = 0; i < tabulationResult1.size(); ++i)
	{
		if (abs(tabulationResult1[i][0] - tabulationResult2[i][0])>=EPS ||
			abs(tabulationResult1[i][1] - tabulationResult2[i][1])>=EPS) return false;
	}
	return true;
}
int main()
{
	long double begin = -1, end = 1, step = 0.001;
	cout << "Enter input:\n";
	cin >> begin >> end >> step;
	clock_t before = clock();
	vector<array<long double, 2>> ansBase = tabulateMulti(begin+step, end, step, 1);
	clock_t after = clock();
	cout << "1 thread version complete!\n Answer:\n";
	//PrintTableOfTabulation(ansBase);
	cout << "Time " << ((after - before)*1000) / CLOCKS_PER_SEC   << "ms\n\n";
	for (int pow2 = 2; pow2 <= 16; pow2 *= 2)
	{
		before = clock();
		vector<array<long double, 2>> ans = tabulateMulti(begin+step, end, step, pow2);
		after = clock();
		cout << pow2 << " thread version complete!\n";
		if (!IsEqual(ans, ansBase)) {
			cout << "Error\n";
			PrintTableOfTabulation(ans);
			break;
		}
		cout << "Time " << ((after - before) * 1000) / CLOCKS_PER_SEC << "ms\n\n";
	}
	cin.get(); cin.get();
	return 0;
}

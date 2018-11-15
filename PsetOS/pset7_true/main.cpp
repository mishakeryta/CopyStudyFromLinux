#include <pthread.h>
#include <iostream>
#include <vector>
#include <array>
#include <ctime>
#include <cmath>
#include <unistd.h>
#include<sys/wait.h>
#include <sys/types.h>
#include <sys/resource.h>

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
    int policy;
        struct sched_param param;

    InputTabulate& input = *((InputTabulate*)inAgs);
    long double current = input.begin;
    unsigned i = input.index;
    while (current < input.end)
    {
       cout<< "x = "<<current
         <<" f(x)  = "<<function11(current)<<'\n';
        current += input.step;
        ++i;
    }
    return 0;
}
vector<array<long double, 2>> tabulateMulti(long double begin, long  double end, long double step, unsigned numberOfThreads,int& process_id)
{
    pid_t  wpid;
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
    int indexOfInput = 0;
    for(int i = 0; process_id && i<numberOfThreads ;++i)
    {
        process_id =fork();
        ++indexOfInput;
        int which = PRIO_PROCESS;
        id_t pid;
        int priority = -20;
        int ret;
        pid = getpid();
        ret = setpriority(which, pid, priority);
    }

    int status= 0;
    while((wpid = wait(&status))>0);

    if(process_id == 0)
    {
        tabulate(&myInputThreads[indexOfInput]);
    }
    cin.get();cin.get();
    if(process_id )
    {
        return move(output);
    }
    return vector<array<long double,2>>();
}

void PrintTableOfTabulation(const vector<array<long double, 2>>& tabulationResult)
{
    cout << "x     (x+1)^(1/3)\n";
    for (unsigned i = 0; i < tabulationResult.size(); ++i)
        cout << tabulationResult[i][0] << ' ' << tabulationResult[i][1] << '\n';
}
int main()
{
    long double begin = 0, end = 0.7, step = 0.000001,number;
    cout << "Enter number of process:\n";
    cin>>number;
    clock_t before = clock();
    int process_id = 1;
    vector<array<long double, 2>> ansBase = tabulateMulti(begin+step, end, step, number,process_id);
    clock_t after = clock();
    if(process_id)
    {
        cout<<"Number of process "<< number << "\nTime : " << ((after-before)*1000)/CLOCKS_PER_SEC<<"ms";

    }
}

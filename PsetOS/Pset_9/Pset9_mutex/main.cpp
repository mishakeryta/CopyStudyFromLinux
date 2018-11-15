#include <pthread.h>

#include <iostream>
#include <vector>
#include <array>
#include <ctime>
#include <cmath>
#include <unistd.h>
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
   for(int i = 0; i < 100000;++i);
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
   InputTabulate(size_t begin, size_t end, vector<array<long double, 2>>&output);
   size_t begin;
   size_t end;
   vector<array<long double, 2>>& output;
};
InputTabulate::InputTabulate(size_t begin, size_t end, vector<array<long double, 2>>&output) :
   begin(begin),
   end(end),
   output(output)
{}
int progress = 0;
void IncrementProcess(size_t size)
{
   int unit = size/20;
   ++progress;
   if(progress%unit || progress == size-1)
   {
       cout<<'\r';
       cout<<'|';
       for(int i = 0;i<progress/unit;++i)
       {
           cout<<'+';
       }
       for(int i = progress/unit;i<20;++i) cout<<'-';
       cout<<'|';
       cout<<progress<<'/'<<size;
   }
}
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
void* tabulate(void* inAgs)
{
   pthread_mutex_lock(&mutex);
   int policy;
       struct sched_param param;
   pthread_getschedparam(pthread_self(), &policy, &param);
   param.sched_priority = sched_get_priority_max(policy);
   pthread_setschedparam(pthread_self(), policy, &param);
   InputTabulate& input = *((InputTabulate*)inAgs);
   unsigned i = input.begin;
   while (i<input.end)
   {
       input.output[i][1] = function5(input.output[i][0]);
       IncrementProcess(input.output.size());
       ++i;
   }
   pthread_mutex_unlock(&mutex);
   pthread_exit(NULL);
}
vector<array<long double, 2>> tabulateMulti(long double begin, long  double end, long double step, unsigned numberOfThreads)
{
   progress = 0;
   unsigned numberOfPoints = (end - begin) / step +1;
   vector<InputTabulate> myInputThreads;
   unsigned pointPerThread = (numberOfPoints - 1) / numberOfThreads;
   vector<array<long double, 2>> output(numberOfPoints);
   for(size_t i = 0; i < output.size();++i)
   {
       output[i][0] = begin+i*step;
   }
   long double stepPerThread = step * pointPerThread;
   size_t outputIndex = 0;

   for (unsigned i = 0; i < numberOfThreads - 1; ++i)
   {
       myInputThreads.emplace_back(outputIndex, outputIndex + pointPerThread, output);
       outputIndex+= pointPerThread;
   }
   myInputThreads.emplace_back(outputIndex,output.size(),output);

   vector<pthread_t> handles;
   for (auto& myInputThread : myInputThreads)
   {
       pthread_t new_thread;
       pthread_create(&new_thread,nullptr, tabulate,(void*)&myInputThread);
       handles.emplace_back(new_thread);
   }
   for (auto handle : handles)
       pthread_join(handle,nullptr);
   return move(output);
}
void PrintTableOfTabulation(const vector<array<long double, 2>>& tabulationResult)
{
   cout << "\nx     (x+1)^(1/3)\n";
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
   long double begin = -1, end = 1, step = 0.001,thread_count=0;
   cout << "Enter input:\n";
   cin >> begin >> end >> step>>thread_count;
   clock_t before = clock();
   vector<array<long double, 2>> ansBase = tabulateMulti(begin, end, step, thread_count);
   clock_t after = clock();
   PrintTableOfTabulation(ansBase);
   cout << "Time " << ((after - before)*1000) / CLOCKS_PER_SEC   << "ms\n\n";
   cin.get(); cin.get();
   return 0;
}


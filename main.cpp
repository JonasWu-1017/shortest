#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <vector>
#include <thread>
#include <mutex>

using namespace std;

typedef struct __tagStShortestItem
{
#ifdef DEBUG
    int thread_id;
#endif
    unsigned int i_src_num;
    int i_subsequence_length; // 0: default | -1: can't gather subsequence | -2: bigger than gMinSubsequenceLen | >0: length of subsequence
} StShortestItem;
int gPeak = 0;
int gMinSubsequenceLen = 0;
int gStop = 0;
mutex gLock;
vector<StShortestItem> gSourceVector;

int tryWrite(int n)
{
    unique_lock<mutex> lock(gLock);
    if (gMinSubsequenceLen < n)
        return -2;
    gMinSubsequenceLen = n;
    return n;
}

void thCheckSubsequence(int size, int threadTotal, int n)
{
    int i = 0, sum = 0, len = 0;
    [[maybe_unused]] int id = n;
    while (n < gStop)
    {
        // if (0 == gSourceVector[n].i_subsequence_length)
        {
#ifdef DEBUG
            gSourceVector[n].thread_id = id;
#endif
            for (i = n, sum = 0; i < size; i++)
            {
                len = i - n + 1;
                if (len > gMinSubsequenceLen)
                {
#ifdef DEBUG                    
                    gSourceVector[n].i_subsequence_length = -2;
#endif                    
                    break;
                }
                sum += gSourceVector[i].i_src_num;
                if (sum >= gPeak)
                {
                    // if (tryWrite(len))
                    //     gSourceVector[n].i_subsequence_length = len;
                    gSourceVector[n].i_subsequence_length = tryWrite(len);
                    break;
                }
            }
            if (size <= i)
            {
                if (gStop > n)
                    gStop = n;
#ifdef DEBUG                    
                gSourceVector[n].i_subsequence_length = -1;
#endif                
                break;
            }
        }
        n += threadTotal;
    }
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        cout << argv[0] << " [PeakNum] [SourceFile]\n";
        return 0;
    }

    auto start = chrono::high_resolution_clock::now();

    gPeak = (unsigned)stoi(argv[1], nullptr, 10);
    const auto processor_count = std::thread::hardware_concurrency();
    int thread_total = processor_count * 1;
    thread threads[thread_total];
    StShortestItem item;
    string line;
    int i = 0, j = 0, length = 0;
    [[maybe_unused]] int sum = 0;

    ifstream myfile(argv[2], ios::out | ios::binary);
    if (myfile.is_open())
    {
        while (getline(myfile, line))
        {
#ifdef DEBUG
            item.thread_id = -1;
#endif
            item.i_subsequence_length = 0;
            item.i_src_num = (unsigned)stoi(line, nullptr, 10);
            gSourceVector.push_back(item);
#ifdef CHECK_WHEN_READ
            if (sum >= 0)
            {
                sum += item.i_src_num;
                if (sum >= gPeak)
                {
                    gMinSubsequenceLen = gSourceVector[0].i_subsequence_length = gSourceVector.size();
                    sum = -1;
                }
            }
#endif
        }
        myfile.close();
    }
    else
    {
        cout << "Unable to open file";
        return 0;
    }

    length = gSourceVector.size();
#ifdef CHECK_WHEN_READ    
    if (sum != -1)
    {
        if (0 < length)
            gSourceVector[0].i_subsequence_length = -1;
    }
    else
    {
        gStop = length;
#else
    {
        gStop = gMinSubsequenceLen = length;
#endif
        for (i = 0; i < thread_total; i++)
#ifdef CHECK_WHEN_READ
            threads[i] = std::thread(thCheckSubsequence, length, thread_total, i + 1);
#else            
            threads[i] = std::thread(thCheckSubsequence, length, thread_total, i);
#endif            
        for (i = 0; i < thread_total; i++)
            threads[i].join();
    }

    auto elapsed = chrono::high_resolution_clock::now() - start;
    long long microseconds = chrono::duration_cast<chrono::microseconds>(elapsed).count();
    long long nanoseconds = chrono::duration_cast<chrono::nanoseconds>(elapsed).count();

#ifdef DEBUG
    cout << "processor_count: " << processor_count << "\n";
    cout << "thread_total: " << thread_total << "\n";
    cout << "gMinSubsequenceLen: " << gMinSubsequenceLen << "\n";
    for (i = 0; i < length; i++)
        cout << gSourceVector[i].thread_id << "\t" << gSourceVector[i].i_src_num << "\t" << gSourceVector[i].i_subsequence_length << "\n";
#endif

    for (i = 0; 0 < gMinSubsequenceLen && i < length; i++)
    {
        if (gSourceVector[i].i_subsequence_length == gMinSubsequenceLen)
        {
            for (j = 0; j < gSourceVector[i].i_subsequence_length; j++)
                cout << gSourceVector[i + j].i_src_num << " ";
            cout << "\n";
        }
    }
    cout << "duration: " << microseconds << " ms / " << nanoseconds << " ns\n";

    return 0;
}
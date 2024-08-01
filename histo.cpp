#include <iostream>
#include <random>
#include <chrono>
#include <iostream>
#include <fstream>
#include <thread>
#include <mutex>

const int dice = 6;
const int rolls = 3;
const int min = rolls;
const int iters = 10000000;
const int max = dice * rolls;

int numbers[iters];
std::mutex mtx;

int histogenerate(int strtInd, int endInd, int threadid) {
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist6(1, dice);
   
    int roll;

    for (int cnt = strtInd; cnt < endInd; cnt++) {
        roll = 0;
        for (int r = 1; r <= rolls; r++){            
            roll += dist6(rng);
        }
        numbers[cnt] = roll;
    }

    return 0;
}

int main(int argc, char* argv[]) { 
    int numThreads = std::thread::hardware_concurrency() - 1;
    if (argc > 1)
    { 
        try{
            numThreads = std::stoi(argv[1]);
        } catch (const std::exception& ex) {}
        std::cout << "Threads = " << numThreads << std::endl;
    }
    
    const int chunk_size = iters / numThreads; // Divide work among threads

    std::ofstream results;
    results.open ("results.txt");


    std::cout << "Start" << std::endl;
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    
    //Call code
    std::thread threads[numThreads];
    for (int i = 0; i < numThreads; ++i) {
        int start = i * chunk_size;
        int end = (i + 1) * chunk_size;
        threads[i] = std::thread(histogenerate, start, end, i);
    }

    // Wait for all threads to finish
    for (int i = 0; i < numThreads; ++i) {
        threads[i].join();
    }

    // Concatinate table
    for (int i = 0; i < max; i++) {
        int tot = 0;
        for (int j = 0; j < numThreads; j++) {
            tot += numbers[i][j];
        }
        numbers[i][0] = tot;
    }
    
    std::cout << "Calculated" << std::endl;
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    for (int cnt = min; cnt <= max; cnt++) {
        results << cnt << ';' << numbers[cnt][0] << std::endl;
    }
    std::cout << "Calc Time = " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "[ms]" << std::endl;
    std::cout << std::endl;

    return 0;
}
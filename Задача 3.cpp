 //Задача 3.cpp 

#include <iostream>
#include <vector>
#include<thread>
#include <chrono>
#include <windows.h>
#include<mutex>
#include "Timer.h"

using namespace std::chrono_literals;


std::mutex mtx;
std::vector<std::shared_ptr<std::atomic<int>>> progresses;

void updateProgressBar(std::shared_ptr<std::atomic<int>> progress, int total) {
    while (*progress < total) {
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        {
            std::unique_lock<std::mutex> lock(mtx);
            (*progress)++;
        }
    }
}

void drawRowProgress(std::shared_ptr<std::atomic<int>> progress, int total, int line, std::mutex& mtx)
{


    auto start_time = std::chrono::high_resolution_clock::now();
    while (*progress < total) {

        int barWidth = 20;
        int pos = (*progress * barWidth) / total;

        std::string progressBar = /*"Progress " +*/ std::to_string(line) + ": ";
        for (int i = 0; i < barWidth; ++i) {
            for (int k = 0; k < 1000000; ++k) {

            }
            if (i < pos) {
                progressBar += "=";
            }
            else {
                progressBar += ' ';
            }
        }

        progressBar += " ";

        std::unique_lock<std::mutex> lock(mtx);

        std::cout << "\033[" << line << ";0H";
        std::cout << std::this_thread::get_id() << ' ' << progressBar << std::flush;
    }

    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double/*, std::milli*/> elapsed_time = end_time - start_time;
    std::cout << elapsed_time.count() << "ms";

}

int main() {
    setlocale(LC_ALL, "Russian");
    int total = 20;
    unsigned int numThreads = std::thread::hardware_concurrency();

    progresses.resize(numThreads);

    std::vector<std::thread> threads;
    std::vector<std::thread> printThreads;

    for (int i = 0; i < numThreads; ++i) {

        progresses[i] = std::make_shared<std::atomic<int>>(0);
        threads.emplace_back(updateProgressBar, progresses[i], total);
        printThreads.emplace_back(drawRowProgress, progresses[i], total, i + 1, std::ref(mtx));
    }

    for (int i = 0; i < numThreads; ++i) {
        threads[i].join();
        printThreads[i].join();
    }

    std::cout << "\033[" << numThreads + 1 << ";0H";

    return 0;
}

//#include <iostream>
//#include <vector>
//#include<thread>
//#include <chrono>
//#include <windows.h>
//#include<mutex>
//#include "Timer.h"
//
//using namespace std::chrono_literals;
//std::mutex m1;
//std::mutex m2;
//std::vector<std::thread::id> T_id;
//auto sharedPtr_T_id = std::make_shared<std::vector<std::thread::id>>(T_id);
//consol_parameter CP5;
//int a{ 17 };
//int b{ 17 };
//int c{ 17 };
//int d{ 17 };
//
//std::once_flag flag;
//void print_once()
//{
//    std::cout << "#" << "\t " << "id" << "\t " << "Progress Bar          " << "\t " << "Time" << std::endl;
//}
//
//std::once_flag flag2;
//void print_id()
//{
//    consol_parameter CP;
//    int k = 2;
//    CP.SetPosition(8, 1);
//    for (auto& number : T_id) {
//        std::this_thread::sleep_for(300ms);
//        std::cout << number;
//        CP.SetPosition(8, k++);
//    }
//
//}
//
//void print_p()
//{
//    for (size_t i = 0; i < 70; i++)
//        std::cout << "=";
//}
//
//
//void print_bar(std::thread::id id)
//{
//
//    std::this_thread::sleep_for(40ms);
//    std::call_once(flag2, print_id);
//    char progresses{ '=' };
//
//    if (id == T_id.at(0))
//    {
//        CP5.SetPosition(a, 1);
//        std::thread t3(print_p);
//        t3.detach();
//        // std::cout << progresses;
//        a++;
//    }
//
//    else if (id == T_id.at(1))
//    {
//        CP5.SetPosition(b, 2);
//        std::cout << progresses;
//        b++;
//    }
//    else if (id == T_id.at(2))
//    {
//        CP5.SetPosition(c, 3);
//        std::cout << progresses;
//        c++;
//    }
//    else if (id == T_id.at(3))
//    {
//        CP5.SetPosition(d, 4);
//        std::cout << progresses;
//        d++;
//    }
//}
//
//void func6(int x)
//{
//    m2.lock();
//    T_id.push_back(std::this_thread::get_id());
//    m2.unlock();
//    auto start1 = std::chrono::high_resolution_clock::now();
//    for (size_t i = 0; i < x; i++)
//    {
//        std::lock_guard<std::mutex> lock(m1);
//        print_bar(std::this_thread::get_id());
//    }
//
//    auto end1 = std::chrono::high_resolution_clock::now();
//    std::chrono::duration<double/*, std::milli*/> time1 = end1 - start1;
//
//    std::cout << "          " << time1.count() << "s" << std::endl;//вывод         
//    std::cout << std::endl;
//
//
//}
//
//void timeSV(int nt, int x)
//{
//    std::call_once(flag, print_once);
//
//    for (int i = 1; i <= nt; ++i) { std::cout << i << std::endl; }
//
//    std::vector<std::thread> TV;
//
//    for (int i = 0; i < nt; i++)
//    {
//        TV.push_back(std::thread(func6, x));
//    }
//    for (auto& t : TV)
//    {
//        t.join();
//    }
//}
//
//
//int main()
//{
//    setlocale(LC_ALL, "ru");
//    SetConsoleCP(1251);
//    SetConsoleOutputCP(1251);
//
//    unsigned int n = std::thread::hardware_concurrency();
//    timeSV(n, 12);
//    std::cout << std::endl;
//
//    return 0;
//}


 




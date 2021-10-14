#pragma once
#include <thread>
#include <string>

class Stopwatch {
    private:
        static bool running;
        static std::thread counter;
        static int milliseconds, seconds, minutes, hours, days, weeks;
    public:
        static void count();
        Stopwatch();
        static void start();
        static void stop();
        static std::string timeTaken();
        static int totalSeconds();
        static void reset();
};
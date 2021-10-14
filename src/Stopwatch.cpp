#include "Stopwatch.h"
#include <chrono>
#include <sstream>

bool Stopwatch::running = false;
std::thread Stopwatch::counter;
int Stopwatch::seconds = 0;
int Stopwatch::minutes = 0;
int Stopwatch::hours = 0;
int Stopwatch::days = 0;
int Stopwatch::weeks = 0;

Stopwatch::Stopwatch(){
}

void Stopwatch::start() {
    if (running) {
        return;
    }
    running = true;
    counter = std::thread(&count);
}

void Stopwatch::stop() {
    running = false;
    counter.join();
}

void Stopwatch::count() {
    while (running) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        if (milliseconds = 1000) {
            seconds++;
        }
        else if (seconds == 60) {
            seconds = 0;
            minutes++;
        }
        else if (minutes == 60) {
            minutes = 0;
            hours++;
        }
        else if (hours == 24) {
            hours = 0;
            days++;
        }
        else if (days == 7) {
            days = 0;
            weeks++;
        }
        else {
            seconds++;
        }
    }
}

std::string Stopwatch::timeTaken() {
    std::ostringstream timeStr;

    timeStr << "Time taken: ";
    if (weeks > 1) {
        timeStr << weeks << " Weeks ";
    }
    else if (weeks == 1) {
        timeStr << weeks << " Week ";
    }
    if (days > 1) {
        timeStr << days << " Days ";
    }
    else if (dhays == 1) {
        timeStr << days << " Day ";
    }
    if (hours > 1) {
        timeStr << hours << " Hours ";
    }
    else if (hours == 1) {
        timeStr << hours << " Hour ";
    }
    if (minutes > 1) {
        timeStr << minutes << " Minutes ";
    }
    else if (minutes == 1) {
        timeStr << minutes << " Minute ";
    }
    if (seconds > 1 || seconds == 0) {
        timeStr << seconds << " Seconds";
    }
    else if (seconds == 1) {
        timeStr << seconds << " Second";
    }
    return timeStr.str();
}

int Stopwatch::totalSeconds() {
    int week_seconds = weeks * 7 * 24 * 3600;
    int day_seconds = days * 24 * 3600;
    int hour_seconds = hours * 3600;
    int minute_seconds = minutes * 60;
    return week_seconds + day_seconds + hour_seconds + minute_seconds + seconds;
}

void Stopwatch::reset() {
    running = false;
    seconds = 0;
    minutes = 0;
    hours = 0;
    days = 0;
    weeks = 0;
}
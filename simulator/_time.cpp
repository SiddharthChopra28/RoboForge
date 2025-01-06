#include "_time.h"

long long get_millis(hr_time_point st){
    hr_duration d = hr_clock::now() - st;
    std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(d);
    return ms.count();
}

long long get_micros(hr_time_point st){
    hr_duration d = hr_clock::now() - st;
    std::chrono::microseconds ms = std::chrono::duration_cast<std::chrono::microseconds>(d);
    return ms.count();
}

void delay(long int ms){
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
    // OR better way to do this is: save starting time and check time in the main loop every N number of loops, and while the delay time is not done, do nothing
}

void delayMicroseconds(long int us){
    std::this_thread::sleep_for(std::chrono::microseconds(us));
}


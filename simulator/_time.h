#ifndef ROBOFORGE__TIME_H
#define ROBOFORGE__TIME_H

#include<chrono>
#include<thread>


#define hr_clock std::chrono::high_resolution_clock
#define hr_time_point hr_clock::time_point
#define hr_duration hr_clock::duration

long long get_millis(hr_time_point st);
long long get_micros(hr_time_point st);
void delay(long int ms);
void delayMicroseconds(long int us);


#endif

#ifndef ROBOFORGE_FUNCS_H
#define ROBOFORGE_FUNCS_H

#define DEFAULT 1
#define EXTERNAL 0
#define INTERNAL1V1 2
#define INTERNAL 3
#define INTERNAL2V56 3

#include "components.h"
#include <iostream>
#include<typeinfo>
using namespace std;

void pinMode_(std::shared_ptr<Arduino> ard, uint8_t pin, uint8_t mode);
void digitalWrite_(std::shared_ptr<Arduino> ard, uint8_t pin, uint8_t val);
int digitalRead_(std::shared_ptr<Arduino> ard, uint8_t pin);
int analogRead_(std::shared_ptr<Arduino> ard, uint8_t pin); // max value can be 1023 and min 0
void analogReference_(std::shared_ptr<Arduino> ard, uint8_t mode);
void analogWrite_(std::shared_ptr<Arduino> ard, uint8_t pin, int val);

//unsigned long pulseIn_(Arduino& ard, uint8_t pin, uint8_t state, unsigned long timeout);
//unsigned long pulseInLong_(Arduino& ard, uint8_t pin, uint8_t state, unsigned long timeout);

//void attachInterrupt_(Arduino& ard, uint8_t interruptNum, void *userFunc(), int mode);
//void detachInterrupt_(Arduino& ard, uint8_t interruptNum);


#endif //ROBOFORGE_FUNCS_H

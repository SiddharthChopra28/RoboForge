//
// Created by siddharth on 10/2/25.
//

#ifndef ROBOFORGE_BASE_H
#define ROBOFORGE_BASE_H

#define ARDUINO 0
#define IR 1
#define LED 2
#define TEMPSENSOR 3

#define DIGITAL 0
#define DIGITALPWM 1
#define ANALOG 2
#define POWER 3

#define HIGH 1
#define LOW  0

#define INPUT 0
#define OUTPUT 1
#define INPUT_PULLUP 2




#include <iostream>
#include <vector>
#include <memory>
using namespace std;

class Component;
class Wire;
class Pin;
class DigitalPin;
class AnalogPin;
class DigitalPwmPin;
class PowerPin;
class Arduino;
class TempSensor;
class Led;
class IRSensor;


#endif //ROBOFORGE_BASE_H
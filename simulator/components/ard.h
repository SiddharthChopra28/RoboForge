//
// Created by siddharth on 10/2/25.
//

#ifndef ROBOFORGE_ARD_H
#define ROBOFORGE_ARD_H

#include "base.h"


// pin ids
#define PIN_D0 0
#define PIN_D1 1
#define PIN_D2 2
#define PIN_D3 3 //pwm
#define PIN_D4 4
#define PIN_D5 5 //pwm
#define PIN_D6 6 //pwm
#define PIN_D7 7
#define PIN_D8 8
#define PIN_D9 9 //pwm
#define PIN_D10 10 //pwm
#define PIN_D11 11 //pwm
#define PIN_D12 12
#define PIN_D13 13
#define LED_BUILTIN 13

#define PIN_A0 14
#define PIN_A1 15
#define PIN_A2 16
#define PIN_A3 17
#define PIN_A4 18
#define PIN_A5 19
#define PIN_A6 20
#define PIN_A7 21

#define PIN_VIN 22
#define PIN_L_GND_1 23
#define PIN_L_GND_2 24
#define PIN_5V 25
#define PIN_3V3 26
#define PIN_RESET 27
#define PIN_IOREF 28

#define PIN_R_GND 29
#define PIN_AREF 30
#define PIN_SDA 31
#define PIN_SCL 32

class Arduino: public Component{
public:
    Arduino(int no_pins, float ov);
    static std::shared_ptr<Arduino> create(int no_pins, float ov);
    void initialize();
    float analogReference;
    float getState() override;
    void setState(float) override;
};

#endif //ROBOFORGE_ARD_H
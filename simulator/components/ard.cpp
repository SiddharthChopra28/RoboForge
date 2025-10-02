//
// Created by siddharth on 10/2/25.
//
#include "component.h"
#include "ard.h"
#include "pin.h"


Arduino::Arduino(int no_pins, float ov) : Component(no_pins, ov){
    analogReference = ov;
}


std::shared_ptr<Arduino> Arduino::create(int no_pins, float ov) {
    auto instance = std::make_shared<Arduino>(no_pins, ov);
    instance->initialize();
    return instance;
}

void Arduino::initialize() {
    auto self= shared_from_this();
    for (int i=0; i<=13; i++){
        if (i==3 || i==5 || i==6 || i==9 || i==10 || i==11){
            pins[i] = std::make_shared<DigitalPwmPin>(self, i);
        }
        else{
            pins[i] = std::make_shared<DigitalPin>(self, i);
        }
    }
    for (int i=14; i<=21; i++){
        pins[i] = std::make_shared<AnalogPin>(self, i);
    }
    for (int i=22; i<=32; i++){
        pins[i] = std::make_shared<PowerPin>(self, i);
    }
}

float Arduino::getState() {
    // return 1 or 0 depending on the state of the led
    if (pins[LED_BUILTIN]->mode == INPUT){
        return 0;
    }
    if (pins[LED_BUILTIN]->getPinPotential() == HIGH*operatingVoltage){
        return 1;
    }
    else{
        return 0;
    }

}

void Arduino::setState(float state) {
    // do nothing
}

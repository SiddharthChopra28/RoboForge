//
// Created by siddharth on 10/2/25.
//

#include "component.h"
#include "pin.h"
#include "led.h"


Led::Led(int no_pins, float ov):Component(no_pins, ov){}

std::shared_ptr<Led> Led::create(int no_pins,float ov) {
    auto instance = std::make_shared<Led>(no_pins, ov);
    instance->initialize();
    return instance;
}

void Led::initialize() {
    auto self= shared_from_this();
    pins[PIN_LED_POS] = std::make_shared<PowerPin>(self, PIN_LED_POS);
    pins[PIN_LED_NEG] = std::make_shared<PowerPin>(self, PIN_LED_NEG);

    pins[PIN_LED_POS] -> mode = INPUT;
    pins[PIN_LED_NEG] -> mode = INPUT;
}

float Led::getState() {
    // returns 1 or 0
    if (pins[PIN_LED_POS]->getPinPotential() == operatingVoltage && pins[PIN_LED_NEG]->getPinPotential() == LOW) {
        return HIGH;
    }
    else{
        return LOW;
    }
}

void Led::setState(float) {
    // do nothing
}
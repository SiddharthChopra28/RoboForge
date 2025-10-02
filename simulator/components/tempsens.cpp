//
// Created by siddharth on 10/2/25.
//

#include "component.h"
#include "pin.h"
#include "tempsens.h"

TempSensor::TempSensor(int no_pins, float ov) : Component(no_pins, ov){
}

std::shared_ptr<TempSensor> TempSensor::create(int no_pins, float ov){
    auto instance = std::make_shared<TempSensor>(no_pins, ov);
    instance->initialize();
    return instance;
}

void TempSensor::initialize() {
    auto self = shared_from_this();
    pins[PIN_TEMPSENS_VCC] = std::make_shared<PowerPin>(self, PIN_TEMPSENS_VCC);
    pins[PIN_TEMPSENS_OUT] = std::make_shared<DigitalPwmPin>(self, PIN_TEMPSENS_OUT);
    pins[PIN_TEMPSENS_GND] = std::make_shared<PowerPin>(self, PIN_TEMPSENS_GND);

    pins[PIN_TEMPSENS_VCC] -> mode = INPUT;
    pins[PIN_TEMPSENS_OUT] -> mode = OUTPUT;
    pins[PIN_TEMPSENS_GND] -> mode = INPUT;
}

float TempSensor::getState() {
    if (pins[PIN_TEMPSENS_VCC]->getPinPotential() == operatingVoltage && pins[PIN_TEMPSENS_GND]->getPinPotential() == LOW){
        return temp;
    }
    else{
        return 0;
    }
}

void TempSensor::setState(float t) {
    // temperature in degree celcius is recevied
    temp = t;
    pins[PIN_TEMPSENS_OUT]->setPinPotential(getState()/100.f*operatingVoltage);
    pins[PIN_TEMPSENS_OUT]->update_potential();
}


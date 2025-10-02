//
// Created by siddharth on 10/2/25.
//

#include "component.h"
#include "pin.h"
#include "irsensor.h"


IRSensor::IRSensor(int no_pins, float ov) : Component(no_pins, ov){}

std::shared_ptr<IRSensor> IRSensor::create(int no_pins, float ov) {
    auto instance = std::make_shared<IRSensor>(no_pins, ov);
    instance->initialize();
    return instance;
}

void IRSensor::initialize() {
    auto self= shared_from_this();
    pins[PIN_IRSENSOR_VIN] = std::make_shared<PowerPin>(self, PIN_IRSENSOR_VIN);
    pins[PIN_IRSENSOR_GND] = std::make_shared<PowerPin>(self, PIN_IRSENSOR_GND);
    pins[PIN_IRSENSOR_OUT] = std::make_shared<DigitalPin>(self, PIN_IRSENSOR_OUT);

    pins[PIN_IRSENSOR_VIN] -> mode = INPUT;
    pins[PIN_IRSENSOR_GND] -> mode = INPUT;
    pins[PIN_IRSENSOR_OUT] -> mode = OUTPUT;
}

float IRSensor::getState() {
    // returns 1 or 0 depending upon whether there is an obstacle present
    if (pins[PIN_IRSENSOR_VIN]->getPinPotential() == operatingVoltage && pins[PIN_IRSENSOR_GND]->getPinPotential() == LOW){
        return obstacle;
    }
    else{
        return LOW;
    }
}

void IRSensor::setState(float state) {
    obstacle = bool(state);
    pins[PIN_IRSENSOR_OUT]->setPinPotential(float(getState())*operatingVoltage);
    pins[PIN_IRSENSOR_OUT]->update_potential();
}


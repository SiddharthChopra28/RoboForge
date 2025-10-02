//
// Created by siddharth on 10/2/25.
//

#include "pin.h"
#include "wire.h"
#include "component.h"
#include "ard.h"

Pin::Pin(std::shared_ptr<Component> pc, int _id){
    parentComponent = pc;
    id = _id;
}

void Pin::update_potential() {
    if (!wireConnected){return;}
    if (wire->get_no_ends_connected() != 2) {
        return;
    }

    std::shared_ptr<Pin> otherpin = (wire->p1 == shared_from_this()) ? wire->p2 : wire->p1;

    if (mode == OUTPUT){
        if (otherpin->mode == OUTPUT){
            // what to do??
            return;
        }
        else if (otherpin->mode == INPUT){
            otherpin->setPinPotential(getPinPotential());

        }
        // there is also input_pullup mode??
    }
    else if(mode == INPUT) {
        if (otherpin->mode == INPUT) {
            return; //what to do?
        }
        else if (otherpin->mode == OUTPUT) {
            setPinPotential(otherpin->getPinPotential());
        }
    }

}


DigitalPin::DigitalPin(std::shared_ptr<Component> pc, int _id): Pin(pc, _id){
    pinPotential = 0; //set it to 0 by default
    defaultPotential = 0;
    type = DIGITAL;
}

float DigitalPin::getPinPotential(){
    return pinPotential; // this will be either 0 or 1
}

void DigitalPin::setPinPotential(float hl){
    // hl should be either 0 or 1*ov
    pinPotential = hl;
}

DigitalPwmPin::DigitalPwmPin(std::shared_ptr<Component> pc, int _id): Pin(pc, _id){
    pinPotential = 0;
    defaultPotential = 0;
    type=DIGITALPWM;
}
float DigitalPwmPin::getPinPotential(){
    return pinPotential;
}

void DigitalPwmPin::setPinPotential(float pp){
    // pp should be less than boardOperatingVoltage
    pinPotential = pp;
}

AnalogPin::AnalogPin(std::shared_ptr<Component> pc, int _id): Pin(pc, _id){
    pinPotential = 0;
    defaultPotential = 0;
    type = ANALOG;
}
float AnalogPin::getPinPotential(){
    return pinPotential;
}

void AnalogPin::setPinPotential(float hl){
    // hl should be either 0 or 1*ov
    pinPotential = hl;
}


PowerPin::PowerPin(std::shared_ptr<Component> pc, int _id): Pin(pc, _id){
    type=POWER;
    if (id == PIN_R_GND || id == PIN_L_GND_1 || id == PIN_L_GND_2){
        pinPotential = 0;
        defaultPotential = 0;
        mode = OUTPUT; // they force the other end/pin also to get to 0 potential
    }
    if (id == PIN_VIN || id == PIN_AREF){
        pinPotential = 0;
        defaultPotential = 0;
        mode = INPUT;
    }
    else if (id == PIN_5V){
        pinPotential = 5;
        defaultPotential = 5;
        mode = OUTPUT;
    }
    else if (id == PIN_3V3) {
        pinPotential = 3.3;
        defaultPotential = 3.3;
        mode = OUTPUT;
    }
    else if (id == PIN_IOREF || id == PIN_SCL || id == PIN_SDA){
        pinPotential = parentComponent->operatingVoltage;
        defaultPotential = parentComponent->operatingVoltage;
        mode = OUTPUT;
    }
    else if (id == PIN_RESET){
        pinPotential = parentComponent->operatingVoltage;
        defaultPotential = parentComponent->operatingVoltage;
        mode = INPUT;
    }
    else{
        pinPotential = 0;
        defaultPotential = 0;
    }
}

float PowerPin::getPinPotential(){
    return pinPotential;
}

void PowerPin::setPinPotential(float pp){
    if (id == PIN_R_GND || id == PIN_L_GND_1 || id == PIN_L_GND_2 || id == PIN_5V || id == PIN_3V3 || id == PIN_IOREF){
        // eat 5 star, do nothing
    }
    else{
        pinPotential = pp;
    }

}
#include "components.h"

Pin::Pin(Component* pc, int _id){
    parentComponent = pc;
    id = _id;
}

Component::~Component(){
    for (int i=0; i<no_of_pins; i++){
        delete pins[i];
    }
    delete[] pins;
}

DigitalPin::DigitalPin(Component* pc, int _id): Pin(pc, _id){
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

DigitalPwmPin::DigitalPwmPin(Component* pc, int _id): Pin(pc, _id){
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

AnalogPin::AnalogPin(Component* pc, int _id): Pin(pc, _id){
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


PowerPin::PowerPin(Component* pc, int _id): Pin(pc, _id){
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


Component::Component(int no_pins, float ov){
    no_of_pins = no_pins;
    pins = new Pin*[no_of_pins];
    operatingVoltage = ov;
}


Arduino::Arduino(int no_pins, float ov) : Component(no_pins, ov){
    analogReference = ov;
    for (int i=0; i<=13; i++){
        if (i==3 || i==5 || i==6 || i==9 || i==10 || i==11){
            pins[i] = new DigitalPwmPin(this,i);
        }
        else{
            pins[i] = new DigitalPin(this,i);
        }
    }
    for (int i=14; i<=21; i++){
        pins[i] = new AnalogPin(this, i);
    }
    for (int i=22; i<=32; i++){
        pins[i] = new PowerPin(this, i);
    }

}

void Component::connectPin(int pinid, Wire &wire) const {
    Pin* pin = pins[pinid];
    if (!wire.connectEnd(pin)){
        return;
    }

    if (wire.get_no_ends_connected() == 2){
        Pin*& otherpin = (wire.p1 == pin) ? wire.p2 : wire.p1;

        if (pin->mode == OUTPUT){
            if (otherpin->mode == OUTPUT){
                // what to do??
                return;
            }
            else if (otherpin->mode == INPUT){
                otherpin->setPinPotential(pin->getPinPotential());
            }
            // there is also input_pullup mode??
        }
        else if(pin->mode == INPUT) {
            if (otherpin->mode == INPUT) {
                return; //what to do?
            }
            else if (otherpin->mode == OUTPUT) {
                pin->setPinPotential(otherpin->getPinPotential());
            }
        }
    }


}

void Component::disconnectPin(int pinid, Wire &wire) const{
    Pin* pin = pins[pinid];

    if (!wire.disconnectEnd(pin)){
        return;
    }

    if (wire.get_no_ends_connected() == 1){
        Pin*& otherpin = (wire.p1 == nullptr) ? wire.p2 : wire.p1;

        if (pin->mode == OUTPUT){
            if (otherpin->mode == OUTPUT){
                return; // what to do??
            }
            else if (otherpin->mode == INPUT){
                otherpin->setPinPotential(otherpin->defaultPotential);
            }
        }
        else if (pin->mode == INPUT){
            if (otherpin->mode == INPUT){
                return; //? what to do?
            }
            if (otherpin->mode == OUTPUT){
                pin->setPinPotential(pin->defaultPotential);
            }
        }
    }

}


IRSensor::IRSensor(int no_pins, float ov) : Component(no_pins, ov){
    pins[PIN_IRSENSOR_VIN] = new PowerPin(this, PIN_IRSENSOR_VIN);
    pins[PIN_IRSENSOR_GND] = new PowerPin(this, PIN_IRSENSOR_VIN);
    pins[PIN_IRSENSOR_OUT] = new DigitalPin(this, PIN_IRSENSOR_VIN);
    pins[PIN_IRSENSOR_VIN] -> mode = INPUT;
    pins[PIN_IRSENSOR_GND] -> mode = INPUT;
    pins[PIN_IRSENSOR_OUT] -> mode = OUTPUT;
}

bool IRSensor::getOutput() {
    if (pins[PIN_IRSENSOR_VIN]->getPinPotential() == operatingVoltage && pins[PIN_IRSENSOR_GND]->getPinPotential() == LOW){
        return obstacle;
    }
    else{
        return LOW;
    }
}

void IRSensor::setInput(bool s) {
    obstacle = s;
}


int Wire::get_no_ends_connected() const {
    if (c1 == nullptr && c2 == nullptr) {
        return 0;
    }
    if ((c1 != nullptr && c2 == nullptr) || (c1 == nullptr && c2 != nullptr)){
        return 1;
    }
    else{
        return 2;
    }
}

bool Wire::connectEnd(Pin* pin) {
    if (c1 != nullptr && c2 != nullptr){
        return false;
    }
    if (pin == p1 || pin == p2){
        return false;
    }
    if (c1 == nullptr){
        p1 = pin;
        c1 = pin->parentComponent;
        pin->wireConnected = true;
        pin->wire = this;
        return true;
    }
    else{
        p2 = pin;
        c2 = pin->parentComponent;
        pin->wireConnected = true;
        pin->wire = this;
        return true;
    }
}


bool Wire::disconnectEnd(Pin* pin){
    if (p1 == pin){
        pin->wireConnected = false;
        pin->wire = nullptr;
        p1 = nullptr;
        c1 = nullptr;
        return true;
    }

    if (p2 == pin){
        pin->wireConnected = false;
        pin->wire = nullptr;
        p2 = nullptr;
        c2 = nullptr;
        return true;
    }
    return false;
}



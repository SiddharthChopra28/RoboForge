#include "components.h"

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


Component::Component(int no_pins, float ov){
    no_of_pins = no_pins;
    pins.resize(no_pins);
    operatingVoltage = ov;
}

std::shared_ptr<Arduino> Arduino::create(int no_pins, float ov) {
    auto instance = std::make_shared<Arduino>(no_pins, ov);
    instance->initialize();
    return instance;
}


Arduino::Arduino(int no_pins, float ov) : Component(no_pins, ov){
    analogReference = ov;
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

void Component::connectPin(int pinid, std::shared_ptr<Wire> wire) {

    std::shared_ptr<Pin> pin = pins[pinid];

    if (!wire->connectEnd(pin)){
        return;
    }

    if (wire->get_no_ends_connected() == 2){
        pin->update_potential();
    }
}

void Component::disconnectPin(int pinid, std::shared_ptr<Wire> wire) const{
    std::shared_ptr pin = pins[pinid];

    if (!wire->disconnectEnd(pin)){
        return;
    }

    if (wire->get_no_ends_connected() == 1){
        std::shared_ptr<Pin> otherpin = (wire->p1 == nullptr) ? wire->p2 : wire->p1;

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

    pins[PIN_IRSENSOR_VIN] -> mode = INPUT;
    pins[PIN_IRSENSOR_GND] -> mode = INPUT;
}

float Led::getState() {
    // returns 1 or 0
    if (pins[PIN_LED_POS]->getPinPotential() == operatingVoltage && pins[PIN_IRSENSOR_GND]->getPinPotential() == LOW) {
        return HIGH;
    }
    else{
        return LOW;
    }
}

void Led::setState(float) {
    // do nothing
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

BreadBoard::BreadBoard(int nop, float opvol) : Component(nop, opvol){}

std::shared_ptr<BreadBoard> BreadBoard::create(int nop, float opvol) {
    auto instance = std::make_shared<BreadBoard>(nop, opvol);

}

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

bool Wire::connectEnd(std::shared_ptr<Pin> pin) {
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
        pin->wire = shared_from_this();
        return true;
    }
    else{
        p2 = pin;
        c2 = pin->parentComponent;
        pin->wireConnected = true;
        pin->wire = shared_from_this();
        return true;
    }
}



bool Wire::disconnectEnd(std::shared_ptr<Pin> pin){
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


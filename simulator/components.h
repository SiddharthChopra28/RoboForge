#ifndef ROBOFORGE_COMPONENTS_H
#define ROBOFORGE_COMPONENTS_H


#include <iostream>

using namespace std;
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

#define PIN_R_GND 29
#define PIN_AREF 30
#define PIN_SDA 31
#define PIN_SCL 32

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

#define PIN_IRSENSOR_VIN 0
#define PIN_IRSENSOR_GND 1
#define PIN_IRSENSOR_OUT 2

#define HIGH 1
#define LOW  0

#define INPUT 0
#define OUTPUT 1
#define INPUT_PULLUP 2

#define DIGITAL 0
#define DIGITALPWM 1
#define ANALOG 2
#define POWER 3


class Wire;
class Component;
class Pin;
class DigitalPin;
class AnalogPin;
class DigitalPwmPin;
class PowerPin;
class Arduino;
class IRSensor;

class Component {
public:
    int no_of_pins;
    Pin** pins;
    float operatingVoltage;

    Component(int no_pins, float opVol);
    void connectPin(int pinid, Wire& wire) const;
    void disconnectPin(int pinid, Wire& wire) const;
    ~Component();
//    void pinMode();

};
class IRSensor: public Component{
public:
    IRSensor(int no_pins, float ov);
    bool state=LOW;
    bool getOutput();
    void setState(bool);
};


class Arduino: public Component{
public:
    Arduino(int no_pins, float ov);
    float analogReference;
};
class Pin{
public:
    int id;
    int mode = INPUT;
    int type;
    bool wireConnected = false;
    Wire* wire = nullptr;
    Component* parentComponent;

    Pin(Component* pc, int id);
    virtual ~Pin() = default;

    virtual float getPinPotential() = 0; // returns voltage of the pin

    virtual void setPinPotential(float) = 0;

    float defaultPotential;
    float pinPotential;
};

class DigitalPin: public Pin{
public:
    // can read high/low and can write high/low
    DigitalPin(Component* pc, int _id);
    float getPinPotential();
    void setPinPotential(float hl);
};

class DigitalPwmPin: public Pin{
public:
    DigitalPwmPin(Component* pc, int _id);
    float getPinPotential();

    void setPinPotential(float pp);
};

class AnalogPin: public Pin{
public:
    AnalogPin(Component* pc, int _id);
    float getPinPotential();
    void setPinPotential(float hl);
};

class PowerPin: public Pin{
public:
    PowerPin(Component* pc, int _id);
    float getPinPotential();

    void setPinPotential(float pp);

};


class Wire{
public:
    Component* c1 = nullptr;
    Component* c2 = nullptr; // the components at the two ends of the wires

    Pin* p1 = nullptr;
    Pin* p2 = nullptr;

    bool connectEnd(Pin* pin); //returns true if success. false if fail
    bool disconnectEnd(Pin* pin); //returns true if success. false if fail
    int get_no_ends_connected() const;
};



#endif
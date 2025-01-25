#ifndef ROBOFORGE_COMPONENTS_H
#define ROBOFORGE_COMPONENTS_H


#include <iostream>
#include <vector>
#include <memory>
using namespace std;

// components ids
#define ARDUINO 0
#define IR 1
#define LED 2

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


#define PIN_IRSENSOR_VIN 0
#define PIN_IRSENSOR_GND 1
#define PIN_IRSENSOR_OUT 2

#define HIGH 1
#define LOW  0

#define INPUT 0
#define OUTPUT 1
#define INPUT_PULLUP 2

#define PIN_LED_POS 0
#define PIN_LED_NEG 1

#define PIN_TEMPSENS_VCC 0
#define PIN_TEMPSENS_OUT 1
#define PIN_TEMPSENS_GND 2

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

class Component : public std::enable_shared_from_this<Component>{
public:
    int no_of_pins;
    std::vector<shared_ptr<Pin>> pins;
    float operatingVoltage;

    Component(int no_pins, float opVol);
    void connectPin(int pinid, std::shared_ptr<Wire> wire);
    void disconnectPin(int pinid, std::shared_ptr<Wire> wire) const;
    virtual int getState()=0;
    virtual void setState(int)=0;

};

class IRSensor: public Component{
public:
    IRSensor(int no_pins, float ov);
    bool obstacle=false;
    int getState() override;
    void setState(int) override;
    static std::shared_ptr<IRSensor> create(int no_pins, float ov);
    void initialize();

};


class Arduino: public Component{
public:
    Arduino(int no_pins, float ov);
    static std::shared_ptr<Arduino> create(int no_pins, float ov);
    void initialize();
    float analogReference;
    int getState() override;
    void setState(int) override;
};

class Led: public Component{
public:
    Led(int no_pins, float ov);
    static std::shared_ptr<Led> create(int no_pins, float ov);
    void initialize();
    int getState() override;
    void setState(int) override;

};

//class BreadBoard: public Component{
//public:
//    BreadBoard(int nop, float opvol);
//    static std::shared_ptr<BreadBoard> create(int nop, float opvol);
//    void initialize();
//    int getState() override;
//    void setState(int) override;
//};

class TempSensor: public Component{
public:
    TempSensor(int no_pins, float ov);
    static std::shared_ptr<TempSensor> create(int no_pins, float ov);
    void initialize();
    int getState() override;
    void setState(int) override;


};


class Pin : public std::enable_shared_from_this<Pin>{
public:
    int id;
    int mode = INPUT;
    int type;
    bool wireConnected = false;
    std::shared_ptr<Wire> wire;

    std::shared_ptr<Component> parentComponent;

    Pin(std::shared_ptr<Component> pc, int id);

    virtual float getPinPotential() = 0; // returns voltage of the pin

    virtual void setPinPotential(float) = 0;

    void update_potential();

    float defaultPotential;
    float pinPotential;
};

class DigitalPin: public Pin{
public:
    // can read high/low and can write high/low
    DigitalPin(std::shared_ptr<Component> pc, int _id);
    float getPinPotential() override;
    void setPinPotential(float hl) override;
};

class DigitalPwmPin: public Pin{
public:
    DigitalPwmPin(std::shared_ptr<Component> pc, int _id);
    float getPinPotential() override;

    void setPinPotential(float pp) override;
};

class AnalogPin: public Pin{
public:
    AnalogPin(std::shared_ptr<Component> pc, int _id);
    float getPinPotential() override;
    void setPinPotential(float hl) override;
};

class PowerPin: public Pin{
public:
    PowerPin(std::shared_ptr<Component> pc, int _id);
    float getPinPotential() override;

    void setPinPotential(float pp) override;

};


class Wire : public std::enable_shared_from_this<Wire>{
public:
    std::shared_ptr<Component> c1 = nullptr;
    std::shared_ptr<Component> c2 = nullptr;

    std::shared_ptr<Pin> p1 = nullptr;
    std::shared_ptr<Pin> p2 = nullptr;


    bool connectEnd(std::shared_ptr<Pin>); //returns true if success. false if fail
    bool disconnectEnd(std::shared_ptr<Pin>); //returns true if success. false if fail
    int get_no_ends_connected() const;
};



#endif
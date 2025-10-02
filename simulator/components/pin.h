//
// Created by siddharth on 10/2/25.
//

#ifndef ROBOFORGE_PIN_H
#define ROBOFORGE_PIN_H

#include "base.h"

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


#endif //ROBOFORGE_PIN_H
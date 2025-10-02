//
// Created by siddharth on 10/2/25.
//

#ifndef ROBOFORGE_COMPONENT_H
#define ROBOFORGE_COMPONENT_H

#include "base.h"

class Component : public std::enable_shared_from_this<Component>{
public:
    int no_of_pins;
    std::vector<shared_ptr<Pin>> pins;
    float operatingVoltage;

    Component(int no_pins, float opVol);
    void connectPin(int pinid, std::shared_ptr<Wire> wire);
    void disconnectPin(int pinid, std::shared_ptr<Wire> wire) const;
    virtual float getState()=0;
    virtual void setState(float)=0;

};


#endif //ROBOFORGE_COMPONENT_H
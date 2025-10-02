//
// Created by siddharth on 10/2/25.
//

#ifndef ROBOFORGE_IRSENSOR_H
#define ROBOFORGE_IRSENSOR_H

#include "base.h"

#define PIN_IRSENSOR_VIN 0
#define PIN_IRSENSOR_GND 1
#define PIN_IRSENSOR_OUT 2


class IRSensor: public Component{
public:
    IRSensor(int no_pins, float ov);
    bool obstacle=false;
    float getState() override;
    void setState(float) override;
    static std::shared_ptr<IRSensor> create(int no_pins, float ov);
    void initialize();

};


#endif //ROBOFORGE_IRSENSOR_H
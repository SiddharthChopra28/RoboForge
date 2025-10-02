//
// Created by siddharth on 10/2/25.
//

#ifndef ROBOFORGE_TEMPSENS_H
#define ROBOFORGE_TEMPSENS_H

#define PIN_TEMPSENS_VCC 0
#define PIN_TEMPSENS_OUT 1
#define PIN_TEMPSENS_GND 2

#include "base.h"

class TempSensor: public Component{
public:
    TempSensor(int no_pins, float ov);
    static std::shared_ptr<TempSensor> create(int no_pins, float ov);
    void initialize();
    float temp=25;
    float getState() override;
    void setState(float) override;

};


#endif //ROBOFORGE_TEMPSENS_H
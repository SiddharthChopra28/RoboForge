//
// Created by siddharth on 10/2/25.
//

#ifndef ROBOFORGE_LED_H
#define ROBOFORGE_LED_H


#include "base.h"

#define PIN_LED_POS 0
#define PIN_LED_NEG 1

class Led: public Component{
public:
    Led(int no_pins, float ov);
    static std::shared_ptr<Led> create(int no_pins, float ov);
    void initialize();
    float getState() override;
    void setState(float) override;

};



#endif //ROBOFORGE_LED_H